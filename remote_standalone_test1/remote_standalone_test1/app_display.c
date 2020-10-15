/*** Includes ***/
// Standard C
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// AVR
#include <avr/io.h>

// Application
#include "context.h"
#include "apps.h"
#include "u8g2.h"
#include "u8x8_avr.h"

/*** Structs ***/
// Computed data for drawing
#define BUF1_SIZE 10
#define BUF2_SIZE 10
#define BUF3_SIZE 20
#define BUF4_SIZE 20
#define BUF5_SIZE 10
typedef struct{
	uint8_t line1_x1, line1_y1, line1_x2, line1_y2, line2_x1, line2_y1, line2_x2, line2_y2;
	uint8_t circle1_x, circle1_y, circle1_r, circle1_m;
	uint8_t box1_x, box1_y, box1_w, box1_h;
	char buf1[BUF1_SIZE], buf2[BUF2_SIZE], buf3[BUF3_SIZE], buf4[BUF4_SIZE], buf5[BUF5_SIZE];
} draw_data_t;

context_t *contextPtr;
u8g2_t u8g2;

/*** Apps ***/
void app_gui_init(void * pvParameters){
	// Get Context Pointer
	contextPtr = (context_t * )pvParameters;
	if(contextPtr == NULL){
		return;
	}
	
	// U8G2 Setup
	u8x8_conf_t u8x8_conf= {
		.ddr_sck = &DDRB,
		.port_sck = &PORTB,
		.pin_sck = 5,
		
		.ddr_mosi = &DDRB,
		.port_mosi = &PORTB,
		.pin_mosi = 3,
		
		.ddr_cs = &DDRB,
		.port_cs = &PORTB,
		.pin_cs = 2,
		
		.ddr_dc = &DDRB,
		.port_dc = &PORTB,
		.pin_dc = 1,
		
		.ddr_rst = &DDRB,
		.port_rst = &PORTB,
		.pin_rst = 0
	};
	u8x8_init_conf(&u8x8_conf);
	//u8g2_Setup_ise97002ks_gu128x64_1(&u8g2, U8G2_R0, u8x8_byte_avr_hw_spi, u8x8_gpio_and_delay);
	u8g2_Setup_ise97002ks_gu128x64_f(&u8g2, U8G2_R0, u8x8_byte_avr_hw_spi, u8x8_gpio_and_delay);
	
	// Start Display
	u8g2_InitDisplay(&u8g2);
	u8g2_SetContrast(&u8g2, 0);
}

draw_data_t draw_data;
void app_gui(){
	/*** Calculate data for UI elements ***/
	// Get the latest ADC data
	adc_data_t adc_buffer = contextPtr->buf_adc_data;
	
	uint16_t x_calc = abs((adc_buffer.x / 2) - 255);
	if(x_calc > 255){
		x_calc = 255;
	}
	uint16_t y_calc = abs((adc_buffer.y / 2) - 255);
	if(y_calc > 255){
		y_calc = 255;
	}
	bool x_dir = adc_buffer.x > 511;
	bool y_dir = adc_buffer.y > 511;
	
	bool move = (abs((adc_buffer.x / 2) - 255) >= 10) || (abs((adc_buffer.y / 2) - 255) >= 10);
	
	snprintf(draw_data.buf3, BUF3_SIZE, "L/R:%3d, DIR:%c", x_calc, x_dir ? 'R' : 'L');
	snprintf(draw_data.buf4, BUF4_SIZE, "F/B:%3d, DIR:%c", y_calc, y_dir ? 'B' : 'F');
	
	snprintf(draw_data.buf5, BUF5_SIZE, "MOVE: %c", move ? 'Y' : 'N');
	
	// Joystick Indicator
	draw_data.line1_x1 = (adc_buffer.x / 32) - 4 + 10 + 50;
	draw_data.line1_y1 = (adc_buffer.y / 32) + 10;
	draw_data.line1_x2 = (adc_buffer.x / 32) + 4 + 10 + 50;
	draw_data.line1_y2 = (adc_buffer.y / 32) + 10;
	
	draw_data.line2_x1 = (adc_buffer.x / 32) + 10 + 50;
	draw_data.line2_y1 = (adc_buffer.y / 32) - 4 + 10;
	draw_data.line2_x2 = (adc_buffer.x / 32) + 10 + 50;
	draw_data.line2_y2 = (adc_buffer.y / 32) + 4 + 10;
	
	draw_data.circle1_x = (adc_buffer.x / 32) + 10 + 50;
	draw_data.circle1_y = (adc_buffer.y / 32) + 10;
	draw_data.circle1_r = 2;
	draw_data.circle1_m = U8G2_DRAW_ALL;
	
	// Input Voltage Indicator
	draw_data.box1_w = 4;
	draw_data.box1_h = 11594l / adc_buffer.vin;
	draw_data.box1_x = 123;
	draw_data.box1_y = 1 + 62 - draw_data.box1_h;
	
	/*** Draw the UI elements ***//*
	u8g2_FirstPage(&u8g2);
	do{
		u8g2_SetFontMode(&u8g2, 0);
		u8g2_SetDrawColor(&u8g2, 1);
		
		u8g2_DrawLine(&u8g2, draw_data.line1_x1, draw_data.line1_y1, draw_data.line1_x2, draw_data.line1_y2);
		u8g2_DrawLine(&u8g2, draw_data.line2_x1, draw_data.line2_y1, draw_data.line2_x2, draw_data.line2_y2);
		u8g2_DrawCircle(&u8g2, draw_data.circle1_x, draw_data.circle1_y, draw_data.circle1_r, draw_data.circle1_m);
		u8g2_DrawFrame(&u8g2, 10 + 50, 10, 32, 32);
		
		u8g2_DrawFrame(&u8g2, 122, 0, 6, 64);
		u8g2_DrawBox(&u8g2, draw_data.box1_x, draw_data.box1_y, draw_data.box1_w, draw_data.box1_h);
		
		u8g2_SetFont(&u8g2, u8g2_font_4x6_mf);
		
		u8g2_DrawStr(&u8g2, 0, 5, draw_data.buf1);
		u8g2_DrawStr(&u8g2, 0, 11, draw_data.buf2);
		u8g2_DrawStr(&u8g2, 0, 17, draw_data.buf3);
		u8g2_DrawStr(&u8g2, 0, 23, draw_data.buf4);
		u8g2_DrawStr(&u8g2, 0, 29, draw_data.buf5);
		
		if(contextPtr->persistant_alarms[0] != alarm_stop){
			u8g2_DrawBox(&u8g2, 0, 31, 23, 9);
			} else {
			u8g2_DrawFrame(&u8g2, 0, 31, 23, 9);
		}
		
		if(contextPtr->persistant_alarms[1] != alarm_stop){
			u8g2_DrawBox(&u8g2, 0, 39, 23, 9);
			} else {
			u8g2_DrawFrame(&u8g2, 0, 39, 23, 9);
		}
		
		if(contextPtr->persistant_alarms[2] != alarm_stop){
			u8g2_DrawBox(&u8g2, 0, 47, 23, 8);
			} else {
			u8g2_DrawFrame(&u8g2, 0, 47, 23, 9);
		}
		
		if(contextPtr->persistant_alarms[3] != alarm_stop){
			u8g2_DrawBox(&u8g2, 0, 55, 23, 9);
			} else {
			u8g2_DrawFrame(&u8g2, 0, 55, 23, 9);
		}
		
		u8g2_SetFontMode(&u8g2, 1);
		u8g2_SetDrawColor(&u8g2, 2);
		
		u8g2_DrawStr(&u8g2, 2, 38, "TEST0");
		u8g2_DrawStr(&u8g2, 2, 46, "TEST1");
		u8g2_DrawStr(&u8g2, 2, 54, "TEST2");
		u8g2_DrawStr(&u8g2, 2, 62, "TEST3");
		
	}while(u8g2_NextPage(&u8g2));*/
	
	
	
	
	u8g2_ClearBuffer(&u8g2);
	
	
	u8g2_SetFontMode(&u8g2, 0);
	u8g2_SetDrawColor(&u8g2, 1);
	
	u8g2_DrawLine(&u8g2, draw_data.line1_x1, draw_data.line1_y1, draw_data.line1_x2, draw_data.line1_y2);
	u8g2_DrawLine(&u8g2, draw_data.line2_x1, draw_data.line2_y1, draw_data.line2_x2, draw_data.line2_y2);
	u8g2_DrawCircle(&u8g2, draw_data.circle1_x, draw_data.circle1_y, draw_data.circle1_r, draw_data.circle1_m);
	u8g2_DrawFrame(&u8g2, 10 + 50, 10, 32, 32);
	
	u8g2_DrawFrame(&u8g2, 122, 0, 6, 64);
	u8g2_DrawBox(&u8g2, draw_data.box1_x, draw_data.box1_y, draw_data.box1_w, draw_data.box1_h);
	
	u8g2_SetFont(&u8g2, u8g2_font_4x6_mf);
	
	u8g2_DrawStr(&u8g2, 0, 5, draw_data.buf1);
	u8g2_DrawStr(&u8g2, 0, 11, draw_data.buf2);
	u8g2_DrawStr(&u8g2, 0, 17, draw_data.buf3);
	u8g2_DrawStr(&u8g2, 0, 23, draw_data.buf4);
	u8g2_DrawStr(&u8g2, 0, 29, draw_data.buf5);
	
	if(contextPtr->persistant_alarms[0] != alarm_stop){
		u8g2_DrawBox(&u8g2, 0, 31, 23, 9);
		} else {
		u8g2_DrawFrame(&u8g2, 0, 31, 23, 9);
	}
	
	if(contextPtr->persistant_alarms[1] != alarm_stop){
		u8g2_DrawBox(&u8g2, 0, 39, 23, 9);
		} else {
		u8g2_DrawFrame(&u8g2, 0, 39, 23, 9);
	}
	
	if(contextPtr->persistant_alarms[2] != alarm_stop){
		u8g2_DrawBox(&u8g2, 0, 47, 23, 8);
		} else {
		u8g2_DrawFrame(&u8g2, 0, 47, 23, 9);
	}
	
	if(contextPtr->persistant_alarms[3] != alarm_stop){
		u8g2_DrawBox(&u8g2, 0, 55, 23, 9);
		} else {
		u8g2_DrawFrame(&u8g2, 0, 55, 23, 9);
	}
	
	u8g2_SetFontMode(&u8g2, 1);
	u8g2_SetDrawColor(&u8g2, 2);
	
	u8g2_DrawStr(&u8g2, 2, 38, "TEST0");
	u8g2_DrawStr(&u8g2, 2, 46, "TEST1");
	u8g2_DrawStr(&u8g2, 2, 54, "TEST2");
	u8g2_DrawStr(&u8g2, 2, 62, "TEST3");
	
	
	
	u8g2_SendBuffer(&u8g2);
	
	
}