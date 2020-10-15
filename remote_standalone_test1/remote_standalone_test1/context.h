#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "apps.h"

#define PERSISTANT_ALARMS_SIZE 4
typedef struct
{
	// Buffer for ADC data
	volatile adc_data_t buf_adc_data;
	
	// Buffer for Serial Data
	volatile ser_data_t buf_ser_data;
	
	// Array for alarm states
	// Each alarm shall have a unique place in the array
	volatile alarm_types_e persistant_alarms[PERSISTANT_ALARMS_SIZE];
} context_t;

#endif