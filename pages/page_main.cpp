/*
  page_main.c
*/

#include "main.h"
#include "page.h"
#include "page_main.h"
#include "win.h"
#include "btns.h"

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
static void page_main_on_event(int event);
static void page_main_draw_rf_meters(void);
static void page_main_draw_audio_meters(void);
static void page_main_draw_batt(void);

/**************************************************
  page_main_proc

  message processor for page
***************************************************/

char page_main_proc(char id, unsigned int param)
{
    switch (id)
    {
    case UI_EV_PAGE_ACTIVE:
        page_main_on_active();
        return 1;
    case UI_EV_PAGE_REFRESH:
        page_main_on_refresh();
        return 1;
    case UI_EV_PAGE_BTN:
        page_main_on_event(param);
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
  win_clear_screen(BLACK);

  //update disp
  page_main_draw_rf_meters(void);
  page_main_draw_audio_meters(void);
  page_main_draw_batt(void);
}

/**************************************************
  page_main_on_refresh

  handle refresh message for page
***************************************************/

void page_main_on_refresh(void)
{
    page_main_draw_rf();
    page_main_draw_audio();
}

/**************************************************
  page_main_on_event

***************************************************/

void page_main_on_event(int event)
{
  switch (event)
    {
      /* up/down buttons */
    case BC_BTN_UP:
      //NOTE make vol increase
      break;
    case BC_BTN_DN:
      //NOTE make vol decrease
      break;
      /* menu & back buttons */
    case BC_BTN_MENU:
      page_show_page(PAGE_MENU, PAGE_MAIN);
      break;
    case BC_BTN_BACK:
      break;
    }
}

/**************************************************
  page_main_draw_rf_meters

  draw various RF meters: RSSI, Ant Diversity, etc
***************************************************/
static const short RSSI_X = RF_METER_RX1_X + RSSI_WIDTH;
static const short RSSI_Y = RF_METER_Y1 + 1;

static void page_main_draw_rf_meters(void)
{
}

/**************************************************
  page_main_draw_audio_meters

  draw audio meter boxes
***************************************************/

void page_main_draw_audio_meters(void)
{
}

/**************************************************
  page_main_draw_batt

  draw battery meter
***************************************************/

void page_main_draw_batt(void)
{
}

