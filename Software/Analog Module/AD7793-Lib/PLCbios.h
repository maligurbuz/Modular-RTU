/************************************************************************************************
*												*
* This code is not supported by Analog Device Inc, has not been fully tested or verified,	*
* is for Demo purposes only and is used at ones own risk.		 			*
*		 								24-11-2008	*
************************************************************************************************/

#ifndef  PLCbios_h
#define  PLCbios_h

//#include "ioADuC7027.h" // The inclusion of this header file is compiler dependent.
#include "ADuC7027.h"
#include "string.h"
#include "stdio.h"
//#include <intrinsic.h>  // The inclusion of this header file is compiler dependent.

typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;

#define SCLK    (1 <<4) // P1.4
#define MOSI    (1 <<6) // P1.6

#define DACCS1  (1 <<2) // P4.2
#define DACCS2  (1 <<3) // P4.3
#define DACCS3  (1 <<4) // P4.4
#define DACCS4  (1 <<5) // P4.5

#define SPISEL1 (1 <<0) // P2.0
#define SPISEL2 (1 <<7) // P2.7
#define SPISEL3 (1 <<1) // P2.1
#define SPISEL4 (1 <<2) // P2.2

#define MISO1   (1 <<0) // P3.0
#define MISO2   (1 <<1) // P3.1
#define MISO3   (1 <<2) // P3.2
#define MISO4   (1 <<3) // P3.3
#define MISO5   (1 <<2) // P1.2
#define MISO6   (1 <<3) // P1.3
#define MISO7   (1 <<1) // P4.1
#define MISO8   (1 <<0) // P4.0

extern  void  InitGPIO(void);
extern  void  DACwrite(BYTE ch, DWORD DACcode);
extern  WORD  DACread(BYTE ch, BYTE regAddr);
extern  void  ADCDis(void);
extern  void  ADCChSel(BYTE ch);
extern  DWORD ADCread(BYTE ch,BYTE RegAdr);
extern  void  ResetAD7793(BYTE ch);
extern  void  ADCwrite(BYTE ch,BYTE RegAdr,DWORD RegData);
extern  void  delayUs(unsigned long us);

#endif
