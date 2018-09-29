#ifndef WIN_H
#define WIN_H

#include "fonts.h"
#include "bmps.h"

// display pins
#define RST_PIN 59
#define CS_PIN 58
#define DAT_CTL_PIN 57

// logic macros
#define BITS_IN_BYTE 8
#define MAX_ROW 7
#define MAX_COL 127
#define FRAME_WIDTH_PIX 128
#define FRAME_HEIGHT_PIX 64
#define FRAME_HEIGHT_ROW 8
#define PIX_PER_ROW 8
#define INVERSE_ON 1
#define INVERSE_OFF 0
#define DEF_INVERSE INVERSE_OFF
#define BLACK 0
#define WHITE 1
#define TRANS_OFF 0
#define TRANS_ON 1

/* create a P25317 obj for OLED interaction */
void win_init(void);

/* draw a single pixel at X and Y */
void win_put_pixel_xy(unsigned char x, unsigned char y);

/* draw a line from X1 to X2 at row Y */
void win_put_line_horz(unsigned char x1, unsigned char x2, unsigned char pix_y);

/* draw a line from Y1 to Y2 at col X */
void win_put_line_vert(unsigned char pix_y1, unsigned char pix_y2, unsigned char x);

/* clear entire screen, either black or white (on or off) */
void win_clear_screen(unsigned char on_off);

/* draw a filled box from X1,Y1 to X2,Y2 */
void win_put_box(unsigned char x1, unsigned char pix_y1,
		 unsigned char x2, unsigned char pix_y2, unsigned char on_ff);

/* draw an empty box (using win_put_line) from X1,Y1, to X2,Y2 */
void win_put_box_empty(unsigned char x1, unsigned char pix_y1,
		       unsigned char x2, unsigned char pix_y2);

/* draw a bitmap image, top left corner at X, Y */
void win_put_bmp_xy(unsigned char x, unsigned char y, BMP_T bmp);

/* write text to the window with top left pixel at X, Y */
void win_put_text_xy(const char *str, unsigned char x, unsigned char y, unsigned char width);

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

/* set transparency */
void win_set_transparent(unsigned char on_off);

#endif //WIN_H
