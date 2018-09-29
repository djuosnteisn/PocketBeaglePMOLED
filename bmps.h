#ifdef __cplusplus
extern "C" {
#endif

#ifndef BMPS_H
#define BMPS_H


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
  //NOTE diag
  const extern BMP_T diag;


#endif /* BMPS_H */

#ifdef __cplusplus
}
#endif
