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
  const extern BMP_T sc_circle;
  const extern BMP_T sc_name;
  const extern BMP_T hp_l;
  const extern BMP_T hp_r;
  const extern BMP_T BTSignal0;
  const extern BMP_T BTSignal1;
  const extern BMP_T BTSignal2;
  const extern BMP_T BTSignal3;
  const extern BMP_T BTSignal4;
  const extern BMP_T BTSignal5;
  const extern BMP_T eq_slider;
  const extern BMP_T vol;


#endif /* BMPS_H */

#ifdef __cplusplus
}
#endif
