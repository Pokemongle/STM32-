#include "adc.h"

__IO uint16_t ADC_ConvertedValue; // 12λAD��ʱ����ֵ

void ADC1_Init(void){
		ADC1_GPIO_Config();
		ADC1_Mode_Config();
}

static void ADC1_GPIO_Config(void){
		GPIO_InitTypeDef GPIO_InitStructure;
		/* 1. clock */
		/* Enable DMA clock */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		/* Enable ADC1 and GPIOA clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
		
		/* 2. structure */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_Init(GPIOA, &GPIO_InitStructure);// ����ʱ������������
	
}

static void ADC1_Mode_Config(void){
		DMA_InitTypeDef DMA_InitStructure;
		ADC_InitTypeDef ADC_InitStructure;
	
		/*-----------DMA channel1 configuration-----------*/
		DMA_DeInit(DMA1_Channel1);
	
		/* 2. structure */
		/* �������ַ��ADC��ַ */
		DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
		/* �ڴ��ַ */
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;
		/* ����Ϊ������Դ */
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		/* �����С DMA_BufferSize = 1 */
		DMA_InitStructure.DMA_BufferSize = 1;
		/* �����ַ���� */
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		/* �ڴ��ַ���� */
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
		/* �������ݵ�λ ����*/
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		/* �ڴ����ݵ�λ ����*/
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		/* DMAģʽ��ѭ������ */
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		/* ���ȼ����� */
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		/* ��ֹ�ڴ浽�ڴ�Ĵ��� */
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA1_Channel1, &DMA_InitStructure);
		
		/* 3. Enable */
		/* Enable DMA channel1 */
		DMA_Cmd(DMA1_Channel1, ENABLE);
		
		/*------------ADC1 Configuration---------------*/
		/* ADC structure*/
		/* ����ADCģʽ */
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		/* ��ֹɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ� */
		ADC_InitStructure.ADC_ScanConvMode = DISABLE ;
		/* ��������ת��ģʽ����ͣ�ؽ���ADCת�� */
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
		/* ��ʹ���ⲿ����ת�� */
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		/* �ɼ������Ҷ��� */
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		/* Ҫת����ͨ����Ŀ */
		ADC_InitStructure.ADC_NbrOfChannel = 1;
		ADC_Init(ADC1, &ADC_InitStructure);
		
		/* clock configuration, PCLK2 DIV8, 9Hz */
		RCC_ADCCLKConfig(RCC_PCLK2_Div8);
		/* ����ADC��ͨ��0Ϊ55.5���������� */
		ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
		
		/* Enable configuration*/
		/* Enable ADC1 DMA*/
		ADC_DMACmd(ADC1, ENABLE);
		/* Enable ADC1 */
		ADC_Cmd(ADC1, ENABLE);
		
		/* ��λУ׼�Ĵ��� */
		ADC_ResetCalibration(ADC1);
		/* �ȴ�У׼�Ĵ�����λ��� */
		while(ADC_GetResetCalibrationStatus(ADC1));
		/* ADCУ׼ */
		ADC_StartCalibration(ADC1);
		/* �ȴ�У׼��� */
		while(ADC_GetCalibrationStatus(ADC1));
		
		/* ����û�в����ⲿ����������ʹ���������ADCת�� */
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
