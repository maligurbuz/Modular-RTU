/************************************************************************************************
*												*
* This code is not supported by Analog Device Inc, has not been fully tested or verified,	*
* is for Demo purposes only and is used at ones own risk.		 			*
*		 								08-12-2008	*
************************************************************************************************/

/************************************************************************************************
*												*
* Author	: Analog Devices Inc.								*
*		  Industrial DAC Group								*
*		  Applications									*
*												*
* Date		: December 2008 							        *
*												*
* File		: PLCbios.c									*
*												*
* Hardware	: PLC_DEMO BOARD V04								*
*												*
* Description	: Low-level input/output functions for SPI communication with the ADC and DAC	*
*		  devices on the PLC Board.							*
*		  InitGPIO()									*
*		  DACwrite(BYTE ch, DWORD data)							*
*		  DACread(BYTE ch, BYTE regAddr)						*
*		  ADCDis()									*
*		  ADCChSel(BYTE ch)								*
*		  ResetAD7793(BYTE ch)								*
*		  ADCread(BYTE ch,BYTE RegAdr)							*
*		  ADCwrite(BYTE ch,BYTE RegAdr,DWORD RegData)					*
*		  delayUs(unsigned long us)							*
*												*
************************************************************************************************/

#include "PLCbios.h"
#include "ad7793.h"


// Initialise required GPIO ports
void InitGPIO()
{
  GP1DAT = (unsigned long)((SCLK  + MOSI) <<24);         // Configure P1.4 as SCLK and P1.6 as MOSI
  GP2DAT = (unsigned long)((SPISEL1 + SPISEL2 + SPISEL3 + SPISEL4) <<24); // Configure P2.0, P2.7, P2.1 and P2.2 as o/p
  GP4DAT = ((DACCS1 + DACCS2 + DACCS3 + DACCS4) << 24); // Configure P4.2 - P4.5 as o/p
  
  return;
}



BYTE  DACCSArray    [4] = {DACCS1,DACCS2,DACCS3,DACCS4};// Allows easy reference to the DACCS (LATCH) of each DAC channel
DWORD DAC_MISO_REG  [4] = {MISO5,MISO6,MISO7,MISO8};    // Allows easy reference to the MISO (DOUT) of each DAC channel

// Write to the selected* DAC.    *(1-4)
void DACwrite(BYTE ch, DWORD data)
{
  BYTE  i;
  DWORD Interrupts;

  //  __disable_interrupt();
  Interrupts = IRQEN;									// Save set interrupt values
  IRQCLR = 0xFFFFFFFF;									// Disable all interrupts

  GP4DAT &= ~((DACCS1 + DACCS2 + DACCS3 + DACCS4) <<16);// Bring P4.2 - P4.5 low (LATCH low on all DACs)
  
  for(i=0;i<24;i++)                                     // 24-bit serial output on MOSI
  {
    if(data & (1<<(23-i)))                              // If bit i of data is:
      GP1DAT |= ((MOSI) <<16);                          // 1, set MOSI high
    else
      GP1DAT &= ~((MOSI) <<16);                         // 0, set MOSI low
    delayUs(1);
    GP1DAT |= ((SCLK) <<16);                            // Bring SCLK high
    delayUs(2);
    GP1DAT &= ~((SCLK) <<16);                           // Bring SCLK low
    delayUs(2);
  }

  GP1DAT &= ~((SCLK  + MOSI) <<16);                     // Bring SCLK and MOSI low
  GP4DAT |= ((DACCSArray[ch]) <<16);                    // Bring LATCH on selected DAC channel high
  delayUs(2);
  GP1DAT |= ((SCLK) <<16);                              // Bring SCLK high
  delayUs(5);

  //  __enable_interrupt();
  IRQEN = Interrupts;									// Re-enable interrupts 
   
  return;
}



// Read from the selected* DAC.    *(1-4)
WORD DACread(BYTE ch, BYTE regAddr)
{
  BYTE  i;
  DWORD data = 0x20000;
  DWORD Interrupts;
  
  //  __disable_interrupt();
  Interrupts = IRQEN;									// Save set interrupt values
  IRQCLR = 0xFFFFFFFF;									// Disable all interrupts
  
  data |= regAddr;                                      // Set/Clear the register address bits in the data
  
  DACwrite(ch, data);                                   // Write the data to the selected DAC
  
  data = 0;
  
  switch(ch)                                            // Channels 1 & 2 are on P1, channels 3 & 4 are on P4
  {
    case 0 :                                            // Channel 1
    case 1 :                                            // Channel 2
      for(i=0;i<24;i++)                                 // 24-bit serial output on MOSI
      {
        GP1DAT &= ~((MOSI) <<16);                       // Write NOP
        delayUs(1);
        GP1DAT |= ((SCLK) <<16);                        // Bring SCLK high
        if(GP1DAT & (DAC_MISO_REG[ch]))
          data += (1<<(23-i));                          // Read from MISO (DOUT)
        delayUs(2);
        GP1DAT &= ~((SCLK) <<16);                       // Bring SCLK low
        delayUs(2);
      }
      break;
      
    case 2 :                                            // Channel 3
    case 3 :                                            // Channel 4
      for(i=0;i<24;i++)                                 // 24-bit serial output on MOSI
      {
        GP1DAT &= ~((MOSI) <<16);                       // Write NOP
        delayUs(1);
        GP1DAT |= ((SCLK) <<16);                        // Bring SCLK high
        if(GP4DAT & (DAC_MISO_REG[ch]))
          data += (1<<(23-i));                          // Read from MISO (DOUT)
        delayUs(2);
        GP1DAT &= ~((SCLK) <<16);                       // Bring SCLK low
        delayUs(2);
      }
      break;
      
    default :
    break;
  }
      
  GP1DAT &= ~((SCLK  + MOSI) <<16);                     // Bring MOSI and SCLK low
  GP4DAT |= ((DACCSArray[ch]) <<16);                    // Bring LATCH on selected DAC channel high
  delayUs(2);
  GP1DAT |= ((SCLK) <<16);                              // Bring SCLK high
  delayUs(5);
  
  //  __enable_interrupt();
  IRQEN = Interrupts;									// Re-enable interrupts 

  return data;
}
  


// Disable all ADCs
void ADCDis()
{
  GP2DAT |= ((SPISEL1 + SPISEL2 + SPISEL3 + SPISEL4) <<16); // Bring P2.0, P2.7, 2.1 and 2.2 high (disable all ADCs)
  
  return;  
}



// Enable selected* ADC   *(1-4)
void ADCChSel(BYTE ch)
{
  switch(ch)
  {
    case 0:                                             // Select ADC1
      GP2DAT &= ~(SPISEL1 <<16);                        // Bring P2.0 low (enable ADC1)
       break;
    case 1:                                             // Select ADC2
      GP2DAT &= ~(SPISEL2 <<16);                        // Bring P2.7 low (enable ADC2)
        break;
    case 2:                                             // Select ADC3
      GP2DAT &= ~(SPISEL3 <<16);                        // Bring P2.1 low (enable ADC3)
       break;
    case 3:                                             // Select ADC4
      GP2DAT &= ~(SPISEL4 <<16);                        // Bring P2.2 low (enable ADC4)
        break;
    default:
        break;
  }
  
  return;  
}



// Define AD7793 register size and mode (read/write)
REGISTER_TYPE REG[ALL_REGS]={
  {8, REG_RD},                                          // Status Reg
  {16,REG_RD_WR},                                       // Mode Reg
  {16,REG_RD_WR},                                       // Configuration Reg
  {24,REG_RD},                                          // Data Reg
  {8, REG_RD},                                          // ID Reg
  {8, REG_RD_WR},                                       // IO Reg
  {24,REG_RD_WR},                                       // Offset Reg
  {24,REG_RD_WR},                                       // Full-Scale Reg
};

WORD  ADC_MISO_REG  [4] = {MISO1,MISO2,MISO3,MISO4};    // Allows easy reference to the MISO (DOUT) of each ADC channel

// Reset AD7793
void ResetAD7793(BYTE ch)
{
  long i;

  ADCChSel(ch);                                         // Enable the specified ADC channel
  for(i=0;i<32;i++)                                     // Write 32 consecutive 1s to reset
  {
    GP1DAT |= ((MOSI) <<16);                            // Bring MOSI high
    GP1DAT &= ~((SCLK) <<16);                           // Bring SCLK low
    delayUs(10);
    GP1DAT |= ((SCLK) <<16);                            // Bring SCLK high
    delayUs(10);
  }
  ADCDis();                                             // Disable all ADCs
  
  return;
}



// Read from one of the registers on the AD7793
DWORD ADCread(BYTE ch,BYTE RegAdr)
{
  DWORD data = 0;
  int i;
  DWORD Interrupts;

  //  __disable_interrupt();
  Interrupts = IRQEN;									// Save set interrupt values
  IRQCLR = 0xFFFFFFFF;									// Disable all interrupts

  ADCChSel(ch);                                         // Enable ADC on selected channel

  GP1DAT |= ((SCLK  + MOSI) <<16);                      // Bring SCLK and MOSI high
  delayUs(20);

  data = RW + RS(RegAdr);                               // Next op is read from specified register (RegAdr)

  for(i=0;i<8;i++)                                      // Initial write to Communications Reg
  {
    delayUs(20);
    GP1DAT &= ~((SCLK) <<16);                           // Bring SCLK low
    if(data & (1<<(7-i)))                               // If bit i of data is:
      GP1DAT |= ((MOSI) <<16);                          // 1; bring MOSI high
    else
      GP1DAT &= ~((MOSI) <<16);                         // 0; bring MOSI low
    delayUs(20);
    GP1DAT |= ((SCLK) <<16);                            // Bring SCLK high
    delayUs(20);
  }
  GP1DAT |= ((SCLK ) <<16);                             // Bring SCLK high
  delayUs(20);

  data =0;

  for(i=0;i<REG[RegAdr].len;i++)                        // Read from specified* reg   *(RegAdr)
  {
    delayUs(20);
    GP1DAT &= ~((SCLK) <<16);                           // Bring SCLK low
    delayUs(20);
    if(GP3DAT & (ADC_MISO_REG[ch]))                     // If DOUT is high
      data += (1<<(REG[RegAdr].len-i-1));               // set bit i of data
    delayUs(20);
    GP1DAT |= ((SCLK) <<16);                            // Bring SCLK high
    delayUs(20);
  }

  GP1DAT |= ((SCLK) <<16);                              // Bring SCLK high
  delayUs(20);
    
  ADCDis();                                             // Disable all ADCs
  delayUs(20);
  
  //  __enable_interrupt();
  IRQEN = Interrupts;									// Re-enable interrupts 

  return data;
}



// Write to one of the registers on the AD7793
void ADCwrite(BYTE ch,BYTE RegAdr,DWORD RegData)
{
  DWORD data = 0;
  int i;
  DWORD Interrupts;
  
  //  __disable_interrupt();
  Interrupts = IRQEN;									// Save set interrupt values
  IRQCLR = 0xFFFFFFFF;									// Disable all interrupts

  ADCChSel(ch);                                         // Enable selected ADC
  
  GP1DAT |= ((SCLK  + MOSI) <<16);                      // Bring SCLK and MOSI high
  delayUs(20);

  data = RS(RegAdr);                                    // data specifies which register is to be written to
  delayUs(20);
  
  for(i=0;i<8;i++)                                      // Initial write to Communications Reg
  {
    delayUs(20);
    GP1DAT &= ~((SCLK) <<16);                           // Bring SCLK low
    if(data & (1<<(7-i)))                               // If bit i of data is:
      GP1DAT |= ((MOSI) <<16);                          // 1; bring MOSI high
    else
      GP1DAT &= ~((MOSI) <<16);                         // 0; bring MOSI low
    delayUs(20);
    GP1DAT |= ((SCLK) <<16);                            // Bring SCLK high
    delayUs(20);
  }

  data = RegData;                                       // Set data equal to the data to be written to the specified register

  for(i=0;i<REG[RegAdr].len;i++)                        // Write to register specified by RegAdr
  {
    delayUs(20);
    GP1DAT &= ~((SCLK) <<16);                           // Bring SCLK low
    delayUs(20);
    if(data & (1<<(REG[RegAdr].len-1-i)))               // If bit i of data is:
      GP1DAT |= ((MOSI) <<16);                          // 1; bring MOSI high
    else
      GP1DAT &= ~((MOSI) <<16);                         // 0; bring MOSI low
    delayUs(20);
    GP1DAT |= ((SCLK) <<16);                            // Bring SCLK high
    delayUs(20);
  }

  GP1DAT |= ((SCLK  ) <<16);                            // Bring SCLK high
  delayUs(20);
  
  ADCDis();                                             // Disable all ADCs
  delayUs(20);
  
  //  __enable_interrupt();
  IRQEN = Interrupts;									// Re-enable interrupts 

  return;
}



// us Delay
void delayUs(unsigned long us)
{
  DWORD T2;
  T2 = us * 2;
  while(T2--);
  
  return;
}

