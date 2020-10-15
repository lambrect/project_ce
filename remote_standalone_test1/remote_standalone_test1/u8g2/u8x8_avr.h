#ifndef U8X8_AVR_H_
#define U8X8_AVR_H_

#include <u8g2.h>
#include <stdint.h>

typedef struct {
	volatile uint8_t *ddr_sck;
	volatile uint8_t *port_sck;
	uint8_t pin_sck;
	
	volatile uint8_t *ddr_mosi;
	volatile uint8_t *port_mosi;
	uint8_t pin_mosi;
	
	volatile uint8_t *ddr_cs;
	volatile uint8_t *port_cs;
	uint8_t pin_cs;
	
	volatile uint8_t *ddr_dc;
	volatile uint8_t *port_dc;
	uint8_t pin_dc;
	
	volatile uint8_t *ddr_rst;
	volatile uint8_t *port_rst;
	uint8_t pin_rst;
} u8x8_conf_t;

uint8_t u8x8_byte_avr_hw_spi(u8x8_t * u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_byte_avr_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_avr_delay(u8x8_t * u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

uint8_t u8x8_gpio_and_delay (u8x8_t * u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void u8x8_init_conf(u8x8_conf_t *arg_conf);

#endif /* U8X8_AVR_H_ */
