#include "key.h"
#include "delay.h"
void KEY_Init(void)	
{
	/***����һ���ṹ��******/
	GPIO_InitTypeDef GPIO_Init_Structure;
	
	/****��ʼ��GPIOʱ��**/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);
	
//	/****��ʼ��GPIOA.0******/
//	GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_IPD;		//��������
//	GPIO_Init_Structure.GPIO_Pin  = GPIO_Pin_0;
//	GPIO_Init(GPIOA,&GPIO_Init_Structure);
	
	/****��ʼ��GPIOC.13******/
	GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_IPU;		//��������
	GPIO_Init_Structure.GPIO_Pin  = GPIO_Pin_12;
	GPIO_Init(GPIOB,&GPIO_Init_Structure);
		
}


uint16_t KEY_Scand(void)//��ⰴ������
{
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)==1)
	{

//			while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0);	//�ȴ������ͷ�
			return KEY1;
	}
	else 
		return 0;
		
}


