/*** Includes ***/
// Standard C
#include <stdlib.h>
#include <stdbool.h>

// AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Application
#include "context.h"
#include "apps.h"

// Global variable between all modules
context_t contextInst;

int main(void){
	// Initialize Modules
	app_adc_init(&contextInst);
	app_gui_init(&contextInst);
	app_speaker_init(&contextInst);
	app_serial_init(&contextInst);
	
	// Start System Timer
	OCR0A = 249;
	TIMSK0 = 1 << OCIE0A;
	TCCR0A = (1 << WGM01) | (1 << WGM00);
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	
	sei();
	
	while(true){
		app_adc();
		app_gui();
		app_speaker();
		app_serial();
	}
}

ISR(TIMER0_COMPA_vect){
	contextInst.millis++;
}