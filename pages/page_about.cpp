/*
  page_about.c
*/

#include "page_about.h"
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

void page_about_on_active(void);
void page_about_on_refresh(void);
static void page_about_on_event(unsigned char btn);
static void page_about_draw_text(void);

/**************************************************
  page_about_proc

  message processor for page
***************************************************/

char page_about_proc(events ev, unsigned char btn)
{
    switch (ev)
    {
    case EV_PAGE_ACTIVE:
        page_about_on_active();
        return 1;
    case EV_REFRESH:
        page_about_on_refresh();
        return 1;
    case EV_BTN:
        page_about_on_event(btn);
        break;
    }
    return 0;
}

/**************************************************
  page_about_on_active

  handle activation message for page
***************************************************/

void page_about_on_active(void)
{
  // clear the entire screen
  win_clear_screen();

  // update disp
  page_about_draw_text();
}

/**************************************************
  page_about_on_refresh

  handle refresh message for page
***************************************************/

void page_about_on_refresh(void)
{
  page_about_draw_text();
}

/**************************************************
  page_about_on_event

***************************************************/

void page_about_on_event(unsigned char btn)
{
  switch (btn)
    {
      /* menu & back buttons */
    case BTN_MENU:
      break;
    case BTN_BACK:
      // return to menu page
      page_show_page(PAGE_MENU);
      break;
      /* up/down buttons */
    case BTN_UP:
      break;
    case BTN_DN:
      break;
    }
}

/**************************************************
  page_about_draw_text

  draw about item text
***************************************************/
static unsigned char ROW_1_Y = 8;
static unsigned char ROW_2_Y = 24;
static unsigned char ROW_3_Y = 40;

static void page_about_draw_text(void)
{
  static unsigned int text_counter = 0;
  const char *str_pid = "DSR/EO2";
  const char *str_sn = "S/N: 1400101";
  const char *str_band = "Band: A1B1";

  win_set_transparent(TRANS_OFF);
  win_set_inverse(INVERSE_OFF);

  win_put_text_xy(str_pid, 0, ROW_1_Y, MAX_COL);
  win_put_text_xy(str_sn, 0, ROW_2_Y, MAX_COL);
  win_put_text_xy(str_band, 0, ROW_3_Y, MAX_COL);
  
  ++text_counter;
}
