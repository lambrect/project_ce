/*** Includes ***/
// Standard C
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// AVR
#include <avr/io.h>

// Libraries
#include "u8g2.h"
#include "u8x8_avr.h"

// Application
#include "display_extras.h"
#include "context.h"
#include "apps.h"

context_t *contextPtr;
u8g2_t u8g2;

// System Time for FPS Calculation
uint16_t last_time;

/*** Apps ***/
void app_gui_init(void * pvParameters){
	// Get Context Pointer
	contextPtr = (context_t * )pvParameters;
	if(contextPtr == NULL){
		return;
	}
	
	// U8G2 Setup
	u8g2_Setup_sh1106_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_avr_hw_i2c, u8x8_avr_delay);
	u8g2_SetI2CAddress(&u8g2, 0x78);
	
	// Start Display
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
	u8g2_SetContrast(&u8g2, 0);
}

void app_gui(){
	// Utility Buffer
	char buffer[50];
	
	/*** Calculate data for UI elements ***/
	// Calculate FPS
	uint16_t diff_time = contextPtr->millis - last_time;
	last_time = contextPtr->millis;
	
	/*** Draw the UI elements ***/
	u8g2_ClearBuffer(&u8g2);
	
	u8g2_SetFontMode(&u8g2, 0);
	u8g2_SetDrawColor(&u8g2, 1);
	
	drawCursorWithFrame(&u8g2, 90, 28, 32, 32, contextPtr->buf_adc_data_raw.x / 32, contextPtr->buf_adc_data_raw.y / 32);
	
	drawHBarGraph(&u8g2, 95, 0, 32, 6, (204L * (32 - 2)) / contextPtr->buf_adc_data_processed.vin, true);
	
	u8g2_SetFont(&u8g2, u8g2_font_4x6_mf);
	
	snprintf(buffer, 50, "TIME: %u", contextPtr->millis);
	u8g2_DrawStr(&u8g2, 0, 5, buffer);
	snprintf(buffer, 50, "FPS: %u", 1000 / diff_time);
	u8g2_DrawStr(&u8g2, 0, 11, buffer);
	
	snprintf(buffer, 50, "L/R:%4d", contextPtr->buf_adc_data_processed.x);
	u8g2_DrawStr(&u8g2, 0, 17, buffer);
	snprintf(buffer, 50, "F/B:%4d", contextPtr->buf_adc_data_processed.y);
	u8g2_DrawStr(&u8g2, 0, 23, buffer);
	snprintf(buffer, 50, "M_X: %c, M_Y: %c", contextPtr->buf_adc_data_processed.move_x ? 'Y' : 'N', contextPtr->buf_adc_data_processed.move_y ? 'Y' : 'N');
	u8g2_DrawStr(&u8g2, 0, 29, buffer);
	
	
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