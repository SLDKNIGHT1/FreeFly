#include "stm32f4xx.h"                  // Device header
#include "includes.h"
#include "ucos_ii.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#if (defined(OS_TRACE_EN) && (OS_TRACE_EN > 0u))
#include "SEGGER_SYSVIEW.h"                                   
#endif

#include "attitude.h"
#include "bluetooth.h"
#include "gy86.h"
#include "led.h"
#include "myI2C.h"
#include "mathkit.h"
#include "motor.h"
#include "receiver.h"

#define TASK_STK_LEN 0x0800
#define TASK_STK_LEN_2 0x0600

OS_STK MainTaskStk[TASK_STK_LEN];	// each stack is 4B wide
OS_STK Task1Stk[TASK_STK_LEN];
OS_STK Task2Stk[TASK_STK_LEN];
OS_STK Task3Stk[TASK_STK_LEN];
OS_STK Task4Stk[TASK_STK_LEN];
OS_STK Task5Stk[TASK_STK_LEN_2];
OS_STK Task6Stk[TASK_STK_LEN];
OS_STK Task7Stk[TASK_STK_LEN];
OS_STK Task8Stk[TASK_STK_LEN];
OS_STK Task9Stk[TASK_STK_LEN];

uint16_t times;

extern Vec3d_t acc_offset, acc_scale, gyro_offset, gyro_filter[2], mag_offset, mag_scale;

extern void My_Systick_Config(uint32_t reload_value);



void task_led_on(void *pdata)
{
	while(1)
	{
		led_on();
		OSTimeDly(1000);
	}
}

void task_led_off(void *pdata)
{
	while(1)
	{
		led_off();
		OSTimeDly(2000);
	}
}

void task_peripheral_init(void *pdata)
{
	BluetoothInit();
	printf("Bluetooth init finished!\n");
	times++;

	MyI2C_Init();
	printf("I2C init finished!\n");
	times++;
	
	GY86Init();
	printf("GY86 init finished!\n");
	times++;
	
	
	TIM1_init();
	Motor_Init();
	
	// OSTimeDly(1000);

//	TIM_SetCompare1(TIM3, 2000);
//	OSTimeDly(4000);
//	TIM_SetCompare1(TIM3, 1000);
//	OSTimeDly(4000);                                                
	
	printf("Initilization finished!\n");
	OSTaskDel(OS_PRIO_SELF);
}

void task_MPU6050(void *pdata)
{
	// AccCalibration(&acc_offset, &acc_scale);
	// GyroCalibration(&gyro_offset, &gyro_filter[2]);
	// MagCalibration(&mag_offset, &mag_scale);
	while(1)
	{
		Vec3d_t acc;
		// GetAccData(&acc);
		// printf("acc: %f, %f, %f\n", acc.x, acc.y, acc.z);

		Vec3d_t gyro;
		// GetGyroData(&gyro);
		// printf("gyro: %f, %f, %f\n", gyro.x, gyro.y, gyro.z);

		Vec3d_t mag;
		GetMagData(&mag);
		double modulus = Vec3Modulus(mag);
		// printf("%f,%f,%f\n", mag.x, mag.y, mag.z);
		printf("mag: %f, %f, %f, M:%f\n", mag.x, mag.y, mag.z, modulus);
		
		OSTimeDly(100);
	}
}

void task_attitude_gyro(void *pdata)
{
	// GyroCalibration(&gyro_offset, &gyro_filter[2]);
	printf("gyro_offset: %f, %f, %f\n", gyro_offset.x, gyro_offset.y, gyro_offset.z);
	Vec4d_t q0 = {1, 0, 0, 0}, q1;
	Vec3d_t gyro0 = {0, 0, 0}, gyro1, euler = {0, 0, 0};
	uint32_t t1, t2;
	while(1)
	{
		t1 = OSTimeGet();
		GetGyroData(&gyro1);
		// printf("gyro: %f, %f, %f\n", gyro1.x, gyro1.y, gyro1.z);
		GyroUpdateQuat(&q0, &q1, &gyro0, &gyro1, 0.001);
		// printf("q: %f, %f, %f, %f\n", q1.w, q1.x, q1.y, q1.z);
		q0 = q1;
		gyro0 = gyro1;
		QuaterToEuler(&q0, &euler);
		// RadToDeg(&euler);
		printf("euler: %f, %f, %f\n", euler.x, euler.y, euler.z);	// a printf takes 4 ticks
		t2 = OSTimeGet();
		// printf("ticks: %d %d %d\n",t1,t2,t2-t1);
		// OSTimeDly(10);
	}
}

void task_attitude_acc(void *pdata)
{
	// AccCalibration(&acc_offset, &acc_scale);
	// GyroCalibration(&gyro_offset, &gyro_filter[2]);
	printf("gyro_offset: %f, %f, %f\n", gyro_offset.x, gyro_offset.y, gyro_offset.z);
	printf("acc_offset: %f, %f, %f\n", acc_offset.x, acc_offset.y, acc_offset.z);
	Vec3d_t acc_data = {0, 0, 0}, gyro_data, euler = {0, 0, 0};
	Vec4d_t q0 = {1, 0, 0, 0}, q1;
	uint32_t t1, t2;
	while(1)
	{
		t1 = OSTimeGet();
		GetAccData(&acc_data);
		// printf("acc: %f, %f, %f\n", acc_data.x, acc_data.y, acc_data.z);
		GetGyroData(&gyro_data);
		AccUpdateQuat(&q0, &q1, &acc_data, &gyro_data, 0.001);
		// printf("q: %10f, %10f, %10f, %10f\n", q1.w, q1.x, q1.y, q1.z);
		q0 = q1;
		QuaterToEuler(&q0, &euler);
		// RadToDeg(&euler);
		printf("euler: %10f, %10f, %10f\n", euler.x, euler.y, euler.z);
		t2 = OSTimeGet();
		OSTimeDly(10);
	}
}

void task_attitude_acc_mag(void *pdata)
{
	// AccCalibration(&acc_offset, &acc_scale);
	// GyroCalibration(&gyro_offset, &gyro_filter[2]);
	// printf("gyro_offset: %f, %f, %f\n", gyro_offset.x, gyro_offset.y, gyro_offset.z);
	// printf("acc_offset: %f, %f, %f\n", acc_offset.x, acc_offset.y, acc_offset.z);
	Vec3d_t acc_data = {0, 0, 0}, mag_data, gyro_data, euler = {0, 0, 0};
	Vec4d_t q0 = {1, 0, 0, 0}, q1;
	uint32_t t1, t2;
	while(1)
	{
		t1 = OSTimeGet();
		GetAccData(&acc_data);
		Vec3Norm(&acc_data);
		GetMagData(&mag_data);
		Vec3Norm(&mag_data);
		// printf("acc: %f, %f, %f\n", acc_data.x, acc_data.y, acc_data.z);
		GetGyroData(&gyro_data);
		AccMagUpdateQuat(&q0, &q1, &acc_data, &gyro_data, &mag_data, 0.001);
		// printf("q: %10f, %10f, %10f, %10f\n", q1.w, q1.x, q1.y, q1.z);
		q0 = q1;
		QuaterToEuler(&q0, &euler);
		// RadToDeg(&euler);
		printf("euler: %10f, %10f, %10f\n", euler.x, euler.y, euler.z);
		t2 = OSTimeGet();
		OSTimeDly(10);
	}
}

void task_attitude_fusion(void *pdata)
{
	Vec4d_t q0 = {1, 0, 0, 0};
	Vec3d_t gyro0 = {0, 0, 0}, euler;
	while(1)
	{
		MadgwickAHRS(&q0, &gyro0, 0.001);
		// printf("q: %10f, %10f, %10f, %10f\n", q0.w, q0.x, q0.y, q0.z);
		QuaterToEuler(&q0, &euler);
		// RadToDeg(&euler);
		printf("euler: %10f, %10f, %10f\n", euler.x, euler.y, euler.z);
	}
}

void first_task(void *pdata) {
    // Initialization
    My_Systick_Config(840000); // AHB = 84MHz
	
    led_init();

    // create LED_ON task
    // OSTaskCreateExt(task_led_on, (void *)0, &Task2Stk[TASK_STK_LEN - 1], 6, 6, Task2Stk, TASK_STK_LEN, (void *)0, 0);
    // OSTaskNameSet(6, (INT8U *)"LED_ON", (INT8U *)"LED_ON_ERR");

    // create LED_OFF task
    // OSTaskCreateExt(task_led_off, (void *)0, &Task3Stk[TASK_STK_LEN - 1], 7, 7, Task3Stk, TASK_STK_LEN, (void *)0, 0);
    // OSTaskNameSet(7, (INT8U *)"LED_OFF", (INT8U *)"LED_OFF_ERR");

	// create peripheral init task
	OSTaskCreateExt(task_peripheral_init, (void *)0, &Task4Stk[TASK_STK_LEN - 1], 8, 8, Task4Stk, TASK_STK_LEN, (void *)0, 0);
	OSTaskNameSet(8, (INT8U *)"PERIPHERAL_INIT", (INT8U *)"PERIPHERAL_INIT_ERR");
	OSTimeDly(3000);

	// create MPU6050 task
	// OSTaskCreateExt(task_MPU60W50, (void *)0, &Task5Stk[TASK_STK_LEN_2 - 1], 9, 9, Task5Stk, TASK_STK_LEN_2, (void *)0, 0);
	// OSTaskNameSet(9, (INT8U *)"MPU6050", (INT8U *)"MPU6050_ERR");
	// OSTimeDly(3000);

	// create attitude control task
	// OSTaskCreateExt(task_attitude_gyro, (void *)0, &Task6Stk[TASK_STK_LEN - 1], 10, 10, Task6Stk, TASK_STK_LEN, (void *)0, 0);
	// OSTaskNameSet(10, (INT8U *)"attitude", (INT8U *)"attitude_ERR");
	// OSTaskCreateExt(task_attitude_acc, (void *)0, &Task7Stk[TASK_STK_LEN - 1], 11, 11, Task7Stk, TASK_STK_LEN, (void *)0, 0);
	// OSTaskNameSet(11, (INT8U *)"attitude", (INT8U *)"attitude_ERR");
	// OSTaskCreateExt(task_attitude_acc_mag, (void *)0, &Task7Stk[TASK_STK_LEN - 1], 11, 11, Task7Stk, TASK_STK_LEN, (void *)0, 0);
	// OSTaskNameSet(11, (INT8U *)"attitude", (INT8U *)"attitude_ERR");
	OSTaskCreateExt(task_attitude_fusion, (void *)0, &Task8Stk[TASK_STK_LEN - 1], 12, 12, Task8Stk, TASK_STK_LEN, (void *)0, 0);
	OSTaskNameSet(12, (INT8U *)"attitude", (INT8U *)"attitude_ERR");
	
    OSTaskDel(OS_PRIO_SELF);
}

int main(void)
{
	
	OSInit();
	// OS_TRACE_INIT(); //	SEGGER_SYSVIEW_Conf();
	// OS_TRACE_START(); // SEGGER_SYSVIEW_Start();
	OSTaskCreate(first_task, (void *)0, &MainTaskStk[TASK_STK_LEN-1], 2);
	OSTaskNameSet(2, (INT8U *)"FIRST_TASK", (INT8U *)"FIRST_TASK_ERR");
	OSStart();
	return 0;
}
