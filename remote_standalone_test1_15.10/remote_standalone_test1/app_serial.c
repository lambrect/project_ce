/*** Includes ***/
// Standard C
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// AVR
#include <avr/io.h>
#include <avr/interrupt.h>

// Drivers
#include "usart.h"

// Application
#include "context.h"

context_t *contextPtr;

/*** Apps ***/
void app_serial_init(void *pvParameters){
	// Get Context Pointer
	contextPtr = (context_t * )pvParameters;
	if(contextPtr == NULL){
		return;
	}
	
	// Init Serial Driver
	uart_set_FrameFormat(USART_8BIT_DATA|USART_1STOP_BIT|USART_NO_PARITY|USART_ASYNC_MODE);
	uart_init(DOUBLE_BAUD_CALC(115200));
};

void app_serial(){
	// Utility Buffers
	char buffer[50];
	
	// Receive ADC data buffer
	adc_data_processed_t adc_buffer = contextPtr->buf_adc_data_processed;
	
	if(adc_buffer.move_x || adc_buffer.move_y){
		snprintf(buffer, 50, "X: %4d, Y: %4d\n\r", adc_buffer.move_x ? adc_buffer.x : 0, adc_buffer.move_y ? adc_buffer.y : 0);
		uart_puts(buffer);
	}
}