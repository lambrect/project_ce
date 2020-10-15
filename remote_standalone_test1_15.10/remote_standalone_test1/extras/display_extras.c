/*** Includes ***/
// Standard C
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Libraries
#include "u8g2.h"

// Application
#include "display_extras.h"

void drawCursorWithFrame(u8g2_t *u8g2, uint8_t pos_x, uint8_t pos_y, uint8_t w, uint8_t h, uint16_t cursor_x, uint16_t cursor_y){
	// Frame
	u8g2_DrawFrame(u8g2, pos_x, pos_y, w, h);
	
	// Cursor
	u8g2_DrawLine(u8g2, pos_x + cursor_x - 4, pos_y + cursor_y, pos_x + cursor_x + 4, pos_y + cursor_y);
	u8g2_DrawLine(u8g2, pos_x + cursor_x, pos_y + cursor_y - 4, pos_x + cursor_x, pos_y + cursor_y + 4);
	u8g2_DrawCircle(u8g2, pos_x + cursor_x, pos_y + cursor_y, 2, U8G2_DRAW_ALL);
}

void drawHBarGraph(u8g2_t *u8g2, uint8_t pos_x, uint8_t pos_y, uint8_t w, uint8_t h, uint8_t fill_px, bool start_from_right){
	// Frame
	u8g2_DrawFrame(u8g2, pos_x, pos_y, w, h);
	
	// Bar
	if(start_from_right){
		u8g2_DrawBox(u8g2, pos_x - 1 + w - fill_px, pos_y + 1, fill_px, h - 2);
	} else {
		u8g2_DrawBox(u8g2, pos_x + 1, pos_y + 1, fill_px, h - 2);
	}
}