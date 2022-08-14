#include "config.h"

GPIO_InitTypeDef RTU_GPIO_INIT_TYPEDEF;


typedef struct{
	GPIO_InitTypeDef *RTU_InitTypdef;
	GPIO_TypeDef* Port;
	uint32_t RTU_Rcc;
	uint32_t Pin;
	GPIOMode_TypeDef	Direction;
}RTU_GPIO_STRUCT;

RTU_GPIO_STRUCT SIMCARD_DETECTED,RUN_LED;
RTU_GPIO_STRUCT MODEM_RESET_PIN,MODEM_ONOFF_PIN,MODEM_WWAN_ONOFF_PIN;


void RTU_GPIO_INIT(void);
void MODEM_RESET(void);
void MODEM_OFF (void);
void MODEM_ON (void);
void MODEM_WWAN_ON (void);
void MODEM_WWAN_OFF (void);
