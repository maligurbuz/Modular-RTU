/************************************************************************************************
*												*
* This code is not supported by Analog Device Inc, has not been fully tested or verified,	*
* is for Demo purposes only and is used at ones own risk.		 			*
*		 								24-11-2008	*
************************************************************************************************/
/************************************************************************************************
*												*
* Author	: Analog Devices Inc.								*
*		  Industrial DAC Group								*
*		  Applications									*
*												*
* Date		: November 2008								*
*												*
* File		: main.c									*
*												*
* Hardware	: PLC_DEMO BOARD V04								*
*												*
* Description	: Top level file which performs serial communication over UART. 		*
*		  Reads incoming UART communication, interprets commands and 			*
*		  thus communicates with the DAC/ADC devices on the demo board.			*
*												*
************************************************************************************************/

#include  "PLCbios.h"
#include  "ad7793.h"


#define   ACK_TX        0xA8
#define   DACWR         0x01
#define   DACRD         0x02
#define   ADCWR         0x03
#define   ADCRD         0x04


int main(void)
{
  BYTE    startByte               = 0;
  BYTE    commandChannelByte      = 0;
  BYTE    command                 = 0;
  BYTE    channel                 = 0;
  BYTE    i                       = 0;
  BYTE    DACData8[2]             = {0,0};
  BYTE    DACRegAddr              = 0;
  BYTE    output1[24]             = "";
  BYTE    ADCRegData8[3]          = {0,0,0};
  BYTE    ADCRegAddr              = 0;
  DWORD   DACData24               = 0;
  DWORD   ADCRegData24            = 0;

  POWKEY1 = 0x01;
  POWCON  = 0x00;		   			// 41.78MHz
  POWKEY2 = 0xF4;

  GP1CON = 0x011;                                       // Setup tx & rx pins on P1.0 and P1.1

  // UART @ 9600bps
  COMCON0 = 0x080;				        // Setting DLAB
  COMDIV0 = 0x088;				        // Setting DIV0 and DIV1 to DL calculated
  COMDIV1 = 0x000;
  COMCON0 = 0x007;                                      // Clearing DLAB


  InitGPIO();                                           // Initialise required GPIO ports


  for (i=0; i<4; i++)                                   // Initialise and Reset ADC channels
  {
    ADCChSel(i);
    ADCDis();
    ResetAD7793(i);
  }


  while(1)
  {
    do
    {
     do
     {
       while ((COMSTA0 & 0x01) != 0x01)                 // Wait for Rx buffer full bit to be set.
        {
        }
        startByte = COMRX;
      }while (startByte != 0x65);                       // Check for correct start byte
      while ((COMSTA0 & 0x01) != 0x01)
      {
      }
      commandChannelByte = COMRX;
      command = commandChannelByte & 0x0F;              // Extract Command
      channel = (commandChannelByte & 0xF0) >> 4;       // Extract Channel
    }while ((command != DACWR)  && (command != DACRD) &&// Check for valid command 
            (command != ADCWR) && (command != ADCRD));


    COMTX = ACK_TX;                                     // Send Acknowledge


    switch(command)
    {
      case DACWR :                                      // Write to the DAC
        DACData8[0] = 0;
        DACData8[1] = 0;
        DACData24   = 0;
        while ((COMSTA0 & 0x01) != 0x01)                // Wait for Lower Byte
        {
        }
        DACData8[0] =   COMRX;
        while ((COMSTA0 & 0x01) != 0x01)                // Wait for Upper Byte
        {
        }
        DACData8[1] =   COMRX;
        while ((COMSTA0 & 0x01) != 0x01)                // Wait for DAC register address
        {
        }
        DACRegAddr  =   COMRX;
        DACData24   |=  DACRegAddr;                     // Assign DACRegAddr to top byte of DACData24
        DACData24   =   DACData24 << 8;                 // Shift left 1 byte
        DACData24   |=  DACData8[1];                    // Assign upper data byte to middle byte of DACData24
        DACData24   =   DACData24 << 8;                 // Shift left 1 byte
        DACData24   |=  DACData8[0];                    // Assign lower data byte to bottom byte of DACData24
        DACwrite(channel,DACData24);                    // Write data to the selected DAC channel
        break;


      case DACRD :                                      // Read from the DAC
        while ((COMSTA0 & 0x01) != 0x01)                // Wait for Register Address
        {
        }
        DACRegAddr    = COMRX;
        DACData24     = DACread(channel, DACRegAddr);   // Read data from the selected DAC channel
        output1[0]    = (char ) DACData24;              // output1[0] set equal to lower byte of DACData24
        output1[1]    = (char ) (DACData24 >> 8);       // output1[1] set equal to upper byte of DACData24
        for(i=0; i<2; i++)
        {    
          COMTX = output1[i];                           // Put data on Tx buffer
          while((COMSTA0 & 0x40) == 0x00)               // Wait for Tx buffer empty bit 
          {                                             // to be set.
          }
        }      

      break;


      case ADCWR :                                      // Write to the ADC
        ADCRegData8[0]  = 0;
        ADCRegData8[1]  = 0;
        ADCRegData8[2]  = 0;
        ADCRegData24    = 0;
        while ((COMSTA0 & 0x01) != 0x01)                // Wait for Register Address
        {
        }
        ADCRegAddr    = COMRX;
        while ((COMSTA0 & 0x01) != 0x01)                // Wait for Lower Byte
        {
        }
        ADCRegData8[0] = COMRX;
        while ((COMSTA0 & 0x01) != 0x01)                // Wait for Middle Byte
        {
        }
        ADCRegData8[1] = COMRX;
        while ((COMSTA0 & 0x01) != 0x01)                // Wait for Upper Byte
        {
        }
        ADCRegData8[2]  =   COMRX;
        ADCRegData24    |=  ADCRegData8[2];             // Assign ADCRegData[2] to top byte of ADCRegData24
        ADCRegData24    =   ADCRegData24 << 8;          // Shift left 1 byte
        ADCRegData24    |=  ADCRegData8[1];             // Assign ADCRegData[1] to middle byte of ADCRegData24
        ADCRegData24    =   ADCRegData24 << 8;          // Shift left 1 byte
        ADCRegData24    |=  ADCRegData8[0];             // Assign ADCRegData[0] to bottom byte of ADCRegData24
        ADCwrite(channel, ADCRegAddr, ADCRegData24);    // Write data to the selected ADC channel
        break;


      case ADCRD :                                      // Read from the ADC
        while ((COMSTA0 & 0x01) != 0x01)                // Wait for Register Address
        {
        }
        ADCRegAddr     = COMRX;
        ADCRegData24  = ADCread(channel, ADCRegAddr);   // Read data from the selected ADC channel
        output1[0]    = (char ) ADCRegData24;           // output1[0] set equal to the lower byte of ADCRegData24
        output1[1]    = (char ) (ADCRegData24 >> 8);    // output1[1] set equal to the middle byte of ADCRegData24
        output1[2]    = (char ) (ADCRegData24 >> 16);   // output1[2] set equal to the upper byte of ADCRegData24
        for(i=0; i<3; i++)
        {    
          COMTX = output1[i];                           // Put data on Tx buffer
          while((COMSTA0 & 0x40) == 0x00)               // Wait for Tx buffer empty bit 
          {                                             // to be set.
          }
        }      
        break;


      default:
        break;
    }
  }
  return 0;
}
