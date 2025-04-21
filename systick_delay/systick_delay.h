// для работы в теле int main (void)  прописать SysTick_init();


#ifndef systick_delay_H_
#define systick_delay_H_

#define F_CPU 64000000UL //частота микроконтроллера

#include "stm32g030xx.h"

void SysTick_init(void);
void _delay_ms(uint32_t);
void SysTick_Handler(void); //обработчик прерывания

extern uint8_t flag;

#endif /* systick_delay_H_ */