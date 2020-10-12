#ifndef XBEE_H_
#define XBEE_H_

#include <stdbool.h>

typedef enum {
	local_at_req = 0x08,
	local_at_queue = 0x09,
	tx_request = 0x10,
	explicit_addr_req = 0x11,
	remote_at_req = 0x17,
	create_src_route = 0x21,
	local_at_response = 0x88,
	modem_stat = 0x8a,
	ext_transmit_stat = 0x8b,
	rx_pkt = 0x90,
	explicit_rx_ind = 0x91,
	io_sample_ind = 0x92,
	xbee_sens_read_ind = 0x94,
	node_id_ind = 0x95,
	remote_at_response = 0x97,
	ext_modum_stat = 0x98,
	ota_stat = 0xa0,
	route_rec_ind = 0xa1,
	mto_route_req_ind = 0xa3
} frame_types_e;

typedef struct {
	uint16_t frame_data_size;
	char *frame_data_ptr;
} frame_raw_t;

typedef struct {
	uint16_t frame_data_size;
	uint16_t frame_rx_pos;
	char *frame_buf_ptr;
	uint8_t cksum_acc;
	frame_types_e frame_type;
} frame_rx_t;



bool xbeeWrapFrame(frame_raw_t *frame_buf, frame_types_e frame_type, uint16_t data_len, char *data);
void xbeeInitRx(frame_rx_t *rx);
int8_t xbeeRecvByte(frame_rx_t *rx, char data);

#endif /* XBEE_H_ */