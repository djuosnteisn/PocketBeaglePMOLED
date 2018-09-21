#ifndef SPI_H_
#define SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PATH_LEN 100
#define MAX_MODE 3
#define MIN_MODE 0
#define MAX_BITS 8
#define MIN_BITS 0
#define MAX_SPEED 5000000
#define MIN_SPEED 500000

  void spi_open(const char *dev_path);
  void spi_close(void);
  void spi_transfer(unsigned char *tx, unsigned char *rx, unsigned int len);
  int get_fd(void);
  void spi_set_mode(unsigned char new_mode);
  unsigned char spi_get_mode(void);
  void spi_set_bits(unsigned char new_bits);
  unsigned char spi_get_bits(void);
  void spi_set_speed(unsigned int new_speed);
  unsigned int spi_get_speed(void);
  

#ifdef __cplusplus
}
#endif
#endif // SPI_H_
