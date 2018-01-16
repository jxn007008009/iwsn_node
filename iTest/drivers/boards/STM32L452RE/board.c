/*
 * Copyright: Beijing Jiaotong University, 2018-2022.
 * Filename: board.c
 * Author: Yipeng Cun <cunyipeng@bjtu.edu.cn>
 * Date: Jan 5th, 2018
 * Function: the source/header of the project
 */

//=========================== include =========================================
#include "board.h"

//=========================== define ==========================================

//=========================== typedef =========================================

//=========================== variables =======================================

//=========================== prototypes ======================================
static void rcc_init(void);
static void gpio_init(void);
//static void systick_init(void);

//=========================== main ============================================

//=========================== public ==========================================
void board_init(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Initialize the RCC clock configuration. */
  rcc_init ();
  
  /* Initialize the GPIO configuration. */
  gpio_init (); 
  
  /* Initialize the SYSTICK configuration. */
  //systick_init();
  
  /* Initialize the LED configuration. */
  led_init();
  
  /* Initialize the BUTTON configuration. */
  //button_init();

  /* Initialize the CTIMER configuration. */
  ctimer_init();
  
  /* Initialize the RTIMER configuration. */
  rtimer_init();
  
  /* Initialize the UART configuration. */
  #ifdef HAVE_UART
    uart_init();
  #endif /* HAVE_UART */
  
  /* Initialize the SPI configuration. */
  spi_init();
  
  /* Initialize the RADIO configuration. */
  radio_init();
}

void board_sleep(void)
{
  // Enable PWR and BKP clock
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  // Disable the SRAM and FLITF clock in sleep mode
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM | RCC_AHBPeriph_FLITF, DISABLE);
  // enter sleep mode
  //__WFI();
}

void board_wakeup(void)
{
}

void board_reset(void)
{
  //NVIC_SystemReset();
}

//=========================== private =========================================
static void rcc_init(void)
{
	  RCC_OscInitTypeDef RCC_OscInitStruct;
	  RCC_ClkInitTypeDef RCC_ClkInitStruct;
	  RCC_PeriphCLKInitTypeDef PeriphClkInit;

	    /**Initializes the CPU, AHB and APB busses clocks
	    */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	  RCC_OscInitStruct.MSICalibrationValue = 0;
	  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	  RCC_OscInitStruct.PLL.PLLM = 1;
	  RCC_OscInitStruct.PLL.PLLN = 36;
	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	    /**Initializes the CPU, AHB and APB busses clocks
	    */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART4;
	  PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
	  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	    /**Configure the main internal regulator output voltage
	    */
	  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	    /**Configure the Systick interrupt time
	    */
	  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	    /**Configure the Systick
	    */
	  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	  /* SysTick_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

static void gpio_init(void)
{
}

/*
static void systick_init(void)
{
  Configure the SysTick to have interrupt in 1ms time basis
  HAL_SYSTICK_Config(SystemCoreClock/PORT_CLOCKS_PER_SEC);

  Configure the SysTick IRQ priority
  HAL_NVIC_SetPriority(SysTick_IRQn, SYSTICK_IRQ_PRIORITY ,0);

}
*/

//=========================== interrupt handlers ==============================
