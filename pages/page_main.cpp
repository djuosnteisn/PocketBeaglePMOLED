/*
  page_main.c
*/

#include "page_main.h"
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

void page_main_on_active(void);
void page_main_on_refresh(void);
static void page_main_on_event(unsigned char btn);
static void page_main_draw_title(void);
static void page_main_draw_rf(void);
static void page_main_draw_audio(void);
static void page_main_draw_batt(void);

/**************************************************
  page_main_proc

  message processor for page
***************************************************/

char page_main_proc(events ev, unsigned char btn)
{
    switch (ev)
    {
    case EV_PAGE_ACTIVE:
        page_main_on_active();
        return 1;
    case EV_REFRESH:
        page_main_on_refresh();
        return 1;
    case EV_BTN:
        page_main_on_event(btn);
        break;
    }
    return 0;
}

/**************************************************
  page_main_on_active

  handle activation message for page
***************************************************/

void page_main_on_active(void)
{
  //clear the entire screen
  win_clear_screen();

  //update disp
  page_main_draw_title();
  page_main_draw_rf();
  page_main_draw_audio();
  page_main_draw_batt();
}

/**************************************************
  page_main_on_refresh

  handle refresh message for page
***************************************************/

void page_main_on_refresh(void)
{
    page_main_draw_rf();
    page_main_draw_audio();
    page_main_draw_batt();
}

/**************************************************
  page_main_on_event

***************************************************/

void page_main_on_event(unsigned char btn)
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
  page_main_draw_title

  draw title at top of page
***************************************************/
static const unsigned char TITLE_X = 64;
static const unsigned char TITLE_Y = 0;

static void page_main_draw_title(void)
{
  const char *str_title = "MAIN PAGE";
  unsigned int temp = win_get_str_len(str_title) / 2;

  win_put_text_xy(str_title, TITLE_X - temp, TITLE_Y, FRAME_WIDTH_PIX);
}

/**************************************************
  page_main_draw_rf_meters

  draw various RF meters: RSSI, Ant Diversity, etc
***************************************************/
static void page_main_draw_rf(void)
{
}

/**************************************************
  page_main_draw_audio_meters

  draw audio meter boxes
***************************************************/

void page_main_draw_audio(void)
{
}

/**************************************************
  page_main_draw_batt

  draw battery meter
***************************************************/

void page_main_draw_batt(void)
{
}
