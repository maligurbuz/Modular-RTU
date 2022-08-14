#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

#define USE_RUN_LED
#define USE_SIMCARD_DETECTED
#define USE_MODEM_RESET_PIN
#define USE_MODEM_ONOFF_PIN
#define USE_MODEM_WWAN_ONOFF_PIN

char *Modem_Messsage, *At_Command;;
