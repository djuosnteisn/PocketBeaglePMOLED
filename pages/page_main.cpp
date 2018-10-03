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
// audio meters
static const unsigned char LR_X = 0;
static const unsigned char L_Y = 0;
static const unsigned char R_Y = 8;
static const unsigned char AUD_X1 = LR_X + hp_l.width;
static const unsigned char AUDL_X2 = AUD_X1 + 80;
static const unsigned char AUDR_X2 = AUD_X1 + 70;
static const unsigned char AUDL_Y1 = L_Y;
static const unsigned char AUDL_Y2 = L_Y + hp_l.height - 1;
static const unsigned char AUDR_Y1 = R_Y;
static const unsigned char AUDR_Y2 = R_Y + hp_r.height - 1;
// BT RF meter
static const unsigned char BT_X = 97;
static const unsigned char BT_Y = 0;
// battery meters
static const unsigned char BATT_TIP_X1 = 119;
static const unsigned char BATT_TIP_X2 = 123;
static const unsigned char BATT_TIP_Y1 = 40;
static const unsigned char BATT_TIP_Y2 = 42;
static const unsigned char BATT_X1 = 116;
static const unsigned char BATT_X2 = 126;
static const unsigned char BATT_TOP_Y1 = 44;
static const unsigned char BATT_TOP_Y2 = 49;
static const unsigned char BATT_MID_Y1 = 51;
static const unsigned char BATT_MID_Y2 = 56;
static const unsigned char BATT_BOT_Y1 = 58;
static const unsigned char BATT_BOT_Y2 = 63;

static void page_main_draw_title(void)
{
  // draw icons
  win_put_bmp_xy(LR_X, L_Y, hp_l);
  win_put_bmp_xy(LR_X, R_Y, hp_r);
  win_put_bmp_xy(BT_X, BT_Y, BTSignal3);

  // draw fake audio boxes
  win_put_box(AUD_X1, AUDL_Y1, AUDL_X2, AUDL_Y2);
  win_put_box(AUD_X1, AUDR_Y1, AUDR_X2, AUDR_Y2);

  // draw song name
  win_put_text_xy("Song Name", 0, 24, MAX_COL);

  // draw battery icon
  win_put_box_empty(BATT_TIP_X1, BATT_TIP_Y1, BATT_TIP_X2, BATT_TIP_Y2);
  win_put_box_empty(BATT_X1, BATT_TOP_Y1, BATT_X2, BATT_TOP_Y2);
  win_put_box(BATT_X1, BATT_MID_Y1, BATT_X2, BATT_MID_Y2);
  win_put_box(BATT_X1, BATT_BOT_Y1, BATT_X2, BATT_BOT_Y2);

  // put eq mode
  win_put_text_xy("EQ:Cust", 0, 48, MAX_COL);
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
