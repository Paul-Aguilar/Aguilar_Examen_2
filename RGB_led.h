/*
 * RGB_led.h
 *
 *  Created on: 13 abr. 2021
 *  Author: Fernanda Galeana & Paul Aguilar
 */


#ifndef RGB_LED_H_
#define RGB_LED_H_

#include "MK64F12.h"
#include "FlexTimer.h"
/*Period of 1/500 (for PMW to control the RGB). Since we are using the system clock divided by 1,
 * MOD = (1/500) / (1/21000000)*/
#define FTM_CHV_RGB_INIT 	(0U)
#define FTM_MOD_RGB 		(41820U)  //Multiple of 255 to make the levels division easier

#define BLUE_CH 	CH_0
#define RED_CH		CH_1
#define GREEN_CH	CH_2

#define RGB_MANUAL_BR_CHANGE (4182U) //FTM_MOD_RGB/10 (10%)

#define RGB_INTENSITY_MAX	(255U)
#define RGB_INTENSITY_MIN	(0U)
#define RGB_INTENSITY_GAIN (164U)

#define PH_LEVELS_MAX (14U)

typedef enum
{
	RED,
	GREEN,
	BLUE
}color_value_t;
typedef enum
{
	INCREASE,
	DICREASE
}increase_or_dicrease_br_t;
typedef struct
{
	uint8_t ftm;
	uint8_t channel;
}rgb_bright_change_t;

typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}rgb_intensity_colors_t;
void RGB_led_initialize(void);
void RGB_led_change_bright(color_value_t color,uint16_t value,increase_or_dicrease_br_t option);
void RGB_led_turn_off(void);
uint16_t RGB_led_intensity_to_duty_cycle(uint8_t intensity);
void RGB_led_set_color(rgb_intensity_colors_t *rgb_code);
void RGB_led_ph_color(uint8_t ph_level);
#endif /* RGB_LED_H_ */
