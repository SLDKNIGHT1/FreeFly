#include "stm32f4xx.h"                  // Device header
#include "MyDelay.h"
#include "MyI2C.h"
#include "GY86.h"
#include "Bluetooth.h"
#include "Motor.h"
#include "Receiver.h"
#include <stdio.h>
#include <stdlib.h>
extern void My_Systick_Config(uint32_t reload_value);
extern uint16_t mpu_acc_x, mpu_acc_y, mpu_acc_z;
extern uint16_t hmc5883_x, hmc5883_y, hmc5883_z;
extern uint32_t PulseWidth, Period, DutyCycle;
extern char TransmitData[1005];
extern uint8_t mpu_acc_x_h, mpu_acc_x_l, mpu_acc_y_h, mpu_acc_y_l, mpu_acc_z_h, mpu_acc_z_l;
uint16_t times;
uint8_t RxData;

RCC_ClocksTypeDef clockwatch;

int main(void)
{
//	RCC_GetClocksFreq(&clockwatch);
	// ahb : 84000000Hz
    // needed tick: 1 tick = 10ms = 100Hz -> reload val = 840000
    My_Systick_Config(840000);

	MyI2C_Init();
	times++;
	MPU6050Init();
	times++;
	HMC5883Init();
	times++;
	
	BluetoothInit();
	
	TIM1_init();
	Motor_Init();

	Delay_ms(4000);
	TIM_SetCompare1(TIM3, 2000);
	Delay_ms(4000);
	TIM_SetCompare1(TIM3, 1000);
	Delay_ms(4000);                                                
	
	Bluetooth_SendString("Initilization finished!\n");
	while(1)
	{
		Bluetooth_SendString("MPU6050 is working...\n");
		Delay_ms(10);

		//MPU6050
		Delay_ms(50);
//		mpu_acc_x = GetACCXMPU6050();
//		mpu_acc_x_h = (mpu_acc_x >>8)&0xFF;
//		mpu_acc_x_l = (mpu_acc_x&0xFF); 
//		Delay_ms(50);
//		mpu_acc_y = GetACCXMPU6050();
//		mpu_acc_y_h = (mpu_acc_y >>8)&0xFF;
//		mpu_acc_y_l = (mpu_acc_y&0xFF); 
//		Delay_ms(50);
//		mpu_acc_z = GetACCXMPU6050();
//		mpu_acc_z_h = (mpu_acc_z >>8)&0xFF;
//		mpu_acc_z_l = (mpu_acc_z&0xFF); 
		
		
		
		// motor and receiver
		Delay_ms(50);
		Motor_SetDutyCycle(DutyCycle);
		
		// Bluetooth
		
		
//		Bluetooth_SendString("mpu_acc_x : ");
//		sprintf(TransmitData, "%d", mpu_acc_x);
//		Bluetooth_SendString(TransmitData);
//		Bluetooth_SendByte('\n');
//		Delay_ms(10);
//		
//		Bluetooth_SendString("mpu_acc_y : ");
//		sprintf(TransmitData, "%d", mpu_acc_y);
//		Bluetooth_SendString(TransmitData);
//		Bluetooth_SendByte('\n');
//		Delay_ms(10);
//		
//		Bluetooth_SendString("mpu_acc_z : ");
//		sprintf(TransmitData, "%d", mpu_acc_z);
//		Bluetooth_SendString(TransmitData);
//		Bluetooth_SendByte('\n');
//		Delay_ms(10);
//		
//		Bluetooth_SendString("DutyCycle : ");
//		sprintf(TransmitData, "%d", DutyCycle);
//		Bluetooth_SendString(TransmitData);
//		Bluetooth_SendByte('\n');
//		Delay_ms(10);
	} 
}
