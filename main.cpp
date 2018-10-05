#include <unistd.h>
#include <stdio.h>
#include <unistd.h> // required for usleep
#include "main.h"
#include "win.h"
#include "page.h"

EQ eq;
unsigned char volume;

int main(void)
{
  page_init();
  eq.mode = DEF_EQ_MODE;
  eq.bass_lvl = 0;
  eq.mid_lvl = 0;
  eq.treb_lvl = 0;
  volume = DEF_VOL;

  while (1)
    {
      page_task();
      usleep(50000);
    }
}
