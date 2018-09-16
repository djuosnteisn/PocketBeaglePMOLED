#ifndef P25317_H_
#define P25317_H_
#include "GPIO.h"

using namespace exploringBB;

#define SPI_PATH "/dev/spidev1.0"
#define MAX_BUF_LEN 100
#define DAT_CTL_DATA  HIGH
#define DAT_CTL_CMD   LOW
#define DISP_ENABLE   1
#define DISP_DISABLE  0

class P25317
{
 private:
  GPIO *rst, *cs, *dat_ctl;
  unsigned int fd, speed;
  unsigned char mode, bits, value, null;
  char *tx, *rx;

 public:
  P25317(int rst_pin, int cs_pin, int dat_ctl_pin); //constructor

  void init_display(void);
  void init_spi(void);
  void enable_display(int en);
  void toggle_10_x(void);
  int spi_transfer(int len);
  void send_ctl_cmd(char *buf, int buf_len);
  void send_dat_cmd(char *buf, int buf_len);
  void send_test_screen(char screen);

  ~P25317(); //destructor
};

#endif
