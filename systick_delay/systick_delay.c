
#include "systick_delay.h"

__IO uint32_t SysTick_cnt=0;

void SysTick_init(void){
	// в регистре CTRL(control) поднимаем необходимые флаги, таким образом настраивая системный таймер systick
	// CLKSOURCE - бит регистра CTRL, отвечающий за выбор источника такттирования(либо от частоты МК==AHB, либо от частоты МК/8==AHB/8)
	// TICKINT - бит бит регистра CTRL, разрешающий прерывания
	// ENABLE - бит бит регистра CTRL, включающий таймеp systick
	
	SysTick->CTRL|=SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;
	SysTick->LOAD&=~SysTick_LOAD_RELOAD_Msk;// сбрасываем в нуль значение в регистре LOAD, где хранится значение счетчика, от которого мы будем отсчитывать
	//SysTick->LOAD = F_CPU/(10000-1);// загружаем в регистр LOAD значение, от которого будет вестись отсчет
	SysTick->VAL &=~SysTick_VAL_CURRENT_Msk;// сбрасываем в нуль значание в регистре VAL, который хранит текущее значение счетчика
}

void SysTick_Handler(void){
 if (SysTick_cnt>0){
	 SysTick_cnt--;
 }
}

void _delay_ms(uint32_t ms){
	SysTick->VAL&=~SysTick_VAL_CURRENT_Msk;
	SysTick->LOAD = F_CPU/1000-1;
	SysTick_cnt=ms;
	while(SysTick_cnt);

}


