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

context_t contextInst;

int main(void){
	app_adc_init(&contextInst);
	app_gui_init(&contextInst);
	app_speaker_init(&contextInst);
	app_serial_init(&contextInst);
	
	sei();
	
	while(true){
		app_adc();
		app_gui();
		app_speaker();
		app_serial();
	}
}

