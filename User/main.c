#include "stm32f10x.h"
#include "sys.h"
#include "FPM10A.h"
//#include "usart.h"	
//#include "usart1.h"
#include "usart4.h"
#include "usart5.h"
#include "sys.h"
#include "stdio.h"	
#include "string.h"
#include "delay.h"
#include "deal_fg.h"
#include "pwm_config.h"
#include "Run.h"
#include "led.h"
#include "bsp_i2c_ee.h"
#include "Computer.h"
#include "bsp_rtc.h"
#include "key.h"


/************************************************
 ALIENTEK ս��STM32F103������ʵ��0
 ����ģ��
 ע�⣬�����ֲ��е��½������½�ʹ�õ�main�ļ� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
unsigned char yi[12];
extern unsigned char si[11]; 
unsigned char yanz[6]={0x01,0x00,0x03,0x11,0x00,0x15};
unsigned char id_table[4];
int Dump_flag = 0;
struct rtc_time systmtime;
extern unsigned char FG_Key[10];
extern unsigned char FP_Pack_Head[6];  //Э���ͷ
extern unsigned char test_fig(void);
extern   unsigned char dat[18];
extern void uart_init(u32 bound);
extern void My_USART2_Init(void);
extern void My_USART3_Init(void);
extern void USART_SendStr(USART_TypeDef* USARTx,char *data1);
extern void USART_SendByte(USART_TypeDef* USARTx,unsigned char temp);
extern unsigned char USART_ReceivByte(USART_TypeDef* USARTx);
extern void USART_ReceiveStr(USART_TypeDef* USARTx,unsigned char ucLength);
extern void Delete_FG();

extern volatile unsigned char USART_ReceStr[24];
extern unsigned char san[11]; 
extern unsigned char er[10]; 
unsigned char deal_dd[11] = {0xfd,0xfe,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}; 


unsigned char cal_p[16]="abcdeadffasdfdas";
unsigned char cal_test[16]={0};
unsigned int Times11[8];
int Flag =0;
uint8_t count =0;
void cleck_FG(void);
unsigned char sss[11];

void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE))  //�����ж�
		{
			Res = USART_ReceiveData(USART1);
			if(Res==0xef)
				count=0;
			if(count<12)		//������ʮ�����ַ�
			{
				yi[count] = Res;
				count++;
			}
			else
			count=0;
		} 
} 

void send_data(uint8_t *com)
{
    int k;

	for(k=0;k<8;k++)
	{
	  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1,com[k]);
	}
	
}





//���ָ��
int Add_FG(uint8_t num)
{
  int id = 0;	
		while(KEY_Scand()==0);		//�ȴ�ָ�Ʒ���
		delay_ms(500);
		if(KEY_Scand()==1)
		{
			id = num;
			 return Add_New_FG(0x00,id);
		}
}
		struct rtc_time systmtime;
		
 int main(void)
 {	

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		My_USART2_Init();
		My_USART3_Init();
		usart4_Configuration(9600);
      
		pwm_config();
		delay_init();	   
		uart_init(9600); 
		
		KEY_Init();
		LED_Init();
    RTC_NVIC_Config();
   RTC_CheckAndConfig(&systmtime);
   delay_ms(1000);
  if (ee_CheckOk() == 0)
	{
		/* û�м�⵽EEPROM */
		printf("û�м�⵽����EEPROM!\r\n");
				
		while (1);	/* ͣ�� */
	}
		do
		{
			unsigned char i;   

			for(i=0;i<6;i++) //��ͷ
			  USART_SendByte(USART1,FP_Pack_Head[i]);   

			for(i=0;i<10;i++) //����ϲ�ָ��ģ��
			  USART_SendByte(USART1,FG_Key[i]);  
		}while(yi[9]!=0x00&&yi[2]!=0xff);
		delay_ms(1000);
//		san[1]=0x00;
		while(1)
		{ 
			LED0=1;
			if(san[0]==0xef&&san[10]==0xff)	
			{
                unsigned char i; 
                san[1]=san[1]+'0';
				for(i=0;i<11;i++) 
                USART_SendByte(USART2,san[i]);  
				san[0]=0x00;
			}
            
            if(er[0]==0xef)         //������λ����ָ��
            {
                delay_ms(5);
                switch(er[1])
                {
                    case 0xcc:  Join_in();   break;      //ע��
                    case 0xdd:  Delete_P();   break;      //ɾ�������û�
                    case 0xee:  Cortrol();   break;      //���ƶ��
                    case 0xbb:  Send_Zigbee();   break;      //�����ն˽ڵ�
                    case 0xda:  Delete_ALL();   break;      //����������ݿ�
                    case 0xab:  Reser();   break;      //��ѯ
                  default: break;
                }
                er[0] = 0;
            }
			cleck_FG();
      
//			siz = 0;
//			siz = usart4_Receive(buff, sizeof(buff));
//			if(siz)
//			{
//				usart4_Send((u8 *)"usart4 recv:",strlen("usart4 recv:"));
//				usart4_Send(buff,siz);
//			}
		}
        
//       while(1)
//       {
//        unsigned char i;
//        ee_WriteBytes((void*)cal_p,  1, sizeof(cal_p));
//		delay_ms(1);
//		ee_ReadBytes((void*)cal_test,1, sizeof(cal_test));
//        for(i=0;i<16;i++) //����ϲ�ָ��ģ��
//          USART_SendByte(USART2,cal_test[i]);
//            delay_ms(1000);
//           
//       }           
}
 

//����ָ��ʶ��
void cleck_FG(void)
{
		if(KEY_Scand()==1)
		{
			delay_ms(500);
			Allow_going();
			while(KEY_Scand()==1);		//�ȴ���ָ�뿪
		}
}



