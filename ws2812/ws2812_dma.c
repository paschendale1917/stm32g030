//������� ���������� �����.������� ������. 
//������� ��� �������: pwm_array ��� ��� �������� ��������� ���, rgb_buffer ��� �������� �������� ����� �� ������� 3 �����(r,g,b) �� 1 ���������(� ���� 320 ����������� *3=960 ����)
//����������� ����� ����� ������ ������� �� ������� 800 ���(�������� ��� ��� ������� ��� ����������� 2812 � ������������ �����) � �� ����� ��� ��� ���������� 
//����������� ��� � ��������� ������, �������� ����������� �� �������� �������� ������ � �� ��������� ��������, ��� ���� ������ ���� � pwm_array � �������� ��� �������� ����� 48 ����(8 ��� �� ����*3 �����=24 ����; 24*2=48 ���. ������ ��� ����� ����������� � ���� ����������)
//�������� ��������� ������ pwm_array ����������, ��������� �������� �� rgb_buffer ��� ���� ������ ������.
//�������� �������� �� ���, �������� ���� � �������. ����� ���������� "�������" �������� ������ pwm_array(������ ���������), ����� ���������� ���������� HT, � ��� ���������� ���������� ��� ���������� "�������" ����� ������� pwm_array
//����� ����� �� ����������, �������� "������" �������� ������� pwm_array, ����� ���� � ����������, �� ��� �� ��������� ��������(TC) � � ���� ���������� ���������� ���������� "������" ����� �������
//��� ��� ���  � ��������� ������, �� ����� ���������� �������� "�������" �������� pwm_array, � ��� ��� ����� �������� ���������� ��� ��� �������� �����, ����� ������, ��� ������ 4 ������� � ��� �����
//�������� ����� �������, ���� �� �������� �� ��������
//�� ��� ������ ����� ����, ��� ���������� ���������� � ����� �������(320 ����)
//����� �������� ����������� ��� ���� ����������� ��������� ������� ������ ����� ��� �����������-�� �������� ������� ����� ������� ������ ��� n ����������� ������� ����� � ����� �� ����� >50 ���. 
//��� ����� �� ���� ������ ��� ���� ����������� �� ����������� ��� == 0(�� �� ���� ��� ������ 2812, ��� ���������� =1/3 �� ������� �������)
//��� ��� �����, ����� �������� ����� � ���� ������ � ������ �� ����������� ����������� �������� ��� ���� �������
//setcolor(ORANGE_LED);
//send_data();
//https://www.thevfdcollective.com/blog/stm32-and-sk6812-rgbw-led

//������������������ �����(g030 �� ������� 320 ������ � �������������� ����� ������,f103  � ��� 20 �� ���������� �������� �������)
//������ ���������� ������ ��� NUM_LED �����������		
//	ws2812_setcolor(NUM_LED, GREEN_LED);
//	ws2812_send();
//� ������ ������� �� ������������ ���������� ������� ��� ������������ ��������.
//�������� ����������� � ����� ������ � �������
// ��� ������������� �� ���������� �� ��������� ��������(��������� ����� ��������)
//������ ������������� ��� � �������� 800 ���
//����������� ���, ����� ������������ � CCR, ����� �������� ������ ��������� ����������, ��� ��������� ����� ���������� � ����������� ���� � �������

#include "ws2812_dma.h"

uint8_t  color_array[DATA_LENTH]={0};
volatile uint32_t num_bytes=NUM_BYTES;
uint8_t bus_ready=0;
uint8_t rgb_buffer[NUM_BYTES]={0,};
uint8_t pwm_array[2*3*8]={0,}; //����� ��� 2� ������ �� 24 ���� �� ����
uint8_t ht_flag=0;
uint8_t ct_flag=0;
uint32_t px=3;

const uint8_t gammaTable[] = {
    0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


void init_gpio_ws2812(void){
	//TIM3 CH1
	SET_BIT(RCC->IOPENR,RCC_IOPENR_GPIOAEN);
	MODIFY_REG(GPIOA->MODER,GPIO_MODER_MODE6,0x02<<GPIO_MODER_MODE6_Pos); 						//�������������� ������� ������
	CLEAR_BIT(GPIOA->OTYPER,GPIO_OTYPER_OT6);																					//push-pull
	CLEAR_BIT(GPIOA->PUPDR,GPIO_PUPDR_PUPD6);																					//No pull-up, pull-down
	MODIFY_REG(GPIOA->OSPEEDR,GPIO_OSPEEDR_OSPEED6,0x03<<GPIO_OSPEEDR_OSPEED6_Pos);
	SET_BIT(GPIOA->AFR[0],GPIO_AFRL_AFSEL6_0);																				//AFR[0]==AFRL; AFR[1]==AFRH;AFSEL6_0 -���� PA6, ������������� ������� AF1, ������� ��� � AFSEL ������������� � �������

}


void init_ws2812(void){
	
	init_gpio_ws2812();
	SET_BIT(RCC->AHBENR,RCC_AHBENR_DMA1EN);
	SET_BIT(RCC->APBENR1,RCC_APBENR1_TIM3EN);
	
	//��������� �������
	SET_BIT(TIM3->CR1,TIM_CR1_ARPE);																									// ����� ������������ ��� �������� ARR
	CLEAR_BIT(TIM3->CR1,TIM_CR1_CMS);																									//���� ���� �� ����, ���� �� �������� ARR
	CLEAR_BIT(TIM3->CR1,TIM_CR1_DIR);																									//���� �����
	MODIFY_REG(TIM3->CCMR1,TIM_CCMR1_OC1M,0x06<<TIM_CCMR1_OC1M_Pos);									//��� mode 1 �� ������  ������ �������
	SET_BIT(TIM3->CCMR1,TIM_CCMR1_OC1PE);																							//����������� �������� CCR1:����� �������� �� ������ ���������  ��� ������ ������� ����������
	SET_BIT(TIM3->CCER,TIM_CCER_CC1E);																								//��������� ������� ������������ ����� ����������������

	//��������� DMA
	
	
	 DMA1_Channel1->CPAR = (uint32_t)(&TIM3->CCR1); 												 //���� �����
	 DMA1_Channel1->CMAR = (uint32_t)pwm_array; 														 //������ ����� ������ ��� ��������
	 DMA1_Channel1->CNDTR = 48;															 								 //�����o��� ���������� ������������ ������	
  
  SET_BIT(DMA1_Channel1->CCR,DMA_CCR_DIR);  															 //1 - �� ������ � ���������
	SET_BIT(DMA1_Channel1->CCR,DMA_CCR_TCIE); 															 // ���������� ��� �� ��������� �������� ������
	SET_BIT(DMA1_Channel1->CCR,DMA_CCR_HTIE); 															 // ���������� ��� �� �������� �������� ������
	CLEAR_BIT(DMA1_Channel1->CCR,DMA_CCR_MEM2MEM| DMA_CCR_PINC);						 //����� MEM2MEM ��������\��������� ������ ��������� ��������
	SET_BIT(DMA1_Channel1->CCR,DMA_CCR_MINC); 															 //�������� ��������� ������ ������
	SET_BIT(DMA1_Channel1->CCR,DMA_CCR_CIRC); 															 //��������� �����
	MODIFY_REG(DMA1_Channel1->CCR,DMA_CCR_PL, 0x00 << DMA_CCR_PL_Pos); 			 //��������� ������
  MODIFY_REG(DMA1_Channel1->CCR,DMA_CCR_MSIZE, 0x00 << DMA_CCR_MSIZE_Pos); //����������� ������ � ������ 8 ���
  MODIFY_REG(DMA1_Channel1->CCR,DMA_CCR_PSIZE, 0x01 << DMA_CCR_PSIZE_Pos); //����������� �������� ������ 16 ��� 
  DMAMUX1_Channel0->CCR=(32 << DMAMUX_CxCR_DMAREQ_ID_Pos); 								 //�������� ���� �������������� DMA ��� ������ 1 ������� TIM3(�� ������� � ������� DMAmux)
	SET_BIT(TIM3->DIER,TIM_DIER_CC1DE);																			 //���������� ������� DMA � ������  ������������ ����� ���������  ������ 1
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

void reset_pwmbuffer(void){
		for (uint8_t i=0;i<48;i++){
			pwm_array[i] 		= WS2812_0;
			}
}

void bus_reset(void){
	for(uint8_t i=0;i<48;i++){                                                 //��������� ������ ����������� ������
	 pwm_array[i] = 0;																
	}
	bus_ready=0; 																															 //����� ��������� ���� ������, ������� �����, ������������� ���� ��������� ���� � ����. � ���� ������ ���������� ��������� ��������� ���������� ������ pwm_array
	CLEAR_BIT(TIM3->CR1,TIM_CR1_CEN);                                          //������� ����� � �������
	CLEAR_BIT(DMA1_Channel1->CCR,DMA_CCR_EN);																	 //���������� �����a ���
}


void send_data(void){
	if(!bus_ready){																															 //���� ���� �� ������
		bus_ready=1;                                                               //������������� ���� ��������� ���� 
		reset_pwmbuffer();
		for (uint8_t i=0;i<8;i++){
			pwm_array[i] 		= (((rgb_buffer[0] << i) & 0x80)) ? WS2812_1 : WS2812_0; //�������������� ���������� ������ ���
			pwm_array[i+8] 	= (((rgb_buffer[1] << i) & 0x80)) ? WS2812_1 : WS2812_0;
			pwm_array[i+16] = (((rgb_buffer[2] << i) & 0x80)) ? WS2812_1 : WS2812_0;
			pwm_array[i+24] = (((rgb_buffer[3] << i) & 0x80)) ? WS2812_1 : WS2812_0;
			pwm_array[i+32] = (((rgb_buffer[4] << i) & 0x80)) ? WS2812_1 : WS2812_0;
			pwm_array[i+40] = (((rgb_buffer[5] << i) & 0x80)) ? WS2812_1 : WS2812_0;
		}
		
	 DMA1->IFCR=1;                                    												  //�������  ����� ���������� DMA
   DMA1_Channel1->CNDTR = 48;                                                 //�����o��� ���������� ������������ ������
   WRITE_REG(TIM3->ARR,BIT_TIME);                                             //��������� ������� ���
   WRITE_REG(TIM3->CNT,0);                                                    //������� �������� �������� 
   WRITE_REG(TIM3->CCR1,0);                                                   //��������� ���������� ��� 0, ����� ����� ���� � ���������� ���������(�� ���������)
   SET_BIT(TIM3->CR1,TIM_CR1_CEN);                                            //������ ����� � �������
   SET_BIT(DMA1_Channel1->CCR,DMA_CCR_EN);																	  //��������� �����a ��� ��� �������� ������������ ������ ���
  
	}		
}


void DMA1_Channel1_IRQHandler(void){
	if(HALF_TRANSFER_COMPLETE){	
		for (uint8_t i=0;i<8;i++){
			pwm_array[i] 		= (((rgb_buffer[px] << i) & 0x80)) ? WS2812_1 : WS2812_0;
			pwm_array[i+8] 	= (((rgb_buffer[px+1] << i) & 0x80)) ? WS2812_1 : WS2812_0;
			pwm_array[i+16] = (((rgb_buffer[px+2] << i) & 0x80)) ? WS2812_1 : WS2812_0;
		}
		RESET_HT_FLAG;
		}
	
	if(TOTAL_TRANSFER_COMPLETE){	
		for (uint8_t i=0;i<8;i++){
			pwm_array[i+24] = (((rgb_buffer[px] << i) & 0x80)) ? WS2812_1 : WS2812_0;
			pwm_array[i+32] = (((rgb_buffer[px+1] << i) & 0x80)) ? WS2812_1 : WS2812_0;
			pwm_array[i+40] = (((rgb_buffer[px+2] << i) & 0x80)) ? WS2812_1 : WS2812_0;
		}
		RESET_TC_FLAG;		
	 }
	
  px+=3; 																																				//������� ��� �������������� ���� � ������� rgb_buffer//��� ������� ���������� ��� �����
	if(px>(NUM_BYTES)){	 
		bus_reset();
		px=3; 																																		 //������ ��� ������� �������� � ������� send_data(), ������� ���� ���������� � 3//� ��� �� ��������� pwm_buffer ��� �������������� ��������
	}
	
}

void led_set_RGB(uint16_t index, uint8_t r, uint8_t g, uint8_t b) {
  rgb_buffer[3 * index    ] = g;
  rgb_buffer[3 * index + 1] = r;
  rgb_buffer[3 * index + 2] = b;
 
}

void led_set_all_RGB(uint8_t r, uint8_t g, uint8_t b) {
  for(uint16_t i = 0; i < NUM_LEDS; i++) led_set_RGB(i, r, g, b);
}

void setcolor(uint32_t color){  //HEX
	uint8_t g,r,b;
	r=(uint8_t)(color>>16);
	g=(uint8_t)(color>>8);
	b=(uint8_t)color;
	for(uint16_t i = 0; i < NUM_LEDS; i++) led_set_RGB(i, r, g, b);
	
}

//void clear_display(void) {
//  for(uint16_t i = 0; i < NUM_LEDS; i++) led_set_RGB(i, 0, 0, 0);
//	send_data();
//}

//void policecolor_breathe(void){  //HEX
//	for(uint8_t g=0;g<255;g++){
//		for(uint16_t i = 0; i <NUM_LEDS/2; i++) led_set_RGB(i, g, 0, 0);
//		for(uint16_t j = NUM_LEDS/2; j <NUM_LEDS; j++) led_set_RGB(j, 0, 0, 0);
//			send_data();
//		_delay_ms(1);
//	}
//	for(uint8_t g=255;g>0;g--){
//		for(uint16_t i = 0; i <NUM_LEDS/2; i++) led_set_RGB(i, g, 0, 0);
//		for(uint16_t j = NUM_LEDS/2; j <NUM_LEDS; j++) led_set_RGB(j, 0, 0, 0);
//			send_data();
//		_delay_ms(1);
//	}
//	for(uint8_t g=0;g<255;g++){
//		for(uint16_t i = 0; i <NUM_LEDS/2; i++) led_set_RGB(i, 0, 0, 0);
//		for(uint16_t j = NUM_LEDS/2; j <NUM_LEDS; j++) led_set_RGB(j, 0, 0, g);
//			send_data();
//		_delay_ms(1);
//	}
// for(uint8_t g=255;g>0;g--){
//		for(uint16_t i = 0; i <NUM_LEDS/2; i++) led_set_RGB(i, 0, 0, 0);
//		for(uint16_t j = NUM_LEDS/2; j <NUM_LEDS; j++) led_set_RGB(j, 0, 0, g);
//		send_data();
//		_delay_ms(1);
//	}
//}

//void policecolor_blink(void){
//	for(uint8_t k=0;k<6;k++){
//		for(uint16_t i = 0; i <NUM_LEDS/2; i++) led_set_RGB(i, 255, 0, 0);
//		for(uint16_t i = NUM_LEDS/2; i <NUM_LEDS; i++) led_set_RGB(i, 0, 0, 0);
//		send_data();
//		_delay_ms(40);
//		clear_display();
//		_delay_ms(40);
//	}

//	for(uint8_t k=0;k<6;k++){
//		for(uint16_t i = 0; i <NUM_LEDS/2; i++) led_set_RGB(i, 0, 0, 0);
//		for(uint16_t i = NUM_LEDS/2; i <NUM_LEDS; i++) led_set_RGB(i, 0, 0, 255);
//		send_data();
//		_delay_ms(40);
//		clear_display();
//		_delay_ms(40);
//	}

//}

//void fakepolicecolor_blink(void){
//	for(uint8_t k=0;k<5;k++){
//		for(uint16_t i = 0; i <NUM_LEDS/2; i++) led_set_RGB(i, 0, 255, 0);
//		for(uint16_t i = NUM_LEDS/2; i <NUM_LEDS; i++) led_set_RGB(i, 0, 0, 0);
//		send_data();
//		_delay_ms(40);
//		clear_display();
//		_delay_ms(40);
//	}

//	for(uint8_t k=0;k<5;k++){
//		for(uint16_t i = 0; i <NUM_LEDS/2; i++) led_set_RGB(i, 0, 0, 0);
//		for(uint16_t i = NUM_LEDS/2; i <NUM_LEDS; i++) led_set_RGB(i, 0xFA, 0x20, 0);
//		send_data();
//		_delay_ms(40);
//		clear_display();
//		_delay_ms(40);
//	}

//}


//void fillwhite(void){
//	setcolor(WHITE_LED);
//	send_data();
//}






















//����� ���� ���� ����� ��������� ��-�� ����, ��� ����������� ������ ����� �����
//320 ������ ��� ����� �������� ��� g030 �� ��� ����(256������ ��� 4 ������� 8*8 �������� ���������)

void ws2812_send(void){
	if(bus_ready){
		
	 bus_ready=0;
	 DMA1->IFCR=1;                                    //�������  ����� ���������� DMA
   DMA1_Channel1->CPAR = (uint32_t)(&TIM3->CCR1);                            //���� �����
   DMA1_Channel1->CMAR = (uint32_t)color_array;                              //������ ����� ������ ��� ��������
   DMA1_Channel1->CNDTR = sizeof(color_array);                               //�����o��� ���������� ������������ ������
   WRITE_REG(TIM3->ARR,BIT_TIME);                                            //��������� ������� ���
   WRITE_REG(TIM3->CNT,0);                                                    //������� �������� �������� 
   WRITE_REG(TIM3->CCR1,0);                                                  //��������� ���������� ��� 0, ����� ����� ���� � ���������� ���������(�� ���������)
   SET_BIT(TIM3->CR1,TIM_CR1_CEN);                                           //������ ����� � �������
   SET_BIT(DMA1_Channel1->CCR,DMA_CCR_EN);                                    //��������� �����a ���
	}
}

//�����������������, ���� ����� ���������������
//����� o�������� �������� ������� ������ ������ ���������� ���������� �� ��������� �������� ������
//void DMA1_Channel1_IRQHandler(void){
//  CLEAR_BIT(TIM3->CR1,TIM_CR1_CEN);                //������� �����  � �������
//  CLEAR_BIT(DMA1_Channel1->CCR,DMA_CCR_EN);       //���������� ������ ���  ��� ����������� ��� ���������
//  DMA1->IFCR=1;                                    //�������  ����� ���������� DMA
//  tick_dma++;
//	bus_ready=0;
//}



 
void ws2812_buff_clear(void){

  for(uint16_t i = 0;i<NUM_LEDS*24; i++)
    color_array[i] = WS2812_0;
	 
	for(uint16_t i = NUM_LEDS*24;i<NUM_LEDS*24+RESET_OFFSET; i++)
    color_array[i] = 0;
	
	ws2812_send();
 }
 
void leds_off(void){
 ws2812_setcolor(NUM_LEDS,OFF_LED);
 ws2812_send();
 }

 void ws2812_setcolor(uint16_t num_leds, uint32_t color){  //���� � hex
 uint8_t tmp=0;
 for(uint8_t u=num_leds;u>0;u--){
	//Blue 
	tmp = (uint8_t) color;
		for(uint8_t i=0; i<8; i++){
			if(tmp & 0x80){
				color_array[u*24-i-1] = WS2812_1;
				}
				else{
					color_array[u*24-i-1] = WS2812_0;
				}
    tmp<<=1;
			}
	//Green 
	tmp =color>>8;
	for(uint8_t i=0; i<8; i++){
    if(tmp & 0x80){
      color_array[u*24-i-1-8] = WS2812_1;
		}
    else{
      color_array[u*24-i-1-8] = WS2812_0;
		}
    tmp<<=1;
  }
	//Red 
  tmp = color>>16;
  for(uint8_t i=0; i<8; i++){
    if(tmp & 0x80){
      color_array[u*24-i-1-16] = WS2812_1;
		}
    else{
      color_array[u*24-i-1-16] = WS2812_0;
		}
    tmp<<=1;
		}

	
	}
}
 
void start_effect(uint8_t delay){
	
	for(uint8_t i=0;i<NUM_LED;i++){
		ws2812_buff_clear();
		ws2812_setcolor(NUM_LED-i-1,GBLUE_LED );//
		ws2812_send();
		_delay_ms(delay);
		}
	for(uint8_t i=0;i<NUM_LED;i++){
		//ws2812_buff_clear();
		ws2812_setcolor(i,ORANGE_LED );//
		ws2812_send();
		_delay_ms(delay);
		}
	for(uint8_t i=0;i<NUM_LED;i++){
		ws2812_setcolor(i,BLACK_LED );//
		ws2812_send();
		_delay_ms(delay);
		}
	}