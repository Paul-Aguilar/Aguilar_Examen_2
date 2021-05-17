/*
 * RGB_led.c
 *
 *  Created on: 13 abr. 2021
 *  Author: Fernanda Galeana & Paul Aguilar
 */
#include <stdio.h>
#include "RGB_led.h"
#include "FlexTimer.h"
#include "GPIO.h"
#include "Bits.h"
#include "MK64F12.h"

const rgb_bright_change_t Change_br_rgb[3]=
{
	{FTM_0, RED_CH},
	{FTM_0, GREEN_CH},
	{FTM_0, BLUE_CH}
};
const ftm_channel_config_t g_configure_ftm_rgb =
{
	EDGE_ALLIGNED_HT,			//uint32_t ftm_mode;
	FTM_CHV_RGB_INIT,			//uint16_t ftm_value;
	DISABLE,					//channel_enable_t ftm_enable_deadtime;
	DISABLE,					//channel_enable_t ftm_enable_complement;
	DISABLE,					//channel_enable_t ftm_enable_combine;
	0x00,						//uint8_t ftm_dtval;
	FLEX_TIMER_CLKS_1,			//uint8_t ftm_clocks;
	FLEX_TIMER_PS_1,			//uint8_t ftm_prescaler;
	DISABLE,					//channel_enable_t ftm_enable_toie;
	DISABLE
};
const rgb_intensity_colors_t g_ph_scale[15] =
{
	{0, 	0, 	 	0},				//0
	{255, 	0, 	 	0},				//1
	{255, 	69, 	0},				//2
	{218, 	165, 	32},			//3
	{255, 	255, 	0},				//4
	{200, 	245, 	10},			//5
	{150, 	201, 	2},				//6
	{21, 	149, 	2},				//7
	{127, 	255, 	212},			//8
	{0, 	255, 	255},			//9
	{0, 	192, 	255},			//10
	{1, 	90, 	255},			//11
	{4, 	31, 	196},			//12
	{148, 	0, 		211},			//13
	{75, 	0, 		130}			//14
};
void RGB_led_initialize(void)
{
	FlexTimer_clock_gating(FTM_0);
	FlexTimer_set_mod(FTM_0,FTM_MOD_RGB);
	FlexTimer_configure_channel(FTM_0,BLUE_CH,&g_configure_ftm_rgb);
	FlexTimer_configure_channel(FTM_0,RED_CH,&g_configure_ftm_rgb);
	FlexTimer_configure_channel(FTM_0,GREEN_CH,&g_configure_ftm_rgb);
}
void RGB_led_change_bright(color_value_t color,uint16_t value,increase_or_dicrease_br_t option)
{
	uint16_t new_value = get_channel_value(Change_br_rgb[color].ftm, Change_br_rgb[color].channel);
	if(INCREASE == option)
	{
		if(FTM_MOD_RGB > new_value)
		{
			new_value += value;
		}
	}
	else
	{
		if(FTM_CHV_RGB_INIT < new_value)
		{
			new_value -= value;
		}
	}
	#ifndef DEBUG_ON
		printf("NEW VALUE = %u\n",new_value);
	#endif
	FlexTimer_change_cnv(Change_br_rgb[color].ftm, Change_br_rgb[color].channel,new_value);
}
void RGB_led_turn_off(void)
{
	FlexTimer_change_cnv(FTM_0,RED_CH,FTM_CHV_RGB_INIT);
	FlexTimer_change_cnv(FTM_0,GREEN_CH,FTM_CHV_RGB_INIT);
	FlexTimer_change_cnv(FTM_0,BLUE_CH,FTM_CHV_RGB_INIT);
}
uint16_t RGB_led_intensity_to_duty_cycle(uint8_t intensity)
{
	uint16_t duty_cycle = intensity * RGB_INTENSITY_GAIN;
	#ifndef DEBUG_ON
		printf("INTENSITY = %u   DUTY CYCLE = %u\n",intensity,duty_cycle);
	#endif
	return(duty_cycle);
}
void RGB_led_set_color(rgb_intensity_colors_t *rgb_code)
{
#ifndef DEBUG_ON
	printf("RED\n");
#endif
	uint16_t duty_cycle_red =  RGB_led_intensity_to_duty_cycle(rgb_code->red);
#ifndef DEBUG_ON
	printf("GREEN\n");
#endif
	uint16_t duty_cycle_green =  RGB_led_intensity_to_duty_cycle(rgb_code->green);
#ifndef DEBUG_ON
	printf("BLUE\n");
#endif
	uint16_t duty_cycle_blue =  RGB_led_intensity_to_duty_cycle(rgb_code->blue);
	FlexTimer_change_cnv(FTM_0, RED_CH, duty_cycle_red);
	FlexTimer_change_cnv(FTM_0, GREEN_CH, duty_cycle_green);
	FlexTimer_change_cnv(FTM_0, BLUE_CH, duty_cycle_blue);
}
void RGB_led_ph_color(uint8_t ph_level)
{
	rgb_intensity_colors_t rgb_code;
	if(PH_LEVELS_MAX < ph_level)
	{
		ph_level = PH_LEVELS_MAX;
	}
	rgb_code = g_ph_scale[ph_level];
	#ifndef DEBUG_ON
		printf("PH LEVEL =%u\n",ph_level);
	#endif
	RGB_led_set_color(&rgb_code);
}
