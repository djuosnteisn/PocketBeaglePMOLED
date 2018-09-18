#ifndef P25317_H_
#define P25317_H_
#include "GPIO.h"
#include "SPIDevice.h"

using namespace exploringBB;

#define SPI_BUS 1
#define SPI_DEV 0
#define SPI_SPEED 2000000
#define SPI_BITS 8
#define DAT_CTL_DATA  HIGH
#define DAT_CTL_CMD   LOW
#define DISP_ENABLE   1
#define DISP_DISABLE  0
#define MAX_SPI_BUF 100

class P25317
{
 private:
  GPIO *rst, *cs, *dat_ctl;
  SPIDevice *spi;

 public:
  P25317(int rst_pin, int cs_pin, int dat_ctl_pin); //constructor

  void init_display(void);
  void init_spi(void);
  void enable_display(int en);
  void close_spi(void);
  void send_ctl_cmd(unsigned char *buf, int buf_len);
  void send_dat_cmd(unsigned char *buf, int buf_len);
  void set_contrast(unsigned char lvl);
  void send_test_screen(char screen);

  ~P25317(); //destructor
};

#endif
