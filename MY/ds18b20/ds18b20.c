#include "ds18b20.h"
#include "delay.h"

void DS18B20_IO_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//DQ=PG11������ģʽ
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
}

void DS18B20_IO_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//DQ=PG11���ģʽ
	GPIO_Init(GPIOG,&GPIO_InitStructure);
}

void DS18B20_Reset(void)	   
{                 
	DS18B20_IO_OUT(); // SET PG11 ���ģʽ
	DS_SetL; //����DQ
	Delay_us(750);    // ����750us  ����480us
	DS_SetH; //DQ=1 
	Delay_us(15);     //15US
}


u8 DS18B20_Check(void) 	   // �ȴ��Ƿ���Ӧ
{   
	u8 retry=0;// ��¼ʱ��ı���
	DS18B20_IO_IN();//SET PG11 INPUT ����ģʽ
    while ( DS_PIN &&retry<200)
	{
		retry++;
		Delay_us(1);
	};
	if(retry>=200)return 1;//���200us��û�м�⵽���ͣ�˵��������
	else retry=0;//���ܴ��ڣ������������㣬��Ҫ���ж��ǲ������ʹ��240us
    while ( !DS_PIN && retry < 240)
	{
		retry++;
		Delay_us(1);
	};//�ø�����
	if(retry>=240)return 1;// һֱ���ͳ���240us��˵�����ܲ����ɴ���������������
	
    return 0;
}


u8 DS18B20_Read_Bit(void) // ��һλ
{
	u8 data;
	DS18B20_IO_OUT();//SET PG11 OUTPUT
	DS_SetL; // ��������
	Delay_us(2);
	DS_SetH; 
	DS18B20_IO_IN();//SET PG11 INPUT
	Delay_us(12);
	if(DS_PIN)data=1;
	else data=0;	 
	Delay_us(50);           
	return data;
}


u8 DS18B20_Read_Byte(void)    // ��һ���ֽ�
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}


void DS18B20_Write_Byte(u8 dat)     
{             
	u8 j;
  u8 testb;// ��¼ȡ������ÿһλ����λ��ǰ
	DS18B20_IO_OUT();//SET PG11 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS_SetL;// Write 1
            Delay_us(2);                            
            DS_SetH;
            Delay_us(60);             
        }
        else 
        {
            DS_SetL;// Write 0
            Delay_us(60);             
            DS_SetH;
            Delay_us(2);                          
        }
    }
}


void DS18B20_Start(void)// ds1820 start convert
{   						               
    DS18B20_Reset();	   
		DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
} 

u8 DS18B20_Init(void){
		GPIO_InitTypeDef  GPIO_InitStructure;
 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
		GPIO_Init(GPIOG,&GPIO_InitStructure);
	 
		DS18B20_Reset();
    return DS18B20_Check();// �鿴�Ƿ�����Ӧ
}


float DS18B20_GetTemperture(void)
{
    u16 temp;
	u8 a,b;
	float value;
    DS18B20_Start();                    // ds1820 start convert
    DS18B20_Reset();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert	    
    a=DS18B20_Read_Byte(); // LSB   
    b=DS18B20_Read_Byte(); // MSB   
	temp=b;
	temp=(temp<<8)+a;
  if((temp&0xf800)==0xf800)//�¶�Ϊ����ǰ5ΪȫΪ1
	{
		temp=(~temp)+1;
		value=temp*(-0.0625);
	}
	else
	{
		value=temp*0.0625;	
	}
	return value;    
}
