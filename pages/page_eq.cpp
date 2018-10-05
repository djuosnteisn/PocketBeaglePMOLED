/*
  page_eq.c
*/
#include "page_eq.h"
#include "../page.h"
#include "../win.h"
#include "../btns.h"

/**************/
/*   macros   */
/**************/

/**************/
/*   locals   */
/**************/

/*****************/
/*   functions   */
/*****************/

void page_eq_on_active(void);
void page_eq_on_refresh(void);
static void page_eq_on_event(unsigned char btn);
static void page_eq_draw_text(void);

/**************************************************
  page_eq_proc

  message processor for page
***************************************************/

char page_eq_proc(events ev, unsigned char btn)
{
    switch (ev)
    {
    case EV_PAGE_ACTIVE:
        page_eq_on_active();
        return 1;
    case EV_REFRESH:
        page_eq_on_refresh();
        return 1;
    case EV_BTN:
        page_eq_on_event(btn);
        break;
    }
    return 0;
}

/**************************************************
  page_eq_on_active

  handle activation message for page
***************************************************/

void page_eq_on_active(void)
{
  //clear the entire screen
  win_clear_screen();

  //update disp
  page_eq_draw_text();
}

/**************************************************
  page_eq_on_refresh

  handle refresh message for page
***************************************************/

void page_eq_on_refresh(void)
{
  page_eq_draw_text();
}

/**************************************************
  page_eq_on_event

***************************************************/

void page_eq_on_event(unsigned char btn)
{
  switch (btn)
    {
      /* up/down buttons */
    case BTN_UP:
      //NOTE make vol increase
      break;
    case BTN_DN:
      //NOTE make vol decrease
      break;
      /* menu & back buttons */
    case BTN_MENU:
      page_show_page(PAGE_MENU);
      break;
    case BTN_BACK:
      break;
    }
}

/**************************************************
  page_eq_draw_text

  draw text objects
***************************************************/

static void page_eq_draw_text(void)
{
  win_set_transparent(TRANS_OFF);
  win_set_inverse(INVERT_OFF);
}
