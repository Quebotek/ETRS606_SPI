/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_netxduo.c
  * @brief   NetXDuo applicative file (DNS + MQTT Fixed Version)
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
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BROKER_HOSTNAME "meteo.quebotek.fr"
#define MQTT_PORT       18830
#define MQTT_CLIENT_ID  "STM32_NUCLEO_METEO"
#define MQTT_TOPIC      "meteo/temperature"
#define MQTT_MSG        "24.5"

#define MQTT_USER       "albert"
#define MQTT_PASS       "pass123"
/* USER CODE END PD */

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
ULONG           mqtt_client_stack[1024]; /* Stack de 4KB */

ULONG          IpAddress;
ULONG          NetMask;
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
  */
UINT MX_NetXDuo_Init(VOID *memory_ptr)
{
  UINT ret = NX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  CHAR *pointer;

  nx_system_initialize();

  /* Allocation Packet Pool */
  tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_APP_PACKET_POOL_SIZE, TX_NO_WAIT);
  nx_packet_pool_create(&NxAppPool, "NetXDuo App Pool", DEFAULT_PAYLOAD_SIZE, pointer, NX_APP_PACKET_POOL_SIZE);

  /* Allocation IP Instance */
  tx_byte_allocate(byte_pool, (VOID **) &pointer, Nx_IP_INSTANCE_THREAD_SIZE, TX_NO_WAIT);
  nx_ip_create(&NetXDuoEthIpInstance, "NetX Ip instance", NX_APP_DEFAULT_IP_ADDRESS, NX_APP_DEFAULT_NET_MASK, &NxAppPool, nx_stm32_eth_driver,
                     pointer, Nx_IP_INSTANCE_THREAD_SIZE, NX_APP_INSTANCE_PRIORITY);

  /* Activation des protocoles */
  tx_byte_allocate(byte_pool, (VOID **) &pointer, DEFAULT_ARP_CACHE_SIZE, TX_NO_WAIT);
  nx_arp_enable(&NetXDuoEthIpInstance, (VOID *)pointer, DEFAULT_ARP_CACHE_SIZE);
  nx_icmp_enable(&NetXDuoEthIpInstance);
  nx_tcp_enable(&NetXDuoEthIpInstance);
  nx_udp_enable(&NetXDuoEthIpInstance);

  /* Main App Thread */
  tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_APP_THREAD_STACK_SIZE, TX_NO_WAIT);
  tx_thread_create(&NxAppThread, "NetXDuo App thread", nx_app_thread_entry, 0, pointer, NX_APP_THREAD_STACK_SIZE,
                         NX_APP_THREAD_PRIORITY, NX_APP_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

  /* DHCP */
  nx_dhcp_create(&DHCPClient, &NetXDuoEthIpInstance, "DHCP Client");
  tx_semaphore_create(&DHCPSemaphore, "DHCP Semaphore", 0);

  /* USER CODE BEGIN MX_NetXDuo_Init */
  /* Création du Thread MQTT */
  tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_APP_THREAD_STACK_SIZE, TX_NO_WAIT);
  tx_thread_create(&AppTCPThread, "App MQTT Thread", App_TCP_Thread_Entry, 0, pointer, NX_APP_THREAD_STACK_SIZE,
                   NX_APP_THREAD_PRIORITY, NX_APP_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_DONT_START);

  /* Création du Link Thread */
  tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_APP_THREAD_STACK_SIZE, TX_NO_WAIT);
  tx_thread_create(&AppLinkThread, "App Link Thread", App_Link_Thread_Entry, 0, pointer, NX_APP_THREAD_STACK_SIZE,
                   LINK_PRIORITY, LINK_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);
  /* USER CODE END MX_NetXDuo_Init */

  return ret;
}

static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr)
{
  nx_ip_address_get(&NetXDuoEthIpInstance, &IpAddress, &NetMask);
  if (IpAddress != 0) tx_semaphore_put(&DHCPSemaphore);
}

static VOID nx_app_thread_entry (ULONG thread_input)
{
  nx_ip_address_change_notify(&NetXDuoEthIpInstance, ip_address_change_notify_callback, NULL);
  nx_dhcp_start(&DHCPClient);
  printf("Recherche DHCP...\n");

  if(tx_semaphore_get(&DHCPSemaphore, TX_WAIT_FOREVER) == TX_SUCCESS)
  {
    PRINT_IP_ADDRESS(IpAddress);
    tx_thread_resume(&AppTCPThread);
  }
}

/* USER CODE BEGIN 1 */
static VOID App_TCP_Thread_Entry(ULONG thread_input)
{
  UINT ret;
  ULONG dns_server_address[3];
  UINT dns_server_size = sizeof(dns_server_address);
  NXD_ADDRESS broker_ip;
  extern UART_HandleTypeDef huart1;

  tx_thread_sleep(100);
  printf("\n--- Sequence DNS & MQTT lancee ---\n");

  /* 1. DNS : Creation avec underscore pour le Linker */
  ret = _nxe_dns_create(&DnsClient, &NetXDuoEthIpInstance, (UCHAR *)"DNS Client");

  if (nx_dhcp_user_option_retrieve(&DHCPClient, NX_DHCP_OPTION_DNS_SVR, (UCHAR *)(dns_server_address), &dns_server_size) == NX_SUCCESS) {
      _nxe_dns_server_add(&DnsClient, dns_server_address[0]);
  } else {
      _nxe_dns_server_add(&DnsClient, IP_ADDRESS(8, 8, 8, 8));
  }

  /* 2. Resolution DNS */
  printf("Resolution de %s...\n", BROKER_HOSTNAME);
  ret = _nxe_dns_host_by_name_get(&DnsClient, (UCHAR *)BROKER_HOSTNAME, &broker_ip.nxd_ip_address.v4, 500);
  broker_ip.nxd_ip_version = NX_IP_VERSION_V4;

  if (ret != NX_SUCCESS) {
      printf("❌ Echec DNS (0x%02x)\n", ret);
      return;
  }
  printf("IP trouvee: %lu.%lu.%lu.%lu\n", (broker_ip.nxd_ip_address.v4 >> 24) & 0xFF, (broker_ip.nxd_ip_address.v4 >> 16) & 0xFF, (broker_ip.nxd_ip_address.v4 >> 8) & 0xFF, (broker_ip.nxd_ip_address.v4) & 0xFF);

  /* 3. MQTT : Creation */
    _nxde_mqtt_client_create(&my_mqtt_client, "STM32_Client", MQTT_CLIENT_ID, strlen(MQTT_CLIENT_ID),
                             &NetXDuoEthIpInstance, &NxAppPool, (VOID*)mqtt_client_stack, sizeof(mqtt_client_stack),
                             2, NX_NULL, 0);

    /* --- NOUVEAU : Configuration de l'authentification --- */
    ret = nxd_mqtt_client_login_set(&my_mqtt_client,
                                    MQTT_USER, strlen(MQTT_USER),
                                    MQTT_PASS, strlen(MQTT_PASS));
    if (ret != NX_SUCCESS) {
        printf("❌ Erreur login_set (0x%02x)\n", ret);
    }
    /* ------------------------------------------------------ */

    /* 4. MQTT : Connexion */
    printf("Connexion Broker avec authentification...\n");
    ret = _nxde_mqtt_client_connect(&my_mqtt_client, &broker_ip, MQTT_PORT, 50, NX_TRUE, NX_WAIT_FOREVER);

  /* 5. Boucle de Publication */
  while(1)
  {
    printf("Envoi Temperature: %s...\n", MQTT_MSG);
    ret = _nxde_mqtt_client_publish(&my_mqtt_client, MQTT_TOPIC, strlen(MQTT_TOPIC), (CHAR*)MQTT_MSG, strlen(MQTT_MSG), NX_TRUE, 0, NX_WAIT_FOREVER);

    if (ret == NX_SUCCESS) {
        HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
    } else {
        printf("⚠️ Erreur envoi (0x%02x)\n", ret);
    }
    tx_thread_sleep(500); /* Toutes les 5s */
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
/* USER CODE END 1 */
