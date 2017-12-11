#ifndef __USART4_H_
#define __USART4_H_
#include "sys.h"

#define USART4_SYS      0    //���ñ���汾��0�ǿ�汾��1�ǼĴ����汾
#define USART4_BUFF     11  //�������ݻ���
#define USART4_485_EN   0    //1 ����485���ܣ�0 �ر�485����

void usart4_Configuration(u32 bound);
void usart4_Send(u8 *buf,u32 len); 
u32  usart4_Receive(u8 *buf, u32 buflen);
void usart4_FreeBuff(void);

#endif


