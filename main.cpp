#include <unistd.h>
#include <stdio.h>
#include <unistd.h> // required for usleep
#include "main.h"
#include "win.h"
#include "page.h"

int main(void)
{
  page_init();

  while (1)
    {
      page_task();
      usleep(1000000);
    }
}
