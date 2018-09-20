#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include "spi.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

  static void pabort(const char *s)
  {
    perror(s);
    abort();
  }

  static char device[MAX_PATH_LEN] = {0}; //NOTE doesn't need to be 100  = "/dev/spidev1.0";
  static int fd;
  /*
  //NOTE tr must be kept static and initialized once to avoid corruption as an autovariable.
  Ideally, ever structure member will be properly initialized, but I haven't found the 
  struct type def yet. If it's not static, the next time the autovariable is created,
  uninitialized members values will be random, and potentially invalid.
  */
  static struct spi_ioc_transfer tr;
  static uint8_t mode = 3;
  static uint8_t bits = 8;
  static uint32_t speed = 1000000;
  static uint16_t delay;

  void spi_open(const char *dev_path)
  {
    int ret;

    /* these transfer params won't change */
    tr.delay_usecs = delay;
    tr.speed_hz = speed;
    tr.bits_per_word = bits;

    /* open device */
    strncpy(device, dev_path, MAX_PATH_LEN);
    fd = open(device, O_RDWR);
    if (fd < 0)
      pabort("can't open device");

    /*
     * spi mode
     */
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
      pabort("can't set spi mode");

    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1)
      pabort("can't get spi mode");

    /*
     * bits per word
     */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
      pabort("can't set bits per word");

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
      pabort("can't get bits per word");

    /*
     * max speed hz
     */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
      pabort("can't set max speed hz");

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
      pabort("can't get max speed hz");
  }

  void spi_close(void)
  {
    close(fd);
  }

  void spi_transfer(unsigned char *tx, unsigned char *rx, unsigned int len)
  {
    int ret;

    tr.tx_buf = (unsigned long)tx;
    tr.rx_buf = (unsigned long)rx;
    tr.len = len;

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    //    printf("sent %d bytes", ret);
    if (ret < 1)
    	pabort("can't send spi message");
  }

  int get_fd(void)
  {
    return fd;
  }

#ifdef __cplusplus
}
#endif
