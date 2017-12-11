/*--------------------------------------------------------------------
*����ͨ�ö�ʱ��2��pwm����˿�ͨ��
---------------------------------------------------------------------*/

#include "stm32f10x.h"

/*-------------�˿�����----------------*/
void gpio_config()
{
  GPIO_InitTypeDef GPIO_InitStructure;//����ṹ��
	
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA,ENABLE);//��a��ʱ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_6;		//PA6 PA7 ͬʱ����ΪPWM���
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);//��ʼ��
}

/*-------------��ʱ��3ʱ����ͨ������----------------*/
void timebase_config()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//����ʱ���ṹ��
	TIM_OCInitTypeDef       TIM_OCInitStructure;//����ʱ���ṹ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//����ʱ��3ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period=199;//20ms
	TIM_TimeBaseInitStructure.TIM_Prescaler=7199;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OCPolarity= TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);//��ʼ����ʱ��2ͨ��0
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity= TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);//��ʼ����ʱ��2ͨ��1
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);//ccrԤװʹ��
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);//ccrԤװʹ��
	TIM_ARRPreloadConfig(TIM3, ENABLE);//arr���ؼĴ���ʹ��
	TIM_Cmd(TIM3,ENABLE);//ʹ�ܶ�ʱ��3
}

void pwm_config(void)
{
	gpio_config();
	timebase_config();
}
