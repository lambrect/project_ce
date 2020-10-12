/*** Includes ***/
// Standard C
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "queue.h"

// Application
#include "context.h"
#include "app_tasks.h"

context_t contextInst;

/*** Support Functions ***/
// Reboot using WDT
void reboot()
{
	portDISABLE_INTERRUPTS();
	wdt_enable(WDTO_15MS);
	while(true);
}

/*** Main ***/
int main(void)
{
	wdt_disable();
	
	// Create the necessary queues
	if((contextInst.queue_adc_data = xQueueCreate(1, sizeof(adc_data_t))) == NULL){
		// Reboot if the queue is not created
		reboot();
	}
	
	// Create the tasks
	xTaskCreate(&task_serial_test, "serial", 150, &contextInst, 2, NULL );
	xTaskCreate(&task_adc, "adc", 100, &contextInst, 3, NULL);
	
	xTaskCreate(&task_speaker, "alarm", 120, &contextInst, 1, NULL);
	xTaskCreate(&task_gui, "gui", 300, &contextInst, 1, NULL);
	
	// Start FreeRTOS
	vTaskStartScheduler();
	
	// Only reaches if FreeRTOS has crashed, reboot
	reboot();
}
