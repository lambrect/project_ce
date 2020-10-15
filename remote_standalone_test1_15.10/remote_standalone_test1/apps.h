#ifndef TASKS_H_
#define TASKS_H_

#include <stdbool.h>

void app_gui_init(void * pvParameters);
void app_gui();

void app_adc_init(void *pvParameters);
void app_adc();

void app_speaker_init(void *pvParameters);
void app_speaker();

void app_serial_init(void *pvParameters);
void app_serial();

// Enum for Alarm Types
typedef enum{
	alarm_stop = 0,
	alarm_sound1 = 8,
	alarm_sound2 = 16,
	alarm_sound3 = 24
} alarm_types_e;

// Struct for Raw ADC Data
typedef struct  
{
	uint16_t x, y, vin;
} adc_data_raw_t;

// Struct for Processed ADC Data
typedef struct
{
	int16_t x, y;
	bool move_x, move_y;
	uint16_t vin;
} adc_data_processed_t;

typedef struct
{
	uint16_t data_len;
	char *data;
} ser_data_t;

#endif