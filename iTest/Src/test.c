/*
 * Copyright: Beijing Jiaotong University, 2018-2022.
 * Filename: test.c
 * Author: Yipeng Cun <cunyipeng@bjtu.edu.cn>
 * Date: Jan 5th, 2018
 * Function: the source/header of the project
 */

//=========================== include =========================================
#include "board.h"
#include "blink.h"
#include "echo.h"
#include "scheduler.h"


//=========================== define ==========================================
enum{
	RADIO_RX  	        = 0,
	RADIO_CIRCULAR_TX 	= 1,
	RADIO_TIMER_TX		= 2,
};

//=========================== typedef =========================================

//=========================== variables =======================================
uint8_t m=RADIO_RX;
//=========================== prototypes ======================================
extern uint8_t radio_spiReadReg(uint8_t reg_addr);
void test_radio_send_pkt();
void test_radio_rx();
void test_radio_startFrameCb(PORT_TIMER_WIDTH n);
void test_radio_endFrameCb(PORT_TIMER_WIDTH n);
extern void sendAdv();
//=========================== main ============================================
int main(void)
{
   /* Initialize. */
   board_init();
   scheduler_init();

   /* Applications Initialize. */
   //blink_init();
   //timers_init();
   //echo_init();


   /* Start. */
   //blink_start();
   iwsn_start();
   scheduler_start();
   //uint8_t register_value;
   //uint8_t value = 0x1E;
   //register_value   = radio_spiReadReg(value);
   //if(register_value == 0x1F)
  // {
	   //test_radio_rx();
	   //while(1){
	   	   //HAL_Delay(1000);
	   	   //test_radio_send_pkt();
	   //}
   //}

   return 0;
}

//=========================== public ==========================================

//=========================== private =========================================
void test_radio_send_pkt()
  {
    led_toggle();
    //radio_setStartFrameCb(radio_capture_cbt cb);
    //test radio sendMsg
    uint8_t rfmsg[] = {0x61,0x88,0x7C,0x01,0x00,0x55,0x00,0x22,0x00,0x20,0x5C,0xFE,0xF2,0xCB,0x00,0x00};

    // Set the radio channel
    radio_setChannel(26);

    // load the packet in the radio's Tx buffer
    radio_loadPacket(&(rfmsg[0]), sizeof(rfmsg));

    // enable the radio in Tx mode. This does not send the packet.
    radio_txEnable();

    // give the 'go' to transmit
    radio_txNow();

  }

void test_radio_rx()
{
    // Turn off the radio
    radio_rfOff();

    //set the start frame callback
    radio_setStartFrameCb(test_radio_startFrameCb);

    // Set the end frame callback
    radio_setEndFrameCb(test_radio_endFrameCb);

    // Set the radio channel
    radio_setChannel(26);

    // Turn on the radio
    radio_rfOn();

    // Enable rx packet
    radio_rxEnable();

    // Start rx packet
    radio_rxNow();

    //led_toggle();
}

void test_radio_startFrameCb(PORT_TIMER_WIDTH n)
{
   led_toggle();
}

void test_radio_endFrameCb(PORT_TIMER_WIDTH n)
{
  //leds_toggle();
  if(m==RADIO_RX)
  {
      //led_toggle();
      // Enable rx packet
      radio_rxEnable();
      // Start rx packet after the first receive the first pkt
      radio_rxNow();
  }
}


//=========================== interrupt handlers ==============================
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3)
	{
	    /* Call ISR. */
	    ctimer_isr();
	}

	if(htim->Instance==TIM2)
	{
		/* Call ISR. */
		rtimer_isr(RTIMER_COMPARE);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2)
	{
	    /* Call ISR. */
	    rtimer_isr(RTIMER_OVERFLOW);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==UART4)//����Ǵ���4
	{
        #ifdef HAVE_UART
           uart_rx_isr();
        #endif /* HAVE_UART */
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_0)
  {
	    //led_toggle();
        #ifdef HAVE_RADIO
           radio_isr();
 	 	#endif /* HAVE_RADIO */
  }
}

//=========================== error processing ==============================
void _Error_Handler(char * file, int line)
{
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
}
