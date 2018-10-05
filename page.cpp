#include <stdio.h>
#include <string.h>
#include "page.h"
#include "btns.h"
#include "win.h"
// page includes
#include "pages/page_main.h"
#include "pages/page_menu.h"
#include "pages/page_contrast.h"
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
#define DEBOUNCE 5 // ~250ms

  BTN_EV ev = btns.get_event();
  if (ev.btn)
    {
      page_on_event(s_cur_page, EV_BTN,  ev.btn);
      // clear the event
      btns.reset_event();
    }
  // post a refresh
  page_on_event(s_cur_page, EV_REFRESH, 0);
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
    case PAGE_MENU:
      page_menu_proc(ev, btn);
      break;
    case PAGE_EQ:
      //page_eq_proc(ev, btn);
      break;
    case PAGE_CONTRAST:
      page_contrast_proc(ev, btn);
      break;
    case PAGE_ABOUT:
      //page_about_proc(ev, btn);
      break;
    }
}
