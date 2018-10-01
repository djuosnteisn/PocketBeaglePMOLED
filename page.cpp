#include <stdio.h>
#include <string.h>
#include "page.h"
#include "btns.h"
#include "win.h"
// page includes
#include "pages/page_main.h"
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

  BTN_EV ev = btns.get_event();
  if (ev.btn)
    {
      page_on_event(s_cur_page, EV_BTN,  ev.btn);
    }
  // post a refresh
  page_on_event(s_cur_page, EV_REFRESH, 0);

  
  // static unsigned char i = 0;
  // static unsigned int debounce = DEBOUNCE;
  // BTN_EV ev = btns.get_event();

  // if (ev.event && !debounce--)
  //   {
  //     btns.reset_event();
  //     debounce = DEBOUNCE;
  //     win_clear_screen();
  //     switch (ev.event)
  // 	{
  // 	case BTN_EV_MENU:
  // 	  win_put_text_xy("Menu Press", 5, 24, FRAME_WIDTH_PIX);
  // 	  break;
  // 	case BTN_EV_BACK:
  // 	  win_put_text_xy("Back Press", 5, 24, FRAME_WIDTH_PIX);
  // 	  break;
  // 	case BTN_EV_UP:
  // 	  win_put_text_xy("Up Press", 5, 24, FRAME_WIDTH_PIX);
  // 	  break;
  // 	case BTN_EV_DN:
  // 	  win_put_text_xy("Down Press", 5, 20, FRAME_WIDTH_PIX);
  // 	  break;
  // 	case BTN_EV_MENU | BTN_EV_UP:
  // 	  win_put_text_xy("Menu & Up", 5, 20, FRAME_WIDTH_PIX);
  // 	  if (win_get_invert())
  // 	    win_set_invert(0);
  // 	  else
  // 	    win_set_invert(1);
  // 	  break;
  // 	case BTN_EV_BACK | BTN_EV_DN:
  // 	  win_put_bmp_xy(33, 0, sc_circle);
  // 	  break;
  // 	default:
  // 	  win_put_text_xy("Unknown", 5, 20, FRAME_WIDTH_PIX);
  // 	  break;
  // 	}
  //   }
}

void page_show_page(pages page)
{
  page_on_event(page, EV_PAGE_ACTIVE, 0);
  s_cur_page = page;
}

void page_on_event(pages page, events ev, unsigned char btn)
{
  switch (page)
    {
    case PAGE_MAIN:
      page_main_proc(ev, btn);
      break;
    case PAGE_EQ:
      //page_eq_proc(ev, btn);
      break;
    case PAGE_CONTRAST:
      //page_contrast_proc(ev, btn);
      break;
    case PAGE_ABOUT:
      //page_about_proc(ev, btn);
      break;
    }
}
