#ifndef MAIN_H
#define MAIN_H

/* macros */
// EQ stuff
#define EQ_MODE_FLAT 0
#define EQ_MODE_PARTY 1
#define EQ_MODE_HIGH 2
#define EQ_MODE_LOW 3
#define EQ_MODE_ROCK 4
#define EQ_MODE_SURROUND 5
#define EQ_MODE_CUST 6
#define DEF_EQ_MODE EQ_MODE_FLAT
#define MAX_EQ_MODE EQ_MODE_CUST
#define MIN_EQ_MODE EQ_MODE_FLAT
// custom EQ stuff
#define MAX_LVL 10
#define MIN_LVL (-10)
// Active Noise Control stuff
#define MAX_ANC_LVL 3
#define MIN_ANC_LVL 0
#define DEF_ANC_LVL MAX_ANC_LVL
// Volume
#define VOL_MAX 100
#define VOL_MIN 0
#define DEF_VOL 50


/* types */

typedef struct tag_EQ
{
  unsigned char mode;
  signed char bass_lvl;
  signed char mid_lvl;
  signed char treb_lvl;
} EQ;

/* globals */

extern unsigned int tick;
extern EQ eq;
extern unsigned char volume;
extern unsigned char anc_lvl;

#endif //MAIN_H
