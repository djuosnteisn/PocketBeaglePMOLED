#ifdef __cplusplus
extern "C" {
#endif

#ifndef FONTS_H
#define FONTS_H

/* Font data structure */
typedef struct
{
  unsigned char bytes_per_char;
  unsigned char font_height;
  unsigned char font_width;
  unsigned char first_char;
  unsigned char last_char;
  unsigned char *font_table;
  unsigned char *font_width_table;
} FONT_T;

/* special indexes */
/* #define SYMBOL_QUARTER          "\x81" */
/* #define SYMBOL_HALF             "\x82" */
/* #define SYMBOL_THREE_QUARTER    "\x83" */

/* font tables */
extern const FONT_T font_big; // 16 pix high


#endif /* FONTS_H */

#ifdef __cplusplus
}
#endif
