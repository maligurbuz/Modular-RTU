/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "http/httpserver-socket.h"
#include "http/netconf.h"


#include "ETH_Driver/stm32f4x7_eth.h"
#include "lwip/tcpip.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/



/*--------------- Tasks Priority -------------*/
#define MAIN_TASK_PRIO   ( tskIDLE_PRIORITY + 1 )
#define DHCP_TASK_PRIO   ( tskIDLE_PRIORITY + 4 )      
#define LED_TASK_PRIO    ( tskIDLE_PRIORITY + 3 )
#define MODEM_TASK_PRIO  ( tskIDLE_PRIORITY + 3 )
uint8_t sim_pin=0;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void LED_Init(void);
void ToggleLed4(void * pvParameters);
void Main_task(void * pvParameters);
void UsbModem(void * pvParameters);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

  /*!< At this stage the microcontroller clock setting is already configured to 
       168 MHz, this is done through SystemInit() function which is called from
       startup file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */  
  
  /* Configures the priority grouping: 4 bits pre-emption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  /* Init task */
  xTaskCreate(Main_task, (int8_t *) "Main", configMINIMAL_STACK_SIZE * 2, NULL,MAIN_TASK_PRIO, NULL);

  /* Start scheduler */
  vTaskStartScheduler();

  /* We should never get here as control is now taken by the scheduler */
  for( ;; );
}

void Main_task(void * pvParameters)
{


  /*Initialize LCD and Leds */ 
	RTU_GPIO_INIT();

	Usart_Init();
	Lcd_Init(20,4);
	Lcd_Light_On();
	Lcd_Puts(1,0," RTU ");
  /* configure ethernet (GPIOs, clocks, MAC, DMA) */ 

	if(!GPIO_ReadInputDataBit(SIMCARD_DETECTED.Port,SIMCARD_DETECTED.Pin))
		Sim_status=OK;
	else
		Sim_status=FAIL;

	//MODEM_ON();
	NAND_Init();

  ETH_BSP_Config();

  USBH_Init(&USB_OTG_Core,USB_OTG_HS_CORE_ID,&USB_Host,&CDC_cb,&USR_Callbacks);

  /* Initilaize the LwIP stack */
  LwIP_Init();

  /* Initialize webserver demo */
  http_server_socket_init();



  #ifdef USE_DHCP
  /* Start DHCPClient */
  xTaskCreate(LwIP_DHCP_task, (int8_t *) "DHCP", configMINIMAL_STACK_SIZE * 2, NULL,DHCP_TASK_PRIO, NULL);
#endif

  /* Start toogleLed4 task : Toggle LED4  every 250ms */
  xTaskCreate(ToggleLed4, (int8_t *) "LED4", configMINIMAL_STACK_SIZE, NULL, LED_TASK_PRIO, NULL);

  xTaskCreate(UsbModem, (int8_t *) "MODEM", configMINIMAL_STACK_SIZE*1.5, NULL, MODEM_TASK_PRIO, NULL);



  for( ;; )
  {

      vTaskDelete(NULL);

  }
}

/**
  * @brief  Toggle Led4 task
  * @param  pvParameters not used
  * @retval None
  */
void UsbModem (void * pvParameters){


	USART_puts("\n\rUSB MODEM\n\r");


	for( ;; )
	  {
		switch (GPIO_ReadInputDataBit(SIMCARD_DETECTED.Port,SIMCARD_DETECTED.Pin)) {
			case 0:

				if(Sim_status==FAIL)
					MODEM_RESET();

					Sim_status=OK;



				break;
			case 1:

				Sim_status=FAIL;

				Modem_Messsage="Sim Kart Takili Degil Lutfen Sim Karti Takin\n\r";

				break;

				default:

				break;
		}
		if(Sim_status==OK)
		USBH_Process(&USB_OTG_Core , &USB_Host);


		Lcd_Puts(0,1,Modem_Messsage);
	  }

}

void ToggleLed4(void * pvParameters)
{

  for( ;; )
  {

    /* toggle LED4 each 250ms */


	  GPIO_ToggleBits(RUN_LED.Port,RUN_LED.Pin);

	  vTaskDelay(1000);
  }
}

/**
  * @brief  Initializes the LCD and LEDs resources.
  * @param  None
  * @retval None
  */


/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of Ticks to delay.
  * @retval None
  */
void Delay(uint32_t nCount)
{
  vTaskDelay(nCount);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


