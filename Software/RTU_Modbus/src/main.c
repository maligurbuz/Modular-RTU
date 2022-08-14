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
#include "ETH_Driver/stm32f4x7_eth.h"
#include "netconf.h"
#include "main.h"
#include "httpd.h"


/* ----------------------- Modbus includes ----------------------------------*/
#include "Modbus/mb.h"
#include "Modbus/mbtcp.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START         1000
#define REG_INPUT_NREGS         4
#define REG_HOLDING_START       2090
#define REG_HOLDING_NREGS       150


/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];
static USHORT   usRegHoldingStart = REG_HOLDING_START;
static USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SYSTEMTICK_PERIOD_MS  10
static UCHAR Vendor[14] = "WVC 0.2 - dB14";


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;
GPIO_InitTypeDef GPIO_InitDef;
USART_InitTypeDef USART_InitStruct;
/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	SystemInit();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
  /*Initialize LCD and Leds */
  //LCD_LED_Init();

	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
//Mode output
GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
//Output type push-pull
GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
//Without pull resistors
GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
//50MHz pin speed
GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;

//Initialize pins on GPIOG port
GPIO_Init(GPIOG, &GPIO_InitDef);
GPIO_SetBits(GPIOG,GPIO_Pin_0);

GPIO_InitDef.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
    //Mode output
GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
    //Output type push-pull
GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    //Without pull resistors
GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
    //50MHz pin speed
GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;

    //Initialize pins on GPIOG port
    GPIO_Init(GPIOE, &GPIO_InitDef);
    GPIO_ResetBits(GPIOE,GPIO_Pin_13);
    GPIO_SetBits(GPIOE,GPIO_Pin_15);
	//GPIO_SetBits(GPIOG,GPIO_Pin_1);
  /*!< At this stage the microcontroller clock setting is already configured to
       168 MHz, this is done through SystemInit() function which is called from
       startup file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */


#ifdef SERIAL_DEBUG
  DebugComPort_Init();
#endif


  /* configure ethernet (GPIOs, clocks, MAC, DMA) */
  ETH_BSP_Config();

  /* Initilaize the LwIP stack */
  LwIP_Init();

  /* Http webserver Init */

  httpd_init();
 eMBTCPInit(502);
 eMBInit(MB_RTU,MB_TCP_PSEUDO_ADDRESS,0,9600,MB_PAR_NONE);



  eMBEnable(MB_RTU);
  eMBEnable(MB_TCP);


  eMBSetSlaveID( MB_TCP_PSEUDO_ADDRESS, TRUE, Vendor, sizeof(Vendor) );
  /* Infinite loop */
  while (1)
  {
	eMBPoll();
    /* check if any packet received */
    if (ETH_CheckFrameReceived())
    {
      /* process received ethernet packet */
      LwIP_Pkt_Handle();
    }
    /* handle periodic timers for LwIP */
    LwIP_Periodic_Handle(LocalTime);

  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of 10ms periods to wait for.
  * @retval None
  */
void Delay(uint32_t nCount)
{
  /* Capture the current local time */
  timingdelay = LocalTime + nCount;

  /* wait until the desired delay finish */
  while(timingdelay > LocalTime)
  {
  }
}

/**
  * @brief  Updates the system local time
  * @param  None
  * @retval None
  */
void Time_Update(void)
{
  LocalTime += SYSTEMTICK_PERIOD_MS;




}

/**
  * @brief  Initializes the STM324xG-EVAL's LCD and LEDs resources
  * @param  None
  * @retval None
  */


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


eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    usRegInputBuf[0] = (GPIOG->ODR & 0x000F);
    usRegInputBuf[1] = usRegInputBuf[0];

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;


    if( ( usAddress >= REG_HOLDING_START ) &&
        ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        switch ( eMode )
        {
            /* Pass current register values to the protocol stack. */
        case MB_REG_READ:
            while( usNRegs > 0 )
            {


                *pucRegBuffer++ = ( UCHAR ) ( usRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( UCHAR ) ( usRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;

            /* Update current register values with new values from the
             * protocol stack. */
        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
