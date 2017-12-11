//                            _ooOoo_  
//                           o8888888o  
//                           88" . "88  
//                           (| -_- |)  
//                            O\ = /O  
//                        ____/`---'\____  
//                      .   ' \\| |// `.  
//                       / \\||| : |||// \  
//                     / _||||| -:- |||||- \  
//                       | | \\\ - /// | |  
//                     | \_| ''\---/'' | |  
//                      \ .-\__ `-` ___/-. /  
//                   ___`. .' /--.--\ `. . __  
//                ."" '< `.___\_<|>_/___.' >'"".  
//               | | : `- \`.;`\ _ /`;.`/ - ` : | |  
//                 \ \ `-. \_ __\ /__ _/ .-` / /  
//         ======`-.____`-.___\_____/___.-`____.-'======  
//                            `=---='  
//  
//         .............................................  
//                  ���汣��             ����BUG 
//          ��Ի:  
//                 д��¥��д�ּ䣬д�ּ������Ա��  
//                 ������Աд�������ó��򻻾�Ǯ��  
//                 ����ֻ���������������������ߣ�  
//                 ��������ո��գ����������긴�ꡣ  
//                 ��Ը�������Լ䣬��Ը�Ϲ��ϰ�ǰ��  
//                 ���۱������Ȥ���������г���Ա��  
//                 ����Ц��߯��񲣬��Ц�Լ���̫����  
//                 ��������Ư���ã��ĸ���ó���Ա��  
//////////////////////////////////////////////////////////

/*
*******************************************************************************
*	                                  
*	ģ������ : usartģ��    
*	�ļ����� : usart4.c
*	˵    �� : ʵ�ִ���3�ӿڵ����ݽ����뷢�ͣ�֧��232ģʽͨ�š�
*              485ģʽͨ�ţ�ͨ������usart3.h�ļ���ʵ�ֲ�ͬ��ͨ�ŷ�ʽ����
*			   485ͨ�ŵ�����£�����IO�ø����Լ���ʵ���������Ӧ�ĸĶ���
*
*******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
//#include "stm32_config.h"
#include <string.h>
#include "usart4.h"
#include "delay.h"

/* ��󻺴� */
#define MAXRECV USART4_BUFF
static u8 RX_BUFF[MAXRECV]={0};  //���ջ���
static u32 RX_SIZ=0;               //���յ������ݳ���  

#if (USART4_485_EN == 1)
#define RS485_TX_EN	PDout(4)	   //485ģʽ����.0,����;1,����.
#endif

/*********************************************************
  * @function  usart4_Configuration
  * @role      USART4���ڳ�ʼ��
  * @input     bound:������
  * @output    None
  * @return    None
  ********************************************************/
#if USART4_SYS
void usart4_Configuration(u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)((72/2)*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	
	RCC->APB2ENR|=1<<4;   //ʹ��PORTA��ʱ��  
	RCC->APB1ENR|=1<<19;  //ʹ�ܴ���ʱ�� 
	GPIOC->CRH&=0XFFFF00FF;//IO״̬����
	GPIOC->CRH|=0X00008B00;//IO״̬����

#if USART4_485_EN
	/* 485IO������ */	
#endif

	RCC->APB1RSTR|=1<<19;   //��λ����1
	RCC->APB1RSTR&=~(1<<19);//ֹͣ��λ	   	   
	//����������
 	UART4->BRR=mantissa; // ����������	 
	UART4->CR1|=0X200C;  //1λֹͣ,��У��λ.
	  //���ʹ���˽���
	//ʹ�ܽ����ж�
	UART4->CR1|=1<<8;    //PE�ж�ʹ��
	UART4->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,UART4_IRQChannel,2);//��2��������ȼ� 
}

#else
void usart4_Configuration(u32 bound)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* config UART4 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//PC10 TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);    
	
	//PC11 RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

#if (USART4_485_EN == 1)
	//RE PD.4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		 //�����������IO		
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);	
#endif

	/* UART4 mode config */
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

    USART_Init(UART4, &USART_InitStructure); //��ʼ������
  
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn; //ʹ�ܴ���4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
    
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ��� 
	USART_ClearFlag(UART4, USART_FLAG_TC);       // ���־
	RX_SIZ = 0;

#if (USART4_485_EN == 1)
	RS485_TX_EN=0;			//Ĭ��Ϊ����ģʽ
#endif
}
#endif


uint8_t s;            
unsigned char si[11]; 
/*********************************************************
  * @function  UART4_IRQHandler
  * @role      ���ڽ����ж�
  * @input     None
  * @output    None
  * @return    None
  ********************************************************/
void UART4_IRQHandler(void)
{
	u8 buff = 0;
#if USART4_SYS
	if(UART4->SR&(1<<5))//���յ�����
	{	 
		if(RX_SIZ < MAXRECV){
			buff = UART4->DR;//��¼���յ���ֵ
			if(buff || RX_SIZ){
				RX_BUFF[RX_SIZ++]  = buff;
			}
		}		 									     
	}
#else
 	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) //���յ�����
	{	 	 
		if(RX_SIZ < MAXRECV){
			buff = USART_ReceiveData(UART4);//��¼���յ���ֵ
			if(buff || RX_SIZ){
				RX_BUFF[RX_SIZ++]  = buff;
			}
		} 
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);    //����жϱ�־.
	}

	if(USART_GetFlagStatus(UART4,USART_FLAG_ORE)==SET)//���?
	{
		USART_ClearFlag(UART4,USART_FLAG_ORE);//��SR
		USART_ReceiveData(UART4);//��DR?
	}
#endif
}

/*********************************************************
  * @function  usart4_Send
  * @role      ����len���ֽ�.
  * @input     Ҫ���͵������볤�ȣ�
               (Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ���� MAXRECV ���ֽ�)
  * @output    None
  * @return    None
  ********************************************************/
void usart4_Send(u8 *buf,u32 len)
{
	u32 t;
#if (USART4_485_EN == 1)
	RS485_TX_EN=1;
#endif
	
#if USART4_SYS
	for(t=0;t<len;t++)
	{
		while((UART4->SR&0X40)==0);//ѭ������,ֱ���������   
		UART4->DR = (u8) buf[t]; 	
	}
	while((UART4->SR&0X40)==0);//�ȴ����ͽ���
#else	
  	for(t=0;t<len;t++)		//ѭ����������
	{
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);	  
		USART_SendData(UART4,buf[t]);
	}	 
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
	while(USART_GetFlagStatus(UART4, USART_FLAG_TXE)==RESET){//����һ���ַ���ɲ����ж�
		USART_ClearITPendingBit(UART4 ,USART_IT_TXE);//�巢���ж�
	}
#endif
	
#if (USART4_485_EN == 1)
	RS485_TX_EN=0;
#endif
}

/*********************************************************
  * @function  usart4_Receive
  * @role      RS232��ѯ���յ�������
  * @input     buf:���ջ����׵�ַ���뻺�泤��
  * @output    None
  * @return    ���յ����ݳ���
  ********************************************************/
u32 usart4_Receive(u8 *buf, u32 buflen)
{
	u32 rxlen=RX_SIZ;
	u32 i=0;
	u32 ret = 0;        //Ĭ��Ϊ0		
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==RX_SIZ && rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;(i < rxlen) && (i < (buflen - 1));i++)
		{
			buf[i]=RX_BUFF[i];
			RX_BUFF[i] = 0;			
		}		
		buf[i]='\0';
		ret=i;	//��¼�������ݳ���
		RX_SIZ=0;		//����
	}
	return ret;
}

/*********************************************************
  * @function  usart4_FreeBuff
  * @role      ��ջ����е�����
  * @input     None
  * @output    None
  * @return    None
  ********************************************************/
void usart4_FreeBuff(void)
{
	u32 rxlen=RX_SIZ;
	u32 i=0;
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	while(rxlen)
	{
		if(rxlen==RX_SIZ && rxlen)//���յ�������,�ҽ��������
		{
			for(i=0;(i<rxlen) && (i < (MAXRECV-1)) ;i++){
				RX_BUFF[i] = 0;
			}	
			RX_SIZ=0;		//����
		}
		delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
		rxlen=RX_SIZ;
	}
}



