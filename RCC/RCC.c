#include "stm32g0xx.h"
#include "RCC.h"

void set_sysclk64(void){
	RCC->PLLCFGR|=RCC_PLLCFGR_PLLSRC_HSI; //������������ �� HSI16
	
	FLASH->ACR&=~FLASH_ACR_PRFTEN; //��������� ����
	FLASH->ACR|=FLASH_ACR_PRFTEN;
	FLASH->ACR|=FLASH_ACR_LATENCY_2; 
	
	RCC->PLLCFGR&=~RCC_PLLCFGR_PLLM_Msk;//M==1 �������� ������ PLL(HSI16/1=16)
	
	RCC->PLLCFGR&=~RCC_PLLCFGR_PLLN_Msk;//������� �� ������ ������
	RCC->PLLCFGR|=RCC_PLLCFGR_PLLN_3;		//N==8 ���������(PLLvco=16*8=128)
	
	RCC->PLLCFGR|=RCC_PLLCFGR_PLLR_0;  //R==2 �������� PLLCLK=SYSCLK=128/2=64 MHz //��������� ������� SYSCLK
	
	RCC->CFGR|=(1<<RCC_CFGR_HPRE_Pos);//AHB prescaler 1 //���� AHB=SYSCLK/1=64MHz
	
	MODIFY_REG(RCC->CFGR,RCC_CFGR_PPRE,0x00<<RCC_CFGR_PPRE_Pos);//APB prescaler 1;APB = 64/1=64 MHz //������� TIM2 � TIM3 64 MHz
	
	//�������� ��� ��������� ������� ������ ADC
	RCC->PLLCFGR&=~RCC_PLLCFGR_PLLP_Msk;//������� �� ������ ������
	RCC->PLLCFGR|=RCC_PLLCFGR_PLLP_2;//P==4 // ������������  adc 64/4=16 MHz
	RCC->PLLCFGR|=RCC_PLLCFGR_PLLPEN;//PLLPCLK clock output enable
	
	

	RCC->CR |=RCC_CR_PLLON; //��������� pll
	while(RCC->CR & (RCC_CR_PLLRDY));
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;
	RCC->CFGR|=RCC_CFGR_SW_1;//������������ �� PLLRCLK 64 MHz
	
}