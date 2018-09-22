#ifndef FONTS_H
#define FONTS_H

/* Font data structure */
typedef struct
{
  int16_t font_height;
  uint8_t  first_char;
  uint8_t  last_char;
  uint16_t *font_table;
  uint8_t  *font_width_table;
} FONT_T;

/* special indexes */
#define SYMBOL_QUARTER          "\x81"
#define SYMBOL_HALF             "\x82"
#define SYMBOL_THREE_QUARTER    "\x83"

/* font tables */
extern const FONT_T font_small; // 8 pix high
extern const FONT_T font_big; // 16 pix high


#endif /* FONTS_H */
