#include "stm32f4xx.h"

uint32_t systick_cnt;

extern uint32_t s_cnt;

void My_Systick_Config(uint32_t reload_value)
{
	
    SysTick->LOAD |= reload_value-1;	// set reload value
    SysTick->VAL |= SysTick->LOAD;	// clear current value
    SysTick->CTRL |= 1<<2;	// clock source : AHB
    SysTick->CTRL |= 1<<1;	// enable interrupt
    SysTick->CTRL |= 0x01;	// enable counter

}

//void OSTickISR()
//{
//	
//	systick_cnt++;
//	if(systick_cnt == 100)
//	{
//		systick_cnt = 0;
//		s_cnt ++ ;
//	}
//}

//void delay_ms(uint32_t time)
//{
//	Systik_Config();
//	for(uint32_t i = 0; i < time; i++)
//	{
//		//����STK_CTRL��COUNTFLAG��־λ��ֻҪ��Ϊ1����һֱwhileѭ����ֱ������ֵ���㣬����һ��0.125s
//		while( !((SysTick -> CTRL) & (1 << 16)) );
//	}
//	SysTick->CTRL = 0;//�رռ�����
//	SysTick->VAL = 0;//��ռ�����
//	//SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk;//��CTRL�����һλ��0��ʧ�ܼ�ʱ������Ȼ����һֱ�ڼ���
//}

