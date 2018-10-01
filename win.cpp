#include <stdio.h>
#include <string.h>
#include <unistd.h> // required for usleep
#include "win.h"
#include "P25317.h"
#include "fonts.h"
#include "bmps.h"

static P25317 disp(RST_PIN, CS_PIN, DAT_CTL_PIN);
static FONT_T const *s_font;
static unsigned char s_inverse = DEF_INVERSE, s_trans = TRANS_ON;
unsigned char frame_buf[FRAME_WIDTH_PIX][FRAME_HEIGHT_ROW];

static unsigned char s_win_get_row(unsigned char y_pix);
static unsigned char s_win_get_row_off(unsigned char y_pix);


/***********************************************/
/* win_init                                    */
/*                                             */
/* Init various window parameters              */
/***********************************************/
void win_init(void)
{
  // initialize our display driver
  disp.init_display();
  disp.init_spi();
  usleep(10000);
  disp.enable_display(DISP_ENABLE);
  disp.clear_screen(BLACK);
  // initialize our font pointer
  s_font = &font_big;
  // initialize our frame
  memset(frame_buf, 0, sizeof(frame_buf));
}


/***********************************************/
/* win_put_pixel_xy                            */
/*                                             */
/* draw a single pixel at X and Y              */
/*   X is col in pixels, 0 - 127               */
/*   Y is row in pixels, 0 - 63                */
/***********************************************/
void win_put_pixel_xy(unsigned char x, unsigned char y)
{
  // get our page row from the y pixel
  unsigned char row = s_win_get_row(y);
  // get our offset within the row for unaligned writes
  unsigned char row_off = s_win_get_row_off(y);
  // write the pixel to our frame buf
  if (s_trans)
    frame_buf[x][row] |= (1 << row_off);
  else
    frame_buf[x][row] = (1 << row_off);
}


/***********************************************/
/* win_put_line_horz                           */
/*                                             */
/* draw a line from X1 to X2                   */
/* at row Y                                    */
/***********************************************/
void win_put_line_horz(unsigned char x1, unsigned char x2, unsigned char y)
{
  // get the row and offest values
  unsigned char row = s_win_get_row(y);
  unsigned char row_off = s_win_get_row_off(y);

  // make sure it fits on the screen
  if (x1 > MAX_COL)
    x1 = MAX_COL;
  if (x2 > MAX_COL)
    x2 = MAX_COL;

  // swap if necessary
  if (x1 > x2)
    {
      unsigned char temp = x1;
      x1 = x2;
      x2 = temp;
    }

  // update our frame buf and build a transfer buf
  unsigned char len = x2 - x1;
  unsigned char transfer_buf[len];
  for (int i = 0; i < len; i++)
    {
      if (s_trans)
	frame_buf[x1 + i][row] |= (1 << row_off);
      else
	frame_buf[x1 + i][row] = (1 << row_off);
      transfer_buf[i] = frame_buf[x1 + i][row];
    }

  // now set start and stop row/cols and send the data
  disp.set_start_col(x1);
  disp.set_stop_col(x2);
  disp.set_start_row(row);
  disp.set_stop_row(row);
  disp.send_dat_cmd(transfer_buf, len);
}


/***********************************************/
/* win_put_line_vert                           */
/*                                             */
/* draw a line from Y1 to Y2                   */
/* at col X                                    */
/***********************************************/
void win_put_line_vert(unsigned char y1, unsigned char y2, unsigned char x)
{
  // make sure it fits on the screen
  if (y1 > (FRAME_HEIGHT_PIX - 1))
    y1 = FRAME_HEIGHT_PIX - 1;
  if (y2 > (FRAME_HEIGHT_PIX - 1))
    y2 = FRAME_HEIGHT_PIX - 1;

  // swap if necessary
  if (y1 > y2)
    {
      unsigned char temp = y1;
      y1 = y2;
      y2 = y1;
    }

  // get the row and offset values
  unsigned char row1 = s_win_get_row(y1);
  unsigned char row_off1 = s_win_get_row_off(y1);
  unsigned char row2 = s_win_get_row(y2);
  unsigned char row_off2 = s_win_get_row_off(y2);
  if (row_off2) // get second row
    row2++;
  
  // update our frame buf and build a transfer buf
  unsigned char len = row2 - row1;
  unsigned char transfer_buf[len];
  for (int i = 0; i < len; i++)
    {
      unsigned char byte = 0;
      if (i == 0)
	byte |= (0xFF << row_off1);
      else
	byte |= 0xFF;
      if (i == (len - 1) && row_off2)
	byte &= ~((unsigned char)0xFF << row_off2);
      if (s_trans)
	frame_buf[x][row1 + i] |= byte;
      else
	frame_buf[x][row1 + i] = byte;
      transfer_buf[i] = frame_buf[x][row1 + i];
    }

  // now set start and stop row/cols and send the data
  disp.set_start_col(x);
  disp.set_stop_col(x);
  disp.set_start_row(row1);
  disp.set_stop_row(row2);
  disp.send_dat_cmd(transfer_buf, len);
}


/***********************************************/
/* win_clear_screen                            */
/*                                             */
/* Clear entire screen                         */
/***********************************************/
void win_clear_screen(void)
{
  memset(frame_buf, BLACK, sizeof(frame_buf));
  //set start and stop row/cols
  disp.set_start_col(MIN_COL);
  disp.set_stop_col(MAX_COL);
  disp.set_start_row(MIN_ROW);
  disp.set_stop_row(MAX_ROW);
  disp.clear_screen(BLACK);
}


/***********************************************/
/* win_put_box                                 */
/*                                             */
/* draw a filled box from X1, Y1 to X2, Y2     */
/***********************************************/
void win_put_box(unsigned char x1, unsigned char y1,
		 unsigned char x2, unsigned char y2, unsigned char on_ff)
{
}


/***********************************************/
/* win_put_box_empty                           */
/*                                             */
/* draw an empty box from X1, Y1, to X2, Y2    */
/* (uses win_put_line)                         */
/***********************************************/
void win_put_box_empty(unsigned char x1, unsigned char y1,
		       unsigned char x2, unsigned char y2)
{
  win_put_line_horz(x1, x2, y1);
  win_put_line_horz(x1, x2, y2);
  win_put_line_vert(y1, y2, x1);
  win_put_line_vert(y1, y2, x2);
}


/***********************************************/
/* win_put_bmp_xy                              */
/*                                             */
/* draws a bitmap image, top left corner at    */
/* X, Y.                                       */
/***********************************************/
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
		    frame_buf[x + i][row + j] |= (bmp.image[index] << row_off);
		  else
		    frame_buf[x + i][row + j] = (bmp.image[index] << row_off);
		  ++index;
		}
	      // middle bytes
	      else if (j < (row_height - 1))
		{
		  if (s_trans)
		    frame_buf[x + i][row + j] |=
		      ((bmp.image[index - 1] >> (BITS_IN_BYTE - row_off)) |
		       (bmp.image[index] << row_off));
		  else
		    frame_buf[x + i][row + j] =
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
		    frame_buf[x + i][row + j] |= (bmp.image[index] >> (BITS_IN_BYTE - row_off));
		  else
		    frame_buf[x + i][row + j] = (bmp.image[index] >> (BITS_IN_BYTE - row_off));
		  ++index;
		}
	      transfer_buf[transfer_index++] = frame_buf[x + i][row + j];
	    }
	}
      else // row aligned
	{
	  // straight forward byte mapping
	  for (int j = 0; j < row_height; j++)
	    {
	      if (s_trans)
		frame_buf[x + i][row + j] |= bmp.image[index];
	      else
		frame_buf[x + i][row + j] = bmp.image[index];
	      ++index;
	      transfer_buf[transfer_index++] = frame_buf[x + i][row + j];
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


/***********************************************/
/* win_put_text_xy                             */
/*                                             */
/* Write text to the window with top left      */
/* pixel at X, Y. Replace any unsupported      */
/* ascii characters with a '.' and stop        */
/* printing if it encounters the end field     */
/* width or end of the display. It's the       */
/* callers responsibility to ensure the string */
/* will fit on the screen. Out-of-bounds       */
/* behavior is undefined.                      */
/***********************************************/
//NOTE need to implement width logic to stop
// printing... TBD.
void win_put_text_xy(const char *str, unsigned char x, unsigned char y, unsigned char width)
{
  unsigned char chr, ascii_index, bytes_in_chr;
  unsigned short font_index, transfer_index = 0;

  // get the row and offset values
  unsigned char row = s_win_get_row(y), row_off = s_win_get_row_off(y);
  // character height = necessary bytes to cover font height, e.g. 10 pix = 2 bytes
  unsigned char char_height = (s_font->font_height % BITS_IN_BYTE) ?
    (s_font->font_height / BITS_IN_BYTE + 1) : (s_font->font_height / BITS_IN_BYTE);
  // if font isn't row aligned, need to print additional row
  unsigned char row_height = row_off ? char_height + 1 : char_height;

  unsigned char transfer_buf[((s_font->font_height / BITS_IN_BYTE) + 1) * MAX_COL];
  // get the total allowed num of bytes to copy over
  unsigned short max_bytes = s_font->font_height / BITS_IN_BYTE * MAX_COL;
  unsigned char f_index = x;
  unsigned short tot_width = 0;
  int i, j, k;
  // build our kerned string buffer to be written
  for (i = 0; i < strlen(str); i++)
    {
      // only print valid characters
      if (str[i] < s_font->first_char || str[i] > s_font->last_char)
	chr = '.';
      else
	chr = str[i];
      // get proper indexes into our tables
      ascii_index = chr - s_font->first_char;
      font_index = ascii_index * s_font->bytes_per_char;
      int width = s_font->font_width_table[ascii_index];
      // copy chars into our output buffer
      for (j = 0; j < width; j++)
	{
	  if (row_off) // not row aligned
	    {
	      // we need to merge offset bytes across row boundries
	      for (k = 0; k < row_height; k++)
		{
		  /* calc the byte across boundries */
		  unsigned char byte = 0;
		  // top byte
		  if (k == 0)
		    {
		      byte = s_font->font_table[font_index] << row_off;
		      ++font_index;
		    }
		  // middle bytes
		  else if (k < (row_height - 1))
		    {
		      byte = (s_font->font_table[font_index] << row_off) |
			(s_font->font_table[font_index - 1] >> (BITS_IN_BYTE - row_off));
		      if (k < (char_height - 1))
		  	++font_index;
		    }
		  // bottom byte
		  else
		    {
		      byte = s_font->font_table[font_index] >> (BITS_IN_BYTE - row_off);
		      ++font_index;
		    }
		  /* update frame buf */
		  if (s_trans) // transparent
		    {
		      if (s_inverse) // inverted
			{
			  if (k == 0)
			    frame_buf[f_index][row + k] |= ~byte & (0xFF << row_off);
			  else if (k < (row_height - 1))
			    frame_buf[f_index][row + k] |= ~byte;
			  else
			    frame_buf[f_index][row + k] |= ~byte & (0xFF >> (BITS_IN_BYTE - row_off));
			}
		      else
			{
			  frame_buf[f_index][row + k] |= byte;
			}
		    }
		  else
		    { // not transparent
		      if (s_inverse) // inverted
			{
			  if (k == 0)
			    frame_buf[f_index][row + k] = ~byte & (0xFF << row_off);
			  else if (k < (row_height - 1))
			    frame_buf[f_index][row + k] = ~byte;
			  else
			    frame_buf[f_index][row + k] = ~byte & (0xFF >> (BITS_IN_BYTE - row_off));
			}
		      else
			{
			  frame_buf[f_index][row + k] = byte;
			}
		    }
		  transfer_buf[transfer_index++] = frame_buf[f_index][row + k];
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
			frame_buf[f_index][row + k] |= s_font->font_table[font_index];
		      else
			frame_buf[f_index][row + k] |= ~s_font->font_table[font_index];
		    }
		  else
		    {
		      if (s_inverse == INVERSE_OFF)
			frame_buf[f_index][row + k] = s_font->font_table[font_index];
		      else
			frame_buf[f_index][row + k] = ~s_font->font_table[font_index];
		    }
		  ++font_index;
		  transfer_buf[transfer_index++] = frame_buf[f_index][row + k];
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


/***********************************************/
/* win_set_font                                */
/*                                             */
/* sets the font pointer to the desired font   */
/***********************************************/
void win_set_font(FONT_T font)
{
  // make sure the font is legal first
  // maybe make a font_list and switch it
}


/***********************************************/
/* win_get_font_height                         */
/*                                             */
/* returns the current font's height in pixels */
/***********************************************/
unsigned char win_get_font_height(void)
{
  return s_font->font_height;
}


/***********************************************/
/* win_get_str_len                             */
/*                                             */
/* returns the length of a string in pixels for */
/* current font                                */
/***********************************************/
unsigned char win_get_str_len(const char *str)
{
  unsigned char buf_index = 0, chr, ascii_index;
  unsigned short font_index;

  // build our kerned string buffer to be written
  for (int i = 0; i < strlen(str); i++)
    {
      // only print valid characters
      if (str[i] < s_font->first_char || str[i] > s_font->last_char)
	chr = '.';
      else
	chr = str[i];
      // get proper indexes into our tables
      ascii_index = chr - s_font->first_char;
      font_index = ascii_index * s_font->bytes_per_char;
      buf_index += s_font->font_width_table[ascii_index];
    }

  return buf_index;
}


/***********************************************/
/* win_set_invert                              */
/*                                             */
/* invert the black white color                */
/***********************************************/
void win_set_invert(unsigned char inv)
{
  s_inverse = inv? INVERSE_ON : INVERSE_OFF;
}


/***********************************************/
/* win_get_invert                              */
/*                                             */
/* return invert state                         */
/***********************************************/
unsigned char win_get_invert(void)
{
  return s_inverse? INVERSE_ON : INVERSE_OFF;
}


/***********************************************/
/* win_set_transparent                         */
/*                                             */
/* leave existing pixles or not                */
/***********************************************/
void win_set_transparent(unsigned char trans)
{
  s_trans = trans? TRANS_ON : TRANS_OFF;
}


/***********************************************/
/* win_get_transparent                         */
/*                                             */
/* return trans state                          */
/***********************************************/
unsigned char win_get_transparent(void)
{
  return  s_trans? TRANS_ON : TRANS_OFF;
}


/***********************************************/
/* s_win_get_row                               */
/*                                             */
/* return the page row for a pixel row         */
/***********************************************/
unsigned char s_win_get_row(unsigned char y_pix)
{
  return y_pix / PIX_PER_ROW;
}


/***********************************************/
/* s_win_get_row_off                           */
/*                                             */
/* return the row offset for a pixel row       */
/***********************************************/
unsigned char s_win_get_row_off(unsigned char y_pix)
{
  return y_pix % PIX_PER_ROW;
}
