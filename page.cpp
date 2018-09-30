#include <stdio.h>
#include <string.h>
#include "page.h"
#include "btns.h"
#include "win.h"
//NOTE temporary
#include "bmps.h"

static BTNS btns(PIN_MENU, PIN_BACK, PIN_UP, PIN_DN);
static pages s_cur_page = PAGE_MAIN;

void page_init(void)
{
  win_init();
  page_show_page(PAGE_MAIN);
}

/* page_task()
   called every 50ms.  Checks for UI events
   and dispatches to current page.
*/
void page_task(void)
{
#define DEBOUNCE 3 // ~150ms
  /*
  while (btns.get_event())
    {
      ev = EV_BTN;
      page_on_event(s_cur_page, EV_BTN);
    }
  // post a refresh
  page_on_event(s_cur_page, EV_REFRESH);
  */

  static unsigned char i = 0;
  static unsigned int debounce = DEBOUNCE;
  BTN_EV ev = btns.get_event();

  if (ev.event && !debounce--)
    {
      btns.reset_event();
      win_clear_screen(BLACK);
      if (i)
  	{
  	  i = 0;
  	  win_put_bmp_xy(33, 0, sc_circle);
  	}
      else
  	{
  	  i = 1;
	  win_put_bmp_xy(0, 4, sc_name);
	  //win_put_bmp_xy(0, 4, diag);
	  //win_put_text_xy("BLA BLA", 0, 0, 100);
  	}
      debounce = DEBOUNCE;
    }
}

void page_show_page(pages page)
{
  page_on_event(page, EV_PAGE_ACTIVE);
  s_cur_page = page;
}

void page_on_event(pages page, events ev)
{
  switch (page)
    {
    case PAGE_MAIN:
      break;
    case PAGE_EQ:
      break;
    case PAGE_ABOUT:
      break;
    }
}
