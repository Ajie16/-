/*******************************************************************************************
* ���ڳ�ʼ��
* ����汾��V1.0
* �������ڣ�2020-8-18
* �������ߣ����
*******************************************************************************************/
#include "stm32f10x.h"
#include "usart.h"
#include "os.h"
extern u16 USART_RX_STA[2];       //����״̬���	  
u8 USART2_RX_BUF[USART_REC_LEN];

/*****************************************************************************
* ��  ����void usart2_init(uint32_t baudrate)
* ��  ��: ����2�ĳ�ʼ��
* ��  ��: bud ������
* ����ֵ����
* ��  ע: ��
*****************************************************************************/
void usart2_init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;

	// ����ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_DeInit(USART2); //��λ����2 -> ����û��

	// ��ʼ�� ���ڶ�ӦIO�� TX-PA2 RX-PA3
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);

	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStrue);

	// ��ʼ�� ����ģʽ״̬
	USART_InitStrue.USART_BaudRate=bound; // ������
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // Ӳ��������
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; // ���� ���� ģʽ��ʹ��
	USART_InitStrue.USART_Parity=USART_Parity_No; // û����żУ��
	USART_InitStrue.USART_StopBits=USART_StopBits_1; // һλֹͣλ
	USART_InitStrue.USART_WordLength=USART_WordLength_8b; // ÿ�η������ݿ��Ϊ8λ
	USART_Init(USART2,&USART_InitStrue);

	USART_Cmd(USART2,ENABLE);//ʹ�ܴ���
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//���������ж�

	// ��ʼ�� �ж����ȼ�
	NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStrue);

}
/*****************************************************************************
* ��  ����void usart_send(uint8_t *data,uint8_t len)
* ��  ��: ���ڷ��ͺ���
* ��  ��: data-�������ݣ�len-�ֳ�
* ����ֵ����
* ��  ע: ��
*****************************************************************************/

void usart2_send(uint8_t *data,uint8_t len)
{
		 
}

void USART2_IRQHandler(void)                	//����2�жϷ������
{
	u8 Res;
	
	#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
		OSIntEnter();    
	#endif
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART2);	//��ȡ���յ�������
		
		if((USART_RX_STA[1]&0x8000)==0)//����δ���
		{

			if(Res=='E')USART_RX_STA[1]|=0x8000;
			else
			{
				USART2_RX_BUF[USART_RX_STA[1]&0X3FFF]=Res ;
				USART_RX_STA[1]++;
				if(USART_RX_STA[1]>(200-1))USART_RX_STA[1]=0;//�������ݴ���,���¿�ʼ����	  
			}		 
			
		}
   } 	
	
	#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
		OSIntExit();  											 
	#endif	
	 
} 



