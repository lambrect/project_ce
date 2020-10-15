#include <util/delay.h>
#include "u8x8_avr.h"
#include "i2cmaster.h"

#define P_CPU_NS (1000000000UL / F_CPU)

u8x8_conf_t hal_conf;

void u8x8_init_conf(u8x8_conf_t *arg_conf){
	hal_conf = *arg_conf;
}

uint8_t u8x8_byte_avr_hw_spi (u8x8_t * u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  uint8_t *data;

  switch (msg) {
    case U8X8_MSG_BYTE_INIT:
      *(hal_conf.ddr_sck) |= (1 << hal_conf.pin_sck);
      *(hal_conf.ddr_mosi) |= (1 << hal_conf.pin_mosi);

      SPCR = ((1 << SPE) | (1 << MSTR));

      switch (u8x8->display_info->spi_mode) {
        case 0:
          break;
        case 1:
          SPCR |= (1 << CPHA);
          break;
        case 2:
          SPCR |= (1 << CPOL);
          break;
        case 3:
          SPCR |= (1 << CPOL);
          SPCR |= (1 << CPHA);
          break;
      };

      switch (F_CPU / u8x8->display_info->sck_clock_hz) {
        case 2:
          SPSR |= (1 << SPI2X);
          break;
        case 4:
          break;
        case 8:
          SPSR |= (1 << SPI2X);
          SPCR |= (1 << SPR0);
          break;
        case 16:
          SPCR |= (1 << SPR0);
          break;
        case 32:
          SPSR |= (1 << SPI2X);
          SPCR |= (1 << SPR1);
          break;
        case 64:
          SPCR |= (1 << SPR1);
          break;
        case 128:
          SPCR |= (1 << SPR1);
          SPCR |= (1 << SPR0);
          break;
      }

      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      break;
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_gpio_SetDC(u8x8, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
      break;
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *) arg_ptr;
      while (arg_int > 0) {
          SPDR = (uint8_t) * data;
          while (!(SPSR & (1 << SPIF)));
          data++;
          arg_int--;
      }
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      break;
    default:
      return 0;
  }
  
  return 1;
}

uint8_t u8x8_byte_avr_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	uint8_t *data;
	switch(msg){
		case U8X8_MSG_BYTE_SEND:
			data = (uint8_t *)arg_ptr;
			while( arg_int-- )
				i2c_write(*data++);
			break;
		case U8X8_MSG_BYTE_INIT:
			i2c_init();
			break;
		case U8X8_MSG_BYTE_SET_DC:
			/* ignored for i2c */
			break;
		case U8X8_MSG_BYTE_START_TRANSFER:
			i2c_start_wait(u8x8_GetI2CAddress(u8x8)+I2C_WRITE);
			break;
		case U8X8_MSG_BYTE_END_TRANSFER:
			i2c_stop();
			break;
		default:
			return 0;
	}
	return 1;
}

uint8_t u8x8_avr_delay (u8x8_t * u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
	uint8_t cycles;

	switch(msg) {
		case U8X8_MSG_DELAY_NANO:  // delay arg_int * 1 nano second
			// At 20Mhz, each cycle is 50ns, the call itself is slower.
			break;
		case U8X8_MSG_DELAY_100NANO:       // delay arg_int * 100 nano seconds
			// Approximate best case values...
			#define CALL_CYCLES 26UL
			#define CALC_CYCLES 4UL
			#define RETURN_CYCLES 4UL
			#define CYCLES_PER_LOOP 4UL

			cycles = (100UL * arg_int) / (P_CPU_NS * CYCLES_PER_LOOP);

			if (cycles > CALL_CYCLES + RETURN_CYCLES + CALC_CYCLES)
				break;

			__asm__ __volatile__ (
			  "1: sbiw %0,1" "\n\t"  // 2 cycles
			  "brne 1b":"=w" (cycles):"0" (cycles)  // 2 cycles
			);
			break;
		case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
			while( arg_int-- ) _delay_us(10);
			break;
		case U8X8_MSG_DELAY_MILLI:  // delay arg_int * 1 milli second
			while( arg_int-- ) _delay_ms(1);
			break;
		default:
			return 0;
	}

	return 1;
}

uint8_t u8x8_gpio_and_delay (u8x8_t * u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
	// Re-use library for delays
	if (u8x8_avr_delay(u8x8, msg, arg_int, arg_ptr))
	return 1;

	switch (msg) {
		// called once during init phase of u8g2/u8x8
		// can be used to setup pins
		case U8X8_MSG_GPIO_AND_DELAY_INIT:
			*(hal_conf.ddr_cs) |= (1 << hal_conf.pin_cs);
			*(hal_conf.ddr_dc) |= (1 << hal_conf.pin_dc);
			*(hal_conf.ddr_rst) |= (1 << hal_conf.pin_rst);
		break;
		// CS (chip select) pin: Output level in arg_int
		case U8X8_MSG_GPIO_CS:
			if (arg_int)
				*(hal_conf.port_cs) |= (1 << hal_conf.pin_cs);
			else
				*(hal_conf.port_cs) &= ~(1 << hal_conf.pin_cs);
			break;
		// DC (data/cmd, A0, register select) pin: Output level in arg_int
		case U8X8_MSG_GPIO_DC:
			if (arg_int)
				*(hal_conf.port_dc) |= (1 << hal_conf.pin_dc);
			else
				*(hal_conf.port_dc) &= ~(1 << hal_conf.pin_dc);
			break;
		// Reset pin: Output level in arg_int
		case U8X8_MSG_GPIO_RESET:
			if (arg_int)
				*(hal_conf.port_rst) |= (1 << hal_conf.pin_rst);
			else
				*(hal_conf.port_rst) &= ~(1 << hal_conf.pin_rst);
			break;
		default:
			u8x8_SetGPIOResult(u8x8, 1);
		break;
	}
	return 1;
}