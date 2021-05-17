/**
	\file 	PIT.c
	\brief
		Source file for the PIT.h for Kinetis K64.
		It contains all the implementations for configuration and runtime functions.
	\author Paul Aguilar & Fernanda Galeana
	\date	25/02/2021
 */
#include "GPIO.h"
#include "Bits.h"
#include "MK64F12.h"
#include "PIT.h"

#include "stdio.h"

static void (*pit0_callback)(void) = 0;
static void (*pit1_callback)(void) = 0;
static void (*pit2_callback)(void) = 0;
static void (*pit3_callback)(void) = 0;
uint8_t g_interrupt_flag = 0x00;


void PIT_delay(PIT_timer_t pit_timer, My_float_pit_t system_clock , My_float_pit_t delay)
{
		uint32_t timer_start_value = (uint32_t)((((system_clock) * delay)) - 1);
		PIT_clear_interrupt_flag();
		//LOADS TIMER START VALUE
		PIT->CHANNEL[pit_timer].LDVAL = timer_start_value;
		//ENABLES PIT INTERRUPTION
		PIT_enable_interrupt(pit_timer);
		//TIMER ENABLE
		PIT->CHANNEL[pit_timer].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void PIT_clock_gating(void)
{
	//ENABLES CLOCK GATE CONTROL FOR PIT
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
}

uint8_t PIT_get_interrupt_flag_status(void)
{
	//RETURNS THE VALUE OF THE FLAG CREATED
	return(g_interrupt_flag);
}


void PIT_clear_interrupt_flag(void)
{
	//flag value becomes 0
	g_interrupt_flag = 0x00;
}

void PIT_enable(void)
{
	//ENABLES PIT
	PIT->MCR &= ~(PIT_MCR_MDIS_MASK);
	PIT->MCR |= PIT_MCR_FRZ_MASK;
}

void PIT_enable_interrupt(PIT_timer_t pit)
{
	//PIT FLAG MASK IS CLEARED
	PIT->CHANNEL[pit].TFLG |= PIT_TFLG_TIF_MASK;

	//ENABLE THE INTERRUPT
	PIT->CHANNEL[pit].TCTRL |= PIT_TCTRL_TIE_MASK;
}

void PIT_stop(PIT_timer_t pit)
{
	PIT->CHANNEL[pit].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
}
void PIT_callback_init(PIT_timer_t pit, void(*handler)(void))
{
	switch(pit)
		{
		case PIT_0:
			pit0_callback = handler;
			break;
		case PIT_1:
			pit1_callback = handler;
			break;
		case PIT_2:
			pit2_callback = handler;
			break;
		default:
			pit3_callback = handler;
			break;
		}
}

void PIT0_IRQHandler()
{
	volatile uint32_t dummyRead;
	PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;
	dummyRead = PIT->CHANNEL[0].TCTRL;
	if(pit0_callback)
	{
		pit0_callback();
	}
	g_interrupt_flag |= 0x01;
}

void PIT1_IRQHandler()
{
	volatile uint32_t dummyRead;
	PIT->CHANNEL[1].TFLG |= PIT_TFLG_TIF_MASK;
	dummyRead = PIT->CHANNEL[1].TCTRL;
	if(pit1_callback)
	{
		pit1_callback();
	}
	g_interrupt_flag |= 0x02;
}
void PIT2_IRQHandler()
{
	volatile uint32_t dummyRead;
	PIT->CHANNEL[2].TFLG |= PIT_TFLG_TIF_MASK;
	dummyRead = PIT->CHANNEL[2].TCTRL;
	if(pit2_callback)
	{
		pit2_callback();
	}
	g_interrupt_flag |= 0x04;
}
void PIT3_IRQHandler()
{
	volatile uint32_t dummyRead;
	PIT->CHANNEL[3].TFLG |= PIT_TFLG_TIF_MASK;
	dummyRead = PIT->CHANNEL[3].TCTRL;
	if(pit3_callback)
	{
		pit3_callback();
	}
	g_interrupt_flag |= 0x08;
}

