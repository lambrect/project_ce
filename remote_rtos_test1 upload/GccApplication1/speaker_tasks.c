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

/*** Support Functions ***/
// Set timer 2 to output a waveform
void setBuzzer(uint16_t val){
	val = val * 2;
	portENTER_CRITICAL();
	{
		if(val > 0){
			DDRD |= (1 << DDD3);
			OCR2A = val;
			OCR2B = val / 2;
			TCNT2 = 0;
			TIMSK2 = 0;
			TCCR2A = 0b00100011;
			TCCR2B = 0b00001110;
			}else{
			TCCR2B = 0;
		}
	}
	portEXIT_CRITICAL();
}

// Rudimentary function to play sound patterns
void playSound(uint8_t sound){
	portTickType lastCalledTime;
	
	switch(sound){
		case alarm_sound1:
		for(int varLoop0 = 0; varLoop0 < 3; varLoop0++){
			for(int varLoop1 = 0; varLoop1 < 3; varLoop1++){
				lastCalledTime = xTaskGetTickCount();
				setBuzzer(12);
				vTaskDelayUntil(&lastCalledTime, 30 / portTICK_PERIOD_MS);
				lastCalledTime = xTaskGetTickCount();
				setBuzzer(16);
				vTaskDelayUntil(&lastCalledTime, 20 / portTICK_PERIOD_MS);
				lastCalledTime = xTaskGetTickCount();
				setBuzzer(0);
				vTaskDelayUntil(&lastCalledTime, 40 / portTICK_PERIOD_MS);
			}
			lastCalledTime = xTaskGetTickCount();
			vTaskDelayUntil(&lastCalledTime, 60 / portTICK_PERIOD_MS);
		}
		lastCalledTime = xTaskGetTickCount();
		vTaskDelayUntil(&lastCalledTime, 750 / portTICK_PERIOD_MS);
		break;
		
		case alarm_sound2:
		lastCalledTime = xTaskGetTickCount();
		setBuzzer(10);
		vTaskDelayUntil(&lastCalledTime, 1000 / portTICK_PERIOD_MS);
		lastCalledTime = xTaskGetTickCount();
		setBuzzer(16);
		vTaskDelayUntil(&lastCalledTime, 1000 / portTICK_PERIOD_MS);
		setBuzzer(0);
		break;
		
		case alarm_sound3:
		for(int varLoop1 = 0; varLoop1 < 5; varLoop1++){
			lastCalledTime = xTaskGetTickCount();
			setBuzzer(10);
			vTaskDelayUntil(&lastCalledTime, 50 / portTICK_PERIOD_MS);
			lastCalledTime = xTaskGetTickCount();
			setBuzzer(0);
			vTaskDelayUntil(&lastCalledTime, 50 / portTICK_PERIOD_MS);
		}
		break;
		
		default:
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}

/*** Tasks ***/
// Speaker Task
void task_speaker(void *pvParameters){
	// Get Context Pointer
	context_t *contextPtr = (context_t * )pvParameters;
	if(contextPtr == NULL){
		vTaskDelete(NULL);
	}
	
	// Check for alarms
	while(true){
		for(uint8_t varLoop = 0; varLoop < PERSISTANT_ALARMS_SIZE; varLoop++){
			if(contextPtr->persistant_alarms[varLoop] != alarm_stop){
				playSound(contextPtr->persistant_alarms[varLoop]);
				break;
			}
		}
		
		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
}