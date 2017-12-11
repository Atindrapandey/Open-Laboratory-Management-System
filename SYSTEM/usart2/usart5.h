#ifndef __USART5_H_
#define __USART5_H_
#include "sys.h"

#define USART5_SYS      0    //���ñ���汾��0�ǿ�汾��1�ǼĴ����汾
#define USART5_BUFF     200  //�������ݻ���
#define USART5_485_EN   0    //1 ����485���ܣ�0 �ر�485����

void usart5_Configuration(u32 bound);
void usart5_Send(u8 *buf,u32 len); 
u32  usart5_Receive(u8 *buf, u32 buflen);
void usart5_FreeBuff(void);

#endif



