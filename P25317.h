#ifndef P25317_H_
#define P25317_H_
#include "GPIO.h"

using namespace exploringBB;

#define SPI_PATH "/dev/spidev1.0"
#define MAX_BUF_LEN 100

class P25317
{
 private:
  GPIO *rst, *cs;
  unsigned int fd, speed;
  unsigned char mode, bits, value, null;
  char *tx, *rx;

 public:
  //  P25317(int rst_pin, int cs_pin, char *rx, char *tx); //constructor
  P25317(int rst_pin, int cs_pin); //constructor

  void init_display(void);
  void init_spi(void);
  void enable_display(int en);
  void toggle_10_x(void);
  int spi_transfer(int len);

  ~P25317(); //destructor
};

#endif
