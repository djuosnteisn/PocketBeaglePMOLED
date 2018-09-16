#include <iostream>
#include <unistd.h> // required for usleep
#include "P25317.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <linux/spi/spidev.h>


using namespace std;
using namespace exploringBB;


P25317::P25317(int rst_pin, int cs_pin, int dat_ctl_pin)
{
  rst = new GPIO(rst_pin);
  cs = new GPIO(cs_pin);
  dat_ctl = new GPIO(dat_ctl_pin);
  rx = new char[MAX_BUF_LEN];
  tx = new char[MAX_BUF_LEN];
  speed = 1000000;
  bits = 8;
  mode = 3;
  // open and configure the spi handle
  if ((fd = open(SPI_PATH, O_RDWR)) < 0)
    {
      perror("SPI Error: Can't open device");
      return;
    }
  printf("fd=%d\n", fd);
}

P25317::~P25317()
{
  delete rst;
  delete cs;
  delete dat_ctl;
  delete rx;
  delete tx;
}

void P25317::init_display(void)
{
  // reset and CS are outputs
  rst -> setDirection(OUTPUT);
  cs -> setDirection(OUTPUT);
  dat_ctl -> setDirection(OUTPUT);
  // keep the reset asserted
  enable_display(0);
}

void P25317::init_spi(void)
{
  if (ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1)
    {
      perror("SPI: Can't set SPI mode");
      return;
    }
  if (ioctl(fd, SPI_IOC_RD_MODE, &mode) == -1)
    {
      perror("SPI: Can't get SPI mode");
      return;
    }
  if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1)
    {
      perror("SPI: Can't set bits per word.");
      return;
    }
  if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) == -1)
    {
      perror("SPI: Can't get bits per word");
      return;
    }
  if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1)
    {
      perror("SPI: Can't set max speed HZ");
      return;
    }
  if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) == -1)
    {
      perror("SPI: Can't get max speed HZ");
      return;
    }
  // check properties have been set
  printf("SPI Mode is: %d\n", mode);
  printf("SPI Bits is: %d\n", bits);
  printf("SPI Speed is: %d\n", speed);
}

void P25317::enable_display(int en)
{
  char config[] =
    {
      0xA8, 0x3F, // Set MUX
      0xD3, 0x00, // Set Display Offset
      0x40,       // Set Display Start line
      0xA0,       // Set Segment Remap (A0 or A1)
      0xC0,       // Set COM Output Scan Dir (C0 or C8)
      0xDA, 0x02, // Set COM Pins HW Config
      0x81, 0x7F, // Set Contrast
      0xA4,       // Disable Entire Display On
      0xA6,       // Set Normal Display
      0xD5, 0x80, // Set OSC Freq
      0x8D, 0x14, // Enable Charge Pump
      0xAF,       // Display On
    };
  
  if (en)
    {
      // deassert reset line
      rst -> setValue(HIGH);
      // send various configuration commands
      send_ctl_cmd(config, sizeof(config));
    }
  else
    {
      // assert reset line
      rst -> setValue(LOW);
    }
}

void P25317::toggle_10_x(void)
{
  // just toggle all GPIOs and send dummy data
  for (int i = 0; i < 10; i++)
    {
      rst -> setValue(HIGH);
      cs -> setValue(HIGH);
      dat_ctl -> setValue(HIGH);
      usleep(500000);
      rst -> setValue(LOW);
      cs -> setValue(LOW);
      dat_ctl -> setValue(LOW);
      usleep(500000);
      
      tx[0] = i;
      if (spi_transfer(1) == -1)
	{
	  perror("Failed to send SPI word");
	  return;
	}
    }
}

int P25317::spi_transfer(int len)
{
  /* VooDoo alert
     For some reason, there needs to be a 
     char array declared in order for the
     spi_ioc_transfer structure to work
     properly... probably has something to
     do with alignment on the stack.  For
     now, we'll just roll with the voodoo,
     but at some point it would be worthwhile
     to figure out what exactly is going on */
  char voodoo_buf[5];

  // create and configure transfer struct
  spi_ioc_transfer transfer;
  transfer.tx_buf = (unsigned long)tx;
  transfer.rx_buf = (unsigned long)rx;
  transfer.len = 1;
  transfer.speed_hz = 1000000;
  transfer.bits_per_word = 8;
  transfer.delay_usecs = 0;

  // send the spi message
  int status = ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
  if (status < 0)
    {
      perror("SPI: SPI_IOC_MESSAGE failed");
      return -1;
    }
  return status;
}

void P25317::send_ctl_cmd(char *buf, int buf_len)
{
  // deassert data pin
  dat_ctl -> setValue(DAT_CTL_CMD);
  // drop CS
  cs -> setValue(LOW);
  // send the message
  for (int i = 0; i < buf_len; i++)
    {
      tx[0] = buf[i];
      spi_transfer(1);
    }
  // raise CS
  cs -> setValue(HIGH);
}

void P25317::send_dat_cmd(char *buf, int buf_len)
{
  // assert data pin
  dat_ctl -> setValue(DAT_CTL_DATA);
  // drop CS
  cs -> setValue(LOW);
  // send the message
  for (int i = 0; i < buf_len; i++)
    {
      tx[0] = buf[i];
      spi_transfer(1);
    }
  // raise CS
  cs -> setValue(HIGH);
}

void P25317::send_test_screen(char screen)
{
  char buf_ff[32], buf_00[32];
  for (int i = 0; i < 32; i++)
    {
      buf_ff[i] = 0xff;
      buf_00[i] = 0x00;
    }
  if (screen)
    {
      for (int i = 0; i < 8; i++)
       	{
  	  for (int j = 0; j < 4; j++)
  	    {
  	      if (j % 2)
  		{
  		  send_dat_cmd(buf_ff, sizeof(buf_ff));
  		}
  	      else
  		{
  		  send_dat_cmd(buf_00, sizeof(buf_00));
  		}
  	    }
	}
    }
  else
    {
      for (int i = 0; i < 8; i++)
       	{
  	  for (int j = 0; j < 4; j++)
  	    {
  	      if (j % 2)
  		{
  		  send_dat_cmd(buf_00, sizeof(buf_00));
  		}
  	      else
  		{
  		  send_dat_cmd(buf_ff, sizeof(buf_ff));
  		}
  	    }
	}
    }
}


int main()
{
  P25317 my_disp(59, 58, 57); // 59:rst_pin, 58:cs_pin, 57:dat_ctl_pin
  
  my_disp.init_display();
  my_disp.init_spi();
  my_disp.enable_display(DISP_ENABLE);
  my_disp.send_test_screen(1);
  usleep(500000);
  my_disp.send_test_screen(0);
  usleep(500000);
  my_disp.send_test_screen(1);
  usleep(500000);
  my_disp.send_test_screen(0);
  usleep(500000);
  my_disp.send_test_screen(1);
  usleep(500000);
  my_disp.send_test_screen(0);
  usleep(500000);
  my_disp.send_test_screen(1);
  usleep(500000);
  my_disp.send_test_screen(0);
  usleep(500000);


  return 0;
}
