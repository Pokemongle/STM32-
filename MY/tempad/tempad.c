#include "tempad.h"

__IO uint16_t ADC_ConvertedValue; // �ɼ�����12λADֵ
double Current_Temp;// �������¶�ֵ

void Temp_ADC1_Init(void){
		ADC1_GPIO_Config();
		ADC1_Mode_Config();
}

void ADC1_GPIO_Config(void){
		// Enable DMA clock 
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		// Enable ADC1 clock and GPIOC clock  
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}

void ADC1_Mode_Config(void){
		DMA_InitTypeDef DMA_InitStructure;
		ADC_InitTypeDef ADC_InitStructure;
		
		/*-----------DMA channel1 configuration-----------*/
		DMA_DeInit(DMA1_Channel1);
		
		/* structure */
		// �������ַ��ADC��ַ  
		DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
		// �ڴ��ַ  
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;
		// ����Ϊ������Դ  
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		// �����С DMA_BufferSize = 1  
		DMA_InitStructure.DMA_BufferSize = 1;
		// �����ַ����  
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		// �ڴ��ַ����  
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
		// �������ݵ�λ ���� 
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		// �ڴ����ݵ�λ ���� 
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		// DMAģʽ��ѭ������  
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		// ���ȼ�����  
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		// ��ֹ�ڴ浽�ڴ�Ĵ���  
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA1_Channel1, &DMA_InitStructure);
		/* Enable DMA */
		// Enable DMA channel1  
		DMA_Cmd(DMA1_Channel1, ENABLE);
	
		/*------------ADC1 Configuration---------------*/
		/* ADC structure*/
		// ����ADCģʽ  
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		// ����ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�  
		ADC_InitStructure.ADC_ScanConvMode = ENABLE ;
		// ��������ת��ģʽ����ͣ�ؽ���ADCת��  
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
		// ��ʹ���ⲿ����ת�����������������  
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		// �ɼ������Ҷ���  
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		// Ҫת����ͨ����Ŀ  
		ADC_InitStructure.ADC_NbrOfChannel = 1;
		ADC_Init(ADC1, &ADC_InitStructure);
		
		// ADC clock configuration, PCLK2 DIV8, 9Hz 
		RCC_ADCCLKConfig(RCC_PCLK2_Div8);
		// ����ADC��ͨ��IN16�����ò���ʱ��  
		ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5);
		
		/* Enable configuration */
		// ʹ���¶ȴ��������ڲ��ο���ѹ
		ADC_TempSensorVrefintCmd(ENABLE);
		// Enable ADC1 DMA 
		ADC_DMACmd(ADC1, ENABLE);
		// Enable ADC1  
		ADC_Cmd(ADC1, ENABLE);
		
		// ��λУ׼�Ĵ���  
		ADC_ResetCalibration(ADC1);
		// �ȴ�У׼�Ĵ�����λ���  
		while(ADC_GetResetCalibrationStatus(ADC1));
		// ADCУ׼  
		ADC_StartCalibration(ADC1);
		// �ȴ�У׼���  
		while(ADC_GetCalibrationStatus(ADC1));
		
		// ����û�в����ⲿ����������ʹ���������ADCת��  
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
