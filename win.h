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
#define MIN_ROW 0
#define MAX_ROW 7
#define MIN_COL 0
#define MAX_COL 127
#define FRAME_WIDTH_PIX 128
#define FRAME_HEIGHT_PIX 64
#define FRAME_HEIGHT_ROW 8
#define PIX_PER_ROW 8
#define INVERSE_ON 1
#define INVERSE_OFF 0
#define DEF_INVERSE INVERSE_OFF
#define BLACK 0x00
#define WHITE 0xff
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
void win_clear_screen(void);

/* draw a filled box from X1,Y1 to X2,Y2 */
void win_put_box(unsigned char x1, unsigned char y1,
		 unsigned char x2, unsigned char y2);

/* draw an empty box (using win_put_line) from X1,Y1, to X2,Y2 */
void win_put_box_empty(unsigned char x1, unsigned char y1,
		       unsigned char x2, unsigned char y2);

/* draw a bitmap image, top left corner at X, Y */
void win_put_bmp_xy(unsigned char x, unsigned char y, BMP_T bmp);

/* write text to the window with top left pixel at X, Y */
void win_put_text_xy(const char *str, unsigned char x, unsigned char y, unsigned char width);

/* sets the font pointer to the desired font */
void win_set_font(FONT_T font);

/* returns current fonts height in pixels */
unsigned char win_get_font_height(void);

/* returns the length of a string in pixels for current font */
unsigned char win_get_str_len(const char *str);

/* invert colors on display */
void win_set_inverse(unsigned char inv);
unsigned char win_get_invert(void);

/* set transparency */
void win_set_transparent(unsigned char on_off);
unsigned char win_get_transparent(void);

#endif //WIN_H
