/*
 * uart_log.c
 *
 *  Created on: 21 Jul 2016
 *      Author: root
 */

#include "usart_log.h"

void stdio_setup()
 {

     // Turn off buffers, so I/O occurs immediately
     setvbuf(stdin, NULL, _IONBF, 0);
     setvbuf(stdout, NULL, _IONBF, 0);
     setvbuf(stderr, NULL, _IONBF, 0);
 }

 void Usart_Init(void){
	 stdio_setup();
	 string_full=0;
		GPIO_InitTypeDef GPIO_InitStruct; // this is for the GPIO pins used as TX and RX
		USART_InitTypeDef USART_InitStruct; // this is for the USART1 initilization
		NVIC_InitTypeDef NVIC_InitStructure; // this is used to configure the NVIC (nested vector interrupt controller)

		/* enable APB2 peripheral clock for USART1
		 * note that only USART1 and USART6 are connected to APB2
		 * the other USARTs are connected to APB1
		 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		/* enable the peripheral clock for the pins used by
		 * USART1, PB6 for TX and PB7 for RX
		 */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		/* This sequence sets up the TX and RX pins
		 * so they work correctly with the USART1 peripheral
		 */
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // Pins 6 (TX) and 7 (RX) are used
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 			// the pins are configured as alternate function so the USART peripheral has access to them
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// this defines the IO speed and has nothing to do with the baudrate!
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			// this defines the output type as push pull mode (as opposed to open drain)
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// this activates the pullup resistors on the IO pins
		GPIO_Init(GPIOB, &GPIO_InitStruct);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers

		/* The RX and TX pins are now connected to their AF
		 * so that the USART1 can take over control of the
		 * pins
		 */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); //
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

		/* Now the USART_InitStruct is used to define the
		 * properties of USART1
		 */
		USART_InitStruct.USART_BaudRate = 115200;				// the baudrate is set to the value we passed into this init function
		USART_InitStruct.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
		USART_InitStruct.USART_StopBits = USART_StopBits_1;		// we want 1 stop bit (standard)
		USART_InitStruct.USART_Parity = USART_Parity_No;		// we don't want a parity bit (standard)
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
		USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
		USART_Init(USART1, &USART_InitStruct);					// again all the properties are passed to the USART_Init function which takes care of all the bit setting


		/* Here the USART1 receive interrupt is enabled
		 * and the interrupt controller is configured
		 * to jump to the USART1_IRQHandler() function
		 * if the USART1 receive interrupt occurs
		 */
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt

		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		 // we want to configure the USART1 interrupts
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART1 interrupts
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 // this sets the subpriority inside the group
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART1 interrupts are globally enabled
		NVIC_Init(&NVIC_InitStructure);							 // the properties are passed to the NVIC_Init function which takes care of the low level stuff

		// finally this enables the complete USART1 peripheral

		USART_Cmd(USART1, ENABLE);

}
 void USART_puts( volatile char *s){

 	while(*s){
 		// wait until data register is empty
 		while( !(USART1->SR & 0x00000040) );
 		USART_SendData(USART1, *s);
 		*s++;
 	}
 }
 void USART_PutChar( uint8_t ch)
 {
   while(!(USART1->SR & USART_SR_TXE));
   USART1->DR = ch;
 }


 /******************************************************************************
  *
  ******************************************************************************/

 int _read(int file, char *data, int len)
 {
     int bytes_read;

     if (file != STDIN_FILENO)
     {
         errno = EBADF;
         return -1;
     }

     for (bytes_read = 0; bytes_read < len; bytes_read++)
     {
         *data = (char) USART1->DR;
         data++;
     }

     return bytes_read;
 }

 /******************************************************************************
  *
  ******************************************************************************/

 int _write(int file, char *data, int len)
 {
     int bytes_written;

     if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
     {
         errno = EBADF;
         return -1;
     }

     for (bytes_written = 0; bytes_written < len; bytes_written++)

    	 USART_PutChar((uint8_t)data[bytes_written]);



     return bytes_written;
 }

 /******************************************************************************
  *
  ******************************************************************************/

 int _close(int file)
 {
     return -1;
 }

 /******************************************************************************
  *
  ******************************************************************************/

 int _lseek(int file, int ptr, int dir)
 {
     return 0;
 }

 /******************************************************************************
  *
  ******************************************************************************/

 int _fstat(int file, struct stat *st)
 {
     st->st_mode = S_IFCHR;
     return 0;
 }

 /******************************************************************************
  *
  ******************************************************************************/

 int _isatty(int file)
 {
     if ((file == STDOUT_FILENO) ||
         (file == STDIN_FILENO) ||
         (file == STDERR_FILENO))
     {
         return 1;
     }

     errno = EBADF;
     return 0;
 }
