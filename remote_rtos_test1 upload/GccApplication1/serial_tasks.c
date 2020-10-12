/*** Includes ***/
// Standard C
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// AVR
#include <avr/io.h>
#include <avr/interrupt.h>

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "queue.h"

// Application
#include "context.h"
#include "app_tasks.h"
#include "xbee.h"

// Drivers
#include "serial.h"

void task_serial_xbee(void *pvParameters){
	// Get Context Pointer
	context_t *contextPtr = (context_t * )pvParameters;
	if(contextPtr == NULL){
		vTaskDelete(NULL);
	}
	
	// Init Serial Driver
	xComPortHandle serialInstance = xSerialPortInit( 115200ul, 128 );
	
	// Prepare test data
	char test_data[] = {0x01, 0x48, 0x56};
	
	while(true){
		frame_raw_t test_frame;
		
		// Wrap the test data frame for output
		xbeeWrapFrame(&test_frame, 0x08, 0x0003, test_data);
		
		// Output the built frame through the serial port
		xSerialPutArray(serialInstance, test_frame.frame_data_ptr, test_frame.frame_data_size, 1);
		
		// Free the allocated memory
		vPortFree(test_frame.frame_data_ptr);
		
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}