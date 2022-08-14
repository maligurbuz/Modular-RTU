
#ifndef __SD_SPI_STM32_H__
#define __SD_SPI_STM32_H__

//#include "stm32f2xx.h"

#include "stm32f4xx.h"



// demo uses a command line option for this (see Makefile):
// #define STM32_SD_USE_DMA

#ifdef STM32_SD_USE_DMA
#warning "Information only - using DMA"
#endif

#define USE_MINI_STM32

#if defined(USE_MINI_STM32)
 #define CARD_SUPPLY_SWITCHABLE   0
 #define SOCKET_WP_CP_CONNECTED   0
 #define SPI_SD                   SPI1
 #define GPIO_CS                  GPIOB
 #define RCC_APB2Periph_GPIO_CS   RCC_APB2Periph_GPIOB
 #define GPIO_Pin_CS              GPIO_Pin_6
 #define DMA_Channel_SPI_SD_RX    DMA1_Channel2
 #define DMA_Channel_SPI_SD_TX    DMA1_Channel3
 #define DMA_FLAG_SPI_SD_TC_RX    DMA1_FLAG_TC2
 #define DMA_FLAG_SPI_SD_TC_TX    DMA1_FLAG_TC3
 #define GPIO_SPI_SD              GPIOA
 #define GPIO_Pin_SPI_SD_SCK      GPIO_Pin_5
 #define GPIO_Pin_SPI_SD_MISO     GPIO_Pin_6
 #define GPIO_Pin_SPI_SD_MOSI     GPIO_Pin_7
 #define RCC_APBPeriphClockCmd_SPI_SD  RCC_APB2PeriphClockCmd
 #define RCC_APBPeriph_SPI_SD     RCC_APB2Periph_SPI1
 /* - for SPI1 and full-speed APB2: 72MHz/4 */
 #define SPI_BaudRatePrescaler_SPI_SD  SPI_BaudRatePrescaler_4

#else
#error "unsupported board"
#endif


/* Definitions for MMC/SDC command */
#define CMD0	(0x40+0)	/* GO_IDLE_STATE */
#define CMD1	(0x40+1)	/* SEND_OP_COND (MMC) */
#define ACMD41	(0xC0+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(0x40+8)	/* SEND_IF_COND */
#define CMD9	(0x40+9)	/* SEND_CSD */
#define CMD10	(0x40+10)	/* SEND_CID */
#define CMD12	(0x40+12)	/* STOP_TRANSMISSION */
#define ACMD13	(0xC0+13)	/* SD_STATUS (SDC) */
#define CMD16	(0x40+16)	/* SET_BLOCKLEN */
#define CMD17	(0x40+17)	/* READ_SINGLE_BLOCK */
#define CMD18	(0x40+18)	/* READ_MULTIPLE_BLOCK */
#define CMD23	(0x40+23)	/* SET_BLOCK_COUNT (MMC) */
#define ACMD23	(0xC0+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(0x40+24)	/* WRITE_BLOCK */
#define CMD25	(0x40+25)	/* WRITE_MULTIPLE_BLOCK */
#define CMD55	(0x40+55)	/* APP_CMD */
#define CMD58	(0x40+58)	/* READ_OCR */

/* Card-Select Controls  (Platform dependent) */
#define SELECT()        GPIO_ResetBits(GPIO_CS, GPIO_Pin_CS)    /* MMC CS = L */
#define DESELECT()      GPIO_SetBits(GPIO_CS, GPIO_Pin_CS)      /* MMC CS = H */

/* Manley EK-STM32F board does not offer socket contacts -> dummy values: */
#define SOCKPORT	1			/* Socket contact port */
#define SOCKWP		0			/* Write protect switch (PB5) */
#define SOCKINS		0			/* Card detect switch (PB4) */


void power_on (void);
int USER_NAND_Application(void);

typedef enum
{
   USH_USR_FS_INIT,
   USH_USR_FS_READLIST,
   USH_USR_FS_WRITEFILE,
   USH_USR_FS_READFILE
  
}ApplicationStateTypedef;
#endif	// __SD_SPI_STM32_H__

