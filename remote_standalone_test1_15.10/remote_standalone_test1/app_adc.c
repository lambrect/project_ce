/*** Includes ***/
// Standard C
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Application
#include "context.h"
#include "apps.h"

// Drivers
#include "adc.h"

// Locally stored pointer
context_t *contextPtr;

/*** Apps ***/
void app_adc_init(void *pvParameters){
	// Get Context Pointer
	// Save a copy of the incoming pointer
	contextPtr = (context_t * )pvParameters;
	if(contextPtr == NULL){
		return;
	}
}

void app_adc(){
	adc_data_processed_t adc_buffer;
	
	// Set ADC Params
	adc_set_refs(AVcc);		// Set Reference
	adc_set_adps(DIV32);	// Set Prescaler
	adc_enable();
	
	// Get reading for X axis
	adc_set_channel(ADC0);
	while(adc_is_converting());
	adc_start_single();
	while(!adc_is_finished());
	contextPtr->buf_adc_data_raw.x = adc_get_value();			// Save Raw Data
	adc_buffer.x = (contextPtr->buf_adc_data_raw.x >> 1) - 256;	// Save the calculated Data
	
	// Get reading for Y axis
	adc_set_channel(ADC1);
	while(adc_is_converting());
	adc_start_single();
	while(!adc_is_finished());
	contextPtr->buf_adc_data_raw.y = adc_get_value();
	adc_buffer.y = 255 - (contextPtr->buf_adc_data_raw.y >> 1);
	
	/* 
	 * Get reading for Vcc
	 * 
	 * Need to wait after switching to 1V1 before getting useful data.
	 * https://www.avrfreaks.net/forum/avrs-measure-their-own-vcc-do-it-badly
	 */
	adc_set_channel(C1V1);
	_delay_ms(1);
	adc_start_single();
	while(!adc_is_finished());
	contextPtr->buf_adc_data_raw.vin = adc_get_value();
	adc_buffer.vin = contextPtr->buf_adc_data_raw.vin;
	
	// Set the move flag if value crossed deadzone
	adc_buffer.move_x = abs(adc_buffer.x) >= 10;
	adc_buffer.move_y = abs(adc_buffer.y) >= 10;
	
	// Send or cancel a test alarm
	if(PIND & (1 << PIND2)){
		contextPtr->persistant_alarms[1] = alarm_sound3;
	}else{
		contextPtr->persistant_alarms[1] = alarm_stop;
	}
	
	// Send the adc values to buffer
	// Critical section, ensure data integrity
	cli();
	contextPtr->buf_adc_data_processed = adc_buffer;
	sei();
}
