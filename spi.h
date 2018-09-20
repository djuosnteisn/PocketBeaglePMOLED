#ifndef SPI_H_
#define SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PATH_LEN 100

extern void spi_open(const char *dev_path);
extern void spi_close(void);
extern void spi_transfer(unsigned char *tx, unsigned char *rx, unsigned int len);
extern int get_fd(void);

#ifdef __cplusplus
}
#endif
#endif // SPI_H_
