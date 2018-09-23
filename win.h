#ifndef WIN_H
#define WIN_H

#include "fonts.h"

// display pins
#define RST_PIN 59
#define CS_PIN 58
#define DAT_CTL_PIN 57

// logic macros
#define BITS_IN_BYTE 8

//NOTE temporary
#define BMP_T unsigned char
#define COLOR_T unsigned short

/* create a P25317 obj for OLED interaction */
void win_init(void);

/* draw a single pixel at X and Y */
void win_put_pixel_xy(unsigned char x, unsigned char y);
/* draw a line from X1,Y1 to X2,Y2 */
void win_put_line(unsigned char x1, unsigned char y1,
		  unsigned char x2, unsigned char y2);
/* clear entire screen, either black or white (on or off) */
void win_clear_screen(unsigned char on_off);
/* draw a filled box from X1,Y1 to X2,Y2 */
void win_put_box(unsigned char x1, unsigned char y1,
		 unsigned char x2, unsigned char y2, unsigned char on_ff);
/* draw an empty box (using win_put_line) from X1,Y1, to X2,Y2 */
void win_put_box_empty(unsigned char x1, unsigned char y1,
		       unsigned char x2, unsigned char y2);
/* draw a bitmap image, top left corner at X, Y */
void win_put_bmp_xy(unsigned char x, unsigned char y, BMP_T bmp);
/* write text to the window with top left pixel at X, Y */
void win_put_text_xy(const char *text, unsigned char x, unsigned char y);
/* write a single character to the window with top left pixel at X, Y */
void win_put_char_xy(const char chr, unsigned char x, unsigned char y);
/* sets the font pointer to the desired font */
void win_set_font(FONT_T font);
/* returns current fonts height in pixels */
unsigned char win_get_font_height(void);
/* returns the length of a string in pixels for current font */
unsigned char win_get_str_len(const char *str);
/* sets background color to white or black (on or off) */
void win_set_bg_color(unsigned char on_off);
/* invert colors on display */
void win_invert_color(unsigned char inv);

#endif //WIN_H
