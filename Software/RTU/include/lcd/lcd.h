#include "main.h"




/* Control pins */
/* RS - Register select pin */
#define LCD_RS_PORT			GPIOE
#define LCD_RS_PIN			GPIO_Pin_14
/* E - Enable pin */
#define LCD_E_PORT			GPIOE
#define LCD_E_PIN			GPIO_Pin_11

/* D4 - Data 4 pin */
#define LCD_D4_PORT			GPIOF
#define LCD_D4_PIN			GPIO_Pin_12
/* D5 - Data 5 pin */
#define LCD_D5_PORT			GPIOD
#define LCD_D5_PIN			GPIO_Pin_10
/* D6 - Data 6 pin */
#define LCD_D6_PORT			GPIOB
#define LCD_D6_PIN			GPIO_Pin_13
/* D7 - Data 7 pin */
#define LCD_D7_PORT			GPIOB
#define LCD_D7_PIN			GPIO_Pin_12
/* Light - Light Pin */
#define LCD_LIGHT_PORT			GPIOE
#define LCD_LIGHT_PIN			GPIO_Pin_12


void Lcd_Light_On(void);
void Lcd_Light_Off(void);

void Lcd_Init(uint8_t cols, uint8_t rows);

/**
 * @brief  Turn display on
 * @param  None
 * @retval None
 */
void Lcd_DisplayOn(void);

/**
 * @brief  Turn display off
 * @param  None
 * @retval None
 */
void Lcd_DisplayOff(void);

/**
 * @brief  Clears entire LCD
 * @param  None
 * @retval None
 */
void Lcd_Clear(void);

/**
 * @brief  Puts string on lcd
 * @param  x location
 * @param  y location
 * @param  *str: pointer to string to display
 * @retval None
 */
void Lcd_Puts(uint8_t x, uint8_t y, char* str);

/**
 * @brief  Enables cursor blink
 * @param  None
 * @retval None
 */
void Lcd_BlinkOn(void);

/**
 * @brief  Disables cursor blink
 * @param  None
 * @retval None
 */
void Lcd_BlinkOff(void);

/**
 * @brief  Shows cursor
 * @param  None
 * @retval None
 */
void Lcd_CursorOn(void);

/**
 * @brief  Hides cursor
 * @param  None
 * @retval None
 */
void Lcd_CursorOff(void);

/**
 * @brief  Scrolls display to the left
 * @param  None
 * @retval None
 */
void Lcd_ScrollLeft(void);

/**
 * @brief  Scrolls display to the right
 * @param  None
 * @retval None
 */
void Lcd_ScrollRight(void);

/**
 * @brief  Creates custom character
 * @param  location: Location where to save character on LCD. LCD supports up to 8 custom characters, so locations are 0 - 7
 * @param *data: Pointer to 8-bytes of data for one character
 * @retval None
 */
void Lcd_CreateChar(uint8_t location, uint8_t* data);

/**
 * @brief  Puts custom created character on LCD
 * @param  location: Location on LCD where character is stored, 0 - 7
 * @retval None
 */
void Lcd_PutCustom(uint8_t x, uint8_t y, uint8_t location);
