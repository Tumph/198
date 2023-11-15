#include "main.h"

#include "stm32f4xx_hal.h"

#include "stm32f4xx_hal_adc.h"

#include <stdio.h>



ADC_HandleTypeDef hadc1;



void SystemClock_Config(void);

static void MX_GPIO_Init(void);

static void MX_ADC1_Init(void);

void Error_Handler(void);



int main(void)

{

    HAL_Init();

    SystemClock_Config();



    MX_GPIO_Init();

    MX_ADC1_Init();



    uint32_t uv_data;



    while (1)

    {

        // Check if the black button (B2) is pressed to start recording UV data

        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) // Assuming B2 is connected to PC13

        {

            // Start recording UV data

            HAL_ADC_Start(&hadc1);

            HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

            uv_data = HAL_ADC_GetValue(&hadc1);

            HAL_ADC_Stop(&hadc1);



            // Output UV data to the terminal

            printf("UV Data: %lu\r\n", uv_data);



            // Wait for a short duration before taking the next reading

            HAL_Delay(1000);

        }

        else

        {

            // Stop recording when the black button (B2) is pressed again

            HAL_ADC_Stop(&hadc1);



            // Wait for the button to be released

            while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET)

            {

                // Add any other code or functionality needed during the idle state

            }

        }

    }

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

    __HAL_RCC_GPIOC_CLK_ENABLE();



    // Configure GPIO pin PC13 as input with pull-up

    GPIO_InitStruct.Pin = GPIO_PIN_13;

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

    GPIO_InitStruct.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}



void Error_Handler(void)

{

    // Add error handling code as needed

    while (1)

    {

        // Infinite loop to indicate an error

    }

}



#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)

{

    // User can add his own implementation to report the file name and line number

}

#endif /* USE_FULL_ASSERT */

