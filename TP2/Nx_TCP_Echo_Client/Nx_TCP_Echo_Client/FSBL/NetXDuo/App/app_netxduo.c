/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_netxduo.c
  * @brief   NetXDuo + AI NPU Fusion (DNS + MQTT + LL_ATON Inférence)
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_netxduo.h"

/* Private includes ----------------------------------------------------------*/
#include "nxd_dhcp_client.h"
/* USER CODE BEGIN Includes */
#include "nxd_dns.h"
#include "nxd_mqtt_client.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

/* --- INCLUDES IA (LL_ATON) --- */
#include "ll_aton.h"
#include "network.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 #define BROKER_HOSTNAME "meteo.quebotek.fr"
#define MQTT_PORT 18830
#define MQTT_CLIENT_ID "STM32_NUCLEO_METEO"

#define MQTT_TOPIC "meteo/data"
#define MQTT_USER "albert"
#define MQTT_PASS "pass123"
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TX_THREAD      NxAppThread;
NX_PACKET_POOL NxAppPool;
NX_IP          NetXDuoEthIpInstance;
TX_SEMAPHORE   DHCPSemaphore;
NX_DHCP        DHCPClient;

/* USER CODE BEGIN PV */
TX_THREAD AppTCPThread;
TX_THREAD AppLinkThread;

/* Objets DNS et MQTT */
NX_DNS          DnsClient;
NXD_MQTT_CLIENT my_mqtt_client;
ULONG           mqtt_client_stack[1024];

ULONG          IpAddress;
ULONG          NetMask;

/* --- VARIABLES D'ENTRÉE/SORTIE POUR L'IA --- */
// On alloue des tableaux aux tailles précises définies dans network.h
static float in_data[LL_ATON_DEFAULT_IN_1_SIZE_BYTES / sizeof(float)];
static float out_data[LL_ATON_DEFAULT_OUT_1_SIZE_BYTES / sizeof(float)];

/* --- PROTOTYPES IA LL_ATON --- */
extern int LL_ATON_Set_User_Input_Buffer_Default(unsigned int num, void* buffer, unsigned int size);
extern int LL_ATON_Set_User_Output_Buffer_Default(unsigned int num, void* buffer, unsigned int size);
extern int LL_ATON_EC_Network_Init_Default(void);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static VOID nx_app_thread_entry (ULONG thread_input);
static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr);
/* USER CODE BEGIN PFP */
static VOID App_TCP_Thread_Entry(ULONG thread_input);
static VOID App_Link_Thread_Entry(ULONG thread_input);
/* USER CODE END PFP */

/**
  * @brief  Application NetXDuo Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT MX_NetXDuo_Init(VOID *memory_ptr)
{
  UINT ret = NX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  CHAR *pointer;

  /* Initialize the NetXDuo system. */
  nx_system_initialize();

  /* Allocate the memory for packet_pool.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_APP_PACKET_POOL_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  ret = nx_packet_pool_create(&NxAppPool, "NetXDuo App Pool", DEFAULT_PAYLOAD_SIZE, pointer, NX_APP_PACKET_POOL_SIZE);
  if (ret != NX_SUCCESS) return NX_POOL_ERROR;

  /* Allocate the memory for Ip_Instance */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, Nx_IP_INSTANCE_THREAD_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  ret = nx_ip_create(&NetXDuoEthIpInstance, "NetX Ip instance", NX_APP_DEFAULT_IP_ADDRESS, NX_APP_DEFAULT_NET_MASK, &NxAppPool, nx_stm32_eth_driver,
                     pointer, Nx_IP_INSTANCE_THREAD_SIZE, NX_APP_INSTANCE_PRIORITY);
  if (ret != NX_SUCCESS) return NX_NOT_SUCCESSFUL;

  /* Allocate the memory for ARP */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, DEFAULT_ARP_CACHE_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  ret = nx_arp_enable(&NetXDuoEthIpInstance, (VOID *)pointer, DEFAULT_ARP_CACHE_SIZE);
  if (ret != NX_SUCCESS) return NX_NOT_SUCCESSFUL;

  ret = nx_icmp_enable(&NetXDuoEthIpInstance);
  if (ret != NX_SUCCESS) return NX_NOT_SUCCESSFUL;

  ret = nx_tcp_enable(&NetXDuoEthIpInstance);
  if (ret != NX_SUCCESS) return NX_NOT_SUCCESSFUL;

  ret = nx_udp_enable(&NetXDuoEthIpInstance);
  if (ret != NX_SUCCESS) return NX_NOT_SUCCESSFUL;

  /* Allocate the memory for main thread */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_APP_THREAD_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  ret = tx_thread_create(&NxAppThread, "NetXDuo App thread", nx_app_thread_entry , 0, pointer, NX_APP_THREAD_STACK_SIZE,
                         NX_APP_THREAD_PRIORITY, NX_APP_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);
  if (ret != TX_SUCCESS) return TX_THREAD_ERROR;

  ret = nx_dhcp_create(&DHCPClient, &NetXDuoEthIpInstance, "DHCP Client");
  if (ret != NX_SUCCESS) return NX_DHCP_ERROR;

  ret = tx_semaphore_create(&DHCPSemaphore, "DHCP Semaphore", 0);
  if (ret != NX_SUCCESS) return NX_DHCP_ERROR;

  /* USER CODE BEGIN MX_NetXDuo_Init */
  /* Thread MQTT & IA */
  tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_APP_THREAD_STACK_SIZE, TX_NO_WAIT);
  tx_thread_create(&AppTCPThread, "App MQTT Thread", App_TCP_Thread_Entry, 0, pointer, NX_APP_THREAD_STACK_SIZE,
                   NX_APP_THREAD_PRIORITY, NX_APP_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_DONT_START);

  /* Link Thread */
  tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_APP_THREAD_STACK_SIZE, TX_NO_WAIT);
  tx_thread_create(&AppLinkThread, "App Link Thread", App_Link_Thread_Entry, 0, pointer, NX_APP_THREAD_STACK_SIZE,
                   LINK_PRIORITY, LINK_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);
  /* USER CODE END MX_NetXDuo_Init */

  return ret;
}

/**
* @brief  Main thread entry.
*/
static VOID nx_app_thread_entry (ULONG thread_input)
{
  UINT ret = NX_SUCCESS;

  /* register the IP address change callback */
  nx_ip_address_change_notify(&NetXDuoEthIpInstance, ip_address_change_notify_callback, NULL);

  /* start the DHCP client */
  ret = nx_dhcp_start(&DHCPClient);
  if (ret != NX_SUCCESS)
  {
    printf("❌ Erreur DHCP Start\n");
  }

  printf("Looking for DHCP server...\n");

  /* wait until an IP address is ready */
  if(tx_semaphore_get(&DHCPSemaphore, TX_WAIT_FOREVER) == TX_SUCCESS)
  {
      tx_thread_resume(&AppTCPThread); // Lance la suite une fois l'IP obtenue
  }
}

/* USER CODE BEGIN 1 */
static VOID App_TCP_Thread_Entry(ULONG thread_input)
{
  UINT ret;
  ULONG dns_server_address[3];
  UINT dns_server_size = sizeof(dns_server_address);
  NXD_ADDRESS broker_ip;

  tx_thread_sleep(100);
  printf("\n--- Demarrage Services Réseau & IA (LL_ATON) ---\n");

  /* 1. DNS */
  ret = _nxe_dns_create(&DnsClient, &NetXDuoEthIpInstance, (UCHAR *)"DNS Client");
  if (nx_dhcp_user_option_retrieve(&DHCPClient, NX_DHCP_OPTION_DNS_SVR, (UCHAR *)(dns_server_address), &dns_server_size) == NX_SUCCESS) {
      _nxe_dns_server_add(&DnsClient, dns_server_address[0]);
  } else {
      _nxe_dns_server_add(&DnsClient, IP_ADDRESS(8, 8, 8, 8));
  }

  /* 2. Résolution DNS du Broker */
  printf("Resolution de %s...\n", BROKER_HOSTNAME);
  ret = _nxe_dns_host_by_name_get(&DnsClient, (UCHAR *)BROKER_HOSTNAME, &broker_ip.nxd_ip_address.v4, 500);
  broker_ip.nxd_ip_version = NX_IP_VERSION_V4;

  if (ret != NX_SUCCESS) {
      printf("❌ Echec DNS (0x%02x)\n", ret);
      return;
  }

  /* 3. MQTT : Configuration et Connexion */
  _nxde_mqtt_client_create(&my_mqtt_client, "STM32_Client", MQTT_CLIENT_ID, strlen(MQTT_CLIENT_ID),
                             &NetXDuoEthIpInstance, &NxAppPool, (VOID*)mqtt_client_stack, sizeof(mqtt_client_stack),
                             2, NX_NULL, 0);

  nxd_mqtt_client_login_set(&my_mqtt_client, MQTT_USER, strlen(MQTT_USER), MQTT_PASS, strlen(MQTT_PASS));

  printf("Connexion au broker MQTT...\n");
  ret = _nxde_mqtt_client_connect(&my_mqtt_client, &broker_ip, MQTT_PORT, 50, NX_TRUE, NX_WAIT_FOREVER);

  if (ret != NX_SUCCESS) {
      printf("❌ Echec MQTT (0x%02x)\n", ret);
      return;
  }
  printf("✅ MQTT Connecté !\n");

  /* 4. INITIALISATION DU NPU (LL_ATON) */
  printf("Initialisation de l'IA (LL_ATON)...\n");

  // Attribution de nos buffers (in_data et out_data) aux ports d'E/S du NPU
  LL_ATON_Set_User_Input_Buffer_Default(1, in_data, sizeof(in_data));
  LL_ATON_Set_User_Output_Buffer_Default(1, out_data, sizeof(out_data));

  printf("✅ IA LL_ATON prête !\n");

  /* 5. BOUCLE PRINCIPALE : LECTURE -> IA -> MQTT */
  while(1)
  {
    char mqtt_payload[150];

    /* A. Récupération des données brutes */
    float raw_data[8] = {24.5f, 55.0f, 1013.2f, 12.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    /* B. Normalisation (Z-Score) */
    float mean[8] = {20.0f, 50.0f, 1015.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float std[8]  = {5.0f,  15.0f,   10.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f};

    for(int i = 0; i < 8; i++) {
        in_data[i] = (raw_data[i] - mean[i]) / std[i];
    }

    /* C. Inférence NPU (LL_ATON) */
    int run_status = LL_ATON_EC_Network_Init_Default();

    if (run_status) {
        float prediction = out_data[0];

        /* D. Construction du JSON */
        int payload_len = snprintf(mqtt_payload, sizeof(mqtt_payload),
                                   "{\"dev\": \"%s\", \"temp\": %.1f, \"press\": %.1f, \"pred\": %.4f}",
                                   MQTT_CLIENT_ID, raw_data[0], raw_data[2], prediction);

        /* E. Publication MQTT */
        ret = _nxde_mqtt_client_publish(&my_mqtt_client, MQTT_TOPIC, strlen(MQTT_TOPIC),
                                        (CHAR*)mqtt_payload, payload_len,
                                        NX_TRUE, 0, NX_WAIT_FOREVER);

        if (ret == NX_SUCCESS) {
            printf("🚀 MQTT Send OK: %s\n", mqtt_payload);
            HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
        } else {
            printf("⚠️ Erreur envoi MQTT (0x%02x)\n", ret);
        }
    } else {
        printf("❌ Erreur execution LL_ATON\n");
    }

    tx_thread_sleep(500); // Attente 5 secondes
  }
}

static VOID App_Link_Thread_Entry(ULONG thread_input)
{
  ULONG status;
  while(1) {
    nx_ip_interface_status_check(&NetXDuoEthIpInstance, 0, NX_IP_LINK_ENABLED, &status, 10);
    tx_thread_sleep(100);
  }
}

/**
* @brief Callback déclenché par NetXDuo quand l'IP est reçue.
*/
static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr)
{
  nx_ip_address_get(ip_instance, &IpAddress, &NetMask);
  if (IpAddress != 0) {
      tx_semaphore_put(&DHCPSemaphore); // Libère le thread principal !
  }
}
/* USER CODE END 1 */
