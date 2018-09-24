#ifndef P25317_H
#define P25317_H
#include "GPIO.h"

using namespace exploringBB;

// SPI macros
#define SPI_MODE 3
#define SPI_SPEED 2000000
#define SPI_BITS 8
#define SPI_DEV_PATH "/dev/spidev1.0"

// logic macros
#define DAT_CTL_DATA HIGH
#define DAT_CTL_CMD LOW
#define DISP_ENABLE 1
#define DISP_DISABLE 0
#define NUM_COLS 128
#define NUM_ROWS 64

// P25317 macros
#define DEF_CONTRAST 0x80
#define ADDR_MODE_HORZ 0x00
#define ADDR_MODE_VERT 0X01
#define ADDR_MODE_PAGE 0x10
#define DEF_ADDR_MODE ADDR_MODE_VERT
#define ROW_REG 0x22
#define DEF_START_ROW 0
#define DEF_STOP_ROW 7
#define MAX_ROW 7
#define COL_REG 0x21
#define DEF_START_COL 0
#define DEF_STOP_COL 0x7F
#define MAX_COL 127
#define COL_OFF_REG 0xD3
#define DEF_COL_OFF 0x00
#define INV_ON 0xA7
#define INV_OFF 0xA6
#define DEF_INVERT INV_OFF
#define SLEEP_ON 0xAE
#define SLEEP_OFF 0xAF
#define DEF_SLEEP SLEEP_OFF

class P25317
{
 private:
  GPIO *rst, *cs, *dat_ctl;
  unsigned char contrast, addr_mode, start_row, stop_row;
  unsigned char start_col, stop_col, col_off, invert, sleep;

 public:
  P25317(int rst_pin, int cs_pin, int dat_ctl_pin); //constructor
  ~P25317(); //destructor

  void init_display(void);
  void init_spi(void);
  void close_spi(void);
  void enable_display(int en);
  void sleep_display(unsigned char state);
  void send_ctl_cmd(unsigned char *buf, int buf_len);
  void send_dat_cmd(unsigned char *buf, int buf_len);
  void set_contrast(unsigned char lvl);
  unsigned char get_contrast(void);
  void set_addr_mode(unsigned char mode);
  unsigned char get_addr_mode(void);
  void set_start_row(unsigned char row);
  unsigned char get_start_row(void);
  void set_stop_row(unsigned char row);
  unsigned char get_stop_row(void);
  void set_start_col(unsigned char col);
  unsigned char get_start_col(void);
  void set_stop_col(unsigned char col);
  unsigned char get_stop_col(void);
  void set_invert(unsigned char inv);
  unsigned char get_invert(void);
  void clear_screen(unsigned char on_off);
  void send_test_screen(char screen);
};

#endif //P25317_H
