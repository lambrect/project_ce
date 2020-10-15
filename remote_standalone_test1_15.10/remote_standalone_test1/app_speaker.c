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

context_t *contextPtr;

/*** Support Functions ***/
// Set timer 2 to output a waveform
void setBuzzer(uint16_t val){
	if(val > 0){
		OCR2A = val;
		OCR2B = val / 2;
		TCCR2A |= 1 << COM2B1;
	}else{
		TCCR2A &= ~(1 << COM2B1);
	}
}

/*** Apps ***/
void app_speaker_init(void *pvParameters){
	// Get Context Pointer
	contextPtr = (context_t * )pvParameters;
	if(contextPtr == NULL){
		return;
	}
	
	DDRD |= (1 << DDD3);
	OCR2A = 0;
	OCR2B = 0;
	TCNT2 = 0;
	TIMSK2 = 0;
	TCCR2A = (1 << WGM21) | (1 << WGM20);
	TCCR2B = (1 << WGM22) | (1 << CS22) | (1 << CS21);
	
	setBuzzer(alarm_sound3);
	_delay_ms(100);
	setBuzzer(alarm_stop);
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