/**
  ******************************************************************************
  * @file    ff_user_interface.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19/09/2012
  * @brief   This file includes the user application layer
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  */ 
#include "ff_user_interface.h"
#include "diskio.h"
#include "nand_drv.h"
#include "ff.h"
#include <stdio.h>
#include <string.h> /* memcpy */
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"



#define HOST_FAT_FS

uint8_t filenameString[15]  = {0};

#ifdef HOST_EFSL
EmbeddedFileSystem          efs;
EmbeddedFile                file;
EmbeddedFile                fileR;
DirList                     list;
#endif

FATFS fatfs;
FIL file;

uint32_t ImgByteOffset;
uint8_t temp[]                        = "          \n";
ApplicationStateTypedef USBH_USR_ApplicationState = USH_USR_FS_INIT;

extern uint32_t MassBlockCount;
extern uint16_t NandBlockSize;
extern uint16_t RW_PageSize;
/*--------------------------------------------------------------------------

   Module Private Functions and Variables

---------------------------------------------------------------------------*/

static volatile DSTATUS Stat = STA_NOINIT;	/* Disk status */
FRESULT read_files (char* path);


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv		/* Physical drive number (0) */
)
{
    
    Stat &= ~STA_NOINIT;
  
  return Stat;
  
  
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv		/* Physical drive number (0) */
)
{
	if (drv) return STA_NOINIT;		/* Supports only single drive */
	return Stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE drv,	/* Physical drive number (0) */
	BYTE *buff,	/* Pointer to the data buffer to store read data */
	DWORD sector,	/* Start sector number (LBA) */
	UINT count      /* Sector count (1..255) */
)
{
   BYTE status;
	if (drv || !count) return RES_PARERR;
	if (Stat & STA_NOINIT) return RES_NOTRDY;
        
	//NAND_GarbageCollection();

    status = NAND_Read(512*sector, (uint8_t *)buff, count*512);
    
  
  if(status == NAND_OK)
    return RES_OK;
  return RES_ERROR;

}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive number (0) */
	const BYTE *buff,	/* Pointer to the data to be written */
	DWORD sector,		/* Start sector number (LBA) */
	UINT count			/* Sector count (1..255) */
)
{
  BYTE status;
	if (drv || !count) return RES_PARERR;
	if (Stat & STA_NOINIT) return RES_NOTRDY;
	if (Stat & STA_PROTECT) return RES_WRPRT;



    status = NAND_Write(512*sector, (uint8_t*)buff, count*512);
    NAND_GarbageCollection();


  if(status == NAND_OK)
    return RES_OK;
  return RES_ERROR;
}
#endif /* _READONLY == 0 */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL != 0
DRESULT disk_ioctl (
                    BYTE drv,	/* Physical drive number (0) */
                    BYTE ctrl,	/* Control code */
                    void *buff	/* Buffer to send/receive control data */
                      )
{
  DRESULT res;
  
  if (drv) return RES_PARERR;
  
  res = RES_ERROR;
  
  if (Stat & STA_NOINIT) return RES_NOTRDY;
  
  switch (ctrl) {
  case CTRL_SYNC :		/* Make sure that no pending write process */
    
    res = RES_OK;
    break;
    
  case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
    
    *(DWORD*)buff = (DWORD) MassBlockCount;
      res = RES_OK;
    break;
    
  case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
    *(WORD*)buff = RW_PageSize;
    res = RES_OK;
    break;
    
  case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
    
    *(DWORD*)buff = RW_PageSize * NandBlockSize;
    
    break;
    
    
  default:
    res = RES_PARERR;
  }
  
  
  
  return res;
}
#endif /* _USE_IOCTL != 0 */


/**
  * @brief  DisplayBMPImage 
  *         Displays BMP image
  * @param  None
  * @retval None
  */
//void DisplayBMPImage(void)
//{
//  uint8_t imageBuf[1] = {0};
//  uint8_t imageData[4]= {0};
//  uint8_t databyteCnt = 0;
//  UINT numOfBytesToRead = 1;
//  unsigned int bytesRead;
//  uint8_t skipBMPheader = 54;
//  ImgByteOffset = 0;
//
//  /*Set the LCD window*/
//  LCD_SetDisplayWindow(239, 319, 240, 320);
//  LCD_WriteReg(LCD_REG_3, 0x1008);
//  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
//
//  while(skipBMPheader--)
//  {
//    f_read(&file, imageBuf, numOfBytesToRead,&bytesRead);
//  }
//
//  while ((ImgByteOffset < 153600))
//  {
//  f_read(&file, imageBuf, numOfBytesToRead,&bytesRead);
//  ImgByteOffset++ ;
//
//  imageData[databyteCnt] = imageBuf[0] ;
//
//  if ((databyteCnt == 1) )
//  {
//    LCD_WriteRAM(imageData[1] << 8 | imageData[0]);
//  }
//  databyteCnt = 1 - databyteCnt;
//  }
//
//  LCD_WriteReg(LCD_REG_3, 0x1018);
//}
//
///**
//  * @brief  USER_NAND_Application
//  *         Demo application for mass storage
//  * @param  None
//  * @retval Staus
//  */
//int USER_NAND_Application(void)
//{
//  char path[] = "0:/pics";
//
//  unsigned int bytesWritten, bytesToWrite;
//
//  char writeTextBuff[70] = "STM32 Connectivity line Host \
//    Demo application using FAT_FS   ";
//
//  switch(USBH_USR_ApplicationState)
//  {
//  case USH_USR_FS_INIT:
//
//    if ( f_mount( 0, &fatfs ) != FR_OK )
//    {
//      /* fat_fs initialization fails*/
//      return(-1);
//    }
//
//    USBH_USR_ApplicationState = USH_USR_FS_READLIST;
//    break;
//
//  case USH_USR_FS_READLIST:
//
//    USBH_USR_ApplicationState = USH_USR_FS_READFILE;//USH_USR_FS_WRITEFILE;
//    break;
//
//  case USH_USR_FS_WRITEFILE:
//
//    /* Writes a text file, HOSTDEMO.TXT in the disk*/
//
//    /* Register work area for logical drives */
//    f_mount(0, &fatfs);
//
//    if(f_open(&file,
//              "0:Host_Write_Demo.TXT",
//              /*FA_CREATE_NEW*/ FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
//    {
//      /* Write buffer to file */
//      bytesToWrite = 60;
//      f_write (&file, writeTextBuff, bytesToWrite, &bytesWritten);
//
//      /*close file and filesystem*/
//      f_close(&file);
//      f_mount(0, NULL);
//      USBH_USR_ApplicationState = USH_USR_FS_READFILE;
//    }
//
//    else
//    {
//
//      USBH_USR_ApplicationState = USH_USR_FS_READFILE;
//    }
//
//    break;
//
//  case USH_USR_FS_READFILE:
//
//    if ( f_mount( 0, &fatfs ) != FR_OK )
//    {
//      /* fat_fs initialisation fails*/
//      return(-1);
//    }
//    read_files(path);
//
//    break;
//  default: break;
//  }
//  return(0);
//}
//
//FRESULT read_files (char* path)
//{
//  FRESULT res;
//  FILINFO fno;
//  DIR dir;
//  int i;
//  char *fn;
//#if _USE_LFN
//  static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
//  fno.lfname = lfn;
//  fno.lfsize = sizeof(lfn);
//#endif
//  static char longFileName[256];
//  unsigned char extn = 4, count=255;
//  unsigned int x = 0, y = 0;
//
//  res = f_opendir(&dir, path);
//  if (res == FR_OK) {
//    i = strlen(path);
//    for (;;) {
//      res = f_readdir(&dir, &fno);
//      if (res != FR_OK || fno.fname[0] == 0) break;
//      if (fno.fname[0] == '.') continue;
//#if _USE_LFN
//      fn = *fno.lfname ? fno.lfname : fno.fname;
//#else
//      fn = fno.fname;
//#endif
//      if (fno.fattrib & AM_DIR) {
//        res = read_files(path);
//        if (res != FR_OK) break;
//        path[i] = 0;
//      } else {
//      }
//      x = 0;
//      y = 8;
//      extn = 4;
//      count = 255;
//      /*Flushing the longFileName buffer*/
//      while(count--)
//      {
//        longFileName[count] = 0;
//      }
//      longFileName[0] = 0;
//      longFileName[0] = '0';
//      longFileName[1] = ':';
//      longFileName[2] = '/';
//      longFileName[3] = 'p';
//      longFileName[4] = 'i';
//      longFileName[5] = 'c';
//      longFileName[6] = 's';
//      longFileName[7] = '/';
//      while(!(fn[x] == '.'))
//      {
//        longFileName[y] = fn[x];
//        x++ ;
//        y++;
//      }
//      while(extn--)
//      {
//        longFileName[y] = fn[x];
//        x++ ;
//        y++;
//      }
//
//      res = f_open(&file, longFileName, FA_OPEN_EXISTING | FA_READ);
//      DisplayBMPImage();
//      Delay_Routine(100);
//      /*close file and filesystem*/
//      f_close(&file);
//
//
//    }
//  }
//
//  return res;
//}

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
