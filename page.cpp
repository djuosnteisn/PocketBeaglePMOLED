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
#define DEBOUNCE 4 // ~200ms
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
      debounce = DEBOUNCE;
      win_clear_screen();
      switch (ev.event)
	{
	case BTN_EV_MENU:
	  win_put_text_xy("Menu Press", 5, 24, FRAME_WIDTH_PIX);
	  break;
	case BTN_EV_BACK:
	  win_put_text_xy("Back Press", 5, 24, FRAME_WIDTH_PIX);
	  break;
	case BTN_EV_UP:
	  win_put_text_xy("Up Press", 5, 24, FRAME_WIDTH_PIX);
	  break;
	case BTN_EV_DN:
	  win_put_text_xy("Down Press", 5, 20, FRAME_WIDTH_PIX);
	  break;
	case BTN_EV_MENU | BTN_EV_UP:
	  win_put_text_xy("Menu & Up", 5, 20, FRAME_WIDTH_PIX);
	  if (win_get_invert())
	    win_set_invert(0);
	  else
	    win_set_invert(1);
	  break;
	case BTN_EV_BACK | BTN_EV_DN:
	  win_put_bmp_xy(33, 0, sc_circle);
	  break;
	default:
	  win_put_text_xy("Unknown", 5, 20, FRAME_WIDTH_PIX);
	  break;
	}
      // if (i)
      // 	{
      // 	  i = 0;
      // 	  win_put_bmp_xy(33, 0, sc_circle);
      // 	}
      // else
      // 	{
      // 	  i = 1;
      // 	  //win_put_bmp_xy(0, 10, sc_name);
      // 	  //win_put_bmp_xy(0, 7, diag);
      // 	  win_put_text_xy("BLA BLA", 10, 10, 100);
      // 	  unsigned char len = win_get_str_len("BLA BLA");
      // 	  win_put_line_horz(10, 10 + len, 27);
      // 	  win_put_box_empty(10, 30, 118, 60);
      // 	  win_put_box_empty(12, 32, 116, 58);
      // 	}
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
