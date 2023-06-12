#include "pwm.h"

/** @brief TIM3����źų�ʼ�������ú�����TIM3�ĸ�ͨ���ͻ���PWM�ź����
	* @param void
	*
	*/
void TIM3_PWM_Init(u16 per, u16 pse){
		// GPIO configuration
		GPIO_InitTypeDef GPIO_InitStructure;
		/*ʹ��TIM3ʱ��*/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		/*TIM3 ������ӳ��*/
		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);//TIM3������ӳ�� TIM3_CH2->PB5
		/*GPIOB Config, TIM3 channel 2*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ��������ģʽ
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
		// mode configuration
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
		TIM_OCInitTypeDef TIM_OCInitStructure;

		/* PWM ���ƽ����ֵ */
		u16 CCR2_Val = 500;
		
		/* Time base configuration */
		//��ʱ������per��Ϊһ������
		TIM_TimeBaseStructure.TIM_Period = per;
		//����Ԥ��Ƶ����Ԥ��Ƶ��Ϊ72MHz
		TIM_TimeBaseStructure.TIM_Prescaler = pse;
		//����ʱ�ӷ�Ƶϵ��������Ƶ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
		//���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		
		/* PWM output configuration */
		//����ΪPWMģʽ1
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		//��Ч��ƽΪ�ߵ�ƽ
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
		/* PWM1 Mode configuration: Channel2 */
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
		//����ͨ��2�ĵ�ƽ����ֵ
		//TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
		//ʹ��ͨ��2
		TIM_OC2Init(TIM3, &TIM_OCInitStructure); 
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

		// ʹ��ͨ�� TIM3 ���ؼĴ��� ARR
		TIM_ARRPreloadConfig(TIM3, ENABLE);
		 
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3
}

