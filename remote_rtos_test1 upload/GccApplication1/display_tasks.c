/*** Includes ***/
// Standard C
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// AVR
#include <avr/io.h>

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "queue.h"

// Application
#include "context.h"
#include "app_tasks.h"
#include "u8g2.h"
#include "u8x8_avr.h"

/*** Structs ***/
// Computed data for drawing
#define BUF1_SIZE 10
typedef struct{
	uint8_t line1_x1, line1_y1, line1_x2, line1_y2, line2_x1, line2_y1, line2_x2, line2_y2;
	uint8_t circle1_x, circle1_y, circle1_r, circle1_m;
	uint8_t box1_x, box1_y, box1_w, box1_h;
	char buf1[BUF1_SIZE];
} draw_data_t;

/*** Tasks ***/
void task_gui(void *pvParameters){
	// Get Context Pointer
	context_t *contextPtr = (context_t * )pvParameters;
	if(contextPtr == NULL){
		vTaskDelete(NULL);
	}
	
	portTickType lastCalledTime, thisCalledTime;
	
	draw_data_t draw_data;
	
	// U8G2 Setup
	u8g2_t u8g2;
	u8g2_Setup_ise97002ks_gu128x64_1(&u8g2, U8G2_R0, u8x8_byte_avr_hw_spi, u8x8_gpio_and_delay);
	
	// Start Display
	u8g2_InitDisplay(&u8g2);
	u8g2_SetContrast(&u8g2, 0);
	
	while(true){
		/*** Calculate data for UI elements ***/
		// FPS counter
		thisCalledTime = xTaskGetTickCount();
		uint8_t fps = 1000 / (thisCalledTime - lastCalledTime);
		lastCalledTime = thisCalledTime;
		
		snprintf(draw_data.buf1, BUF1_SIZE, "FPS: %d", fps);
		
		// Get the latest ADC data
		// Using xQueuePeek to not remove the item from the queue
		adc_data_t adc_buffer;
		xQueuePeek(contextPtr->queue_adc_data, &adc_buffer, 1);
		
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
		
		/*** Draw the UI elements ***/
		u8g2_FirstPage(&u8g2);
		do{
			
			u8g2_DrawLine(&u8g2, draw_data.line1_x1, draw_data.line1_y1, draw_data.line1_x2, draw_data.line1_y2);
			u8g2_DrawLine(&u8g2, draw_data.line2_x1, draw_data.line2_y1, draw_data.line2_x2, draw_data.line2_y2);
			u8g2_DrawCircle(&u8g2, draw_data.circle1_x, draw_data.circle1_y, draw_data.circle1_r, draw_data.circle1_m);
			u8g2_DrawFrame(&u8g2, 10 + 50, 10, 32, 32);
			
			u8g2_DrawFrame(&u8g2, 122, 0, 6, 64);
			u8g2_DrawBox(&u8g2, draw_data.box1_x, draw_data.box1_y, draw_data.box1_w, draw_data.box1_h);
			
			u8g2_SetFont(&u8g2, u8g2_font_4x6_mf);
			u8g2_DrawStr(&u8g2, 0, 5, draw_data.buf1);
			
		}while(u8g2_NextPage(&u8g2));
		
		vTaskDelayUntil(&lastCalledTime, 1 / portTICK_PERIOD_MS);
	}
}