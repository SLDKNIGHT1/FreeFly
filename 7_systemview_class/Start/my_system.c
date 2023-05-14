#include "stm32f4xx.h"                  // Device header
/*
// load
extern uint8_t Load$$RW_IRAM1$$Base;

//execution
extern uint8_t Image$$RW_IRAM1$$Base;
extern uint8_t Image$$RW_IRAM1$$Length;

// ZI sections
extern uint8_t Image$$RW_IRAM1$$ZI$$Base;
extern uint8_t Image$$RW_IRAM1$$ZI$$Length;

extern int main(void);

void my__main(void)
{
	uint8_t* src = &Load$$RW_IRAM1$$Base;
	uint8_t* dst = &Image$$RW_IRAM1$$Base;
	uint32_t len = (uint32_t)&Image$$RW_IRAM1$$Length;
	
	for(;len;len-=sizeof(uint8_t))
	{
		*dst++ = *src++;
	}
	
	dst = &Image$$RW_IRAM1$$ZI$$Base;
	len = (uint32_t)&Image$$RW_IRAM1$$ZI$$Length;
	//�ڶ��� ZI������
	for(;len;len-=sizeof(uint8_t))
	{
			*dst++ = 0;
	}
		
	main();
	
	return;
}*/


#include "stm32f4xx.h" 
int main(void);

//ZI ִ����
extern uint8_t Image$$RW_IRAM1$$ZI$$Base;
//ZI ����
extern uint8_t Image$$RW_IRAM1$$ZI$$Length;

//RW ִ����
extern uint8_t	Image$$RW_IRAM1$$Base;
//RW ����
extern uint8_t Image$$RW_IRAM1$$Length;

//RW ������
extern uint8_t Load$$RW_IRAM1$$Base;



extern uint32_t NMI_Handler;
extern uint32_t HardFault_Handler;
extern uint32_t SysTick_Handler;
void my__main()
{
		
		uint8_t* src = &Load$$RW_IRAM1$$Base;
		uint8_t* drc = &Image$$RW_IRAM1$$Base;
		uint32_t length = (uint32_t)&Image$$RW_IRAM1$$Length;
		//��һ�� ����RW
		for(;length;length-=sizeof(uint8_t))
		{
				*drc++ = *src++;
		}
		
		drc = &Image$$RW_IRAM1$$ZI$$Base;
		length = (uint32_t)&Image$$RW_IRAM1$$ZI$$Length;
		//�ڶ��� ZI������
		for(;length;length-=sizeof(uint8_t))
		{
				*drc++ = 0;
		}
		main();
		
}
