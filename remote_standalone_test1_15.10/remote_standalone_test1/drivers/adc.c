/*** Includes ***/
// Standard C
#include <stdlib.h>
#include <stdbool.h>

// AVR
#include <avr/io.h>
#include <avr/interrupt.h>

// Application
#include "adc.h"

// Global Variables
volatile bool adc_finish = false;
volatile uint16_t adc_value;

// Change the ADC Channel
void adc_set_channel(adc_channels_e channel){
	ADMUX = (ADMUX & 0xf0) | (0x0f & channel);
	DIDR0 = 0x3f & (1 << channel);
}

// Change the ADC Reference
void adc_set_refs(adc_refs_e refs){
	ADMUX = (ADMUX & 0xC0) | ((0x03 & refs) << 6);
}

// Change the ADC Prescaler
void adc_set_adps(adc_adps_e adps){
	ADCSRA = (ADCSRA & 0xf8) | (0x07 & adps);
}

// Start the ADC in single conversion mode.
void adc_start_single(){
	ADCSRA = (ADCSRA & 0x87) | (1 << ADEN) | (1 << ADSC) | (1 << ADIE);
	adc_finish = false;
}

// Enable the ADC
void adc_disable(){
	ADCSRA |= 1 << ADEN;
}

// Disable the ADC
void adc_enable(){
	ADCSRA &= ~(1 << ADEN);
}

// Get if a conversion has finished
bool adc_is_finished(){
	return adc_finish;
}

// Get if a conversion is in progress
bool adc_is_converting(){
	return ADCSRA & (1 << ADSC);
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