/*** Includes ***/
// Standard C
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// AVR
#include <avr/io.h>

// Application
#include "context.h"
#include "apps.h"

context_t *contextPtr;

/*** Support Functions ***/
// Set timer 2 to output a waveform
void setBuzzer(uint16_t val){
	val = val * 2;
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

/*** Apps ***/
void app_speaker_init(void *pvParameters){
	// Get Context Pointer
	contextPtr = (context_t * )pvParameters;
	if(contextPtr == NULL){
		return;
	}
}

void app_speaker(){
	// Check for alarms
	bool alarm_present;
	for(uint8_t varLoop = 0; varLoop < PERSISTANT_ALARMS_SIZE; varLoop++){
		if(contextPtr->persistant_alarms[varLoop] != alarm_stop){
			alarm_present = true;
			setBuzzer(contextPtr->persistant_alarms[varLoop]);
			break;
		} else {
			alarm_present = false;
		}
	}
	
	if(!alarm_present){
		setBuzzer(alarm_stop);
	}
}