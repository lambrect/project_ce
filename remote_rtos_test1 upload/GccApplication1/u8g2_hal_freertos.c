/*** Includes ***/
// Standard C
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"

// Application
#include "u8g2_hal.h"

u8g2_hal_conf_t *u8g2_hal_conf;

/*** Functions ***/
void u8g2_hal_init(u8g2_hal_conf_t *u8g2_hal_pins_param){
	u8g2_hal_conf = u8g2_hal_pins_param;
}

uint8_t u8g2_hal_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr){
	switch(msg){
		case U8X8_MSG_DELAY_NANO:			// delay arg_int * 1 nano second
			break;
			
		case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
		
			break;
			
		case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
			vTaskDelay(1 / portTICK_PERIOD_MS);	
			break;
			
		case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
			vTaskDelay(1 / portTICK_PERIOD_MS);	
			break;
			
		default:
			switch(u8g2_hal_conf->if_type){
				case u8g2_hal_if_sw_spi:
				switch(msg)
				{
					case U8X8_MSG_GPIO_AND_DELAY_INIT:	// called once during init phase of u8g2/u8x8
					*(u8g2_hal_conf->ddr_clk) |= 1 << u8g2_hal_conf->pin_clk;
					*(u8g2_hal_conf->ddr_sdo) |= 1 << u8g2_hal_conf->pin_sdo;
					*(u8g2_hal_conf->ddr_cs)  |= 1 << u8g2_hal_conf->pin_cs;
					*(u8g2_hal_conf->ddr_dc)  |= 1 << u8g2_hal_conf->pin_dc;
					*(u8g2_hal_conf->ddr_rs)  |= 1 << u8g2_hal_conf->pin_rs;
					break;							// can be used to setup pins
					
					case U8X8_MSG_GPIO_SPI_CLOCK:		// clock pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_clk) |= 1 << u8g2_hal_conf->pin_clk;
						} else {
						*(u8g2_hal_conf->port_clk) &= ~(1 << u8g2_hal_conf->pin_clk);
					}
					break;
					
					case U8X8_MSG_GPIO_SPI_DATA:		// SPI data pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_sdo) |= 1 << u8g2_hal_conf->pin_sdo;
						} else {
						*(u8g2_hal_conf->port_sdo) &= ~(1 << u8g2_hal_conf->pin_sdo);
					}
					break;
					
					case U8X8_MSG_GPIO_CS:				// CS (chip select) pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_cs) |= 1 << u8g2_hal_conf->pin_cs;
						} else {
						*(u8g2_hal_conf->port_cs) &= ~(1 << u8g2_hal_conf->pin_cs);
					}
					break;
					
					case U8X8_MSG_GPIO_DC:				// DC (data/cmd, A0, register select) pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_dc) |= 1 << u8g2_hal_conf->pin_dc;
						} else {
						*(u8g2_hal_conf->port_dc) &= ~(1 << u8g2_hal_conf->pin_dc);
					}
					break;
					
					case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_rs) |= 1 << u8g2_hal_conf->pin_rs;
						} else {
						*(u8g2_hal_conf->port_rs) &= ~(1 << u8g2_hal_conf->pin_rs);
					}
					break;
				}
				break;
				
				case u8g2_hal_if_sw_i2c:
				switch(msg)
				{
					case U8X8_MSG_GPIO_AND_DELAY_INIT:	// called once during init phase of u8g2/u8x8
					*(u8g2_hal_conf->ddr_scl) |= 1 << u8g2_hal_conf->pin_scl;
					*(u8g2_hal_conf->ddr_sda) |= 1 << u8g2_hal_conf->pin_sda;
					*(u8g2_hal_conf->ddr_rs) |= 1 << u8g2_hal_conf->pin_rs;
					break;								// can be used to setup pins
					
					case U8X8_MSG_DELAY_I2C:			// arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
					vTaskDelay(1);
					break;								// arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
					
					case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_rs) |= 1 << u8g2_hal_conf->pin_rs;
						} else {
						*(u8g2_hal_conf->port_rs) &= ~(1 << u8g2_hal_conf->pin_rs);
					}
					break;
					
					case U8X8_MSG_GPIO_I2C_CLOCK:		// arg_int=0: Output low at I2C clock pin
					if(arg_int){
						*(u8g2_hal_conf->port_sda) |= 1 << u8g2_hal_conf->pin_sda;
						} else {
						*(u8g2_hal_conf->port_sda) &= ~(1 << u8g2_hal_conf->pin_sda);
					}
					break;								// arg_int=1: Input dir with pullup high for I2C clock pin
					
					case U8X8_MSG_GPIO_I2C_DATA:		// arg_int=0: Output low at I2C data pin
					if(arg_int){
						*(u8g2_hal_conf->port_scl) |= 1 << u8g2_hal_conf->pin_scl;
						} else {
						*(u8g2_hal_conf->port_scl) &= ~(1 << u8g2_hal_conf->pin_scl);
					}
					break;								// arg_int=1: Input dir with pullup high for I2C data pin
				}
				break;
				
				case u8g2_hal_if_sw_parallel:
				switch(msg)
				{
					case U8X8_MSG_GPIO_AND_DELAY_INIT:	// called once during init phase of u8g2/u8x8
					*(u8g2_hal_conf->ddr_d0) |= 1 << u8g2_hal_conf->pin_d0;
					*(u8g2_hal_conf->ddr_d1) |= 1 << u8g2_hal_conf->pin_d1;
					*(u8g2_hal_conf->ddr_d2) |= 1 << u8g2_hal_conf->pin_d2;
					*(u8g2_hal_conf->ddr_d3) |= 1 << u8g2_hal_conf->pin_d3;
					*(u8g2_hal_conf->ddr_d4) |= 1 << u8g2_hal_conf->pin_d4;
					*(u8g2_hal_conf->ddr_d5) |= 1 << u8g2_hal_conf->pin_d5;
					*(u8g2_hal_conf->ddr_d6) |= 1 << u8g2_hal_conf->pin_d6;
					*(u8g2_hal_conf->ddr_d7) |= 1 << u8g2_hal_conf->pin_d7;
					*(u8g2_hal_conf->ddr_en) |= 1 << u8g2_hal_conf->pin_en;
					*(u8g2_hal_conf->ddr_cs) |= 1 << u8g2_hal_conf->pin_cs;
					*(u8g2_hal_conf->ddr_dc) |= 1 << u8g2_hal_conf->pin_dc;
					*(u8g2_hal_conf->ddr_rs) |= 1 << u8g2_hal_conf->pin_rs;
					break;							// can be used to setup pins
					
					case U8X8_MSG_GPIO_D0:				// D0 or SPI clock pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_d0) |= 1 << u8g2_hal_conf->pin_d0;
						} else {
						*(u8g2_hal_conf->port_d0) &= ~(1 << u8g2_hal_conf->pin_d0);
					}
					break;
					
					case U8X8_MSG_GPIO_D1:				// D1 or SPI data pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_d1) |= 1 << u8g2_hal_conf->pin_d1;
						} else {
						*(u8g2_hal_conf->port_d1) &= ~(1 << u8g2_hal_conf->pin_d1);
					}
					break;
					
					case U8X8_MSG_GPIO_D2:				// D2 pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_d2) |= 1 << u8g2_hal_conf->pin_d2;
						} else {
						*(u8g2_hal_conf->port_d2) &= ~(1 << u8g2_hal_conf->pin_d2);
					}
					break;
					
					case U8X8_MSG_GPIO_D3:				// D3 pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_d3) |= 1 << u8g2_hal_conf->pin_d3;
						} else {
						*(u8g2_hal_conf->port_d3) &= ~(1 << u8g2_hal_conf->pin_d3);
					}
					break;
					
					case U8X8_MSG_GPIO_D4:				// D4 pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_d4) |= 1 << u8g2_hal_conf->pin_d4;
						} else {
						*(u8g2_hal_conf->port_d4) &= ~(1 << u8g2_hal_conf->pin_d4);
					}
					break;
					
					case U8X8_MSG_GPIO_D5:				// D5 pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_d5) |= 1 << u8g2_hal_conf->pin_d5;
						} else {
						*(u8g2_hal_conf->port_d5) &= ~(1 << u8g2_hal_conf->pin_d5);
					}
					break;
					
					case U8X8_MSG_GPIO_D6:				// D6 pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_d6) |= 1 << u8g2_hal_conf->pin_d6;
						} else {
						*(u8g2_hal_conf->port_d6) &= ~(1 << u8g2_hal_conf->pin_d6);
					}
					break;
					
					case U8X8_MSG_GPIO_D7:				// D7 pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_d7) |= 1 << u8g2_hal_conf->pin_d7;
						} else {
						*(u8g2_hal_conf->port_d7) &= ~(1 << u8g2_hal_conf->pin_d7);
					}
					break;
					
					case U8X8_MSG_GPIO_E:				// E/WR pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_en) |= 1 << u8g2_hal_conf->pin_en;
						} else {
						*(u8g2_hal_conf->port_en) &= ~(1 << u8g2_hal_conf->pin_en);
					}
					break;
					
					case U8X8_MSG_GPIO_CS:				// CS (chip select) pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_cs) |= 1 << u8g2_hal_conf->pin_cs;
						} else {
						*(u8g2_hal_conf->port_cs) &= ~(1 << u8g2_hal_conf->pin_cs);
					}
					break;
					
					case U8X8_MSG_GPIO_DC:				// DC (data/cmd, A0, register select) pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_dc) |= 1 << u8g2_hal_conf->pin_dc;
						} else {
						*(u8g2_hal_conf->port_dc) &= ~(1 << u8g2_hal_conf->pin_dc);
					}
					break;
					
					case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int
					if(arg_int){
						*(u8g2_hal_conf->port_rs) |= 1 << u8g2_hal_conf->pin_rs;
						} else {
						*(u8g2_hal_conf->port_rs) &= ~(1 << u8g2_hal_conf->pin_rs);
					}
					break;
				}
				break;
			}
	}
	
	return 1;
}