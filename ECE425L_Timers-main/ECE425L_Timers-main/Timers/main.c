/*
 * @file main.c
 *
 * @brief Main source code for the Timers program.
 *
 * This file contains the main entry point and function definitions for the Timers program.
 * It interfaces with the following:
 *	- EduBase Board LEDs (LED0 - LED3)
 *	- EduBase Board Push Buttons (SW2 - SW5)
 *	- EduBase Board Seven-Segment Display
 *
 * The following timers are used to demonstrate periodic interrupts: Timer 0A and Timer 1A.
 *
 * To verify the pinout of the user LED, refer to the Tiva C Series TM4C123G LaunchPad User's Guide
 * Link: https://www.ti.com/lit/pdf/spmu296
 *
 * @author Aaron Nanas
 */
 
#include "TM4C123GH6PM.h"
#include "Timer_0A_Interrupt.h"
#include "GPIO.h"
#include "SysTick_Delay.h"
#include "Seven_Segment_Display.h"

//Declare the function prototype for the user defined function for timer 0A
void Timer_0A_Periodic_Task(void);
//initialize a global variable for timer 0A to keep track of elapsed time in ms
uint32_t Timer_0A_ms_elapsed = 0;
// Initiallize  global variable to store the returned toggle rate in ms
uint32_t toggle_rate_ms = 0;
//initialize an unsigned 8-bit global variable that will be 
//incramented in the periodic task for a timer 0A
uint8_t count = 0;



int main(void)
{
	EduBase_LEDs_Init();
	
	//Initialize the LEDs on the Edubase board (Port B)
	EduBase_Button_Init();
	
	//initialize timer 0A to generate periodic interrupts every 1 ms
	Timer_0A_Interrupt_Init(&Timer_0A_Periodic_Task);
	
	//initialize the systick timer used to provide blocking delay functions
	SysTick_Delay_Init();
	
	//initialize the seven segment display on the edubase board
	Seven_Segment_Display_Init();
	
	
	while(1)
	{
		uint8_t button_status = Get_EduBase_Button_Status();
		toggle_rate_ms = Change_Counter_Speed(button_status);
		Seven_Segment_Display(count);
	
	}
}

void Timer_0A_Periodic_Task(void)
{
	//Incrament the Timer_0A_ms_elapsed global variable by 1
	//for each time timer 0A generates an interrupt 
	Timer_0A_ms_elapsed++;
	
	//if 500 ms have passed, clear Timer_0A_ms_elapsed to zero 
	// and toggle LED0 on the EduBase board
	if(Timer_0A_ms_elapsed >= toggle_rate_ms)
	{
		Timer_0A_ms_elapsed = 0;
		GPIOB->DATA = GPIOB->DATA ^ 0x01;
		
		if (count >= 15)
		{
			count = 0;
		}
		else
		{
			count = count +1;
		}
	}
}