/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_netxduo.c
  * @brief   NetXDuo + AI Fusion (CPU Mode) + Capteurs I2C HTS221/LPS22HH
  *          - DHCP + SNTP + MQTT
  *          - Inférence IA depuis capteurs réels
  *          - Timestamp Unix incrémenté
  *          - Features sin/cos heure française
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_netxduo.h"
#include "nxd_dhcp_client.h"

/* USER CODE BEGIN Includes */
#include "nxd_dns.h"
#include "nxd_mqtt_client.h"
#include "nxd_sntp_client.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

/* --- INCLUDES IA --- */
#include "app_x-cube-ai.h"
#include "ai_datatypes_defines.h"

/* --- INCLUDES CAPTEURS --- */
#include "hts221_reg.h"
#include "lps22hh_reg.h"

extern ai_i8 *data_ins[];
extern ai_i8 *data_outs[];
extern int ai_run(void);

/* Handle I2C depuis main.c */
extern I2C_HandleTypeDef hi2c1;
/* USER CODE END Includes */

/* -------------------------------------------------------------------------- */
/* Private defines                                                             */
/* -------------------------------------------------------------------------- */
#define BROKER_HOSTNAME     "meteo.quebotek.fr"
#define MQTT_PORT           18830
#define MQTT_CLIENT_ID      "STM32_ALBERT_N6"
#define MQTT_TOPIC          "meteo/data"
#define MQTT_USER           "albert"
#define MQTT_PASS           "pass123"

#define SNTP_SERVER_IP      IP_ADDRESS(172, 232, 44, 156)
#define NTP_TO_UNIX_OFFSET  2208988800UL
#define DHCP_TIMEOUT_TICKS  (30 * NX_IP_PERIODIC_RATE)

/* -------------------------------------------------------------------------- */
/* Private variables                                                           */
/* -------------------------------------------------------------------------- */
TX_THREAD      NxAppThread;
NX_PACKET_POOL NxAppPool;
NX_IP          NetXDuoEthIpInstance;
TX_SEMAPHORE   DHCPSemaphore;
NX_DHCP        DHCPClient;

TX_THREAD       AppTCPThread;
TX_THREAD       AppLinkThread;

NX_DNS          DnsClient;
NXD_MQTT_CLIENT MqttClient;
ULONG           mqtt_client_stack[2048];
NX_SNTP_CLIENT  SntpClient;

ULONG           IpAddress;
ULONG           NetMask;

static ULONG    sntp_unix_base = 0;
static ULONG    sntp_hal_base  = 0;
static UINT     sntp_ready     = 0;

/* -------------------------------------------------------------------------- */
/* Fonctions pont I2C pour les drivers ST                                     */
/* -------------------------------------------------------------------------- */
static int32_t hts221_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len) {
    return (HAL_I2C_Mem_Write((I2C_HandleTypeDef*)handle, HTS221_I2C_ADDRESS,
                               reg, I2C_MEMADD_SIZE_8BIT,
                               (uint8_t*)bufp, len, 1000) == HAL_OK) ? 0 : -1;
}
static int32_t hts221_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len) {
    reg |= 0x80; /* Auto-increment obligatoire HTS221 */
    return (HAL_I2C_Mem_Read((I2C_HandleTypeDef*)handle, HTS221_I2C_ADDRESS,
                              reg, I2C_MEMADD_SIZE_8BIT,
                              bufp, len, 1000) == HAL_OK) ? 0 : -1;
}
static int32_t lps22hh_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len) {
    return (HAL_I2C_Mem_Write((I2C_HandleTypeDef*)handle, LPS22HH_I2C_ADD_H,
                               reg, I2C_MEMADD_SIZE_8BIT,
                               (uint8_t*)bufp, len, 1000) == HAL_OK) ? 0 : -1;
}
static int32_t lps22hh_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len) {
    return (HAL_I2C_Mem_Read((I2C_HandleTypeDef*)handle, LPS22HH_I2C_ADD_H,
                              reg, I2C_MEMADD_SIZE_8BIT,
                              bufp, len, 1000) == HAL_OK) ? 0 : -1;
}

/* -------------------------------------------------------------------------- */
/* Private function prototypes                                                 */
/* -------------------------------------------------------------------------- */
static VOID nx_app_thread_entry(ULONG thread_input);
static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr);
static VOID App_TCP_Thread_Entry(ULONG thread_input);
static VOID App_Link_Thread_Entry(ULONG thread_input);

/* -------------------------------------------------------------------------- */
static ULONG get_unix_time(void)
{
  if (!sntp_ready) return 0;
  return sntp_unix_base + (HAL_GetTick() - sntp_hal_base) / 1000;
}

static void unix_to_datetime(ULONG unix_ts,
                              int *year, int *month, int *day,
                              int *hour, int *min, int *sec)
{
  *sec   = (int)(unix_ts % 60); unix_ts /= 60;
  *min   = (int)(unix_ts % 60); unix_ts /= 60;
  *hour  = (int)(unix_ts % 24); unix_ts /= 24;
  ULONG jd = unix_ts + 2440588UL;
  ULONG p  = jd + 68569UL;
  ULONG q  = 4UL * p / 146097UL;
  p = p - (146097UL * q + 3UL) / 4UL;
  ULONG r = 4000UL * (p + 1UL) / 1461001UL;
  p = p - 1461UL * r / 4UL + 31UL;
  ULONG s = 80UL * p / 2447UL;
  *day   = (int)(p - 2447UL * s / 80UL);
  *month = (int)(s + 2UL - 12UL * (s / 11UL));
  *year  = (int)(100UL * (q - 49UL) + r + s / 11UL);
}

static int is_dst_france(int year, int month, int day, int hour)
{
  int lsm, lso;
  { int y=year,m=3,d=31; if(m<3){m+=12;y--;} int k=y%100,j=y/100;
    int dow=(d+(13*(m+1))/5+k+k/4+j/4-2*j)%7; dow=((dow%7)+7)%7; dow=(dow+6)%7;
    lsm=31-dow; }
  { int y=year,m=10,d=31; if(m<3){m+=12;y--;} int k=y%100,j=y/100;
    int dow=(d+(13*(m+1))/5+k+k/4+j/4-2*j)%7; dow=((dow%7)+7)%7; dow=(dow+6)%7;
    lso=31-dow; }
  if (month<3||month>10) return 0;
  if (month>3&&month<10) return 1;
  if (month==3)  return (day>lsm||(day==lsm&&hour>=2))?1:0;
  if (month==10) return (day<lso||(day==lso&&hour<3))?1:0;
  return 0;
}

static UINT mqtt_connect(void)
{
  ULONG broker_ip = 0;
  if (nx_dns_host_by_name_get(&DnsClient, (UCHAR *)BROKER_HOSTNAME,
                               &broker_ip, NX_IP_PERIODIC_RATE * 10) != NX_SUCCESS) {
    printf("!! DNS broker echoue\r\n"); return 0;
  }
  NXD_ADDRESS broker_addr;
  broker_addr.nxd_ip_version    = NX_IP_VERSION_V4;
  broker_addr.nxd_ip_address.v4 = broker_ip;
  nxd_mqtt_client_login_set(&MqttClient, MQTT_USER, strlen(MQTT_USER),
                             MQTT_PASS, strlen(MQTT_PASS));
  if (nxd_mqtt_client_connect(&MqttClient, &broker_addr, MQTT_PORT,
                               300, NX_FALSE, NX_IP_PERIODIC_RATE * 10) == NX_SUCCESS) {
    printf(">> MQTT connecte a %s:%d\r\n", BROKER_HOSTNAME, MQTT_PORT);
    return 1;
  }
  printf("!! MQTT connexion echouee\r\n"); return 0;
}

/* -------------------------------------------------------------------------- */
UINT MX_NetXDuo_Init(VOID *memory_ptr)
{
  UINT ret = NX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  CHAR *pointer;

  nx_system_initialize();

  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_APP_PACKET_POOL_SIZE, TX_NO_WAIT) != TX_SUCCESS)
    return TX_POOL_ERROR;
  ret = nx_packet_pool_create(&NxAppPool, "NetXDuo App Pool", DEFAULT_PAYLOAD_SIZE,
                               pointer, NX_APP_PACKET_POOL_SIZE);
  if (ret != NX_SUCCESS) return NX_POOL_ERROR;

  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, Nx_IP_INSTANCE_THREAD_SIZE, TX_NO_WAIT) != TX_SUCCESS)
    return TX_POOL_ERROR;
  ret = nx_ip_create(&NetXDuoEthIpInstance, "NetX Ip instance",
                     NX_APP_DEFAULT_IP_ADDRESS, NX_APP_DEFAULT_NET_MASK,
                     &NxAppPool, nx_stm32_eth_driver,
                     pointer, Nx_IP_INSTANCE_THREAD_SIZE, NX_APP_INSTANCE_PRIORITY);
  if (ret != NX_SUCCESS) return NX_NOT_SUCCESSFUL;

  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, DEFAULT_ARP_CACHE_SIZE, TX_NO_WAIT) != TX_SUCCESS)
    return TX_POOL_ERROR;
  ret = nx_arp_enable(&NetXDuoEthIpInstance, (VOID *)pointer, DEFAULT_ARP_CACHE_SIZE);
  if (ret != NX_SUCCESS) return NX_NOT_SUCCESSFUL;

  ret = nx_icmp_enable(&NetXDuoEthIpInstance); if (ret != NX_SUCCESS) return NX_NOT_SUCCESSFUL;
  ret = nx_tcp_enable(&NetXDuoEthIpInstance);  if (ret != NX_SUCCESS) return NX_NOT_SUCCESSFUL;
  ret = nx_udp_enable(&NetXDuoEthIpInstance);  if (ret != NX_SUCCESS) return NX_NOT_SUCCESSFUL;

  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, 4096, TX_NO_WAIT) != TX_SUCCESS)
    return TX_POOL_ERROR;
  ret = tx_thread_create(&NxAppThread, "NetXDuo App thread",
                         nx_app_thread_entry, 0, pointer, 4096,
                         NX_APP_THREAD_PRIORITY, NX_APP_THREAD_PRIORITY,
                         TX_NO_TIME_SLICE, TX_AUTO_START);
  if (ret != TX_SUCCESS) return TX_THREAD_ERROR;

  ret = nx_dhcp_create(&DHCPClient, &NetXDuoEthIpInstance, "DHCP Client");
  if (ret != NX_SUCCESS) return NX_DHCP_ERROR;
  ret = tx_semaphore_create(&DHCPSemaphore, "DHCP Semaphore", 0);
  if (ret != NX_SUCCESS) return NX_DHCP_ERROR;

  /* Thread IA + capteurs (16 Ko — calibration + inférence + MQTT) */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, 16384, TX_NO_WAIT) != TX_SUCCESS)
    return TX_POOL_ERROR;
  tx_thread_create(&AppTCPThread, "App IA Thread",
                   App_TCP_Thread_Entry, 0, pointer, 16384,
                   NX_APP_THREAD_PRIORITY, NX_APP_THREAD_PRIORITY,
                   TX_NO_TIME_SLICE, TX_DONT_START);

  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, 2048, TX_NO_WAIT) != TX_SUCCESS)
    return TX_POOL_ERROR;
  tx_thread_create(&AppLinkThread, "App Link Thread",
                   App_Link_Thread_Entry, 0, pointer, 2048,
                   LINK_PRIORITY, LINK_PRIORITY,
                   TX_NO_TIME_SLICE, TX_AUTO_START);

  return ret;
}

/* -------------------------------------------------------------------------- */
static VOID nx_app_thread_entry(ULONG thread_input)
{
  UINT  ret;
  ULONG actual_status;

  nx_ip_address_change_notify(&NetXDuoEthIpInstance,
                               ip_address_change_notify_callback, NULL);

  printf(">> Attente lien Ethernet...\r\n");
  ret = nx_ip_interface_status_check(&NetXDuoEthIpInstance, 0,
                                      NX_IP_LINK_ENABLED, &actual_status,
                                      NX_IP_PERIODIC_RATE * 10);
  if (ret != NX_SUCCESS) {
    printf("!! Lien ETH absent - mode IA sans reseau\r\n");
    tx_thread_resume(&AppTCPThread); return;
  }

  ULONG available = 0, total = 0;
  nx_packet_pool_info_get(&NxAppPool, &total, &available, NX_NULL, NX_NULL, NX_NULL);
  printf(">> Pool: %lu/%lu paquets\r\n", available, total);

  printf(">> Demarrage DHCP...\r\n");
  nx_dhcp_start(&DHCPClient);
  printf(">> DHCP started, waiting semaphore...\r\n");

  ret = tx_semaphore_get(&DHCPSemaphore, DHCP_TIMEOUT_TICKS);
  printf(">> semaphore ret = 0x%02X\r\n", ret);

  if (ret != TX_SUCCESS) {
    printf("!! DHCP timeout - mode IA sans reseau\r\n");
    tx_thread_resume(&AppTCPThread); return;
  }

  tx_thread_sleep(50);
  nx_ip_address_get(&NetXDuoEthIpInstance, &IpAddress, &NetMask);
  printf(">> IP: %lu.%lu.%lu.%lu\r\n",
         (IpAddress>>24)&0xFF,(IpAddress>>16)&0xFF,
         (IpAddress>> 8)&0xFF, IpAddress     &0xFF);

  ret = nx_dns_create(&DnsClient, &NetXDuoEthIpInstance, (UCHAR *)"DNS Client");
  if (ret == NX_SUCCESS) {
    ULONG gw = (IpAddress & 0xFFFFFF00) | 0x01;
    nx_dns_server_add(&DnsClient, gw);
    nx_dns_server_add(&DnsClient, IP_ADDRESS(8, 8, 8, 8));
    printf(">> DNS initialise\r\n");
  }

  ret = nx_sntp_client_create(&SntpClient, &NetXDuoEthIpInstance, 0,
                               &NxAppPool, NULL, NULL, NULL);
  if (ret == NX_SUCCESS) {
    nx_sntp_client_initialize_unicast(&SntpClient, SNTP_SERVER_IP);
    nx_sntp_client_set_local_time(&SntpClient, 0, 0);
    nx_sntp_client_run_unicast(&SntpClient);
    for (UINT i = 0; i < 20; i++) {
      ULONG ntp_raw = 0, ms = 0;
      if (nx_sntp_client_get_local_time(&SntpClient, &ntp_raw, &ms, NX_NULL) == NX_SUCCESS
          && ntp_raw > 0) {
        sntp_unix_base = ntp_raw - NTP_TO_UNIX_OFFSET;
        sntp_hal_base  = HAL_GetTick();
        sntp_ready     = 1;
        int y,mo,d,h,mi,s;
        unix_to_datetime(sntp_unix_base,&y,&mo,&d,&h,&mi,&s);
        h = (h + 1 + is_dst_france(y,mo,d,h)) % 24;
        printf(">> SNTP OK: %02d/%02d/%04d %02d:%02d:%02d (Paris)\r\n",d,mo,y,h,mi,s);
        break;
      }
      tx_thread_sleep(50);
    }
    if (!sntp_ready) printf("!! SNTP non sync\r\n");
  }

  printf(">> Reseau pret - demarrage IA\r\n");
  tx_thread_resume(&AppTCPThread);
}

/* -------------------------------------------------------------------------- */
/**
  * @brief  Thread IA : init capteurs → calibration → inférence + MQTT.
  */
static VOID App_TCP_Thread_Entry(ULONG thread_input)
{
  tx_thread_sleep(200); /* stabilisation matériel */

  printf("\r\n--- CPU Inference + Capteurs + MQTT ---\r\n");

  /* ------------------------------------------------------------------ */
  /* Init capteurs I2C                                                   */
  /* ------------------------------------------------------------------ */
  stmdev_ctx_t dev_ctx_hts221;
  stmdev_ctx_t dev_ctx_lps22hh;

  dev_ctx_hts221.write_reg  = hts221_write;
  dev_ctx_hts221.read_reg   = hts221_read;
  dev_ctx_hts221.handle     = (void*)&hi2c1;

  dev_ctx_lps22hh.write_reg = lps22hh_write;
  dev_ctx_lps22hh.read_reg  = lps22hh_read;
  dev_ctx_lps22hh.handle    = (void*)&hi2c1;

  /* Vérification présence */
  uint8_t id_hts = 0, id_lps = 0;
  hts221_device_id_get(&dev_ctx_hts221, &id_hts);
  lps22hh_device_id_get(&dev_ctx_lps22hh, &id_lps);

  if (id_hts == HTS221_ID)  printf("HTS221  OK (ID: %02X)\r\n", id_hts);
  else                       printf("!! HTS221  KO (ID: %02X)\r\n", id_hts);
  if (id_lps == LPS22HH_ID) printf("LPS22HH OK (ID: %02X)\r\n", id_lps);
  else                       printf("!! LPS22HH KO (ID: %02X)\r\n", id_lps);

  /* Configuration LPS22HH */
  lps22hh_block_data_update_set(&dev_ctx_lps22hh, PROPERTY_ENABLE);
  lps22hh_data_rate_set(&dev_ctx_lps22hh, LPS22HH_1_Hz_LOW_NOISE);

  /* Configuration HTS221 */
  hts221_block_data_update_set(&dev_ctx_hts221, PROPERTY_ENABLE);
  hts221_power_on_set(&dev_ctx_hts221, PROPERTY_ENABLE);
  hts221_data_rate_set(&dev_ctx_hts221, HTS221_ODR_1Hz);

  /* Calibration HTS221 */
  float T0_degC, T1_degC, H0_rh, H1_rh;
  int16_t T0_out, T1_out, H0_T0_out, H1_T0_out;
  uint8_t b0, b1, t0_t1_msb;

  hts221_read_reg(&dev_ctx_hts221, 0x30, &b0, 1); H0_rh = b0 / 2.0f;
  hts221_read_reg(&dev_ctx_hts221, 0x31, &b0, 1); H1_rh = b0 / 2.0f;
  hts221_read_reg(&dev_ctx_hts221, 0x32, &b0, 1);
  hts221_read_reg(&dev_ctx_hts221, 0x33, &b1, 1);
  hts221_read_reg(&dev_ctx_hts221, 0x35, &t0_t1_msb, 1);
  T0_degC = (float)(b0 | ((t0_t1_msb & 0x03) << 8)) / 8.0f;
  T1_degC = (float)(b1 | ((t0_t1_msb & 0x0C) << 6)) / 8.0f;
  hts221_read_reg(&dev_ctx_hts221, 0x36, &b0, 1); hts221_read_reg(&dev_ctx_hts221, 0x37, &b1, 1);
  H0_T0_out = (int16_t)(((uint16_t)b1 << 8) | b0);
  hts221_read_reg(&dev_ctx_hts221, 0x3A, &b0, 1); hts221_read_reg(&dev_ctx_hts221, 0x3B, &b1, 1);
  H1_T0_out = (int16_t)(((uint16_t)b1 << 8) | b0);
  hts221_read_reg(&dev_ctx_hts221, 0x3C, &b0, 1); hts221_read_reg(&dev_ctx_hts221, 0x3D, &b1, 1);
  T0_out = (int16_t)(((uint16_t)b1 << 8) | b0);
  hts221_read_reg(&dev_ctx_hts221, 0x3E, &b0, 1); hts221_read_reg(&dev_ctx_hts221, 0x3F, &b1, 1);
  T1_out = (int16_t)(((uint16_t)b1 << 8) | b0);
  printf("Calibration HTS221 OK\r\n");

  /* ------------------------------------------------------------------ */
  /* Init IA                                                             */
  /* ------------------------------------------------------------------ */
  MX_X_CUBE_AI_Init();
  printf("Moteur IA initialise\r\n");

  /* ------------------------------------------------------------------ */
  /* Init MQTT                                                           */
  /* ------------------------------------------------------------------ */
  UINT mqtt_connected = 0;
  if (IpAddress != 0) {
    UINT ret = nxd_mqtt_client_create(&MqttClient, "MQTT Client",
                                       MQTT_CLIENT_ID, strlen(MQTT_CLIENT_ID),
                                       &NetXDuoEthIpInstance, &NxAppPool,
                                       (VOID *)mqtt_client_stack,
                                       sizeof(mqtt_client_stack),
                                       NX_APP_THREAD_PRIORITY, NX_NULL, 0);
    if (ret == NX_SUCCESS)
      mqtt_connected = mqtt_connect();
  } else {
    printf(">> Pas d'IP - MQTT desactive\r\n");
  }

  /* ------------------------------------------------------------------ */
  /* Paramètres quantification                                           */
  /* ------------------------------------------------------------------ */
  const float  input_scale      =  0.027173925f;
  const int8_t input_zeropoint  =  23;
  const float  output_scale     =  0.00390625f;
  const int8_t output_zeropoint = -128;

  const float mean[8] = { 12.96f,  7.53f,  72.87f, 1017.13f,
                           -0.001f,  0.001f, -0.003f,  0.009f };
  const float std[8]  = {  7.18f,  5.68f,  17.99f,    9.44f,
                            0.707f,  0.707f,  0.708f,  0.705f };

  const char *labels[5] = { "Degage", "Nuageux", "Pluie", "Brouillard", "Orage" };

  ai_i8 *ai_in  = (ai_i8 *)data_ins[0];
  ai_i8 *ai_out = (ai_i8 *)data_outs[0];

  /* ------------------------------------------------------------------ */
  /* Boucle principale                                                   */
  /* ------------------------------------------------------------------ */
  while (1)
  {
    /* 1. Lecture capteurs */
    float temp = 0.0f, hum = 0.0f, pres = 0.0f;

    uint8_t lps_status = 0, hts_status = 0;
    uint32_t raw_pressure = 0;
    int16_t  raw_temperature = 0, raw_humidity = 0;

    lps22hh_read_reg(&dev_ctx_lps22hh, LPS22HH_STATUS, &lps_status, 1);
    if (lps_status & 0x01) {
      lps22hh_pressure_raw_get(&dev_ctx_lps22hh, &raw_pressure);
      pres = lps22hh_from_lsb_to_hpa(raw_pressure);
    }

    hts221_read_reg(&dev_ctx_hts221, HTS221_STATUS_REG, &hts_status, 1);
    if (hts_status & 0x02) {
      hts221_temperature_raw_get(&dev_ctx_hts221, &raw_temperature);
      temp = (raw_temperature - T0_out) * (T1_degC - T0_degC)
             / (float)(T1_out - T0_out) + T0_degC;
    }
    if (hts_status & 0x01) {
      hts221_humidity_raw_get(&dev_ctx_hts221, &raw_humidity);
      hum = (raw_humidity - H0_T0_out) * (H1_rh - H0_rh)
            / (float)(H1_T0_out - H0_T0_out) + H0_rh;
    }

    /* 2. Timestamp + features sin/cos */
    ULONG unix_now = get_unix_time();

    float heure_sin = 0.5f, heure_cos = 0.8f;
    float mois_sin  = 0.1f, mois_cos  = -0.2f;

    if (sntp_ready && unix_now > 0) {
      int y, mo, d, h, mi, s;
      unix_to_datetime(unix_now, &y, &mo, &d, &h, &mi, &s);
      h = (h + 1 + is_dst_france(y, mo, d, h)) % 24;
      heure_sin = sinf(2.0f * (float)M_PI * (float)h  / 24.0f);
      heure_cos = cosf(2.0f * (float)M_PI * (float)h  / 24.0f);
      mois_sin  = sinf(2.0f * (float)M_PI * (float)mo / 12.0f);
      mois_cos  = cosf(2.0f * (float)M_PI * (float)mo / 12.0f);
    }

    /* 3. Point de rosée estimé */
    float dwpt = temp - ((100.0f - hum) / 5.0f);

    float raw[8] = { temp, dwpt, hum, pres,
                     heure_sin, heure_cos, mois_sin, mois_cos };

    /* 4. Normalisation + quantification */
    memset(ai_in, 0, 8);
    for (int i = 0; i < 8; i++) {
      float normalized = (raw[i] - mean[i]) / std[i];
      int32_t q = (int32_t)roundf(normalized / input_scale) + input_zeropoint;
      if (q >  127) q =  127;
      if (q < -128) q = -128;
      ai_in[i] = (int8_t)q;
    }

    /* 5. Inférence CPU */
    int res = ai_run();

    if (res == 0) {
      int   best = 0;
      float max_p = -1.0f, sum = 0.0f;
      float probas[5];
      for (int i = 0; i < 5; i++) {
        probas[i] = ((float)ai_out[i] - (float)output_zeropoint) * output_scale;
        sum += probas[i];
        if (probas[i] > max_p) { max_p = probas[i]; best = i; }
      }
      int pct = (sum > 0.0f) ? (int)((max_p / sum) * 100.0f) : 0;

      /* Affichage console */
      printf("[%ldC %ld%% %ldhPa] >> %s (%d%%)\r\n",
             (long)temp, (long)hum, (long)pres, labels[best], pct);

      /* 6. Publication MQTT */
      if (mqtt_connected) {
        char payload[128];
        int  plen;
        if (sntp_ready && unix_now > 0) {
          plen = snprintf(payload, sizeof(payload),
              "{\"ts\":%lu,\"meteo\":\"%s\",\"proba\":%d,"
              "\"temp\":%d,\"hum\":%d,\"pres\":%d}",
              unix_now, labels[best], pct,
              (int)temp, (int)hum, (int)pres);
        } else {
          plen = snprintf(payload, sizeof(payload),
              "{\"meteo\":\"%s\",\"proba\":%d,"
              "\"temp\":%d,\"hum\":%d,\"pres\":%d}",
              labels[best], pct,
              (int)temp, (int)hum, (int)pres);
        }
        UINT mret = nxd_mqtt_client_publish(&MqttClient,
                                             MQTT_TOPIC, strlen(MQTT_TOPIC),
                                             payload, plen,
                                             NX_FALSE, 1,
                                             NX_IP_PERIODIC_RATE * 10);
        if (mret == NX_SUCCESS) {
          printf(">> MQTT: %s\r\n", payload);
        } else {
          printf("!! MQTT erreur (0x%02X) - reconnexion...\r\n", mret);
          mqtt_connected = 0;
          nxd_mqtt_client_disconnect(&MqttClient);
          tx_thread_sleep(100);
          mqtt_connected = mqtt_connect();
        }
      }
    } else {
      printf("!! Erreur inference: %d\r\n", res);
    }

    tx_thread_sleep(200); /* 2 secondes */
  }
}

/* -------------------------------------------------------------------------- */
static VOID App_Link_Thread_Entry(ULONG thread_input)
{
  ULONG status;
  ULONG old_status = 0;
  while (1) {
    nx_ip_interface_status_check(&NetXDuoEthIpInstance, 0,
                                  NX_IP_LINK_ENABLED, &status, 10);
    if (status != old_status) {
      printf(">> Link status changed: %lu\r\n", status);
      old_status = status;
    }
    /* Stats paquets */
    printf(">> RX:%lu TX:%lu Pool:%lu/%lu\r\n",
           NetXDuoEthIpInstance.nx_ip_receive_packets_dropped,
           NetXDuoEthIpInstance.nx_ip_total_packets_sent,
           NxAppPool.nx_packet_pool_available,
           NxAppPool.nx_packet_pool_total);
    tx_thread_sleep(100);
  }
}

/* -------------------------------------------------------------------------- */
static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr)
{
  printf(">> IP callback!\r\n");  // ajoute cette ligne
  ULONG ip   = 0;
  ULONG mask = 0;
  nx_ip_address_get(ip_instance, &ip, &mask);
  if (ip != 0) {
    tx_semaphore_put(&DHCPSemaphore);
  }
  (void)ptr;
}
