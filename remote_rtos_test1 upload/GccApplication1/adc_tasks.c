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

// Drivers
#include "adc.h"

/*** Tasks ***/
void task_adc(void *pvParameters){
	// Get Context Pointer
	context_t *contextPtr = (context_t * )pvParameters;
	if(contextPtr == NULL){
		vTaskDelete(NULL);
	}
	
	while(true){
		adc_data_t adc_buffer;
		
		// Get reading for X axis
		adc_start_single(AVcc, DIV32, ADC0);
		while(!adc_is_finished());
		adc_buffer.x = adc_get_value();
		
		// Get reading for Y axis
		adc_start_single(AVcc, DIV32, ADC1);
		while(!adc_is_finished());
		adc_buffer.y = adc_get_value();
		
		/* 
		 * Get reading for Vcc
		 * 
		 * Need to wait after switching to 1V1 before getting useful data.
		 * https://www.avrfreaks.net/forum/avrs-measure-their-own-vcc-do-it-badly
		 */
		adc_start_single(AVcc, DIV2, C1V1);
		vTaskDelay(1 / portTICK_PERIOD_MS);
		adc_start_single(AVcc, DIV32, C1V1);
		while(!adc_is_finished());
		adc_buffer.vin = adc_get_value();
		
		// Send or cancel a test alarm
		if(adc_buffer.x > 800){
			contextPtr->persistant_alarms[1] = alarm_sound2;
		}
		
		if(adc_buffer.y > 800){
			contextPtr->persistant_alarms[1] = alarm_stop;
		}
		
		// Send the adc values to corresponding queue
		xQueueSendToFront(contextPtr->queue_adc_data, &adc_buffer, 1);
		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
}
