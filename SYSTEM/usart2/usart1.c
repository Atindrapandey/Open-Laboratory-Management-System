#include "usart1.h"
#include <string.h>
#define uchar unsigned char

extern int8_t up,left;
extern int Dump_flag;
extern unsigned char id_table[4];
volatile unsigned char USART_ReceStr[24];
// �� USARTx ����һ���ֽ�    x = 1 2 3  
void USART_SendByte(USART_TypeDef* USARTx,unsigned char temp)
{
    USART_SendData(USARTx, temp);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

void USART_SendStr(USART_TypeDef* USARTx,char *data1)
{
    int i;
    for(i=0; i<=strlen(data1);i++)
    {
    USART_SendByte(USARTx,data1[i]);     
    }

}

void USART_SendBytes(USART_TypeDef* USARTx,unsigned char *data)
{
    int i;
    for(i=0; i<=strlen(data);i++)
    {
		USART_SendByte(USARTx,data[i]);     
    }

}

////��ջ�����
//void FreeBuff(uchar *data1)
//{
//    int i;
//		for(i=0;i < strlen(data1) ;i++)
//			data1[i] = 0;


//}



// �� USARTx ��ȡһ���ֽ�        x = 1 2 3  
unsigned char USART_ReceivByte(USART_TypeDef* USARTx)
{
    unsigned char recev;
 while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
    recev = USART_ReceiveData(USARTx);
 return recev;
}

//�Ӵ����ж�ȡһ�����ȵ��ַ���
void USART_ReceiveStr(USART_TypeDef* USARTx,unsigned char ucLength)
{
	  unsigned char i;
		for (i=0;i<ucLength;i++)
			USART_ReceStr[i] = USART_ReceivByte(USARTx);
}


void My_USART3_Init(void)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;	  //���ڶ˿����ýṹ�����
	USART_InitTypeDef USART_InitStructure;	  //���ڲ������ýṹ�����

	//ʹ�� USART3 ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//�򿪴��ڸ���ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //��PB�˿�ʱ��
  	
	//��USART3 Tx�����ͽţ���GPIO����Ϊ���츴��ģʽ   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				  //ѡ���ĸ�IO�� ��ѡ��PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //�趨IO�ڵ�����ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);    				  //��ʼ��GPIOB
	
	//��USART3 Rx�����սţ���GPIO����Ϊ��������ģʽ														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				  //ѡ���ĸ�IO�� ��ѡ��PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);                    //��ʼ��GPIOA
	  
	//����USART3����
	USART_InitStructure.USART_BaudRate = 115200;	                    //���������ã�2400
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�����뷢�Ͷ�ʹ��
	USART_Init(USART3, &USART_InitStructure);                       //��ʼ��USART3
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    //�򿪷����жϺͽ����ж�(�����Ҫ�ж�)
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);  // ʹ��ָ����USART3�����ж�
  	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // ʹ��ָ����USART3�����ж�

	//ʹ�� USART3�� �������
	USART_Cmd(USART3, ENABLE);                             // USART3ʹ��

	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
    USART_ClearFlag(USART3, USART_FLAG_TC);                //�崮��3���ͱ�־
}
uint8_t z,x;    
extern void Run();
volatile unsigned char san[11];  
void USART3_IRQHandler(void)
{
	u8 Res;

	if(USART_GetITStatus(USART3, USART_IT_RXNE))  //�����ж�
		{
			Res = USART_ReceiveData(USART3);
			if(Res==0xef)
				x=0;
			if(x<11)		//������ʮ�����ַ�
			{
				san[x] = Res;
				x++;
			}
			else 
			{	
				x=0;
			}
			
		} 

}
 





void My_USART2_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef    GPIO_InitStructure;	   //���ڶ˿����ýṹ�����
	USART_InitTypeDef   USART_InitStructure;   //���ڲ������ýṹ�����

	//ʹ�� USART2 ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//�򿪴��ڸ���ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);   //��PA�˿�ʱ��

	//��USART2 Tx�����ͽţ���GPIO����Ϊ���츴��ģʽ   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				  //ѡ���ĸ�IO�� ��ѡ��PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //�趨IO�ڵ�����ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);    				  //��ʼ��GPIOA

	//��USART2 Rx�����սţ���GPIO����Ϊ��������ģʽ														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				  //ѡ���ĸ�IO�� ��ѡ��PA3
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);                    //��ʼ��GPIOA
	  
	//����USART2����
	USART_InitStructure.USART_BaudRate = 115200;	                    //���������ã�115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�����뷢�Ͷ�ʹ��
	USART_Init(USART2, &USART_InitStructure);                       //��ʼ��USART2
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	//�򿪷����жϺͽ����ж�(�����Ҫ�ж�)
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  // ʹ��ָ����USART2�����ж�
  	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // ʹ��ָ����USART2�����ж�

	//ʹ�� USART2�� �������
	USART_Cmd(USART2, ENABLE);                             // USART2ʹ��

	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
    USART_ClearFlag(USART2, USART_FLAG_TC);                //�崮��2���ͱ�־
	
}


uint8_t s1;            
unsigned char er[10]; 
void USART2_IRQHandler(void)
{
	u8 Res;

	if(USART_GetITStatus(USART2, USART_IT_RXNE))  //�����ж�
		{
			Res = USART_ReceiveData(USART2);
			if(Res==0xef)
				s1=0;
			if(s1<10)		//������ʮ�����ַ�
			{
				er[s1] = Res;
				s1++;
			}
			else
			s1=0;
		} 
}
 



