#include <stdio.h>
#include <string.h>
#include <unistd.h> // required for usleep
#include "win.h"
#include "P25317.h"
#include "fonts.h"

static P25317 disp(RST_PIN, CS_PIN, DAT_CTL_PIN);
static void s_win_set_xy(unsigned char x, unsigned char y);
static FONT_T s_font;

/* create a P25317 obj for OLED interaction */
void win_init(void)
{
  // initialize our display driver
  disp.init_display();
  disp.init_spi();
  usleep(10000);
  disp.enable_display(DISP_ENABLE);
  disp.clear_screen(0);
  // initialize our font pointer
  s_font = font_big;
}

/* draw a single pixel at X and Y */
void win_put_pixel_xy(unsigned char x, unsigned char y)
{
  unsigned char temp[1];
  static char val = 1, count = 0;

  temp[0] = val;
  disp.send_dat_cmd(temp, sizeof(temp));

  val |= (1 << count);
  ++count;
  if (count > 7)
    {
      val = count = 0;
    }
}

/* draw a line from X1,Y1 to X2,Y2 */
void win_put_line(unsigned char x1, unsigned char y1,
		  unsigned char x2, unsigned char y2)
{
  char i = 0;
}

/* clear entire screen, either black or white (on or off) */
void win_clear_screen(unsigned char on_off)
{
  char i = 0;
}

/* draw a filled box from X1,Y1 to X2,Y2 */
void win_put_box(unsigned char x1, unsigned char y1,
		 unsigned char x2, unsigned char y2, unsigned char on_ff)
{
  char i = 0;
}

/* draw an empty box (using win_put_line) from X1,Y1, to X2,Y2 */
void win_put_box_empty(unsigned char x1, unsigned char y1,
		       unsigned char x2, unsigned char y2)
{
  char i = 0;
}

/* draw a bitmap image, top left corner at X, Y */
void win_put_bmp_xy(unsigned char x, unsigned char y, BMP_T bmp)
{
  char i = 0;
}

/* write text to the window with top left pixel at X, Y */
void win_put_text_xy(const char *text, unsigned char x, unsigned char y)
{
  char i = 0;
  // unsigned int i = 0;

  // s_win_set_xy(x, y);
  
  // /* Continue until the entire string is output */
  // while (text [i] != '\0')
  // {
  //   if (((UNS_8) text [i] >= win->font->first_char)
  //            && ((UNS_8) text [i] <= win->font->last_char))
  //   {
  //     /* Put character on screen */
  //     swim_put_char(win, text [i]);
  //   }

  //   i++;
  // }
}

/* write a single character to the window with top left pixel at X, Y */
void win_put_char_xy(const char chr, unsigned char x, unsigned char y)
{
  unsigned char ascii_index, buf[s_font.bytes_per_char];
  unsigned short font_index;

  // only print valid characters
  if (chr < s_font.first_char || chr > s_font.last_char)
    return;

  ascii_index = chr - s_font.first_char;
  font_index = ascii_index * s_font.bytes_per_char;
  for (int i = 0; i < s_font.font_width_table[ascii_index]; i++)
    buf[i] = s_font.font_table[font_index + i];
  disp.send_dat_cmd(buf, s_font.font_width_table[ascii_index]);
  
   
    // char ascii_char;
    // INT_32 i, j;
    // INT_32 charindex;
    // UNS_16 *charfields, chardata;
    // COLOR_T *fb;

    // s_win_set_xy(x, y);

    // //replace character with a '.' if it's unprintable
    // ascii_char = textchar;
    // if (ascii_char < win->font->first_char || ascii_char > win->font->last_char)
    //     ascii_char = 0x2e; //replace invalid chars with a period.
    // /* Determine index to character data */
    // charindex = (INT_32) ascii_char - (INT_32) win->font->first_char;

    // /* Will the character fit on the display? */
    // if ((win->xvpos + (INT_32) win->font->font_width_table [charindex]) >
    //     win->xpvmax)
    // {
    //   /* Will not fit, return */
    //   return;
    // }

    // /* Determine the start of the bitfields for the character */
    // charfields = win->font->font_table + (charindex *
    //                                       win->font->font_height);

    // /* Map character to the window */
    // for (i = 0; i < (INT_32) win->font->font_height; i++)
    // {
    //   /* Get starting pixel location for font mapping in window */
    //   fb = win->fb + win->xvpos + ((win->yvpos + i) * win->xpsize);

    //   /* Get character line mapping data */
    //   chardata = charfields [i];

    //   /* Convert character line bit data to a pixel line in
    //      window */
    //   for (j =
    //          (INT_32) win->font->font_width_table [charindex];
    //        j > 0; j--)
    //   {
    //     if ((chardata & 0x8000) != 0)
    //     {
    //       *fb++ = win->pen;
    //     }
    //     else if (win->tfont != 0)
    //     {
    //       fb++;
    //     }
    //     else
    //     {
    //       *fb++ = win->bkg;
    //     }

    //     /* Next bit in character line */
    //     chardata = chardata << 1;
    //   }
}

/* sets the font pointer to the desired font */
void win_set_font(FONT_T font)
{
  char i = 0;
}

/* returns current fonts height in pixels */
unsigned char win_get_font_height(void)
{
  char i = 0;
}

/* returns the length of a string in pixels for current font */
unsigned char win_get_str_len(const char *str)
{
  char i = 0;
}

/* sets background color to white or black (on or off) */
void win_set_bg_color(unsigned char on_off)
{
  char i = 0;
}

/* invert colors on display */
void win_invert_color(unsigned char inv)
{
  char i = 0;
}

/* set the X and Y coords of display */
void s_win_set_xy(unsigned char x, unsigned char y)
{
  char i = 0;
}


int main()
{
  static unsigned char chr = ' ';

  win_init();

  // for (int i = 0; i < 20; i++)
  //   {
      win_put_char_xy('G', 0, 0);
    //   usleep(10000);
    // }
}
