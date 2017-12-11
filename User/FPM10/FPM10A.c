#include "FPM10A.h"
#include "usart.h"
#include "delay.h"
#include "usart1.h"
  //Ӧ�������
  unsigned char dat[18];
	//FINGERPRINTͨ��Э�鶨��
unsigned char FP_Pack_Head[6] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF};  //Э���ͷ
unsigned char FP_Get_Img[6] = {0x01,0x00,0x03,0x01,0x00,0x05};    //���ָ��ͼ��
unsigned char FP_Templete_Num[6] ={0x01,0x00,0x03,0x1D,0x00,0x21 }; //���ģ������
unsigned char FP_Search[11]={0x01,0x0,0x08,0x04,0x01,0x0,0x0,0x03,0xA1,0x0,0xB2}; //����ָ��������Χ0 - 929
unsigned char FP_Search_0_9[11]={0x01,0x0,0x08,0x04,0x01,0x0,0x0,0x0,0x13,0x0,0x21}; //����0-9��ָ��
unsigned char FP_Img_To_Buffer1[7]={0x01,0x0,0x04,0x02,0x01,0x0,0x08}; //��ͼ����뵽BUFFER1
unsigned char FP_Img_To_Buffer2[7]={0x01,0x0,0x04,0x02,0x02,0x0,0x09}; //��ͼ����뵽BUFFER2
unsigned char FP_Reg_Model[6]={0x01,0x0,0x03,0x05,0x0,0x09}; //��BUFFER1��BUFFER2�ϳ�����ģ��
unsigned char FP_Delet_All_Model[6]={0x01,0x0,0x03,0x0d,0x00,0x11};//ɾ��ָ��ģ�������е�ģ��
volatile unsigned char  FP_Save_Finger[9]={0x01,0x00,0x06,0x06,0x01,0x00,0x0B,0x00,0x19};//��BUFFER1�е��������ŵ�ָ����λ��
unsigned char FP_Delete_Model[10]={0x01,0x00,0x07,0x0C,0x0,0x0,0x0,0x1,0x0,0x0}; //ɾ��ָ����ģ��


volatile unsigned char UART1_FINGERPRINT_RECEVICE_BUFFER[24];

unsigned char FP_duibi_Num[6] ={0x01,0x00,0x03,0x03,0x00,0x07 }; //�Ա�ָ��
unsigned char Zhiwenyan[9] = {0x01,0x07,0x13,0xff,0xff,0xff,0xff,0x04,0x17};
/*------------------ FINGERPRINT������ --------------------------*/


//ָ����֤


//FINGERPRINT_���ָ��ͼ������
void FINGERPRINT_Cmd_Get_Img(void)
{
    unsigned char i;

    for(i=0;i<6;i++) //���Ͱ�ͷ
       
    USART_SendByte(USART1,FP_Pack_Head[i]);
    for(i=0;i<6;i++) //�������� 0x1d
       USART_SendByte(USART1,FP_Get_Img[i]);
//    for(i=0;i<12;i++)//����Ӧ����Ϣ
//    dat[i]=USART_ReceivByte(USART1);
}

//���ָ��ģ��¼��ָ�����������00��ʾ¼��ɹ���02����ָ��03¼��ʧ��
unsigned char test_fig()
{
     unsigned char fig_dat;
     FINGERPRINT_Cmd_Get_Img();
     delay_ms(20);
     fig_dat=dat[9];
     return(fig_dat);
}

//��ͼ��ת��������������Buffer1��
void FINGERPRINT_Cmd_Img_To_Buffer1(void)
{
 	    unsigned char i;
    
	       for(i=0;i<6;i++)    //���Ͱ�ͷ
	         {
    	      USART_SendByte(USART1,FP_Pack_Head[i]);   
   		     }
           
   		   for(i=0;i<7;i++)   //�������� ��ͼ��ת���� ������ ����� CHAR_buffer1
             {
      		   USART_SendByte(USART1,FP_Img_To_Buffer1[i]);
   		     }
//			for(i=0;i<12;i++)//����Ӧ����Ϣ
//			 dat[i]=USART_ReceivByte(USART1);
}

//��ͼ��ת��������������Buffer2��
void FINGERPRINT_Cmd_Img_To_Buffer2(void)
{
     unsigned char i;
           for(i=0;i<6;i++)    //���Ͱ�ͷ
	         {
    	       USART_SendByte(USART1,FP_Pack_Head[i]);   
   		     }
           
   		   for(i=0;i<7;i++)   //�������� ��ͼ��ת���� ������ ����� CHAR_buffer1
             {
      		   USART_SendByte(USART1,FP_Img_To_Buffer2[i]);
   		     }
}

//�Ա�ָ��
void FINGERPRINT_duibi(void)
{
     unsigned char i;
           for(i=0;i<6;i++)    //���Ͱ�ͷ
	         {
    	       USART_SendByte(USART1,FP_Pack_Head[i]);   
   		     }
           
   		   for(i=0;i<6;i++)   //�������� ��ͼ��ת���� ������ ����� CHAR_buffer1
             {
      		   USART_SendByte(USART1,FP_duibi_Num[i]);
   		     }
}

//��BUFFER1 �� BUFFER2 �е�������ϲ���ָ��ģ��
void FINGERPRINT_Cmd_Reg_Model(void)
{
    unsigned char i;    

    for(i=0;i<6;i++) //��ͷ
    {
      USART_SendByte(USART1,FP_Pack_Head[i]);   
    }

    for(i=0;i<6;i++) //����ϲ�ָ��ģ��
    {
      USART_SendByte(USART1,FP_Reg_Model[i]);   
    }

}

//ɾ��ָ��ģ���������ָ��ģ��
void FINGERPRINT_Cmd_Delete_All_Model(void)
{
     unsigned char i;    

    for(i=0;i<6;i++) //��ͷ
      USART_SendByte(USART1,FP_Pack_Head[i]);   

    for(i=0;i<6;i++) //����ϲ�ָ��ģ��
      USART_SendByte(USART1,FP_Delet_All_Model[i]);   
}



//ɾ��ָ��ģ�����ָ��ָ��ģ��
void FINGERPRINT_Cmd_Delete_Model(unsigned int uiID_temp)
{
    volatile unsigned int uiSum_temp = 0;
	unsigned char i;    
	 
	FP_Delete_Model[4]=(uiID_temp&0xFF00)>>8;
	FP_Delete_Model[5]=(uiID_temp&0x00FF);
	
	for(i=0;i<8;i++)
	    uiSum_temp = uiSum_temp + FP_Delete_Model[i];
	
	//UART0_Send_Byte(uiSum_temp);	
			
	FP_Delete_Model[8]=(uiSum_temp&0xFF00)>>8;
	FP_Delete_Model[9]=uiSum_temp&0x00FF;
	 

    for(i=0;i<6;i++) //��ͷ
      USART_SendByte(USART1,FP_Pack_Head[i]);   

    for(i=0;i<10;i++) //����ϲ�ָ��ģ��
      USART_SendByte(USART1,FP_Delete_Model[i]);   
}




//���ָ��ģ������
void FINGERPRINT_Cmd_Get_Templete_Num(void)
{  unsigned int i;
//   unsigned char temp[14];

   for(i=0;i<6;i++) //��ͷ
      USART_SendByte(USART1,FP_Pack_Head[i]);

   //�������� 0x1d
   for(i=0;i<6;i++)
     USART_SendByte(USART1,FP_Templete_Num[i]);
   
  
}





//����ȫ���û�999ö
void FINGERPRINT_Cmd_Search_Finger(void)
{
       unsigned char i;	   
	   for(i=0;i<6;i++)   //������������ָ�ƿ�
           {
    	      USART_SendByte(USART1,FP_Pack_Head[i]);   
   		   }

       for(i=0;i<11;i++)
           {
    	      USART_SendByte(USART1,FP_Search[i]);   
   		   }


}


//����ȫ���û�999ö
void FINGERPRINT_Cmd_Search_Finger_Admin(void)
{
       unsigned char i;	   
	   for(i=0;i<6;i++)   //������������ָ�ƿ�
           {
    	      USART_SendByte(USART1,FP_Pack_Head[i]);   
   		   }

       for(i=0;i<11;i++)
           {
    	      USART_SendByte(USART1,FP_Search_0_9[i]);   
   		   }


}

void FINGERPRINT_Cmd_Save_Finger( unsigned char ucH_Char,unsigned char ucL_Char )
{
           unsigned long temp = 0;
		   unsigned char i;

//           SAVE_FINGER[9]={0x01,0x00,0x06,0x06,0x01,0x00,0x0B,0x00,0x19};//��BUFFER1�е��������ŵ�ָ����λ��

           FP_Save_Finger[5] = ucH_Char;
           FP_Save_Finger[6] = ucL_Char;
           
		   for(i=0;i<7;i++)   //����У���
		   	   temp = temp + FP_Save_Finger[i];
			    
		   FP_Save_Finger[7]=(temp & 0x00FF00) >> 8; //���У������
		   FP_Save_Finger[8]= temp & 0x0000FF;
		   
           for(i=0;i<6;i++)    
    	       USART_SendByte(USART1,FP_Pack_Head[i]);        //���Ͱ�ͷ

           for(i=0;i<9;i++)  
      		   USART_SendByte(USART1,FP_Save_Finger[i]);      //�������� ��ͼ��ת���� ������ ����� CHAR_buffer1
}



//���շ������ݻ���
void FINGERPRINT_Recevice_Data(unsigned char ucLength)
{
  unsigned char i;

  for (i=0;i<ucLength;i++)
     UART1_FINGERPRINT_RECEVICE_BUFFER[i] = USART_ReceiveData(USART1);

}





