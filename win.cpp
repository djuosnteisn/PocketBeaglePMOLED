#include <stdio.h>
#include <string.h>
#include <unistd.h> // required for usleep
#include "win.h"
#include "P25317.h"
#include "fonts.h"
#include "bmps.h"

static P25317 disp(RST_PIN, CS_PIN, DAT_CTL_PIN);
static FONT_T s_font;
static unsigned char s_inverse = DEF_INVERSE, s_trans = TRANS_OFF;
static struct tagFrame
{
  unsigned char buf[FRAME_WIDTH_PIX][FRAME_HEIGHT_ROW];
  unsigned char dirty[FRAME_WIDTH_PIX][FRAME_HEIGHT_ROW];
} frame;

static void s_win_set_xy(unsigned char x, unsigned char y);
static unsigned char s_win_get_row(unsigned char y_pix);
static unsigned char s_win_get_row_off(unsigned char y_pix);

/* create a P25317 obj for OLED interaction */
void win_init(void)
{
  // initialize our display driver
  disp.init_display();
  disp.init_spi();
  usleep(10000);
  disp.enable_display(DISP_ENABLE);
  disp.clear_screen(BLACK);
  // initialize our font pointer
  s_font = font_big;
  // initialize our frame
  memset(frame.buf, 0, sizeof(frame.buf));
  memset(frame.dirty, 0, sizeof(frame.dirty));
  // init other things
  s_trans = TRANS_OFF;
}

/* draw a single pixel at X and Y
   X is col in pixels, 0 - 127
   Y is row in pixels, 0 - 63
*/
void win_put_pixel_xy(unsigned char x, unsigned char y)
{
  // get our page row from the y pixel
  unsigned char row = s_win_get_row(y);
  // get our offset within the row for unaligned writes
  unsigned char row_off = s_win_get_row_off(y);
  // write the pixel to our frame buf
  if (s_trans)
    frame.buf[x][row] |= (1 << row_off);
  else
    frame.buf[x][row] = (1 << row_off);
  frame.dirty[x][row] = 1;
}

/* draw a line from X1 to X2 at row Y */
void win_put_line_horz(unsigned char x1, unsigned char x2, unsigned char y)
{
}

/* draw a line from Y1 to Y2 at col X */
void win_put_line_vert(unsigned char y1, unsigned char y2, unsigned char x)
{
}

/* clear entire screen, either black or white (on or off) */
void win_clear_screen(unsigned char on_off)
{
  disp.clear_screen(s_inverse);
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


/* Draw a bitmap image, top left corner at X, Y.
   This function updates our frame buffer, and uses it to build
   a transfer buffer used to send to the display.
 */
void win_put_bmp_xy(unsigned char x, unsigned char y, BMP_T bmp)
{
  // get the row and offset values
  unsigned char row = s_win_get_row(y);
  unsigned char row_off = s_win_get_row_off(y);
  // byte height = necessary bytes to cover pixel height, e.g 10 pix = 2 bytes
  unsigned char byte_height = (bmp.height % BITS_IN_BYTE) ?
    (bmp.height / BITS_IN_BYTE + 1) : (bmp.height / BITS_IN_BYTE);
  // if image isn't row aligned, need to print additional row
  unsigned char row_height = row_off ? byte_height + 1 : byte_height;
  // update our frame buf and build a transfer buffer that we'll
  // send to the disp obj
  unsigned char transfer_buf[bmp.width * row_height];
  int index = 0, transfer_index = 0; //bmp and transfer buf index
  for (int i = 0; i < bmp.width; i++)
    {
      if (row_off) // not row aligned
	{
	  // we need to merge offset bytes across row boundries
	  for (int j = 0; j < row_height; j++)
	    {
	      // top byte
	      if (j == 0)
		{
		  if (s_trans)
		    frame.buf[x + i][row + j] |= (bmp.image[index] << row_off);
		  else
		    frame.buf[x + i][row + j] = (bmp.image[index] << row_off);
		  ++index;
		}
	      // middle bytes
	      else if (j < (row_height - 1))
		{
		  if (s_trans)
		    frame.buf[x + i][row + j] |=
		      ((bmp.image[index - 1] >> (BITS_IN_BYTE - row_off)) |
		       (bmp.image[index] << row_off));
		  else
		    frame.buf[x + i][row + j] =
		      ((bmp.image[index - 1] >> (BITS_IN_BYTE - row_off)) |
		       (bmp.image[index] << row_off));
		  // don't inc index if it's the last part of bmp
		  if (j < (byte_height - 1))
		    ++index;
		}
	      // bottom byte
	      else
		{
		  if (s_trans)
		    frame.buf[x + i][row + j] |= (bmp.image[index] >> (BITS_IN_BYTE - row_off));
		  else
		    frame.buf[x + i][row + j] = (bmp.image[index] >> (BITS_IN_BYTE - row_off));
		  ++index;
		}
	      transfer_buf[transfer_index++] = frame.buf[x + i][row + j];
	    }
	}
      else // row aligned
	{
	  // straight forward byte mapping
	  for (int j = 0; j < row_height; j++)
	    {
	      if (s_trans)
		frame.buf[x + i][row + j] |= bmp.image[index];
	      else
		frame.buf[x + i][row + j] = bmp.image[index];
	      ++index;
	      transfer_buf[transfer_index++] = frame.buf[x + i][row + j];
	    }
	}
    }

  /* now that the frame buf is updated, let's write the disp */
  // set start and stop row/cols
  disp.set_start_col(x);
  disp.set_stop_col(x + bmp.width);
  disp.set_start_row(row);
  disp.set_stop_row(row + row_height - 1);
  // send the data
  disp.send_dat_cmd(transfer_buf, (bmp.width * row_height));
}


/* Write text to the window with top left pixel at X, Y.
   Replace any unsupported ascii characters with a '.'
   and stop printing if it encounters the end field width
   or end of the display. It's the responsibility of the
   caller to ensure the string will fit on the screen;
   out-of-bounds behavior is undefined.
*/
void win_put_text_xy(const char *str, unsigned char x, unsigned char y, unsigned char width)
{
  unsigned char chr, ascii_index, bytes_in_chr;
  unsigned char transfer_buf[s_font.font_height / BITS_IN_BYTE * MAX_COL];
  unsigned short font_index, transfer_index = 0;

  // get the row and offset values
  unsigned char row = s_win_get_row(y), row_off = s_win_get_row_off(y);
  // character height = necessary bytes to cover font height, e.g. 10 pix = 2 bytes
  unsigned char char_height = (s_font.font_height % BITS_IN_BYTE) ?
    (s_font.font_height / BITS_IN_BYTE + 1) : (s_font.font_height / BITS_IN_BYTE);
  // if font isn't row aligned, need to print additional row
  unsigned char row_height = row_off ? char_height + 1 : char_height;
  

  // get the total allowed num of bytes to copy over
  unsigned short max_bytes = s_font.font_height / BITS_IN_BYTE * MAX_COL;

  unsigned char f_index = x;
  unsigned short tot_width = 0;
  // build our kerned string buffer to be written
  for (int i = 0; i < strlen(str); i++)
    {
      // only print valid characters
      if (str[i] < s_font.first_char || str[i] > s_font.last_char)
	chr = '.';
      else
	chr = str[i];
      // get proper indexes into our tables
      ascii_index = chr - s_font.first_char;
      font_index = ascii_index * s_font.bytes_per_char;
      // copy chars into our output buffer
      for (int j = 0; j < s_font.font_width_table[ascii_index]; j++)
	{
	  if (row_off) // not row aligned
	    {
	      // we need to merge offset bytes across row boundries
	      for (int k = 0; k < row_height; k++)
		{
		  // top byte
		  if (k == 0)
		    {
		      if (s_trans)
			{
			  if (s_inverse == INVERSE_OFF)
			    frame.buf[f_index][row + k] |=
			      (s_font.font_table[font_index] << row_off);
			  else
			    frame.buf[f_index][row + k] |=
			      (~s_font.font_table[font_index] << row_off);
			}
		      else
			{
			  if (s_inverse == INVERSE_OFF)
			    frame.buf[f_index][row + k] =
			      (s_font.font_table[font_index] << row_off);
			  else
			    frame.buf[f_index][row + k] =
			      (~s_font.font_table[font_index] << row_off);
			}
		      ++font_index;
		    }
		  // middle bytes
		  else if (k < (row_height -1))
		    {
		      if (s_trans)
			{
			  if (s_inverse == INVERSE_OFF)
			    frame.buf[f_index][row + k] |=
			      (s_font.font_table[font_index] << row_off) |
			      (s_font.font_table[font_index - 1] >> (BITS_IN_BYTE - row_off));
			  else
			    frame.buf[f_index][row + k] |=
			      (~s_font.font_table[font_index] << row_off) |
			      (~s_font.font_table[font_index - 1] >> (BITS_IN_BYTE - row_off));
			}
		      else
			{
			  if (s_inverse == INVERSE_OFF)
			    frame.buf[f_index][row + k] =
			      (s_font.font_table[font_index] << row_off) |
			      (s_font.font_table[font_index - 1] >> (BITS_IN_BYTE - row_off));
			  else
			    frame.buf[f_index][row + k] =
			      (~s_font.font_table[font_index] << row_off) |
			      (~s_font.font_table[font_index - 1] >> (BITS_IN_BYTE - row_off));
			}
		      if (k < (char_height - 1))
			++font_index;
		    }
		  // bottom byte
		  else
		    {
		      if (s_trans)
			{
			  if (s_inverse == INVERSE_OFF)
			    frame.buf[f_index][row + k] |=
			      (s_font.font_table[font_index] >> (BITS_IN_BYTE - row_off));
			  else
			    frame.buf[f_index][row + k] |=
			      (~s_font.font_table[font_index] >> (BITS_IN_BYTE - row_off));
			}
		      else
			{
			  if (s_inverse == INVERSE_OFF)
			    frame.buf[f_index][row + k] =
			      (s_font.font_table[font_index] >> (BITS_IN_BYTE - row_off));
			  else
			    frame.buf[f_index][row + k] =
			      (~s_font.font_table[font_index] >> (BITS_IN_BYTE - row_off));
			}
		      ++font_index;
		    }
		  transfer_buf[transfer_index++] = frame.buf[f_index][row + k];
		}
	    }
	  else // row aligned
	    {
	      // straight forward byte mapping
	      for (int k = 0; k < row_height; k++)
		{
		  if (s_trans)
		    {
		      if (s_inverse == INVERSE_OFF)
			frame.buf[f_index][row + k] |= s_font.font_table[font_index];
		      else
			frame.buf[f_index][row + k] |= ~s_font.font_table[font_index];
		    }
		  else
		    {
		      if (s_inverse == INVERSE_OFF)
			frame.buf[f_index][row + k] = s_font.font_table[font_index];
		      else
			frame.buf[f_index][row + k] = ~s_font.font_table[font_index];
		    }
		  ++font_index;
		  transfer_buf[transfer_index++] = frame.buf[f_index][row + k];
		}
	    }
	  ++f_index;
	  ++tot_width;
	}
    }

  /* now that the frame buf is updated, let's write the disp */
  // set start and stop row/cols
  disp.set_start_col(x);
  if ((x + tot_width) <= MAX_COL)
    disp.set_stop_col(x + tot_width);
  else
  disp.set_stop_col(MAX_COL);
  disp.set_start_row(row);
  if (row + row_height <= MAX_ROW)
    disp.set_stop_row(row + row_height - 1);
  else
    disp.set_stop_row(MAX_ROW);
  // send the data
  disp.send_dat_cmd(transfer_buf, transfer_index);
}


/* Write a single character to the window with top left pixel at X, Y.
   It's the caller's responsibility to make sure the pixel fits on the
   screen.
 */
void win_put_char_xy(const char chr, unsigned char x, unsigned char y)
{
  unsigned char ascii_index, buf[s_font.bytes_per_char];
  unsigned short font_index, bytes_in_chr;

  // only print valid characters
  if (chr < s_font.first_char || chr > s_font.last_char)
    return;

  // get proper indexes into our tables
  ascii_index = chr - s_font.first_char;
  font_index = ascii_index * s_font.bytes_per_char;
  bytes_in_chr = (s_font.font_height / BITS_IN_BYTE) * s_font.font_width_table[ascii_index];

  // set the starting X, Y based off active font height
  disp.set_start_col(x);
  disp.set_stop_col(x + s_font.font_width_table[ascii_index]);
  disp.set_start_row(y);
  disp.set_stop_row(y + (s_font.font_height / BITS_IN_BYTE) - 1);

  for (int i = 0; i < bytes_in_chr; i++)
    {
      if (s_inverse == INVERSE_OFF)
	buf[i] = s_font.font_table[font_index + i];
      else
	buf[i] = ~s_font.font_table[font_index + i];
    }
  disp.send_dat_cmd(buf, bytes_in_chr);
}

/* sets the font pointer to the desired font */
void win_set_font(FONT_T font)
{
  // make sure the font is legal first
  // maybe make a font_list and switch it
}

/* returns current fonts height in pixels */
unsigned char win_get_font_height(void)
{
  return s_font.font_height;
}

/* returns the length of a string in pixels for current font */
unsigned char win_get_str_len(const char *str)
{
  unsigned char buf_index = 0, chr, ascii_index;
  unsigned short font_index;

  // build our kerned string buffer to be written
  for (int i = 0; i < strlen(str); i++)
    {
      // only print valid characters
      if (str[i] < s_font.first_char || str[i] > s_font.last_char)
	chr = '.';
      else
	chr = str[i];
      // get proper indexes into our tables
      ascii_index = chr - s_font.first_char;
      font_index = ascii_index * s_font.bytes_per_char;
      buf_index += s_font.font_width_table[ascii_index];
    }

  return buf_index;
}

/* sets background color to white or black (on or off) */
void win_set_bg_color(unsigned char on_off)
{
  char i = 0;
}

/* invert colors on display */
void win_invert_color(unsigned char inv)
{
  s_inverse = inv? INVERSE_ON : INVERSE_OFF;
}

/* set the X and Y coords of display */
void s_win_set_xy(unsigned char x, unsigned char y)
{
}

/* return the page row for a pixel row */
unsigned char s_win_get_row(unsigned char y_pix)
{
  return y_pix / PIX_PER_ROW;
}

/* return the row offset for a pixel row */
unsigned char s_win_get_row_off(unsigned char y_pix)
{
  return y_pix % PIX_PER_ROW;
}
