/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body (FSBL Version)
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32n6xx_hal.h"
#include <stdio.h>
#include "hts221_reg.h"
#include "lps22hh_reg.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
COM_InitTypeDef BspCOMInit;
__IO uint32_t BspButtonState = BUTTON_RELEASED;

// On utilise I2C1 (celui du FSBL)
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ICACHE_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// --- Fonctions I2C (sans décalage car les macros ST sont déjà en 8-bits) ---
int32_t hts221_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len) {
    if (HAL_I2C_Mem_Write((I2C_HandleTypeDef*)handle, HTS221_I2C_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)bufp, len, 1000) == HAL_OK) return 0;
    return -1;
}
int32_t hts221_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len) {
    reg |= 0x80;

    if (HAL_I2C_Mem_Read((I2C_HandleTypeDef*)handle, HTS221_I2C_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, bufp, len, 1000) == HAL_OK) return 0;
    return -1;
}

int32_t lps22hh_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len) {
    if (HAL_I2C_Mem_Write((I2C_HandleTypeDef*)handle, LPS22HH_I2C_ADD_H, reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)bufp, len, 1000) == HAL_OK) return 0;
    return -1;
}
int32_t lps22hh_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len) {
    if (HAL_I2C_Mem_Read((I2C_HandleTypeDef*)handle, LPS22HH_I2C_ADD_H, reg, I2C_MEMADD_SIZE_8BIT, bufp, len, 1000) == HAL_OK) return 0;
    return -1;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ICACHE_Init();
  MX_I2C1_Init();

  /* Initialize COM1 port */
  BspCOMInit.BaudRate   = 115200;
  BspCOMInit.WordLength = COM_WORDLENGTH_8B;
  BspCOMInit.StopBits   = COM_STOPBITS_1;
  BspCOMInit.Parity     = COM_PARITY_NONE;
  BspCOMInit.HwFlowCtl  = COM_HWCONTROL_NONE;
  if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN 2 */

    // DELAI : Ouvre Putty maintenant !
    HAL_Delay(4000);

    printf("\r\n======================================\r\n");
    printf("--- Demarrage FSBL Capteurs + MQTT ---\r\n");
    printf("======================================\r\n");

    stmdev_ctx_t dev_ctx_hts221;
    stmdev_ctx_t dev_ctx_lps22hh;

    dev_ctx_hts221.write_reg = hts221_write;
    dev_ctx_hts221.read_reg  = hts221_read;
    dev_ctx_hts221.handle    = (void*)&hi2c1;

    dev_ctx_lps22hh.write_reg = lps22hh_write;
    dev_ctx_lps22hh.read_reg  = lps22hh_read;
    dev_ctx_lps22hh.handle    = (void*)&hi2c1;

    uint8_t id_hts = 0, id_lps = 0;

    printf("\r\n--- Initialisation des capteurs ---\r\n");

    // Test HTS221
    hts221_device_id_get(&dev_ctx_hts221, &id_hts);
    if (id_hts == HTS221_ID) printf("HTS221 OK! (ID: %02X)\r\n", id_hts);
    else printf("Erreur HTS221 (ID lu: %02X - Probleme I2C ?)\r\n", id_hts);

    // Test LPS22HH
    lps22hh_device_id_get(&dev_ctx_lps22hh, &id_lps);
    if (id_lps == LPS22HH_ID) printf("LPS22HH OK! (ID: %02X)\r\n", id_lps);
    else printf("Erreur LPS22HH (ID lu: %02X - Probleme I2C ?)\r\n", id_lps);

    /* --- Configuration des capteurs --- */
    lps22hh_block_data_update_set(&dev_ctx_lps22hh, PROPERTY_ENABLE);
    lps22hh_data_rate_set(&dev_ctx_lps22hh, LPS22HH_1_Hz_LOW_NOISE);

    hts221_block_data_update_set(&dev_ctx_hts221, PROPERTY_ENABLE);
    hts221_power_on_set(&dev_ctx_hts221, PROPERTY_ENABLE);
    hts221_data_rate_set(&dev_ctx_hts221, HTS221_ODR_1Hz);

    printf("Capteurs configures. Lancement de la boucle...\r\n");

  /* USER CODE END 2 */

  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_RED);
  BSP_LED_Init(LED_GREEN);
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* --- CALIBRATION DU HTS221 (Corrigée octet par octet) --- */
      printf("Lecture de la calibration HTS221...\r\n");
      float T0_degC, T1_degC, H0_rh, H1_rh;
      int16_t T0_out, T1_out, H0_T0_out, H1_T0_out;
      uint8_t b0, b1, t0_t1_msb;

      // Calibration Humidité
      hts221_read_reg(&dev_ctx_hts221, 0x30, &b0, 1);
      H0_rh = b0 / 2.0f;
      hts221_read_reg(&dev_ctx_hts221, 0x31, &b0, 1);
      H1_rh = b0 / 2.0f;

      // Calibration Température
      hts221_read_reg(&dev_ctx_hts221, 0x32, &b0, 1);
      hts221_read_reg(&dev_ctx_hts221, 0x33, &b1, 1);
      hts221_read_reg(&dev_ctx_hts221, 0x35, &t0_t1_msb, 1);
      T0_degC = (float)(b0 | ((t0_t1_msb & 0x03) << 8)) / 8.0f;
      T1_degC = (float)(b1 | ((t0_t1_msb & 0x0C) << 6)) / 8.0f;

      // Valeurs brutes de référence (Humidité)
      hts221_read_reg(&dev_ctx_hts221, 0x36, &b0, 1);
      hts221_read_reg(&dev_ctx_hts221, 0x37, &b1, 1);
      H0_T0_out = (int16_t)(((uint16_t)b1 << 8) | b0);

      hts221_read_reg(&dev_ctx_hts221, 0x3A, &b0, 1);
      hts221_read_reg(&dev_ctx_hts221, 0x3B, &b1, 1);
      H1_T0_out = (int16_t)(((uint16_t)b1 << 8) | b0);

      // Valeurs brutes de référence (Température)
      hts221_read_reg(&dev_ctx_hts221, 0x3C, &b0, 1);
      hts221_read_reg(&dev_ctx_hts221, 0x3D, &b1, 1);
      T0_out = (int16_t)(((uint16_t)b1 << 8) | b0);

      hts221_read_reg(&dev_ctx_hts221, 0x3E, &b0, 1);
      hts221_read_reg(&dev_ctx_hts221, 0x3F, &b1, 1);
      T1_out = (int16_t)(((uint16_t)b1 << 8) | b0);

      printf("Calibration HTS221 terminee !\r\n");

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
        // <ROUGE> - Occupé en train de lire les capteurs
    	float humidity_perc = 0.0f;
    	float temperature_celsius = 0.0f;
        BSP_LED_On(LED_RED);
        BSP_LED_Off(LED_GREEN);

        uint32_t raw_pressure = 0;
        int16_t raw_temperature = 0;
        int16_t raw_humidity = 0;
        float pressure_hPa = 0.0f;

        // --- Lecture LPS22HH (Pression) ---
        uint8_t lps_status = 0;
        lps22hh_read_reg(&dev_ctx_lps22hh, LPS22HH_STATUS, &lps_status, 1);
        if (lps_status & 0x01) {
            lps22hh_pressure_raw_get(&dev_ctx_lps22hh, &raw_pressure);
            pressure_hPa = lps22hh_from_lsb_to_hpa(raw_pressure);
            printf("LPS22HH - Pression : %.2f hPa\r\n", pressure_hPa);
        } else {
            printf("LPS22HH - En attente...\r\n");
        }

                // --- Lecture HTS221 (Température et Humidité) ---
                uint8_t hts_status = 0;
                hts221_read_reg(&dev_ctx_hts221, HTS221_STATUS_REG, &hts_status, 1);

                if (hts_status & 0x02) {
                    hts221_temperature_raw_get(&dev_ctx_hts221, &raw_temperature);
                    // L'équation magique :
                    temperature_celsius = (raw_temperature - T0_out) * (T1_degC - T0_degC) / (T1_out - T0_out) + T0_degC;
                    printf("HTS221  - Temperature : %.2f C\r\n", temperature_celsius);
                }

                if (hts_status & 0x01) {
                    hts221_humidity_raw_get(&dev_ctx_hts221, &raw_humidity);
                    // L'équation magique :
                    humidity_perc = (raw_humidity - H0_T0_out) * (H1_rh - H0_rh) / (H1_T0_out - H0_T0_out) + H0_rh;
                    printf("HTS221  - Humidite : %.2f %%\r\n", humidity_perc);
                }

        printf("----------------------------------\r\n");

        // <VERT> - Disponible et en attente d'une prochaine lecture
        BSP_LED_Off(LED_RED);
        BSP_LED_On(LED_GREEN);

        // <-- C'est ici que tu mettras ton envoi MQTT plus tard ! -->

        HAL_Delay(2000);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
/* USER CODE BEGIN CLK 1 */
/* USER CODE END CLK 1 */

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  if (HAL_PWREx_ConfigSupply(PWR_EXTERNAL_SOURCE_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct);
  if ((RCC_ClkInitStruct.CPUCLKSource == RCC_CPUCLKSOURCE_IC1) ||
     (RCC_ClkInitStruct.SYSCLKSource == RCC_SYSCLKSOURCE_IC2_IC6_IC11))
  {
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_CPUCLK | RCC_CLOCKTYPE_SYSCLK);
    RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_HSI;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
  }

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL1.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL1.PLLM = 4;
  RCC_OscInitStruct.PLL1.PLLN = 75;
  RCC_OscInitStruct.PLL1.PLLFractional = 0;
  RCC_OscInitStruct.PLL1.PLLP1 = 1;
  RCC_OscInitStruct.PLL1.PLLP2 = 1;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_CPUCLK|RCC_CLOCKTYPE_HCLK
                              |RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2|RCC_CLOCKTYPE_PCLK5
                              |RCC_CLOCKTYPE_PCLK4;
  RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_IC1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_IC2_IC6_IC11;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;
  RCC_ClkInitStruct.APB5CLKDivider = RCC_APB5_DIV1;
  RCC_ClkInitStruct.IC1Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC1Selection.ClockDivider = 2;
  RCC_ClkInitStruct.IC2Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC2Selection.ClockDivider = 3;
  RCC_ClkInitStruct.IC6Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC6Selection.ClockDivider = 4;
  RCC_ClkInitStruct.IC11Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC11Selection.ClockDivider = 3;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  */
static void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x30C0EDFF;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ICACHE Initialization Function
  */
static void MX_ICACHE_Init(void)
{
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configuration des broches pour I2C1 : PC1 (SDA) et PH9 (SCL) */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
}

void BSP_PB_Callback(Button_TypeDef Button)
{
  if (Button == BUTTON_USER)
  {
    BspButtonState = BUTTON_PRESSED;
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */
