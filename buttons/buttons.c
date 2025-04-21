/* шаблон дл¤ использовани¤
	switch(readButtonState()){
			case BUTTON_LEFT:
				resetButton();
				break ;
				
			 case BUTTON_RIGHT:
				resetButton();
				break ;
			
      case BUTTON_UP:
				resetButton();
				break ;
			
      case BUTTON_DOWN:
				resetButton();
				break ;
			
			case BUTTON_SELECT:
				resetButton();
				break ;
			
      default: 
				break ;
    }
*/
//таймер TIM1

#include "buttons.h"


uint8_t  shortpress_left=0,
				 shortpress_right=0,
				 shortpress_ok=0,
				 longpress_left=0,
				 longpress_right=0,
				 longpress_ok=0;								 
																	

uint8_t flagPressed=0;
uint16_t cnt_temp=0; 														//на будущее
uint16_t cnt_delay=0;														//счетчик дл¤ задержек


timer tim={.sec=0,
					  .min=0,
					  .hour=0
			};

void buttons_init(void){   
	SET_BIT(RCC->IOPENR,RCC_IOPENR_GPIOBEN); 								//тактирование порта B
	MODIFY_REG(GPIOB->MODER,GPIO_MODER_MODE6,0x00<<GPIO_MODER_MODE6_Pos); 	//mode[0:0] режим входа дл¤ пина PB6
	MODIFY_REG(GPIOB->PUPDR,GPIO_PUPDR_PUPD6,0x01<<GPIO_PUPDR_PUPD6_Pos); 	//подт¤га к питанию

	MODIFY_REG(GPIOB->MODER,GPIO_MODER_MODE7,0x00<<GPIO_MODER_MODE7_Pos);	 //mode[0:0] режим входа дл¤ пина PB7
	MODIFY_REG(GPIOB->PUPDR,GPIO_PUPDR_PUPD7,0x01<<GPIO_PUPDR_PUPD7_Pos);
	
	SET_BIT(RCC->IOPENR,RCC_IOPENR_GPIOCEN);								 //тактирование порта C
	MODIFY_REG(GPIOC->MODER,GPIO_MODER_MODE15,0x00<<GPIO_MODER_MODE15_Pos);  //mode[0:0] режим входа дл¤ пина PC15
	MODIFY_REG(GPIOC->PUPDR,GPIO_PUPDR_PUPD15,0x01<<GPIO_PUPDR_PUPD15_Pos);	

	SET_BIT(RCC->APBENR2,RCC_APBENR2_TIM1EN);

	SET_BIT(TIM1->CR1,TIM_CR1_ARPE);										//Ѕуферизаци¤ регистров таймера включение
	CLEAR_BIT(TIM1->CR1,TIM_CR1_CMS);										//The counter counts up or down depending on the direction bit
	CLEAR_BIT(TIM1->CR1,TIM_CR1_DIR);										//счет вверх
	
	SET_BIT(TIM1->DIER,TIM_DIER_UIE);										//включаем прерывание при переполнении счетчика
	SET_BIT(TIM1->DIER,TIM_DIER_CC1IE);										//включаем прерывание при совпадении в значений в счетном регистре и регистре сравнени¤ CCR1//использовать дл¤ задержек
	SET_BIT(TIM1->CR2,TIM_CR2_CCPC);
	WRITE_REG(TIM1->CCR1,1000);
	WRITE_REG(TIM1->PSC,TIM1_PRESCALE);
	WRITE_REG(TIM1->ARR,TIM1_AUTORELOAD);									//TIM1->ARR = 100; 
	NVIC_EnableIRQ(TIM1_CC_IRQn);        									//глобальное разрешeние прерываний при совпадении
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);								//глобальное разрешение прерываний при переполнении и еще некоторых событи¤х(описано в stm32g030xx.h)
	TIM_EnableCounter(TIM1);
}
 void TIM1_CC_IRQHandler(void){
		 if(READ_BIT(TIM1->SR, TIM_SR_CC1IF)){
		CLEAR_BIT(TIM1->SR,TIM_SR_CC1IF);
	 }
		 cnt_delay++;

}
 

//void _delay_ms_tim1(uint16_t ms){
//	cnt_delay=ms;
//	SET_BIT(TIM1->DIER,TIM_DIER_CC1IE);//включаем прерывание при совпадении в значений в счетном регистре и регистре сравнени¤ CCR1//использовать дл¤ задержек
//	CLEAR_BIT(TIM1->SR,TIM_SR_CC1IF);//сброс флага прерывани¤  по каналу 2 таймера

//	}	 

//обработчик прерывани¤ при переполнении счетного регистра
 void TIM1_BRK_UP_TRG_COM_IRQHandler( void){
	 
	if(READ_BIT(TIM1->SR, TIM_SR_CC1IF)){
		CLEAR_BIT(TIM1->SR,TIM_SR_UIF);										//сброс флага прерывани¤  по каналу 1 таймера
	}


	 static uint8_t count_left = 0;
	 static uint8_t count_right = 0;
	 static uint8_t count_ok = 0;
	
	//button left
	if(!(GPIOC->IDR & GPIO_IDR_ID15)){
	
		if(count_left < 255)  count_left++;
		}
	
	if(count_left>0&& count_left< SHORT_TIMEOUT) {
		if((GPIOC->IDR & GPIO_IDR_ID15)){
			shortpress_left=1;
			count_left=0;
		}
	}
	if(count_left >LONG_TIMEOUT && count_left<255) {
		if((GPIOC->IDR & GPIO_IDR_ID15)){
			longpress_left=1;
			count_left=0;
		}
	}
	//button ok
		if(!(GPIOB->IDR & GPIO_IDR_ID6)){	
			if(count_ok < 255)  count_ok++;
		}
		
	if(count_ok>0&& count_ok< SHORT_TIMEOUT) {
		if((GPIOB->IDR & GPIO_IDR_ID6)){
			shortpress_ok=1;
			count_ok=0;
		}
	}
	if(count_ok >LONG_TIMEOUT && count_ok<255) {
		if((GPIOB->IDR & GPIO_IDR_ID6)){
			longpress_ok=1;
			count_ok=0;
		}
	}
	//button right
		if(!(GPIOB->IDR & GPIO_IDR_ID7)){		
			if(count_right < 255)  count_right++;
		}
		
	if(count_right>0&& count_right< SHORT_TIMEOUT) {
		if((GPIOB->IDR & GPIO_IDR_ID7)){
			shortpress_right=1;
			count_right=0;
		}
	}
	if(count_right >LONG_TIMEOUT && count_right<255) {
		if((GPIOB->IDR & GPIO_IDR_ID7)){
			longpress_right=1;
			count_right=0;
		}
	}
}





uint8_t readButtonState(void){
	if(shortpress_ok){
		return BUTTON_SELECT;          
	}else 
		if(longpress_right){
			return BUTTON_UP;
	}else 
		if(longpress_left){
			return BUTTON_DOWN;
	}else 
		if(shortpress_left){
			return BUTTON_LEFT;
	}
	else 
		if(shortpress_right){
			return BUTTON_RIGHT;
	}else 
		if(longpress_ok){
		return BUTTON_MAINMENU;          
	}
		return BUTTON_NOTHING;
}

void resetButton(void){
	
	switch(readButtonState()){
		case BUTTON_SELECT:
			shortpress_ok=0;
			break;
		case BUTTON_UP :
			longpress_right=0;
		  break;
		case BUTTON_DOWN:
			longpress_left=0;
			break;
		case BUTTON_LEFT :
			shortpress_left=0;
			break;
		case BUTTON_RIGHT :
			shortpress_right=0;
			break;
		case BUTTON_MAINMENU:
			longpress_ok=0;
			break;
	}
 } 
