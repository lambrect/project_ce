#ifndef TASKS_H_
#define TASKS_H_

void task_serial_xbee(void *pvParameters);
void task_serial_test(void *pvParameters);
void task_adc(void *pvParameters);
void task_speaker(void *pvParameters);
void task_gui(void *pvParameters);

// Enum for alarm types
typedef enum{
	alarm_stop,
	alarm_sound1,
	alarm_sound2,
	alarm_sound3
} alarm_types_e;

// Struct for ADC data
typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t vin;
} adc_data_t;

#endif