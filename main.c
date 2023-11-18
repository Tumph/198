#include "main.h"

#include "stm32f4xx_hal.h"

#include "stm32f4xx_hal_adc.h"

#include <stdio.h>



ADC_HandleTypeDef hadc1;



void SystemClock_Config(void);

static void MX_GPIO_Init(void)

static void MX_ADC1_Init(void);



float calculateUVIndex(uint16_t uvData);



int main(void)

{

  HAL_Init();

  SystemClock_Config();



  MX_GPIO_Init();

  MX_ADC1_Init();



  // Variables for UV data and timing

  uint16_t uvData = 0;

  uint32_t startTime = 0;

  uint32_t endTime = 0;



  while (1)

  {

    // Check if the black button (B2) is pressed to start UV exposure tracking

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET)

    {

      // Start UV exposure tracking

      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // Set GPIOA Pin 5 high



      // Record the start time

      startTime = HAL_GetTick();

      printf("UV Exposure Tracking Started!\r\n");



      // Read UV data from the sensor

      if (HAL_ADC_Start(&hadc1) == HAL_OK &&

          HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK)

      {

        uvData = HAL_ADC_GetValue(&hadc1);

      }



      // Display UV data in the terminal

      printf("UV Data: %d\r\n", uvData);



      // Delay for a short period (adjust as needed)

      HAL_Delay(1000);



      // Stop UV exposure tracking if the button is pressed again

      if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET)

      {

        // Record the end time

        endTime = HAL_GetTick();

        printf("UV Exposure Tracking Stopped!\r\n");



        // Stop UV exposure tracking

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // Reset GPIOA Pin 5 low



        // Calculate and display the final UV index

        float uvIndex = calculateUVIndex(uvData);



        // Add your post-exposure logic here

        if (uvIndex > 6.0)

        {

          // UV index is high, prompt the user to take protective measures

          printf("UV index is high. Take necessary precautions!\r\n");

          // Add your code to trigger warnings, alerts, or other actions

          // For example, flashing LEDs, sending notifications, etc.

        }

        else

        {

          // UV index is within the safe range

          printf("UV index is within the safe range.\r\n");

          // Add your code for normal operation or display a message

        }



        // Your additional logic goes here



        // Delay for a reasonable period before repeating the loop

        HAL_Delay(5000); // Adjust the delay time as needed

      }

    }

  }

}



// Function to calculate and display the final UV index

float calculateUVIndex(uint16_t uvData)

{

  // Implement your UV index calculation logic based on sensor data

  // For example, a simple linear mapping from UV data to UV index

  // This is a placeholder and should be replaced with your actual calculation

  float uvIndex = (float)uvData / 100.0;



  // Display the calculated UV index

  printf("Final UV Index: %.2f\r\n", uvIndex);



  return uvIndex;

}



void SystemClock_Config(void)

{

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};



  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);



  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;

  RCC_OscInitStruct.HSIState = RCC_HSI_ON;

  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;



  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)

  {

    Error_Handler();

  }



  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |

                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;

  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;

  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;



  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)

  {

    Error_Handler();

  }

}



static void MX_ADC1_Init(void)

{

  hadc1.Instance = ADC1;

  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;

  hadc1.Init.Resolution = ADC_RESOLUTION_12B;

  hadc1.Init.ScanConvMode = DISABLE;

  hadc1.Init.ContinuousConvMode = DISABLE;

  hadc1.Init.DiscontinuousConvMode = DISABLE;

  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;

  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;

  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;

  hadc1.Init.NbrOfConversion = 1;

  hadc1.Init.DMAContinuousRequests = DISABLE;

  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;



  if (HAL_ADC_Init(&hadc1) != HAL_OK)

  {

    Error_Handler();

  }



  ADC_ChannelConfTypeDef sConfig = {0};

  sConfig.Channel = ADC_CHANNEL_0;

  sConfig.Rank = 1;

  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;



  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)

  {

    Error_Handler();

  }

}



static void MX_GPIO_Init(void)

{

  GPIO_InitTypeDef GPIO_InitStruct = {0};



  __HAL_RCC_GPIOA_CLK_ENABLE();



  // Initialize the UV sensor pin (Update with the appropriate pin for your UV sensor)

  GPIO_InitStruct.Pin = GPIO_PIN_0; // Update with the appropriate pin for your UV sensor (e.g., GPIO_PIN_A0)

  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);



  // Initialize the button pin

  GPIO_InitStruct.Pin = GPIO_PIN_10;

  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);



  // Initialize the UV exposure tracking indicator pin

  GPIO_InitStruct.Pin = GPIO_PIN_5;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}



void Error_Handler(void)

{

  while (1)

  {

    // An error occurred, stay in this loop

  }

}



#ifdef USE_FULL

#endif // Add this

