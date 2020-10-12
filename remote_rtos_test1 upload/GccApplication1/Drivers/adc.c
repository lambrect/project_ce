/*** Includes ***/
// Standard C
#include <stdlib.h>
#include <stdbool.h>

// AVR
#include <avr/io.h>
#include <avr/interrupt.h>

// FreeRTOS
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

// Application
#include "adc.h"

// Global Variables
volatile bool adc_finish = false;
volatile uint16_t adc_value;

// Start the ADC in single conversion mode.
void adc_start_single(eRefs refs, eAdps adps, eAdmux channel){
	portENTER_CRITICAL();
	{	
		if(ADCSRA & (1 << ADSC)){
			ADCSRA &= ~(1 << ADEN);
		}
		
		ADMUX = ((0x03 & refs) << 6) | (0x0f & channel);
		DIDR0 = 0x3f & (1 << channel);
		ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADIE) | (0x07 & adps);
		
		adc_finish = false;
	}
	portEXIT_CRITICAL();
}

// Get if a conversion has finished
bool adc_is_finished(){
	return adc_finish;
}

// Get the current ADC value.
uint16_t adc_get_value(){
	adc_finish = false;	// Reset the flag
	return adc_value;	// Return the ADC Value
}

// ISRs
ISR(ADC_vect){	// ADC Interrupt
	adc_value = ADC;
	adc_finish = true;
}