#include <iostream>
#include <unistd.h> // required for usleep
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include "P25317.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "spi.h"
#ifdef __cplusplus
}
#endif

using namespace std;
using namespace exploringBB;


P25317::P25317(int rst_pin, int cs_pin, int dat_ctl_pin)
{
  rst = new GPIO(rst_pin);
  cs = new GPIO(cs_pin);
  dat_ctl = new GPIO(dat_ctl_pin);
}

P25317::~P25317()
{
  delete rst;
  delete cs;
  delete dat_ctl;
}

void P25317::init_display(void)
{
  // reset, data/ctrl, and CS are outputs
  rst->setDirection(OUTPUT);
  cs->setDirection(OUTPUT);
  dat_ctl->setDirection(OUTPUT);
  // init disp params
  contrast = DEF_CONTRAST;
  addr_mode = DEF_ADDR_MODE;
  start_row = DEF_START_ROW;
  stop_row = DEF_STOP_ROW;
  start_col = DEF_START_COL;
  stop_col = DEF_STOP_COL;
  col_off = DEF_COL_OFF;
  invert = DEF_INVERT;
  sleep = DEF_SLEEP;
  // keep the reset asserted
  enable_display(0);
}

void P25317::init_spi(void)
{
  spi_init(SPI_DEV_PATH);
  spi_set_mode(SPI_MODE);
  spi_set_speed(SPI_SPEED);
  spi_set_bits(SPI_BITS);
  spi_open();
}

void P25317::close_spi(void)
{
  spi_close();
}

void P25317::enable_display(int en)
{
  unsigned char config[] =
    {
      0xA8, 0x3F,           // Set MUX
      COL_OFF_REG, col_off, // Set Display Offset
      0x40,                 // Set Display Start line
      0xA0,                 // Set Segment Remap (A0 or A1)
      0xC0,                 // Set COM Output Scan Dir (C0 or C8)
      //      0xDA, 0x02,   // Set COM Pins HW Config
      0x81, contrast,       // Set Contrast
      0xA4,                 // Display uses RAM content
      invert,               // Set Normal Inv Display
      0x20, addr_mode,      // Horizontal Addressing Mode
      0xD5, 0x80,           // Set OSC Freq
      0x8D, 0x14,           // Charge Pump Enable
      sleep,                // Display On/Off
    };
  
  if (en == DISP_ENABLE)
    {
      // deassert reset line
      rst->setValue(HIGH);
      usleep(10000);
      // send various configuration commands
      send_ctl_cmd(config, sizeof(config));
    }
  else
    {
      // assert reset line
      rst->setValue(LOW);
    }
}

void P25317::sleep_display(unsigned char state)
{
  unsigned char buf[] = {state? (unsigned char)SLEEP_ON : (unsigned char)SLEEP_OFF};
  
  if (state)
    {
      // sleep display
      this->sleep = SLEEP_ON;
      send_ctl_cmd(buf, sizeof(buf));
    }
  else
    {
      // wake display
      this->sleep = SLEEP_OFF;
      enable_display(DISP_ENABLE); //send all cmds
    }
}

void P25317::send_ctl_cmd(unsigned char *buf, int buf_len)
{
  unsigned char dummy[buf_len];

  // deassert data pin
  dat_ctl->setValue(DAT_CTL_CMD);
  // drop CS
  cs->setValue(LOW);
  // send the message
  spi_transfer(buf, dummy, buf_len);
  // raise CS
  cs->setValue(HIGH);
}

void P25317::send_dat_cmd(unsigned char *buf, int buf_len)
{
  unsigned char dummy[buf_len];

  // assert data pin
  dat_ctl->setValue(DAT_CTL_DATA);
  // drop CS
  cs->setValue(LOW);
  // send the message
  spi_transfer(buf, dummy, buf_len);
  // raise CS
  cs->setValue(HIGH);
}

void P25317::set_contrast(unsigned char lvl)
{
  this->contrast = lvl;
  unsigned char temp[] =
    {
      0x81,
      this->contrast
    };
  send_ctl_cmd(temp, sizeof(temp));
}

unsigned char P25317::get_contrast(void)
{
  return this->contrast;
}

void P25317::set_addr_mode(unsigned char mode)
{
  switch (mode)
    {
    case ADDR_MODE_HORZ:
    case ADDR_MODE_VERT:
    case ADDR_MODE_PAGE:
      this->addr_mode = mode;
      break;
    default:
      //do nothing
      break;
    }
}

unsigned char P25317::get_addr_mode(void)
{
  return this->addr_mode;
}
  
void P25317::set_start_row(unsigned char row)
{
  if (row <= MAX_ROW)
    {
      this->start_row = row;
      //triple command
      unsigned char buf[] = {ROW_REG, this->start_row, this->stop_row};
      send_ctl_cmd(buf, sizeof(buf));
    }
}
  
unsigned char P25317::get_start_row(void)
{
  return this->start_row;
}
  
void P25317::set_stop_row(unsigned char row)
{
  if (row <= MAX_ROW)
    {
      this->stop_row = row;
      // triple command
      unsigned char buf[] = {ROW_REG, this->start_row, this->stop_row};
      send_ctl_cmd(buf, sizeof(buf));
    }
}
  
unsigned char P25317::get_stop_row(void)
{
  return this->stop_row;
}
  
void P25317::set_start_col(unsigned char col)
{
  if (col <= MAX_COL)
    {
      this->start_col = col;
      // triple command
      unsigned char buf[] = {COL_REG, this->start_col, this->stop_col};
      send_ctl_cmd(buf, sizeof(buf));
    }
}
  
unsigned char P25317::get_start_col(void)
{
  return this->start_col;
}
  
void P25317::set_stop_col(unsigned char col)
{
  if (col <= MAX_COL)
    {
      this->stop_col = col;
      // triple command
      unsigned char buf[] = {COL_REG, this->start_col, this->stop_col};
      send_ctl_cmd(buf, sizeof(buf));
    }
}
  
unsigned char P25317::get_stop_col(void)
{
  return this->stop_col;
}
  
void P25317::set_invert(unsigned char inv)
{
  if (inv)
    this->invert = INV_ON;
  else
    this->invert = INV_OFF;
  unsigned char buf[] = {this->invert};
  send_ctl_cmd(buf, sizeof(buf));
}
  
unsigned char P25317::get_invert(void)
{
  if (this->invert)
    return 1;
  else
    return 0;
}

void P25317::clear_screen(unsigned char on_off)
{
  unsigned char blank[NUM_COLS];

  memset(blank, on_off? 0xff : 0x00, NUM_COLS);
  for (int i = 0; i < NUM_ROWS; i++)
    send_dat_cmd(blank, sizeof(blank));
}

void P25317::send_test_screen(char screen)
{
  unsigned char buf_ff[16], buf_00[16];
  for (int i = 0; i < sizeof(buf_ff); i++)
    {
      buf_ff[i] = 0xff;
      buf_00[i] = 0x00;
    }

  if (screen)
    {
      for (int i = 0; i < 8; i++)
	{
	  for (int j = 0; j < 8; j++)
	    {
	      if (i % 2)
		{
		  if (j % 2)
		    send_dat_cmd(buf_ff, sizeof(buf_ff));
		  else
		    send_dat_cmd(buf_00, sizeof(buf_00));
		}
	      else
		{
		  if (j % 2)
		    send_dat_cmd(buf_00, sizeof(buf_00));
		  else
		    send_dat_cmd(buf_ff, sizeof(buf_ff));
		}
	    }
	}
    }
  else
    {
      for (int i = 0; i < 8; i++)
	{
	  for (int j = 0; j < 8; j++)
	    {
	      if (i % 2)
		{
		  if (j % 2)
		    send_dat_cmd(buf_00, sizeof(buf_00));
		  else
		    send_dat_cmd(buf_ff, sizeof(buf_ff));
		}
	      else
		{
		  if (j % 2)
		    send_dat_cmd(buf_ff, sizeof(buf_ff));
		  else
		    send_dat_cmd(buf_00, sizeof(buf_00));
		}
	    }
	}
    }
}


int main()
{
  P25317 my_disp(59, 58, 57); // 59:rst_pin, 58:cs_pin, 57:dat_ctl_pin
  int i = 0, contrast = 0, dir = 0;
  
  my_disp.init_display();
  my_disp.init_spi();
  usleep(1000000);
  my_disp.enable_display(DISP_ENABLE);

  my_disp.send_test_screen(0);
  while (1)
    {
      if (i)
      	{
      	  i = 0;
	  //      	  my_disp.send_test_screen(0);
      	}
      else
      	{
      	  i = 1;
	  //      	  my_disp.send_test_screen(1);
      	}

      if (dir)
	{
	  contrast += 16;
	  if (contrast > 255)
	    {
	      contrast = 255;
	      dir = 0;
	    }
	}
      else
	{
	  contrast -= 16;
	  if (contrast < 0)
	    {
	      contrast = 0;
	      dir = 1;
	    }
	}
      my_disp.set_contrast(contrast);

      usleep(500000);

      if (i)
	my_disp.set_invert(1);
      else
	my_disp.set_invert(0);
    }
  return 0;
}
