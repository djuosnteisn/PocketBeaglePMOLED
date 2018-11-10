#ifndef MAIN_H
#define MAIN_H

/* macros */
// BACKLIGHT stuff
#define BACKLIGHT_MODE_ON 0
#define BACKLIGHT_MODE_30S 1
#define BACKLIGHT_MODE_5M 2
#define DEF_BACKLIGHT_MODE BACKLIGHT_MODE_ON
#define MAX_BACKLIGHT_MODE BACKLIGHT_MODE_5M
#define MIN_BACKLIGHT_MODE BACKLIGHT_MODE_ON
// custom EQ stuff
#define MAX_LVL 10
#define MIN_LVL (-10)
// Active Noise Control stuff
#define MAX_COMPAT_MODE 3
#define MIN_COMPAT_MODE 0
#define DEF_COMPAT_MODE MIN_COMPAT_MODE
// Volume
#define VOL_MAX 100
#define VOL_MIN 0
#define DEF_VOL 50


/* types */

/* globals */

extern unsigned int tick;
extern unsigned char bl_mode;
extern unsigned char volume;
extern unsigned char compat_mode;

#endif //MAIN_H
