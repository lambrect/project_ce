#ifndef U8G2_HAL_H_
#define U8G2_HAL_H_

#include "u8g2.h"

//#define U8G2_HAL_UNDEFINED -1

/*** Enums ***/
typedef enum {
	u8g2_hal_if_sw_spi,
	u8g2_hal_if_sw_i2c,
	u8g2_hal_if_sw_parallel
} u8g2_hal_if_t;

/*** Structs ***/
typedef struct {
	u8g2_hal_if_t if_type;
	
	/* Parallel */
	volatile uint8_t *port_d0;
	volatile uint8_t *ddr_d0;
	int8_t pin_d0;
	volatile uint8_t *port_d1;
	volatile uint8_t *ddr_d1;
	int8_t pin_d1;
	volatile uint8_t *port_d2;
	volatile uint8_t *ddr_d2;
	int8_t pin_d2;
	volatile uint8_t *port_d3;
	volatile uint8_t *ddr_d3;
	int8_t pin_d3;
	volatile uint8_t *port_d4;
	volatile uint8_t *ddr_d4;
	int8_t pin_d4;
	volatile uint8_t *port_d5;
	volatile uint8_t *ddr_d5;
	int8_t pin_d5;
	volatile uint8_t *port_d6;
	volatile uint8_t *ddr_d6;
	int8_t pin_d6;
	volatile uint8_t *port_d7;
	volatile uint8_t *ddr_d7;
	int8_t pin_d7;
	
	/* SPI */
	volatile uint8_t *port_sdo;
	volatile uint8_t *ddr_sdo;
	int8_t pin_sdo;
	volatile uint8_t *port_clk;
	volatile uint8_t *ddr_clk;
	int8_t pin_clk;
	
	/* I2C */
	volatile uint8_t *port_sda;
	volatile uint8_t *ddr_sda;
	int8_t pin_sda;
	volatile uint8_t *port_scl;
	volatile uint8_t *ddr_scl;
	int8_t pin_scl;
	
	/* Misc */
	volatile uint8_t *port_en;
	volatile uint8_t *ddr_en;
	int8_t pin_en;
	volatile uint8_t *port_cs;
	volatile uint8_t *ddr_cs;
	int8_t pin_cs;	/* Parallel, SPI */
	volatile uint8_t *port_dc;
	volatile uint8_t *ddr_dc;
	int8_t pin_dc;	/* Parallel, SPI */
	volatile uint8_t *port_rs;
	volatile uint8_t *ddr_rs;
	int8_t pin_rs;	/* Parallel, SPI, I2C */
} u8g2_hal_conf_t;

void u8g2_hal_init(u8g2_hal_conf_t *u8g2_hal_pins_param);
uint8_t u8g2_hal_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#endif /* U8G2_HAL_H_ */