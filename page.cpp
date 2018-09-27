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

void page_task(void)
{
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

  if (i)
    {
      //      win_put_bmp_xy(33, 0, sc_circle);
      btns.set_state(i);
      i = 0;
    }
  else
    {
      //      win_put_bmp_xy(0, 2, sc_name);
      btns.set_state(i);
      i = 1;
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
