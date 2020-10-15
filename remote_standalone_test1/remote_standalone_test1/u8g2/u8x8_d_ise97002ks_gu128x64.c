#include "u8x8.h"

static const u8x8_display_info_t u8x8_ise97002ks_gu128x64_display_info =
{
	/* == General == */
	/* chip_enable_level = */ 0,
	/* chip_disable_level = */ 1,
	/* post_chip_enable_wait_ns = */ 40,
	/* pre_chip_disable_wait_ns = */ 150,
	/* reset_pulse_width_ms = */ 2,
	/* post_reset_wait_ms = */ 2,
	
	/* == SPI interface == */
	/* sda_setup_time_ns = */ 40,
	/* sck_pulse_width_ns = */ 80,
	/* sck_clock_hz = */ 5000000UL,
	/* spi_mode = */ 3, /* active low, rising edge */
	
	/* == I2C == */
	/* i2c_bus_clock_100kHz = */ 0,	/* Display does NOT support I2C */
	
	/* == 8 bit interface == */
	/* data_setup_time_ns = */ 30,
	/* write_pulse_width_ns = */ 100,
	
	/* == layout == */
	/* tile_width = */ 16,
	/* tile_hight = */ 8,
	/* default_x_offset = */ 0,
	/* flipmode_x_offset = */ 0,
	/* pixel_width = */ 128,
	/* pixel_height = */ 64
};

static void send_byte(u8x8_t *u8x8, uint8_t isCmd, uint8_t data)
{
	u8x8_cad_StartTransfer(u8x8);
	if (isCmd)
	{
		u8x8_cad_SendCmd(u8x8, data);
	} else {
		u8x8_cad_SendData(u8x8, 1, &data);
	}
	u8x8_cad_EndTransfer(u8x8);
}

//static uint8_t gram_layer = 0;
uint8_t u8x8_d_ise97002ks_gu128x64(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	uint8_t x, c;
	uint8_t *ptr;
	switch (msg)
	{
	case U8X8_MSG_DISPLAY_SETUP_MEMORY:
		u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ise97002ks_gu128x64_display_info);
		break;
		
	case U8X8_MSG_DISPLAY_INIT:
		// setup the communication interface
		u8x8_d_helper_display_init(u8x8);  
		
		// init the display by sending an init sequence
		send_byte(u8x8, 1, 0x5f);
		u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_MILLI, 1);
		
		for (uint8_t varLoop = 0; varLoop < 8; varLoop++)
		{
			send_byte(u8x8, 1, 0x62);
			send_byte(u8x8, 1,  varLoop);
			send_byte(u8x8, 0, 0xff);
		}
		
		send_byte(u8x8, 1, 0x84);	// Set Address Increment Mode to INC_X
		
		send_byte(u8x8, 1, 0x24);
		send_byte(u8x8, 1, 0x40);
		break;
		
	case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
		// arg_int == 0: wakeup, normal mode
		// arg_int == 1: power save 
		
		send_byte(u8x8, 1, 0x24);
		if(arg_int)
		{
			// Shutdown
			send_byte(u8x8, 1, 0x00);
		} else {
			// Normal
			send_byte(u8x8, 1, 0x40);
		}
		break;
		
#ifdef U8X8_WITH_SET_FLIP_MODE
	case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
		// This case should be compiled only with U8X8_WITH_SET_FLIP_MODE
		// arg_int == 0: normal, native orientation
		// arg_int == 1: 180 degree rotation
		break;
#endif
		
#ifdef U8X8_WITH_SET_CONTRAST
	case U8X8_MSG_DISPLAY_SET_CONTRAST:
		// This case should be compiled only with U8X8_WITH_SET_CONTRAST
		// arg_int: 0 (no contrast/brightness) to 255 (max contrast/brightness)
		send_byte(u8x8, 1, 0x40 | ((arg_int ^ 0xff) >> 4));
		break;
#endif
		
	case U8X8_MSG_DISPLAY_DRAW_TILE:
		/* arg_ptr: (u8x8_tile_t *) */
		/* arg_int: How often u8x8_tile_t should be repeated */
		
		x = ((u8x8_tile_t *)arg_ptr)->x_pos;
		x *= 8;
		x += u8x8->x_offset;
		
		send_byte(u8x8, 1, 0x64);
		send_byte(u8x8, 1, 0x7f & x);
		send_byte(u8x8, 1, 0x60);
		/*
		if(gram_layer == 1)
		{
			send_byte(u8x8, 1, 0x0f & (((u8x8_tile_t *)arg_ptr)->y_pos + 0x08));
		}else{
			send_byte(u8x8, 1, 0x0f & ((u8x8_tile_t *)arg_ptr)->y_pos);
		}*/
		send_byte(u8x8, 1, 0x0f & ((u8x8_tile_t *)arg_ptr)->y_pos);
		
		
		do
		{
			c = ((u8x8_tile_t *)arg_ptr)->cnt;
			ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
			
			for (uint8_t varLoop = 0; varLoop < c * 8; varLoop++)
			{
				send_byte(u8x8, 0, *(ptr + varLoop));
			}
			
			arg_int--;
		} while (arg_int > 0);
		break;
		
	case U8X8_MSG_DISPLAY_REFRESH:
		// LCD: Do nothing
		// e-Paper/e-Ink: Make controller RAM visible on the display.
		//    It can be assumed, that the controler is NOT in power save mode.
		/*
		if(gram_layer == 1)
		{
			send_byte(u8x8, 1, 0x28);
			gram_layer = 0;
		} else {
			send_byte(u8x8, 1, 0x24);
			gram_layer = 1;
		}
		send_byte(u8x8, 1, 0x40);*/
		break;
		
	default:
		return 0;
	}
	return 1;
}