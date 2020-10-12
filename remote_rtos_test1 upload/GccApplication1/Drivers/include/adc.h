#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#include <stdbool.h>

/*** Enums ***/
// ADC References
typedef enum
{
	AREF = 0b00,
	AVcc = 0b01,
	R1V1 = 0b11
} eRefs;

// ADC Clock Dividers
typedef enum
{
	DIV2 = 0b000,
	DIV4 = 0b010,
	DIV8 = 0b011,
	DIV16 = 0b100,
	DIV32 = 0b101,
	DIV64 = 0b110,
	DIV128 = 0b111,
} eAdps;

// ADC Channels
typedef enum
{
	ADC0 = 0b0000,
	ADC1 = 0b0001,
	ADC2 = 0b0010,
	ADC3 = 0b0011,
	ADC4 = 0b0100,
	ADC5 = 0b0101,
	ADC6 = 0b0110,
	ADC7 = 0b0111,
	TEMP = 0b1000,
	C1V1 = 0b1110,
	GND  = 0b1111
} eAdmux;

/*** Function Prototypes ***/
void adc_start_single(eRefs refs, eAdps adps, eAdmux channel);
bool adc_is_finished();
uint16_t adc_get_value();


#endif /* ADC_DRIVER_H_ */