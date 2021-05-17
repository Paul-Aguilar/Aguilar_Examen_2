/**
	\file 	GPIO.c
	\brief
		This is the source file for the GPIO device driver for Kinetis K64.
		It contains all the implementation for configuration functions and runtime functions.
		i.e., this is the application programming interface (API) for the GPIO peripheral.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	18/02/2019
 */
#include "MK64F12.h"
#include "GPIO.h"
#include "Bits.h"

static void (*gpio_A_callback)(void) = 0;
static void (*gpio_B_callback)(void) = 0;
static void (*gpio_C_callback)(void) = 0;
static void (*gpio_D_callback)(void) = 0;
static void (*gpio_E_callback)(void) = 0;

static gpio_interrupt_flags_t g_intr_status_flag = {0};



void GPIO_callback_init(gpio_port_name_t port_name,void (*handler)(void))
{
	switch(port_name)
		{
		case GPIO_A:
			gpio_A_callback = handler;
			break;
		case GPIO_B:
			gpio_B_callback = handler;
			break;
		case GPIO_C:
			gpio_C_callback = handler;
			break;
		case GPIO_D:
			gpio_D_callback = handler;
			break;
		default:
			gpio_E_callback = handler;
			break;
		}// end switch
}
void PORTA_IRQHandler(void)
{
	if(gpio_A_callback)
	{
		gpio_A_callback();
	}

	GPIO_clear_interrupt(GPIO_A);
}
void PORTB_IRQHandler(void)
{
	if(gpio_B_callback)
	{
		gpio_B_callback();
	}

	GPIO_clear_interrupt(GPIO_B);
}
void PORTC_IRQHandler(void)
{
	if(gpio_C_callback)
	{
		gpio_C_callback();
	}

	GPIO_clear_interrupt(GPIO_C);
}
void PORTD_IRQHandler(void)
{
	if(gpio_D_callback)
	{
		gpio_D_callback();
	}

	GPIO_clear_interrupt(GPIO_D);
}
void PORTE_IRQHandler(void)
{
	if(gpio_E_callback)
	{
		gpio_E_callback();
	}

	GPIO_clear_interrupt(GPIO_E);
}
void GPIO_clear_irq_status(gpio_port_name_t gpio)
{
	switch(gpio)
	{
		case GPIO_A:
			g_intr_status_flag.flag_port_a = FALSE;
			break;
		case GPIO_B:
			g_intr_status_flag.flag_port_b = FALSE;
			break;
		case GPIO_C:
			g_intr_status_flag.flag_port_c = FALSE;
			break;
		case GPIO_D:
			g_intr_status_flag.flag_port_d = FALSE;
			break;
		default:
			g_intr_status_flag.flag_port_e = FALSE;
			break;
	}// end switch
}

uint8_t GPIO_get_irq_status(gpio_port_name_t gpio)
{
	uint8_t status = 0;

	switch(gpio)
	{
		case GPIO_A:
			status = g_intr_status_flag.flag_port_a;
			break;
		case GPIO_B:
			status = g_intr_status_flag.flag_port_b;
			break;
		case GPIO_C:
			status = g_intr_status_flag.flag_port_c;
			break;
		case GPIO_D:
			status = g_intr_status_flag.flag_port_d;
			break;
		default:
			status = g_intr_status_flag.flag_port_e;
			break;
	}// end switch
	return(status);
}//end function

void GPIO_clear_interrupt(gpio_port_name_t port_name)
{
	switch(port_name)/** Selecting the GPIO to clear interruption*/
	{
		case GPIO_A: /** GPIO A is selected*/
			PORTA->ISFR=0xFFFFFFFF;
			break;
		case GPIO_B: /** GPIO B is selected*/
			PORTB->ISFR=0xFFFFFFFF;
			break;
		case GPIO_C: /** GPIO C is selected*/
			PORTC->ISFR = 0xFFFFFFFF;
			break;
		case GPIO_D: /** GPIO D is selected*/
			PORTD->ISFR=0xFFFFFFFF;
			break;
		default: /** GPIO E is selected*/
			PORTE->ISFR=0xFFFFFFFF;
			break;
	}// end switch
}//end function
uint8_t GPIO_clock_gating(gpio_port_name_t port_name)
{
	switch(port_name)/** Selecting the GPIO for clock enabling*/
			{
				case GPIO_A: /** GPIO A is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTA; /** Bit 9 of SIM_SCGC5 is  set*/
					break;
				case GPIO_B: /** GPIO B is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTB; /** Bit 10 of SIM_SCGC5 is set*/
					break;
				case GPIO_C: /** GPIO C is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTC; /** Bit 11 of SIM_SCGC5 is set*/
					break;
				case GPIO_D: /** GPIO D is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTD; /** Bit 12 of SIM_SCGC5 is set*/
					break;
				case GPIO_E: /** GPIO E is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTE; /** Bit 13 of SIM_SCGC5 is set*/
					break;
				default: /**If doesn't exist the option*/
					return(FALSE);
			}// end switch
	/**Successful configuration*/
	return(TRUE);
}// end function

uint8_t GPIO_pin_control_register(gpio_port_name_t port_name, uint8_t pin,const gpio_pin_control_register_t*  pinControlRegister)
{

	switch(port_name)
			{
				case GPIO_A:/** GPIO A is selected*/
					PORTA->PCR[pin] = *pinControlRegister;
					break;
				case GPIO_B:/** GPIO B is selected*/
					PORTB->PCR[pin] = *pinControlRegister;
					break;
				case GPIO_C:/** GPIO C is selected*/
					PORTC->PCR[pin] = *pinControlRegister;
					break;
				case GPIO_D:/** GPIO D is selected*/
					PORTD->PCR[pin] = *pinControlRegister;
					break;
				case GPIO_E: /** GPIO E is selected*/
					PORTE->PCR[pin]= *pinControlRegister;
					break;
				default:/**If doesn't exist the option*/
					return(FALSE);
			}//end switch
	/**Successful configuration*/
	return(TRUE);
}// end function

void GPIO_write_port(gpio_port_name_t port_name, uint32_t data)
{
	switch(port_name)
			{
				case GPIO_A:/** GPIO A is selected*/
					GPIOA->PDOR = data;	/** Writes data in the port A**/
					break;
				case GPIO_B:/** GPIO B is selected*/
					GPIOB->PDOR = data; /** Writes data in the port B**/
					break;
				case GPIO_C:/** GPIO C is selected*/
					GPIOC->PDOR = data; /** Writes data in the port C**/
					break;
				case GPIO_D:/** GPIO D is selected*/
					GPIOD->PDOR = data; /** Writes data in the port D**/
					break;
				case GPIO_E: /** GPIO E is selected*/
					GPIOE->PDOR = data; /** Writes data in the port E**/
					break;
				default:/**If doesn't exist the option*/
					GPIOB->PDOR = data;
			}//end switch
}// end function

uint32_t GPIO_read_port(gpio_port_name_t port_name)
{
	uint32_t read_data;
	switch(port_name)
			{
				case GPIO_A: /** GPIO A is selected*/
					read_data = GPIOA->PDIR; /** Reads data from port A**/
					break;
				case GPIO_B: /** GPIO B is selected*/
					read_data = GPIOB->PDIR; /** Reads data from port B**/
					break;
				case GPIO_C: /** GPIO C is selected*/
					read_data = GPIOC->PDIR; /** Reads data from port C**/
					break;
				case GPIO_D: /** GPIO D is selected*/
					read_data = GPIOD->PDIR; /** Reads data from port D**/
					break;
				case GPIO_E: /** GPIO E is selected*/
					read_data = GPIOE->PDIR; /** Reads data from port E**/
					break;
				default:/**If doesn't exist the option*/
					read_data = FALSE;
			}//end switch
	/**Returns data read from specified port*/
	return(read_data);
}//end function
uint8_t GPIO_read_pin(gpio_port_name_t port_name, uint8_t pin)
{
	uint8_t read_pin_data;
	switch(port_name)
				{
					case GPIO_A: /** GPIO A is selected*/
						read_pin_data = (GPIOA->PDIR >> (uint32_t)(pin)) && (0x0001); /** Reads data from the specified pin from port A **/
						break;
					case GPIO_B: /** GPIO B is selected*/
						read_pin_data = (GPIOB->PDIR >> (uint32_t)(pin)) && (0x0001); /** Reads data from the specified pin from port B **/
						break;
					case GPIO_C: /** GPIO C is selected*/
						read_pin_data = (GPIOC->PDIR >> (uint32_t)(pin)) && (0x0001); /** Reads data from the specified pin from port C **/
						break;
					case GPIO_D: /** GPIO D is selected*/
						read_pin_data = (GPIOD->PDIR >> (uint32_t)(pin)) && (0x0001); /** Reads data from the specified pin from port D **/
						break;
					case GPIO_E: /** GPIO E is selected*/
						read_pin_data = (GPIOE->PDIR >> (uint32_t)(pin)) && (0x0001); /** Reads data from the specified pin from port E **/
						break;
					default:	/**If doesn't exist the option*/
						read_pin_data = FALSE;
				}//end switch
	/**Returns data read from specified pin in the specified port*/
	return(read_pin_data);
}//end function
void GPIO_set_pin(gpio_port_name_t port_name, uint8_t pin)

{
	switch(port_name)
				{
					case GPIO_A:  /** GPIO A is selected*/
						GPIOA->PSOR |= (uint32_t)((0x1) << pin);	/** Sets specified pin to 1 in the port A**/
						break;
					case GPIO_B:  /** GPIO B is selected*/
						GPIOB->PSOR |= (uint32_t)((0x1) << pin); /** Sets specified pin to 1 in the port B**/
						break;
					case GPIO_C:  /** GPIO C is selected*/
						GPIOC->PSOR |= (uint32_t)((0x1) << pin); /** Sets specified pin to 1 in the port C**/
						break;
					case GPIO_D:  /** GPIO D is selected*/
						GPIOD->PSOR |= (uint32_t)((0x1) << pin); /** Sets specified pin to 1 in the port D**/
						break;
					case GPIO_E: /** GPIO E is selected*/
						GPIOE->PSOR |= (uint32_t)((0x1) << pin); /** Sets specified pin to 1 in the port E**/
						break;
					default:    /**If doesn't exist the option*/
						GPIOA->PSOR |= (uint32_t)((0x1) << pin);
				}//end switch
}//end function

void GPIO_clear_pin(gpio_port_name_t port_name, uint8_t pin)
{
	switch(port_name)
				{
					case GPIO_A:  /** GPIO A is selected*/
						GPIOA->PCOR = (uint32_t)((0x01) << (pin));
						break;
					case GPIO_B:  /** GPIO B is selected*/
						GPIOB->PCOR = (uint32_t)((0x01) << (pin));

						break;
					case GPIO_C:  /** GPIO C is selected*/
						GPIOC->PCOR = (uint32_t)((0x01) << (pin));

						break;
					case GPIO_D:  /** GPIO D is selected*/
						GPIOD->PCOR = (uint32_t)((0x01) << (pin));

						break;
					case GPIO_E:  /** GPIO E is selected*/
						GPIOE->PCOR = (uint32_t)((0x01) << (pin));
						break;
					default:	/**If the option doesn't exist*/
						GPIOA->PCOR = (uint32_t)((0x00) << (pin));
				}//end switch
}//end function
void GPIO_toogle_pin(gpio_port_name_t port_name, uint8_t pin)
{
	switch(port_name)
				{
				case GPIO_A:  /** GPIO A is selected*/
					GPIOA->PTOR = (uint32_t)((0x01)<<(pin));
					break;
				case GPIO_B:  /** GPIO B is selected*/
					GPIOB->PTOR = (uint32_t)((0x01)<<(pin));
					break;
				case GPIO_C:  /** GPIO C is selected*/
					GPIOC->PTOR = (uint32_t)((0x01)<<(pin));
					break;
				case GPIO_D:  /** GPIO D is selected*/
					GPIOD->PTOR = (uint32_t)((0x01)<<(pin));
					break;
				case GPIO_E:  /** GPIO E is selected*/
					GPIOE->PTOR = (uint32_t)((0x01)<<(pin));
					break;
				default:	  /**If the option doesn't exist*/
					GPIOA->PTOR = (uint32_t)((0x00)<<(pin));
				}//end switch
}//end function

void GPIO_data_direction_port(gpio_port_name_t port_name, uint32_t direction)
{
	switch(port_name)
				{
				case GPIO_A:  /** GPIO A is selected*/
					GPIOA->PDDR |= direction;
					break;
				case GPIO_B:  /** GPIO B is selected*/
					GPIOB->PDDR |= direction;
					break;
				case GPIO_C:  /** GPIO C is selected*/
					GPIOC->PDDR |= direction;
					break;
				case GPIO_D:  /** GPIO D is selected*/
					GPIOD->PDDR |= direction;
					break;
				case GPIO_E:  /** GPIO E is selected*/
					GPIOE->PDDR |= direction;
					break;
				default:	  /**If the option doesn't exist*/
					GPIOA->PDDR |= direction;
				}//end switch
}//end function

void GPIO_data_direction_pin(gpio_port_name_t port_name, uint8_t state, uint8_t pin)
{
	switch(port_name)
				{
				case GPIO_A:  /** GPIO A is selected*/
					if (state == GPIO_OUTPUT)
					{
						GPIOA->PDDR |= (uint32_t)((0x01)<<(pin));
					}
					else
					{
						GPIOA->PDDR &= ~(uint32_t)((0x01)<<(pin));
					}
					break;
				case GPIO_B:  /** GPIO B is selected*/
					if (state == GPIO_OUTPUT)
					{
						GPIOB->PDDR |= (uint32_t)((0x01)<<(pin));
					}
					else
					{
						GPIOB->PDDR &= ~(uint32_t)((0x01)<<(pin));
					}
					break;
				case GPIO_C:  /** GPIO C is selected*/
					if (state == GPIO_OUTPUT)
					{
						GPIOC->PDDR |= (uint32_t)((0x01)<<(pin));
					}
					else
					{
						GPIOC->PDDR &= ~(uint32_t)((0x01)<<(pin));
					}
					break;
				case GPIO_D:  /** GPIO D is selected*/
					if (state == GPIO_OUTPUT)
					{
						GPIOD->PDDR |= (uint32_t)((0x01)<<(pin));
					}
					else
					{
						GPIOD->PDDR &= ~(uint32_t)((0x01)<<(pin));
					}
					break;
				case GPIO_E:  /** GPIO E is selected*/
					if (state == GPIO_OUTPUT)
					{
						GPIOE->PDDR |= (uint32_t)((0x01)<<(pin));
					}
					else
					{
						GPIOE->PDDR &= ~(uint32_t)((0x01)<<(pin));
					}
					break;
				default:	  /**If the option doesn't exist*/
					GPIOA->PDDR |= ~(uint32_t)((0x01)<<(pin));
				}//end switch
}//end function


