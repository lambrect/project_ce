#ifndef TASKS_H_
#define TASKS_H_

void app_gui_init(void * pvParameters);
void app_gui();

void app_adc_init(void *pvParameters);
void app_adc();

void app_speaker_init(void *pvParameters);
void app_speaker();

void app_serial_init(void *pvParameters);
void app_serial();


// Enum for alarm types
typedef enum{
	alarm_stop = 0,
	alarm_sound1 = 8,
	alarm_sound2 = 16,
	alarm_sound3 = 24
} alarm_types_e;

// Struct for ADC data
typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t vin;
} adc_data_t;

typedef struct
{
	uint16_t data_len;
	char *data;
} ser_data_t;

#endif