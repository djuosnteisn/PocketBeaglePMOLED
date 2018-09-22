#include <stdio.h>
#include <string.h>
#include <unistd.h> // required for usleep
#include "win.h"
#include "P25317.h"

static P25317 disp(RST_PIN, CS_PIN, DAT_CTL_PIN);
static s_win_set_xy(unsigned char x, unsigned char y);

/* create a P25317 obj for OLED interaction */
void win_init(void)
{
  disp.enable_display(DISP_ENABLE);
}

/* draw a single pixel at X and Y */
void win_put_pixel_xy(unsigned char x, unsigned char y)
{
}

/* draw a line from X1,Y1 to X2,Y2 */
void win_put_line(unsigned char x1, unsigned char y1,
		  unsigned char x2, unsigned char y2)
{
}

/* clear entire screen, either black or white (on or off) */
void win_clear_screen(unsigned char on_off)
{
}

/* draw a filled box from X1,Y1 to X2,Y2 */
void win_put_box(unsigned char x1, unsigned char y1,
		 unsigned char x2, unsigned char y2, unsigned char on_ff)
{
}

/* draw an empty box (using win_put_line) from X1,Y1, to X2,Y2 */
void win_put_box_empty(unsigned char x1, unsigned char y1,
		       unsigned char x2, unsigned char y2)
{
}

/* draw a bitmap image, top left corner at X, Y */
void win_put_bmp_xy(unsigned char x, unsigned char y, BMP_T bmp)
{
}

/* write text to the window with top left pixel at X, Y */
void win_put_text_xy(const char *text, unsigned char x, unsigned char y)
{
  unsigned int i = 0;

  s_win_set_xy(x, y);
  
  /* Continue until the entire string is output */
  while (text [i] != '\0')
  {
    if (((UNS_8) text [i] >= win->font->first_char)
             && ((UNS_8) text [i] <= win->font->last_char))
    {
      /* Put character on screen */
      swim_put_char(win, text [i]);
    }

    i++;
  }
}

/* write a single character to the window with top left pixel at X, Y */
void win_put_char_xy(const char chr, unsigned char x, unsigned char y)
{
    char ascii_char;
    INT_32 i, j;
    INT_32 charindex;
    UNS_16 *charfields, chardata;
    COLOR_T *fb;

    s_win_set_xy(x, y);

    //replace character with a '.' if it's unprintable
    ascii_char = textchar;
    if (ascii_char < win->font->first_char || ascii_char > win->font->last_char)
        ascii_char = 0x2e; //replace invalid chars with a period.
    /* Determine index to character data */
    charindex = (INT_32) ascii_char - (INT_32) win->font->first_char;

    /* Will the character fit on the display? */
    if ((win->xvpos + (INT_32) win->font->font_width_table [charindex]) >
        win->xpvmax)
    {
      /* Will not fit, return */
      return;
    }

    /* Determine the start of the bitfields for the character */
    charfields = win->font->font_table + (charindex *
                                          win->font->font_height);

    /* Map character to the window */
    for (i = 0; i < (INT_32) win->font->font_height; i++)
    {
      /* Get starting pixel location for font mapping in window */
      fb = win->fb + win->xvpos + ((win->yvpos + i) * win->xpsize);

      /* Get character line mapping data */
      chardata = charfields [i];

      /* Convert character line bit data to a pixel line in
         window */
      for (j =
             (INT_32) win->font->font_width_table [charindex];
           j > 0; j--)
      {
        if ((chardata & 0x8000) != 0)
        {
          *fb++ = win->pen;
        }
        else if (win->tfont != 0)
        {
          fb++;
        }
        else
        {
          *fb++ = win->bkg;
        }

        /* Next bit in character line */
        chardata = chardata << 1;
      }
}

/* sets the font pointer to the desired font */
void win_set_font(FONT_T font)
{
}

/* returns current fonts height in pixels */
unsigned char win_get_font_height(void)
{
}

/* returns the length of a string in pixels for current font */
unsigned char win_get_str_len(const char *str)
{
}

/* sets background color to white or black (on or off) */
void win_set_bg_color(unsigned char on_off)
{
}

/* invert colors on display */
void win_invert_color(unsigned char inv)
{
}

/* set the X and Y coords of display */
void s_win_set_xy(unsigned char x, unsigned char y)
{
}


int main()
{
  unsigned char 'G';

  win_init();

  for (int i = 0; i < (128*64); i++)
    {
      win_put_pixel_xy(x, y);
      x++; y++;
    }
}
