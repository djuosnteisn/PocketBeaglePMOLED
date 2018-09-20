#include <iostream>
#include <unistd.h> // required for usleep
#include <stdio.h>
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
  // reset and CS are outputs
  rst->setDirection(OUTPUT);
  cs->setDirection(OUTPUT);
  dat_ctl->setDirection(OUTPUT);
  // keep the reset asserted
  enable_display(0);
}

void P25317::init_spi(void)
{
  spi_open("/dev/spidev1.0");
}

void P25317::close_spi(void)
{
  spi_close();
}

void P25317::enable_display(int en)
{
  unsigned char config[] =
    {
      0xA8, 0x3F, // Set MUX
      0xD3, 0x00, // Set Display Offset
      0x40,       // Set Display Start line
      0xA0,       // Set Segment Remap (A0 or A1)
      0xC0,       // Set COM Output Scan Dir (C0 or C8)
      0xDA, 0x02, // Set COM Pins HW Config
      0x81, 0xFF, // Set Contrast
      0xA4,       // Disable Entire Display On
      0xA6,       // Set Normal Display
      0x20, 0x00, // Horizontal Addressing Mode
      0xD5, 0x80, // Set OSC Freq
      0x8D, 0x14, // Enable Charge Pump
      0xAF,       // Display On
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
  unsigned char temp[] =
    {
      (unsigned char)0x81,
      lvl
    };
  send_ctl_cmd(temp, sizeof(temp));
}

void P25317::send_test_screen(char screen)
{
  unsigned char buf_ff[128], buf_00[128];
  for (int i = 0; i < 128; i++)
    {
      buf_ff[i] = 0xff;
      buf_00[i] = 0x00;
    }

  if (screen)
    {
      for (int i = 0; i < 8; i++)
       	{
	  if (i % 2)
	    send_dat_cmd(buf_ff, sizeof(buf_ff));
	  else
	    send_dat_cmd(buf_00, sizeof(buf_00));
	}
    }
  else
    {
      for (int i = 0; i < 8; i++)
       	{
	  if (i % 2)
	    send_dat_cmd(buf_00, sizeof(buf_00));
	  else
	    send_dat_cmd(buf_ff, sizeof(buf_ff));
	}
    }
}


int main()
{
  P25317 my_disp(59, 58, 57); // 59:rst_pin, 58:cs_pin, 57:dat_ctl_pin
  int i = 0, contrast = 0, dir = 0;
  
  my_disp.init_display();
  my_disp.init_spi();
  usleep(10000);
  my_disp.enable_display(DISP_ENABLE);
  while (1)
    {
      if (i)
	{
	  i = 0;
	  my_disp.send_test_screen(0);
	}
      else
	{
	  i = 1;
	  my_disp.send_test_screen(1);
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
    }
  return 0;
}
