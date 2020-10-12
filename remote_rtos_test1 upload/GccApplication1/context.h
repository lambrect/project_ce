#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "app_tasks.h"

#define PERSISTANT_ALARMS_SIZE 4
typedef struct
{
	// Queue for ADC data
	QueueHandle_t queue_adc_data;
	
	// Array for alarm states
	// Each alarm shall have a unique place in the array
	volatile alarm_types_e persistant_alarms[PERSISTANT_ALARMS_SIZE];
} context_t;

#endif