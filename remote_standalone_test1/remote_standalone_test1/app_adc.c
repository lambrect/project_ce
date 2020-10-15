/*** Includes ***/
// Standard C
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// AVR
#include <avr/io.h>
#include <util/delay.h>

// Application
#include "context.h"
#include "apps.h"

// Drivers
#include "adc.h"

context_t *contextPtr;

/*** Apps ***/
void app_adc_init(void *pvParameters){
	// Get Context Pointer
	contextPtr = (context_t * )pvParameters;
	if(contextPtr == NULL){
		return;
	}
}

void app_adc(){
	adc_data_t adc_buffer;
	
	// Set ADC Params
	adc_set_refs(AVcc);
	adc_set_adps(DIV32);
	adc_enable();
	
	// Get reading for X axis
	adc_set_channel(ADC0);
	while(adc_is_converting());
	adc_start_single();
	while(!adc_is_finished());
	adc_buffer.x = adc_get_value();
	
	// Get reading for Y axis
	adc_set_channel(ADC1);
	while(adc_is_converting());
	adc_start_single();
	while(!adc_is_finished());
	adc_buffer.y = adc_get_value();
	
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
	adc_buffer.vin = adc_get_value();
	
	// Send or cancel a test alarm
	if(PIND & (1 << PIND2)){
		contextPtr->persistant_alarms[1] = alarm_sound2;
	}else{
		contextPtr->persistant_alarms[1] = alarm_stop;
	}
	
	// Send the adc values to buffer
	contextPtr->buf_adc_data = adc_buffer;
}
