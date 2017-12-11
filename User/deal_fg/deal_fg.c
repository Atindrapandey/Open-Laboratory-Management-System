#include "deal_fg.h"
#include "led.h"
#include "bsp_i2c_ee.h"
#include "bsp_rtc.h"
#include "usart4.h"
#include "string.h"


extern unsigned int times_S;
extern uint16_t Mytime[5]; 
unsigned char user[255];
unsigned char user_num;
extern unsigned char yi[12];
uint8_t count1 =0;
unsigned char jc_FG[]={0x01,0x00,0x03,0x11,0x00,0x15};
unsigned char Del_FG[]={0x01,0x00,0x03,0x0d,0x00,0x11};
unsigned char FG_Key[10]={0x01,0x00,0x07,0x13,0xaa,0xaa,0xaa,0xaa,0x02,0xc3};
unsigned char succes[]={0xfc,0xfa,0xa1,0xaa,0xaa,0xff,0xfa,0xa1,0xaa,0xaa,0xff};
unsigned char error[] ={0xfc,0xfa,0xa2,0xaa,0xaa,0xff,0xfa,0xa1,0xaa,0xaa,0xff};
extern void USART_SendBytes(USART_TypeDef* USARTx,unsigned char *data);
extern unsigned char cal_p[16];
extern unsigned char cal_test[16];
extern struct rtc_time systmtime;


void Cleck_Password(void)
{
     unsigned char i;    

    for(i=0;i<6;i++) //��ͷ
      USART_SendData(USART1,FP_Pack_Head[i]);   

    for(i=0;i<10;i++) //����ϲ�ָ��ģ��
      USART_SendData(USART1,FG_Key[i]);   
}



/**********�����ָ��*******/
int Add_New_FG(unsigned char ucH_user,unsigned char ucL_user)
{
			uint16_t i = 0;

		       do {	          		     	 
                    FINGERPRINT_Cmd_Get_Img(); //���ָ��ͼ��
                 }
               while (yi[6]!=0x07&&yi[9]!=0x00); //����Ƿ�ɹ��İ���ָ��


			  	 FINGERPRINT_Cmd_Img_To_Buffer1(); //��ͼ��ת��������������Buffer1��
			 	
				
                do{ 
					 delay_ms(300);
				     FINGERPRINT_Cmd_Get_Img(); //���ָ��ͼ��		 
				 }
				 while(yi[6]!=0x07&&yi[9]!=0x00);
      
				 
                do{
					delay_ms(100);
					FINGERPRINT_Cmd_Img_To_Buffer2(); //��ͼ��ת��������������Buffer2�� 
				 }
				 while(yi[6]!=0x07&&yi[9]!=0x00);

                do{ 
					delay_ms(100);
					FINGERPRINT_Cmd_Reg_Model();//ת����������           123 
				 }
				 while(yi[6]!=0x07&&yi[9]!=0x00);

				 delay_ms(100);
				 FINGERPRINT_Cmd_Save_Finger(ucH_user,ucL_user);

				 delay_ms(1200);
				if(yi[6]==0x07&&yi[9]==0x00)			//���ָ��ƥ��ɹ�    �ô���������֤�ɹ�ҳ��
				{
          return 0;
        }
				else 
					return 1 ;
}


void change(int num,unsigned int a[],int length)
{
//  int i=0;
//  int chushu=10;
//  for(i=length-1;i>=0;i--)
//  {
//    a[i]=num%chushu;
//    num=num/10;
//  }
  if(num>=100000000)  //��֤�����ڰ�λ
  {
    
  }
  else
  {
    a[7] = num/10000000;
    a[6] = num%10000000/1000000;
    a[5] = num%1000000/100000;
    a[4] = num%100000/10000;
    a[3] = num%10000/1000;
    a[2] = num%1000/100; 
    a[1] = num%100/10;  
    a[0] = num%10;       
  }
}

extern unsigned int Times11[8];
extern unsigned char Send_Com[10];
//������֤	����������0x39��ʼ��֤������0x38����ͨ����֤���˵���Ϣ
void Allow_going(void)
{
		uint16_t i = 0;
		uint16_t num = 0;		
   unsigned  int times[8];
//			for(i=0;i<11;i++)
//				yi[i] = 0;
			
			for(i=0;i<6;i++)			
					USART_SendByte(USART1,FP_Pack_Head[i]);			//��ָ��ģ�鷢�ͶԱ�ָ������
				for(i=0;i<6;i++)			
					USART_SendByte(USART1,yanz[i]);
			delay_ms(1100);
			if(yi[6]==0x07&&yi[9]==0x00)			//���ָ��ƥ��ɹ�    �ô���������֤�ɹ�ҳ��
			{
				uint32_t temp = yi[11];
				
				char send[11]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xff,0xff,0xff,0xff};
                //��ȡ���ݿ�
				ee_ReadBytes((void*)cal_test,temp*16, sizeof(cal_test));
        delay_ms(10);
                num = (cal_test[0]-'0')*100+(cal_test[1]-'0')*10+(cal_test[2]-'0');
      //���ݿ�ƥ��ɹ�          
            if(temp==num)	
            {
              unsigned char Send_Zig[10]={0xee,0x01,0x01,0x01,0x02,0x01,0x03,0x01,0x04,0x01};
				//��������ʾָ��ʶ��ɹ�
        USART_SendStr(UART4,"page chatter");
				for(i=0; i<3;i++)
					USART_SendByte(UART4,0xff);
                Send_Zig[1] = num;
      //�»�
                if(cal_test[15]=='U') 
                {
                  int result;
                  int b= 1;
                  int time123 = 0;
                  int leiji = 0;
                  //��ȡ��ǰ����
                  uint32_t shij = RTC_GetCounter(); 
                  shij = shij%10000000;
          //��ʷ�ۼ�ʱ��   �ų���һ��
                  if(cal_test[11]>='0'&&cal_test[11]<='9'&&\
                      cal_test[12]>='0'&&cal_test[12]<='9'&&\
                      cal_test[13]>='0'&&cal_test[13]<='9'&&\
                      cal_test[14]>='0'&&cal_test[14]<='9')
                       leiji = ((cal_test[11]-'0')*10+(cal_test[12]-'0'))*60\
                                        +(cal_test[13]-'0')*10+(cal_test[14]-'0');   
                  else
                    leiji  =0;
                  //��ȡ����ʱ��
                  for(i = 10; i>=3; i--)
                  {
                    result += b * (cal_test[i]-'0');
                    b *= 10;
                  }
                  //��������
                    time123 = shij - result;
                  //��ǰ�ۻ�ʱ��
                    time123 = leiji+ time123/60;
          //�洢��
                  //װ���ŵ��洢��
                  cal_p[0] = cal_test[0];
                  cal_p[1] = cal_test[1];
                  cal_p[2] = cal_test[2];
                  //��յ�ǰʱ�䵽�洢��
                  for(i=3;i<11;i++)
                    cal_p[i] = 'X';
        
                  //װ���ۼ�ʱ�䵽�洢��
                  cal_p[11] = ((time123)/60)/10+'0'; //ʱʮλ
                  cal_p[12] = ((time123)/60)%10+'0'; //ʱ��λ
                  cal_p[13] = ((time123)%60)/10+'0'; //��ʮλ
                  cal_p[14] = ((time123)%60)%10+'0'; //�ָ�λ
                  cal_p[15]='D';
                  
                  ee_WriteBytes((void*)cal_p,  num*16, sizeof(cal_p));
      //������
                  Send_Com[0] = 0xef;
                  //��־λ
                  Send_Com[1] = 'D';
                  //���
                  Send_Com[2] = cal_test[0];
                  Send_Com[3] = cal_test[1];
                  Send_Com[4] = cal_test[2];
                  //�ۼ�ʱ��
                  Send_Com[5] = cal_p[11];
                  Send_Com[6] = cal_p[12];
                  Send_Com[7] = cal_p[13]; 
                  Send_Com[8] = cal_p[14];
                  Send_Com[9] = 0xff;
                  for(i=0;i<11;i++)
                    USART_SendByte(USART2,Send_Com[i]);
       //���͵�Zigbee
                  Send_Zig[3] = 0x01;
                  Send_Zig[5] = 0x00;
                  Send_Zig[7] = 0x01;
                 for(i=0;i<10;i++)
                    USART_SendByte(USART3,Send_Zig[i]);
         
                }
 /*******����******************************************************************************/
                else if(cal_test[15]=='D') 
                {
                  //��ȡ��ǰ����
                  uint32_t shij = RTC_GetCounter()%10000000; 
                  LED0=0;			//����
                  //����ǰʱ���������
                  change(shij,times,8);
                  //װ���ŵ��洢��
                  cal_p[0] = cal_test[0];
                  cal_p[1] = cal_test[1];
                  cal_p[2] = cal_test[2];
                  //����ǰʱ�����洢������
                  for(i=3;i<11;i++)
                    cal_p[i] = times[7-i+3]+'0';
      /*********�洢��******************/  
                  if(cal_test[11]>='0'&&cal_test[11]<='9'&&\
                      cal_test[12]>='0'&&cal_test[12]<='9'&&\
                      cal_test[13]>='0'&&cal_test[13]<='9'&&\
                      cal_test[14]>='0'&&cal_test[14]<='9')
                  {
                      cal_p[11] = cal_test[11];
                      cal_p[12] = cal_test[12];
                      cal_p[13] = cal_test[13];
                      cal_p[14] = cal_test[14];
                  }     
                  else
                  {
                      cal_p[11] = '0';
                      cal_p[12] = '0';
                      cal_p[13] = '0';
                      cal_p[14] = '0';
                  }

                  cal_p[15] =  'U';
                  ee_WriteBytes((void*)cal_p,  num*16, sizeof(cal_p));
     /*********������******************/          
                  Send_Com[0] = 0xef;
                  //��־λ
                  Send_Com[1] = 'U';
                  //���
                  Send_Com[2] = cal_test[0];
                  Send_Com[3] = cal_test[1];
                  Send_Com[4] = cal_test[2];
                  //�ۼ�ʱ��
                  Send_Com[5] = cal_p[11];
                  Send_Com[6] = cal_p[12];
                  Send_Com[7] = cal_p[13]; 
                  Send_Com[8] = cal_p[14]; 
                  Send_Com[9] = 0xff;
                  for(i=0;i<11;i++)
                    USART_SendByte(USART2,Send_Com[i]);     
          //���͵�Zigbee
                  Send_Zig[3] = 0x00;
                  Send_Zig[5] = 0x01;
                  Send_Zig[7] = 0x00;
                 for(i=0;i<10;i++)
                    USART_SendByte(USART3,Send_Zig[i]);
                }

              count1=0;
            }
            else  //���ݿ���û��
            {
     /*********������******************/          
                  Send_Com[0] = 0xef;
                  //��־λ
                  Send_Com[1] = 'C';
                  for(i=2;i<10;i++)
                    Send_Com[i] = 'X';
                  //���
                  for(i=0;i<11;i++)
                    USART_SendByte(USART2,Send_Com[i]);
          
        USART_SendStr(UART4,"page chatter");
				for(i=0; i<3;i++)
					USART_SendByte(UART4,0xff);
            }
        }
			else		//���ָ��ƥ�䲻�ɹ�    �ô���������֤ʧ��ҳ��
			{
     /*********������******************/          
                  Send_Com[0] = 0xef;
                  //��־λ
                  Send_Com[1] = 'A';
                  for(i=2;i<10;i++)
                    Send_Com[i] = 'X';
                  //���
                  for(i=0;i<11;i++)
                    USART_SendByte(USART2,Send_Com[i]);
                
        				USART_SendStr(UART4,"page chatterno");
				for(i=0; i<3;i++)
					USART_SendByte(UART4,0xff);
			}

}



void Delete_FG()
{
	uint16_t i = 0;
		for(i=0;i<6;i++)			
			USART_SendByte(USART1,FP_Pack_Head[i]);			//��ָ��ģ�鷢�ͶԱ�ָ������
		for(i=0;i<6;i++)			
			USART_SendByte(USART1,Del_FG[i]);
}

uint16_t Litime[5]; 
uint8_t shijian[8];

int month_day[12] = {	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

void Add_Time(void)
{
		Time_Show(&systmtime);
}

