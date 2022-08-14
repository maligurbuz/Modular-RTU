/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    09-November-2015
  * @brief   This file includes the usb host library user callbacks
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

/* Includes ------------------------------------------------------------------*/
#include "usart_log.h"

#include "lcd.h"
#include "usbh_usr.h"

#include "usbh_cdc_core.h"

char At_Receive[512];
uint8_t at_cmd_cnt=0,Network_Status;

Modem_TCP_Status tmp_status;
Modem_TCP_Array_Status tmp_array=NOT_FULL;
USR_State Init,D_attached,D_SpeedDet,D_DescAvailable,D_AddressAssigned,C_DescAvailable,E_Done,D_NotSupport,U_RecoverdError,ECHO,AT_Init;
char *AT_CMD_CHECK[]={"AT\r","AT+CREG?\r"};
char *AT_CMD_INIT[]={"ATE0\r",""};
char *split_string;

/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_CDC_DEMO_USER_CALLBACKS
* @{
*/

/** @defgroup USBH_USR
* @brief This file includes the usb host library user callbacks
* @{
*/ 


/** @defgroup USBH_CORE_Exported_TypesDefinitions
* @{
*/ 

/**
* @}
*/ 

/** @addtogroup USER
* @{
*/

/** @defgroup USBH_USR 
* @brief    This file includes the user application layer
* @{
*/ 

/** @defgroup USBH_CORE_Exported_Types
* @{
*/ 



/** @defgroup USBH_USR_Private_Defines
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_USR_Private_Macros
* @{
*/ 
/**
* @}
*/ 

/** @defgroup USBH_USR_Private_Variables
* @{
*/
extern USB_OTG_CORE_HANDLE           USB_OTG_Core;
extern USBH_HOST                     USB_Host; 
extern CDC_LineCodingTypeDef         CDC_GetLineCode;
extern CDC_LineCodingTypeDef         CDC_SetLineCode;
extern CDC_Usercb_TypeDef            UserCb;

uint8_t                              prev_select = 0;







/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_Callbacks =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
};

/**
* @}
*/

/** @defgroup USBH_USR_Private_Constants
* @{
*/
/*--------------- LCD Messages ---------------*/

/**
* @}
*/



/** @defgroup USBH_USR_Private_FunctionPrototypes
* @{
*/ 
/**
* @}
*/ 


static void CDC_OutputData (uint8_t *ptr);
static void CDC_GnssOutputData (uint8_t *ptr);

void Modem_Write(uint8_t *data, uint16_t length);
void At_Cmd_Execute();
/** @defgroup USBH_USR_Private_Functions
* @{
*/ 





/**
* @brief  USBH_USR_Init 
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBH_USR_Init(void)
{





	Init=OK;

	 D_attached=FAIL;
	 D_SpeedDet=FAIL;
	 D_DescAvailable=FAIL;
	 D_AddressAssigned=FAIL;
	 C_DescAvailable=FAIL;
	 E_Done=FAIL;
	 D_NotSupport=OK;
	 U_RecoverdError=OK;
	 ECHO=OK;
	 AT_Init=FAIL;
	 Modem=FAIL;
	 Button_Key=0;

    /* Configure the LEDs */




}

/**
* @brief  USBH_USR_DeviceAttached 
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{
	D_attached=OK;


}


/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError (void)
{
	U_RecoverdError=FAIL;


}


/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval None
*/
void USBH_USR_DeviceDisconnected (void)
{
  


}

/**
* @brief  USBH_USR_ResetUSBDevice 
*         Reset USB Device
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
  /* Users can do their application actions here for the USB-Reset */


}


/**
* @brief  USBH_USR_DeviceSpeedDetected 
*         Displays the message on LCD for device speed
* @param  Devicespeed : Device Speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
  if(DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
  {
	  D_SpeedDet=OK;

  }  
  else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
  {
	  D_SpeedDet=OK;

  }
  else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
  {
	  D_SpeedDet=OK;

  }
  else
  {
	  D_SpeedDet=FAIL;

  }

}

/**
* @brief  USBH_USR_Device_DescAvailable 
*         Displays the message on LCD for device descriptor
* @param  DeviceDesc : device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{

  USBH_DevDesc_TypeDef *hs;
  hs = DeviceDesc;

  if((uint32_t)(*hs).idVendor==0x05C6 &&  (uint32_t)(*hs).idProduct==0x9003)
      	D_DescAvailable=OK;
      else
      	D_DescAvailable=FAIL;



}

/**
* @brief  USBH_USR_DeviceAddressAssigned 
*         USB device is successfully assigned the Address 
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{
	D_AddressAssigned=OK;


}


/**
* @brief  USBH_USR_Conf_Desc 
*         Displays the message on LCD for configuration descriptor
* @param  ConfDesc : Configuration descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
  USBH_InterfaceDesc_TypeDef *id;
  
  id = itfDesc;  




  if((*id).bInterfaceClass  == VENDOR_SPECIFIC)
  {
	  C_DescAvailable=OK;

  }
  else
  {
	  C_DescAvailable=FAIL;

  }

}

/**
* @brief  USBH_USR_Manufacturer_String 
*         Displays the message on LCD for Manufacturer String 
* @param  ManufacturerString : Manufacturer String of Device
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{


}

/**
* @brief  USBH_USR_Product_String 
*         Displays the message on LCD for Product String
* @param  ProductString : Product String of Device
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{


}

/**
* @brief  USBH_USR_SerialNum_String 
*         Displays the message on LCD for SerialNum_String 
* @param  SerialNumString : SerialNum_String of device
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{


}

/**
* @brief  EnumerationDone 
*         User response request is displayed to ask for
*         application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{
  /* Enumeration complete */
	E_Done=OK;

  UserCb.Receive = CDC_OutputData;
  UserCb.GnssReceive = CDC_GnssOutputData;

} 

/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{
	D_NotSupport=FAIL;


}  


/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{

  USBH_USR_Status usbh_usr_status;



    usbh_usr_status = USBH_USR_RESP_OK;

    if(Init && D_attached && D_SpeedDet && D_DescAvailable && D_AddressAssigned && C_DescAvailable && E_Done && D_NotSupport && U_RecoverdError){
    	Modem=OK;
    	Modem_Status=Modem_Check;
    	Prev_Modem_Status=Modem_Idle;
    	At_Command_Status=At_Cmd_Idle;
    	CDC_StartReception(&USB_OTG_Core);
    }



    	else
    		Modem=FAIL;




  return usbh_usr_status;

}

/**
* @brief  USBH_USR_OverCurrentDetected
*         Device Overcurrent detection event
* @param  None
* @retval None
*/
void USBH_USR_OverCurrentDetected (void)
{


}

/**
* @brief  USBH_USR_DeInit
*         Deint User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{


}

/**
* @brief  USBH_USR_Application
*         CDC Main application
* @param  None
* @retval None
*/
int USBH_USR_Application(void)
{

	switch (Modem_Status) {
		case Modem_Check:

					if(At_Command_Status==At_Cmd_Idle && at_cmd_cnt <2){
						At_Command=AT_CMD_CHECK[at_cmd_cnt];
						At_Command_Status=At_Cmd_Send;
					}
					else if(At_Command_Status==At_Cmd_OK && at_cmd_cnt==0){
						at_cmd_cnt++;
						At_Command_Status=At_Cmd_Idle;
					}
					else if(At_Command_Status==At_Cmd_OK && at_cmd_cnt==1){
						split_string=strtok((char *)At_Receive,",");
						split_string=strtok(NULL,",");
						Network_Status=atoi(split_string);
						if(Network_Status==1){
							Modem_Status=Modem_Init;
							at_cmd_cnt=0;
							Modem_Messsage="Modem Check OK.\n\r";
						}
						else{

							Modem_Messsage="Baz istasyonu ile baglanti olusturuluyor...\n\r";
							at_cmd_cnt=1;

						}
						At_Command_Status=At_Cmd_Idle;
						strcpy((char *)At_Receive,"");
					}



		case Modem_Init:
//			if(At_Command_Status==At_Cmd_Idle && at_cmd_cnt !=2){
//						At_Command=AT_CMD_INIT[at_cmd_cnt];
//						At_Command_Status=At_Cmd_Send;
//						}
//			else if(At_Command_Status==At_Cmd_Wait && at_cmd_cnt ==0 ){
//						if(strstr(At_Receive,"+CREG")!=NULL && strstr(At_Receive,"+CREG: 0,1")==NULL);
//
//
//			}
//			else if(at_cmd_cnt==2)
//			{
//				Modem_Status=Modem_Idle;
//				At_Command_Status==At_Cmd_Idle;
//				at_cmd_cnt=0;
//			}
			break;
		case Modem_Connect:

					break;
		case Modem_Request:

					break;
		case Modem_Response:

					break;
		case Modem_Reset:

				Modem_Status=Prev_Modem_Status;
				Prev_Modem_Status=Modem_Reset;
				MODEM_RESET();


							break;
		case Modem_Wwan_On:

				Modem_Status=Prev_Modem_Status;
				Prev_Modem_Status=Modem_Wwan_On;
				MODEM_WWAN_ON();


									break;
		case Modem_Wwan_Off:

				Modem_Status=Prev_Modem_Status;
				Prev_Modem_Status=Modem_Wwan_Off;
				MODEM_WWAN_OFF();


											break;

		default:

			break;
	}
	At_Cmd_Execute();


  return 0;
}

void At_Cmd_Execute(){

	switch (At_Command_Status) {
		case At_Cmd_Send:
			CDC_SendData((uint8_t*)At_Command,strlen(At_Command));
			At_Command_Status=At_Cmd_Wait;
					break;
		case At_Cmd_Receiv:
			if(strstr(At_Receive,"OK")!=NULL)
				At_Command_Status=At_Cmd_OK;
			else if(strstr(At_Receive,"ERROR")!=NULL)
				At_Command_Status=At_Cmd_Error;
			else
				At_Command_Status=At_Cmd_Wait;

					break;
		case At_Cmd_Wait:

						break;
		case At_Cmd_OK:

			break;
		case At_Cmd_Error:

					break;
		default:

			break;
	}
}


void Modem_Write(uint8_t *data, uint16_t length){

	CDC_SendData (data, length);


}



static void CDC_OutputData (uint8_t *ptr)
{
	USART_puts((char *)ptr);
	At_Command_Status=At_Cmd_Receiv;
	strcat(At_Receive,(char *)ptr);
	//At_Cmd_Execute();
//	tmp = strtok (ptr,",");
//	if(syc==TCP_PACKAGES_SIZE-1)
//
//	if (strncmp((char *)&tmp[2],"+QIURC: \"recv\"",14)==0){
//		tmp_status=recv;
//		Tcp_Packages[syc].status=tmp_status;
//		tmp = strtok (NULL, ",");
//		Tcp_Packages[syc].conn=atoi(tmp);
//		tmp = strtok (NULL, ",");
//		rec=(char *) &tmp[(int)((char *)strchr(tmp, '\n')- tmp)+1];
//		strcat((char *)Tcp_Packages[syc].data,rec);
//
//	}
//	else if (strncmp((char *)&tmp[2],"+QIURC: \"incoming\"",18)==0){
//		tmp_status=incoming;
//	}
//	else if (strncmp((char *)&tmp[2],"+QIURC: \"closed\"",16)==0){
//		tmp_status=closed;
//	}
//	else if (strncmp((char *)&tmp[2],"+QIURC: \"incoming full\"",23)==0)
//			tmp_status=incoming_full;
//	else
//		tmp_status=not_status;

//				if(tmp_status>1)
//				{
//					  for (uint8_t i=0;tmp != NULL;i++)
//					  {
//
//
//						  if(syc_w==0)
//							  conn=atoi(tmp);
//
//						  rec=(char *) &tmp[(int)((char *)strchr(tmp, '\n')- tmp)+1];
//
//						  if((strncmp(rec, "GET", 3) == 0))
//							  http_server_serve(0,conn,rec);
//
//						  syc_w++;
//					  }
//					  syc_w=0;




			//GET / HTTP/1.1\r\nHost: 5.26.124.160\r\nCon





}
static void CDC_GnssOutputData (uint8_t *ptr)
{
	  USART_puts((char *)ptr);

}

/**
* @brief  Set the cdc demo intial values
* @param  None
* @retval None
*/
/**
* @}h
*/

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
