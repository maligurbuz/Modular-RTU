#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stdio.h"
#include "errno.h"
#include "sys/stat.h"
#include "sys/unistd.h"

#define MAX_STRLEN 1024 // this is the maximum string length of our string in characters
char *received_string; // this will hold the recieved string
uint32_t str_len;
uint8_t string_full;
 void Usart_Init(void);
 void USART_puts( volatile char *s);
 void USART_PutChar( uint8_t ch);
typedef enum{
	CREATE_FILE=0,
	OPEN_FILE,
	APPEND_FILE,
	CLOSE_FILE,
	READ_FILE,
	FORMAT_FILE,
	MOUNT_FILE,
	INFO_FILE,
	DELETE_FILE,
	READDIR_FILE,
	IDLE_FILE
}
FILER;
typedef enum{
	MENU_CREATE=0,
	MENU_OPEN,
	MENU_DELETE,
	MENU_APPEND,
	MENU_READ,
	MENU_CLOSE,
	MENU_FORMAT,
	MENU_MOUNT,
	MENU_INFO,
	MENU_READDIC,
	MENU_IDLE,
	MENU_PROCCES,
	MENU_COMPLETE

}DIRECTION_M;
typedef enum{
	CMD=3,
	C_DATA,
	C_END,
	C_OK,
	C_ERROR,
	C_IDLE,
	C_PROC,
	C_Data_End

}CMD_IN;

CMD_IN CMD_STATUS;
CMD_IN Command;
DIRECTION_M MENU_DIR;
FILER USART_FILER;
