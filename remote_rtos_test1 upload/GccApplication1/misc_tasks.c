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

// Drivers
#include "serial.h"

/*** Tasks ***/
void task_serial_test(void *pvParameters){
	// Get Context Pointer
	context_t *contextPtr = (context_t * )pvParameters;
	if(contextPtr == NULL){
		vTaskDelete(NULL);
	}
	
	portTickType lastCalledTime;
	
	// Init Serial Driver
	xComPortHandle serialInstance = xSerialPortInit( 115200ul, 128 );
	while(true){
		// Utility Buffers
		char buffer[50];
		adc_data_t adc_buffer;
		
		// Check if there's available ADC reading packets
		if(uxQueueMessagesWaiting(contextPtr->queue_adc_data) > 0){
			// If so, receive and print
			xQueueReceive(contextPtr->queue_adc_data, &adc_buffer, 5 / portTICK_PERIOD_MS);
			snprintf(buffer, 50, "Vin: %4d, X: %4d, Y: %4d, Heap: %d\n\r", adc_buffer.vin, adc_buffer.x, adc_buffer.y, xPortGetFreeHeapSize());
			xSerialPutString(serialInstance, buffer, 50, 2 / portTICK_PERIOD_MS);
		} else {
			// Otherwise, only print free heap
			snprintf(buffer, 50, "Heap: %d\n\r", xPortGetFreeHeapSize());
			xSerialPutString(serialInstance, buffer, 50, 2 / portTICK_PERIOD_MS);
		}
		
		lastCalledTime = xTaskGetTickCount();
		vTaskDelayUntil(&lastCalledTime, 100 / portTICK_PERIOD_MS);
	}
}