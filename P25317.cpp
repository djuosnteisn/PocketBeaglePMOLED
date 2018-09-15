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


//P25317::P25317(int rst_pin, int cs_pin, char *rx, char *tx)
P25317::P25317(int rst_pin, int cs_pin)
{
  rst = new GPIO(rst_pin);
  cs = new GPIO(cs_pin);
  speed = 1000000;
  bits = 8;
  mode = 3;
  rx = new char[MAX_BUF_LEN];
  tx = new char[MAX_BUF_LEN];
  // this -> tx = tx;
  if ((fd = open(SPI_PATH, O_RDWR)) < 0)
    {
      perror("SPI Error: Can't open device");
      return;
    }
  printf("fd=%d\n", fd);
}

void P25317::init_display(void)
{
  // reset and CS are outputs
  rst -> setDirection(OUTPUT);
  cs -> setDirection(OUTPUT);
  // keep the reset asserted
  enable_display(0);
  // open and configure the spi handle
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
  if (en)
    {
      //deassert reset line
      rst -> setValue(HIGH);
      //NOTE test CS pin as well
      cs -> setValue(HIGH);
    }
  else
    {
      //assert reset line
      rst -> setValue(LOW);
      //NOTE test CS pin as well
      cs -> setValue(LOW);
    }
}

void P25317::toggle_10_x(void)
{
  for (int i = 0; i < 10; i++)
    {
      enable_display(1);
      usleep(500000);
      enable_display(0);
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
  // create local buffers for ioctl...
  // these pointers need to be last thing pushed to stack
  //char tx_buf[1];
  char rx_buf[5];

  // create and configure transfer struct
  //struct spi_ioc_transfer transfer;
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

P25317::~P25317()
{
  delete rst;
  delete cs;
}


int main()
{
  // char tx[MAX_BUF_LEN], rx[MAX_BUF_LEN];
  //  P25317 my_disp(59, 58, rx, tx);
  P25317 my_disp(59, 58);
  
  my_disp.init_display();
  my_disp.init_spi();
  my_disp.toggle_10_x();

  /*
  // basic output example
  outGPIO.setDirection(OUTPUT);
  for (int i = 0; i < 10; i++)
    {
      outGPIO.setValue(HIGH);
      usleep(500000);
      outGPIO.setValue(LOW);
      usleep(500000);
    }

  // basic input example
  inGPIO.setDirection(INPUT);
  cout << "The Value of the input is: "<< inGPIO.getValue() << endl;

  // fast write 100 times
  outGPIO.streamOpen();
    for (int i = 0; i < 100; i++)
      {
	outGPIO.streamWrite(HIGH);
	outGPIO.streamWrite(LOW);
      }
  outGPIO.streamClose();
  */
  return 0;
}
