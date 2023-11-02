#include "stm32f4xx_gpio.h"

#include <stdio.h>
#include <string.h>
#include <sys/_stdint.h>

#include "ucos_ii.h"

#include "bluetooth.h"
#include "motor.h"
#include "receiver.h"

void Motor_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;   //����һ���ṹ�������������ʼ��GPIO

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//����һ���ṹ�������������ʼ����ʱ��

	TIM_OCInitTypeDef TIM_OCInitStructure;//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

	/* ����ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 |GPIO_Pin_9;// PC6 7 8 9
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
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);//ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM3,ENABLE);//ʹ��TIMx����
	TIM3->CR1|=(1<<7);
}

BOOLEAN ESC_Unlock(void)
{
	memset(ppm_val, 0, sizeof(ppm_val));
	
	uint32_t fails_cnt = 0;
	while(1)
	{
		if(fails_cnt > 1000)
		{
			// return 0;
		}
 
		BOOLEAN hold_flag = 1;
		Bluetooth_SendString("waiting for 2000\n");
		while(ppm_val[THR] <= PPM_MAX_VAL)
		{
			printf("%d\n", ppm_val[THR]);
		}

		Bluetooth_SendString("Reach 2000\n");

		for(uint16_t i=0; i<300; i++)
		{
			if(ppm_val[THR] <= PPM_MAX_VAL)
			{
				hold_flag = 0;
				break;
			}		
			OSTimeDly(10);	// delay 3S in total
			if(i % 100 == 0)
				printf("Still for %d\n", i);
		}
		if(hold_flag == 0)
		{
			fails_cnt++;
			continue;
		}

		Bluetooth_SendString("2000 verified!\n");

		while(ppm_val[THR] >= PPM_MIN_VAL)
		{
			printf("%d\n", ppm_val[THR]);
		}

		Bluetooth_SendString("Reach 1000\n");

		for(uint16_t i=0; i<300; i++)
		{
			if(ppm_val[THR] >= PPM_MIN_VAL)
			{
				hold_flag = 0;
				break;
			}		
			OSTimeDly(10);	// delay 3S in total
		}
		if(hold_flag == 0)
		{
			fails_cnt++;
			continue;
		}
		
		Bluetooth_SendString("1000 verified!\n");

		break;
	}
	TIM_SetCompare1(TIM3, 2000);
	OSTimeDly(4000);
	TIM_SetCompare1(TIM3, 1000);
	OSTimeDly(4000);  
	
	return 1;
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
