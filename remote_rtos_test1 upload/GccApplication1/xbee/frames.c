/*** Includes ***/
// Standard C
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// Application
#include "xbee.h"

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "queue.h"

/*** Functions ***/
bool xbeeWrapFrame(frame_raw_t *frame_buf, frame_types_e frame_type, uint16_t data_len, char *data){
	uint8_t acc = 0;
	
	frame_buf->frame_data_size = sizeof(char) * (data_len + 5);
	frame_buf->frame_data_ptr = pvPortMalloc(frame_buf->frame_data_size);
	
	if(frame_buf->frame_data_ptr == NULL){
		return false;
	}

	for(uint16_t varLoop = 0; varLoop < data_len; varLoop++){
		acc += data[varLoop];
		frame_buf->frame_data_ptr[varLoop + 4] = data[varLoop];
	}
	
	acc += frame_type;
	frame_buf->frame_data_ptr[data_len + 4] = 0xff - acc;
	
	frame_buf->frame_data_ptr[0] = 0x7e;
	frame_buf->frame_data_ptr[1] = (data_len + 1) & 0xff00;
	frame_buf->frame_data_ptr[2] = (data_len + 1) & 0x00ff;
	frame_buf->frame_data_ptr[3] = frame_type;
	
	return true;
}

void xbeeInitRx(frame_rx_t *rx){
	rx->frame_data_size = 0;
	rx->frame_rx_pos = 0;
	rx->frame_buf_ptr = NULL;
	rx->cksum_acc = 0;
}

int8_t xbeeRecvByte(frame_rx_t *rx, char data){
	if(data == 0x7e){
		rx->frame_rx_pos = 1;
		rx->cksum_acc = 0;
		return 0;
	} else if(rx->frame_rx_pos == 1){
		rx->frame_data_size = data & 0xff00;
		rx->frame_rx_pos = 2;
		return 0;
	} else if(rx->frame_rx_pos == 2){
		rx->frame_data_size = data & 0x00ff;
		rx->frame_data_size -= 1;
		rx->frame_buf_ptr = pvPortMalloc(sizeof(char) * rx->frame_data_size);
		if(rx->frame_buf_ptr == 0){
			rx->frame_rx_pos = 0;
			return -1;
		} else {
			rx->frame_rx_pos = 3;
		}
		return 0;
	} else if(rx->frame_rx_pos == 3){
		rx->frame_type = data;
		rx->cksum_acc += data;
		rx->frame_rx_pos = 4;
		return 0;
	} else if(rx->frame_rx_pos < rx->frame_data_size + 4){
		rx->frame_buf_ptr[rx->frame_rx_pos - 4] = data;
		rx->cksum_acc += data;
		rx->frame_rx_pos++;
		return 0;
	} else if(rx->frame_rx_pos == rx->frame_data_size + 4){
		rx->cksum_acc += data;
		if(rx->cksum_acc != 0xff){
			return -1;
		} else {
			rx->frame_rx_pos = 0;
			return 1;
		}
	} else {
		return -1;
	}
}