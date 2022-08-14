/**
  ******************************************************************************
  * @file    usbh_usr.h
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    09-November-2015
  * @brief   This file is the header file for usb usr file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USH_USR_H__
#define __USH_USR_H__


/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"
#include "usb_conf.h"
#include "gpio.h"
#include "stdio.h"

/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_HID_CDC_USER_CALLBACKS
* @{
*/
  
/** @defgroup USBH_USR 
  * @brief This file is the header file for user action
  * @{
  */ 
uint8_t                            S_Temp[40],Button_Key;
typedef enum {
	FAIL=0,
	OK,

}USR_State;




#define TCP_PACKAGES_SIZE				  10



typedef enum{
	Modem_Check=0,
	Modem_Init,
	Modem_Connect,
	Modem_Response,
	Modem_Request,
	Modem_Idle,
	Modem_Reset,
	Modem_Wwan_On,
	Modem_Wwan_Off,

}MODEM_STATUS;

typedef enum{
	At_Cmd_Idle=0,
	At_Cmd_Send,
	At_Cmd_Receiv,
	At_Cmd_Wait,
	At_Cmd_Error,
	At_Cmd_OK,

}AT_COMMAND_STATUS;

typedef enum
{
	not_status=0,
	incoming_full=1,
	incoming=2,
	closed=3,
	recv=4,


}Modem_TCP_Status;

typedef enum
{
	NOT_FULL=0,
	FULL=1,


}Modem_TCP_Array_Status;

typedef  struct
{
    uint8_t  conn;
    char   data [512];
    uint8_t status;
}Modem_TCP_Packages;




extern  USBH_Usr_cb_TypeDef USR_Callbacks;
MODEM_STATUS Modem_Status,Prev_Modem_Status;
AT_COMMAND_STATUS At_Command_Status;
extern Modem_TCP_Packages Tcp_Packages[TCP_PACKAGES_SIZE];
 USR_State Modem,Wwan_Status,Sim_status;

/**
  * @}
  */


/** @defgroup USBH_CORE_Exported_FunctionsPrototype
  * @{
  */

void USBH_USR_ApplicationSelected(void);
void USBH_USR_Init(void);
void USBH_USR_DeInit(void);
void USBH_USR_DeviceAttached(void);
void USBH_USR_ResetDevice(void);
void USBH_USR_DeviceDisconnected (void);
void USBH_USR_OverCurrentDetected (void);
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed); 
void USBH_USR_Device_DescAvailable(void *);
void USBH_USR_DeviceAddressAssigned(void);
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc);
void USBH_USR_Manufacturer_String(void *);
void USBH_USR_Product_String(void *);
void USBH_USR_SerialNum_String(void *);
void USBH_USR_EnumerationDone(void);
USBH_USR_Status USBH_USR_UserInput(void);
int  USBH_USR_Application(void);
void USBH_USR_DeInit(void);
void USBH_USR_DeviceNotSupported(void);
void USBH_USR_UnrecoveredError(void);



/**
  * @}
  */ 

#endif /* __USBH_USR_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
