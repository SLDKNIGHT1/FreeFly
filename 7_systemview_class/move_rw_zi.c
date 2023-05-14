
#include "stm32f4xx.h" 
int main();

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
void rw_zi()
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





























////ZI ִ����
//extern uint32_t Image$$RW_IRAM1$$ZI$$Base;
////ZI ����
//extern uint32_t Image$$RW_IRAM1$$ZI$$Length;

////RW ִ����
//extern uint32_t	Image$$RW_IRAM1$$Base;
////RW ����
//extern uint32_t Image$$RW_IRAM1$$Length;

////RW ������
//extern uint32_t Load$$RW_IRAM1$$Base;



//void rw_zi()
//{
//	
//	
//		uint8_t* src = (uint8_t*)Load$$RW_IRAM1$$Base;
//		uint8_t* drc = (uint8_t*)Image$$RW_IRAM1$$Base;
//		uint32_t length = Image$$RW_IRAM1$$Length;
//		//��һ�� ����RW
//		for(;length;length-=sizeof(uint8_t))
//		{
//				*drc++ = *src++;
//		}
//		
//		drc = (uint8_t*)Image$$RW_IRAM1$$ZI$$Base;
//		length = Image$$RW_IRAM1$$ZI$$Length;
//		//�ڶ��� ZI������
//		for(;length;length-=sizeof(uint8_t))
//		{
//				*drc++ = 0;
//		}
//		
//}