#ifndef DISPLAY_EXTRAS_H_
#define DISPLAY_EXTRAS_H_

void drawCursorWithFrame(u8g2_t *u8g2, uint8_t pos_x, uint8_t pos_y, uint8_t w, uint8_t h, uint16_t cursor_x, uint16_t cursor_y);
void drawHBarGraph(u8g2_t *u8g2, uint8_t pos_x, uint8_t pos_y, uint8_t w, uint8_t h, uint8_t fill_px, bool start_from_right);


#endif /* DISPLAY_EXTRAS_H_ */