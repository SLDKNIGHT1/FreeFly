#include "stm32f4xx.h"                  // Device header
#include "motor.h"

void Motor_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;   //����һ���ṹ�������������ʼ��GPIO

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//����һ���ṹ�������������ʼ����ʱ��

	TIM_OCInitTypeDef TIM_OCInitStructure;//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

	/* ����ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;// PC6
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//�����������
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIOC->AFR[0] |= 2 << 24; 
	
	//TIM3��ʱ����ʼ��
	//PWM Ƶ�� 50Hz = 84 000 000/(83+1)/(19999+1)
	TIM_TimeBaseInitStructure.TIM_Period = 19999; //�Զ���װ�ؼĴ�������ARR 
	TIM_TimeBaseInitStructure.TIM_Prescaler = 83;//TIM3������ʱ��Ƶ��Ԥ��ƵֵPSC
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, & TIM_TimeBaseInitStructure);

	//PWM��ʼ��	  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM���ʹ��
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;

	TIM_OC1Init(TIM3,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM3,ENABLE);//ʹ�ܻ���ʧ��TIMx����
	TIM3->CR1|=(1<<7);
}

void Motor_SetDutyCycle(uint32_t Compare1)
{
	TIM3->CCR1 = Compare1;
}

void Motor_control(uint32_t motor1, uint32_t motor2, uint32_t motor3, uint32_t motor4)
{
	switch(motor1)
	{
		case 0:
			break;
		case 1:
			TIM3->CCR1 += 1000;
			break;
		case -1:
			TIM3->CCR1 -= 1000;
			break;
	}
	switch(motor2)
	{
		case 0:
			break;
		case 1:
			TIM3->CCR2 += 1000;
			break;
		case -1:
			TIM3->CCR2 -= 1000;
			break;
	}
	switch(motor3)
	{
		case 0:
			break;
		case 1:
			TIM3->CCR3 += 1000;
			break;
		case -1:
			TIM3->CCR3 -= 1000;
			break;
	}
	switch(motor4)
	{
		case 0:
			break;
		case 1:
			TIM3->CCR4 += 1000;
			break;
		case -1:
			TIM3->CCR4 -= 1000;
			break;
	}
}
