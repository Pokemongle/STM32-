#include "stm32f10x.h"
#include "delay.h"
#include "exti.h"
#include "key.h"
#include "led.h"
#include "pwm.h"
#include "tempad.h"
#include "usart1.h"

int main(void){
		USART1_Init();	// ����ͨ��1��ʼ��
		Temp_ADC1_Init(); // ͨ��STM32оƬ����ʵ���¶Ȳ���
		SysTick_Init();		// ϵͳʱ�ӳ�ʼ��
		
		printf("\r\n �ڲ��¶ȴ�����ʵ�� \r\n");
		printf( "\r\n Print current Temperature \r\n");
	
		while(1){
				Delay_us(0x1fffee);
				// ���㷽��1���������ֵ
				//Current_Temp= (1.43-ADC_ConvertedValue*3.3/4096)*1000 / 4.3+ 25 ;
				// ���㷽��2���Զ����
				// -V25: 25���Ӧ�ĵ�ѹֵ
				// -Avg_Slope: �¶�����ƽ��б�� ��λ��mV/��
				Current_Temp=(V25-ADC_ConvertedValue)/Avg_Slope+25;
				// 10������ʾ
				printf("\r\n The current temperature= %3d �� \r\n", Current_Temp);
				// 16������ʾ
				//printf("\r\n The current temperature= %04x \r\n", Current_Temp);
		}
}
