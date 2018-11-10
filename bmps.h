#ifdef __cplusplus
extern "C" {
#endif

#ifndef BMPS_H
#define BMPS_H

  // macros for bluetooth bmp
  #define BT_LEVELS 6
  #define BT_BMP_BYTE_SIZE 60

  /* bitmap data structure */
  typedef struct tagBMP_T
  {
    unsigned char height;
    unsigned char width;
    const unsigned char *image;
  } BMP_T;

  /* bmp images */
  const extern BMP_T lectro_circle;
  const extern BMP_T sc_name;
  const extern BMP_T hp_l;
  const extern BMP_T hp_r;
  const extern BMP_T RFSignal0;
  const extern BMP_T RFSignal1;
  const extern BMP_T RFSignal2;
  const extern BMP_T RFSignal3;
  const extern BMP_T RFSignal4;
  const extern BMP_T RFSignal5;
  const extern BMP_T eq_slider;
  const extern BMP_T vol;


#endif /* BMPS_H */

#ifdef __cplusplus
}
#endif
