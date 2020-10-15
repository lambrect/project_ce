#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "apps.h"

#define PERSISTANT_ALARMS_SIZE 4
typedef struct
{
	// System Time
	volatile uint16_t millis;
	
	// Buffer for Raw ADC Data
	// 0 - 1023 for x, y and vin (input voltage)
	volatile adc_data_raw_t buf_adc_data_raw;
	
	// Buffer for Processed ADC Data
	// -256 - +255 for x and y
	volatile adc_data_processed_t buf_adc_data_processed;
	
	// Buffer for Serial Data
	volatile ser_data_t buf_ser_data;
	
	// Array for alarm states
	// Each alarm shall have a unique place in the array
	volatile alarm_types_e persistant_alarms[PERSISTANT_ALARMS_SIZE];
} context_t;

#endif