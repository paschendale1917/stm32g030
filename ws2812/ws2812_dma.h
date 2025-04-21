//òàéìåð TIM3 íóæíî íàñòðîèòü íà ÷àñòîòó 800 êÃö



#ifndef ws2812_dma_H_
#define ws2812_dma_H_

#include "stm32g0xx.h"
#include "systick_delay.h"


#define F_TIM3 																											64000000 	 	//çàâèñèò îò íàñòðîåê â ñåêöèè RCC
#define F_PWM_TIM3 																										800000 			//÷àñòîòà ØÈÌ 800 000 Ãö

#define NUM_LEDS_MATRIX																										64
#define MATRIX_AMOUNT																										5
#define NUM_LEDS  																										NUM_LEDS_MATRIX*MATRIX_AMOUNT	//êîëè÷åñòâî ñâåòîäèîäîâ
#define NUM_BYTES           																									NUM_LEDS*3
#define BIT_TIME																												0x0050				// 1/64000000=0,00000001562=0,01562 ìèêðîñåêóíäû äëèòñÿ 1 òèê òàéìåðà//ïî äàòàøèòó âðåìÿ äëÿ ïåðåäà÷è 1 èëè 0 äëÿ ws2812 1.25 ìêñ, îòñþäà 1,25/0,01562 =80 òèêîâ òàéìåðà òðåáóåòñÿ äëÿ ïåðåäà÷è îäíîãî áèòà//80==0x0050
#define WS2812_1    																										BIT_TIME/3*2 	//êîëè÷åñòâî òèêîâ äëÿ ïåðåäà÷è 1(èç äàòàøèòà íà 2812 0.4 ìêñ èëè îäíà òðåòü âñåãî âðåìåíè ïîñûëêè áèòà)
#define WS2812_0    																										BIT_TIME/3    //0.85 ìêñ
#define WS2812_RESET 																										BIT_TIME*50
#define RESET_OFFSET																										70
#define DATA_LENTH 				  																						(NUM_LEDS*24+RESET_OFFSET)

#define HALF_TRANSFER_COMPLETE																									DMA1->ISR&DMA_ISR_HTIF1
#define TOTAL_TRANSFER_COMPLETE																									DMA1->ISR&DMA_ISR_TCIF1
#define RESET_HT_FLAG																						  				SET_BIT(DMA1->IFCR,DMA_IFCR_CHTIF1)
#define RESET_TC_FLAG																										SET_BIT(DMA1->IFCR,DMA_IFCR_CTCIF1)


#define WHITE_LED     																										0xFFFFFF
#define OFF_LED      																										0x000000
#define BLUE_LED        																									0x0000FF
#define RED_LED         																									0xFF0000
#define MAGENTA_LED     																									0x00F81F
#define GREEN_LED       																									0x00ff00
#define CYAN_LED        																									0x00FFFF
#define YELLOW_LED      																									0xFFE000
#define BRED_LED        																									0xF81F61
#define GBLUE_LED      																										0x0F70F0
#define GAINSBORO_LED       																									0xDCDCDC
#define LIGHTBLUE_LED   																									0x007D7C
#define ORANGE_LED                                             																					0xFA2000
#define PURPLE_LED																										0x800080
#define TURQUOISE_LED																										0x40E0D0
#define CRIMSON_LED																										0xDC143C

extern uint8_t  color_array[];

extern uint8_t bytes_array[];
extern uint8_t pwm_array[];
extern uint8_t rgb_buffer[];

extern const uint8_t gammaTable[];
extern uint8_t colors[];


void get_high_pwm_data(void);
void get_low_pwm_data(void);
void send_data();
void led_set_RGB(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
void led_set_all_RGB(uint8_t r, uint8_t g, uint8_t b);
void setcolor(uint32_t color);





void init_ws2812(void);
void ws2812_send(void);
void ws2812_setcolor(uint16_t num_leds, uint32_t color);
void ws2812_buff_clear(void);
void init_gpio_ws2812(void);
void ws2812_reset(void);
void leds_off(void);

	


#endif
