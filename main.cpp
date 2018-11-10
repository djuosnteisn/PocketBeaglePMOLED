#include <unistd.h>
#include <stdio.h>
#include <unistd.h> // required for usleep
#include "main.h"
#include "win.h"
#include "page.h"

unsigned char bl_mode;
unsigned char volume;
unsigned char compat_mode;
unsigned int tick;
const unsigned int PAGE_COUNT = 10;

int main(void)
{
  page_init();
  bl_mode = DEF_BACKLIGHT_MODE;
  volume = DEF_VOL;
  compat_mode = DEF_COMPAT_MODE;

  while (1)
    {
      if (!(tick % PAGE_COUNT))
	page_task();
      usleep(5000);
      tick++;
    }
}
