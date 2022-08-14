/*
 * LCD.c
 *
 *  Created on: 4 Aug 2016
 *      Author: root
 */


/**
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * |
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * |
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "lcd/lcd.h"

/* Private LCD structure */
typedef struct {
	uint8_t DisplayControl;
	uint8_t DisplayFunction;
	uint8_t DisplayMode;
	uint8_t Rows;
	uint8_t Cols;
	uint8_t currentX;
	uint8_t currentY;
} LCD_Options_t;

/* Private functions */
static void Lcd_InitPins(void);
static void Lcd_Cmd(uint8_t cmd);
static void Lcd_Cmd4bit(uint8_t cmd);
static void Lcd_Data(uint8_t data);
static void Lcd_CursorSet(uint8_t col, uint8_t row);

/* Private variable */
static LCD_Options_t LCD_Opts;

/* Pin definitions */
#define LCD_RS_LOW              GPIO_ResetBits(LCD_RS_PORT,LCD_RS_PIN);//TM_GPIO_SetPinLow(LCD_RS_PORT, LCD_RS_PIN)
#define LCD_RS_HIGH             GPIO_SetBits(LCD_RS_PORT,LCD_RS_PIN);//TM_GPIO_SetPinHigh(LCD_RS_PORT, LCD_RS_PIN)
#define LCD_E_LOW               GPIO_ResetBits(LCD_RS_PORT,LCD_E_PIN);//TM_GPIO_SetPinLow(LCD_E_PORT, LCD_E_PIN)
#define LCD_E_HIGH              GPIO_SetBits(LCD_RS_PORT,LCD_E_PIN);//TM_GPIO_SetPinHigh(LCD_E_PORT, LCD_E_PIN)

#define LCD_Delay(x)            Delay(x)
#define LCD_E_BLINK             LCD_E_HIGH; LCD_Delay(2); LCD_E_LOW; LCD_Delay(2)


/* Commands*/
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

/* Flags for display entry mode */
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

/* Flags for display on/off control */
#define LCD_DISPLAYON           0x04
#define LCD_CURSORON            0x02
#define LCD_BLINKON             0x01

/* Flags for display/cursor shift */
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

/* Flags for function set */
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00

void Lcd_Init(uint8_t cols, uint8_t rows) {
	/* Initialize delay */


	/* Init pinout */
	Lcd_InitPins();

	/* At least 40ms */
	LCD_Delay(40);

	/* Set LCD width and height */
	LCD_Opts.Rows = rows;
	LCD_Opts.Cols = cols;

	/* Set cursor pointer to beginning for LCD */
	LCD_Opts.currentX = 0;
	LCD_Opts.currentY = 0;

	LCD_Opts.DisplayFunction = LCD_4BITMODE | LCD_5x8DOTS | LCD_1LINE;
	if (rows > 1) {
		LCD_Opts.DisplayFunction |= LCD_2LINE;
	}

	/* Try to set 4bit mode */
	Lcd_Cmd4bit(0x03);
	LCD_Delay(4);

	/* Second try */
	Lcd_Cmd4bit(0x03);
	LCD_Delay(4);

	/* Third goo! */
	Lcd_Cmd4bit(0x03);
	LCD_Delay(4);

	/* Set 4-bit interface */
	Lcd_Cmd4bit(0x02);
	LCD_Delay(1);

	/* Set # lines, font size, etc. */
	Lcd_Cmd(LCD_FUNCTIONSET | LCD_Opts.DisplayFunction);

	/* Turn the display on with no cursor or blinking default */
	LCD_Opts.DisplayControl = LCD_DISPLAYON;
	Lcd_DisplayOn();

	/* Clear lcd */
	Lcd_Clear();

	/* Default font directions */
	LCD_Opts.DisplayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	Lcd_Cmd(LCD_ENTRYMODESET | LCD_Opts.DisplayMode);

	/* Delay */
	LCD_Delay(4);
}

void Lcd_Clear(void) {
	Lcd_Cmd(LCD_CLEARDISPLAY);
	LCD_Delay(3);
}

void Lcd_Puts(uint8_t x, uint8_t y, char* str) {
	Lcd_CursorSet(x, y);
	while (*str) {
		if (LCD_Opts.currentX >= LCD_Opts.Cols) {
			LCD_Opts.currentX = 0;
			LCD_Opts.currentY++;
			Lcd_CursorSet(LCD_Opts.currentX, LCD_Opts.currentY);
		}
		if (*str == '\n') {
			LCD_Opts.currentY++;
			Lcd_CursorSet(LCD_Opts.currentX, LCD_Opts.currentY);
		} else if (*str == '\r') {
			Lcd_CursorSet(0, LCD_Opts.currentY);
		} else {
			Lcd_Data(*str);
			LCD_Opts.currentX++;
		}
		str++;
	}
}

void Lcd_DisplayOn(void) {
	LCD_Opts.DisplayControl |= LCD_DISPLAYON;
	Lcd_Cmd(LCD_DISPLAYCONTROL | LCD_Opts.DisplayControl);
}

void Lcd_DisplayOff(void) {
	LCD_Opts.DisplayControl &= ~LCD_DISPLAYON;
	Lcd_Cmd(LCD_DISPLAYCONTROL | LCD_Opts.DisplayControl);
}

void Lcd_BlinkOn(void) {
	LCD_Opts.DisplayControl |= LCD_BLINKON;
	Lcd_Cmd(LCD_DISPLAYCONTROL | LCD_Opts.DisplayControl);
}

void Lcd_BlinkOff(void) {
	LCD_Opts.DisplayControl &= ~LCD_BLINKON;
	Lcd_Cmd(LCD_DISPLAYCONTROL | LCD_Opts.DisplayControl);
}

void Lcd_CursorOn(void) {
	LCD_Opts.DisplayControl |= LCD_CURSORON;
	Lcd_Cmd(LCD_DISPLAYCONTROL | LCD_Opts.DisplayControl);
}

void Lcd_CursorOff(void) {
	LCD_Opts.DisplayControl &= ~LCD_CURSORON;
	Lcd_Cmd(LCD_DISPLAYCONTROL | LCD_Opts.DisplayControl);
}

void Lcd_ScrollLeft(void) {
	Lcd_Cmd(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void Lcd_ScrollRight(void) {
	Lcd_Cmd(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void Lcd_Bitmap(uint8_t *data){
	uint8_t x=0,y=0,loc=0;
	uint8_t buffer[8]={0};

		for (int i = 1; i < 385; i++) {

			if(i%8==0){
				buffer[7]=data[i-1];
				Lcd_CreateChar(loc++,&buffer);
				Lcd_PutCustom(x++,y,loc++);
				if(loc==16)
				loc=0;
				if(x==20)
				{
					y++;
					x=0;
				}
			}
			else{
				buffer[(i%8)-1]=data[i-1];
			}

		}


}

void Lcd_CreateChar(uint8_t location, uint8_t *data) {
	uint8_t i;
	/* We have 8 locations available for custom characters */
	location &= 0x7;
	Lcd_Cmd(LCD_SETCGRAMADDR | (location << 3));

	for (i = 0; i < 8; i++) {
		Lcd_Data(data[i]);
	}
}

void Lcd_PutCustom(uint8_t x, uint8_t y, uint8_t location) {
	Lcd_CursorSet(x, y);
	Lcd_Data(location);
}

void Lcd_Light_Off(){

	GPIO_ResetBits(LCD_LIGHT_PORT, LCD_LIGHT_PIN);

}
void Lcd_Light_On(){

	GPIO_SetBits(LCD_LIGHT_PORT, LCD_LIGHT_PIN);

}
/* Private functions */
static void Lcd_Cmd(uint8_t cmd) {
	/* Command mode */
	LCD_RS_LOW;

	/* High nibble */
	Lcd_Cmd4bit(cmd >> 4);
	/* Low nibble */
	Lcd_Cmd4bit(cmd & 0x0F);
}

static void Lcd_Data(uint8_t data) {
	/* Data mode */
	LCD_RS_HIGH;

	/* High nibble */
	Lcd_Cmd4bit(data >> 4);
	/* Low nibble */
	Lcd_Cmd4bit(data & 0x0F);
}

static void Lcd_Cmd4bit(uint8_t cmd) {
	/* Set output port */

		GPIO_WriteBit(LCD_D7_PORT, LCD_D7_PIN, (cmd & 0x08));
		GPIO_WriteBit(LCD_D6_PORT, LCD_D6_PIN, (cmd & 0x04));
		GPIO_WriteBit(LCD_D5_PORT, LCD_D5_PIN, (cmd & 0x02));
		GPIO_WriteBit(LCD_D4_PORT, LCD_D4_PIN, (cmd & 0x01));
		LCD_E_BLINK;
}

static void Lcd_CursorSet(uint8_t col, uint8_t row) {
	uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};

	/* Go to beginning */
	if (row >= LCD_Opts.Rows) {
		row = 0;
	}

	/* Set current column and row */
	LCD_Opts.currentX = col;
	LCD_Opts.currentY = row;

	/* Set location address */
	Lcd_Cmd(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

static void Lcd_Pin_Init(GPIO_InitTypeDef GPIO_InitDef, GPIO_TypeDef *Port, uint16_t Pin){

	GPIO_InitDef.GPIO_Pin = Pin ;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_PuPd= GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	//Initialize pins
	GPIO_Init(Port, &GPIO_InitDef);
}



static void Lcd_InitPins(void) {

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitDef;

	/* Init all pins */
	Lcd_Pin_Init(GPIO_InitDef,LCD_RS_PORT, LCD_RS_PIN);
	Lcd_Pin_Init(GPIO_InitDef,LCD_E_PORT, LCD_E_PIN);
	Lcd_Pin_Init(GPIO_InitDef,LCD_D4_PORT, LCD_D4_PIN);
	Lcd_Pin_Init(GPIO_InitDef,LCD_D5_PORT, LCD_D5_PIN);
	Lcd_Pin_Init(GPIO_InitDef,LCD_D6_PORT, LCD_D6_PIN);
	Lcd_Pin_Init(GPIO_InitDef,LCD_D7_PORT, LCD_D7_PIN);
	Lcd_Pin_Init(GPIO_InitDef,LCD_LIGHT_PORT, LCD_LIGHT_PIN);

	/* Set pins low */
	GPIO_ResetBits(LCD_RS_PORT, LCD_RS_PIN);
	GPIO_ResetBits(LCD_E_PORT, LCD_E_PIN);
	GPIO_ResetBits(LCD_D4_PORT, LCD_D4_PIN);
	GPIO_ResetBits(LCD_D5_PORT, LCD_D5_PIN);
	GPIO_ResetBits(LCD_D6_PORT, LCD_D6_PIN);
	GPIO_ResetBits(LCD_D7_PORT, LCD_D7_PIN);
	GPIO_ResetBits(LCD_LIGHT_PORT, LCD_LIGHT_PIN);
}
