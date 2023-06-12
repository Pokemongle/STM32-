#include "stm32f10x.h"
#include "usart1.h"
#include "delay.h"
#include "adc.h"

/* ADC1ת���ĵ�ѹֵͨ��DMA��ʽ���䵽SRAM */
extern __IO uint16_t ADC_ConvertedValue;

//�ֲ����������ڱ���ת�������ĵ�ѹ
float ADC_ConvertedValueLocal;

int main(void){
		USART1_Config();
		ADC1_Init();
		SysTick_Init();
		
		printf("\r\n -------����һ��ADC����------\r\n");
		
		while(1){
				ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; // ��ȡת����ADֵ
				printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue);
				printf("\r\n The current AD value = %f V \r\n",ADC_ConvertedValueLocal);
				Delay_us(0xfffff); // ��ʱ
		}
}
