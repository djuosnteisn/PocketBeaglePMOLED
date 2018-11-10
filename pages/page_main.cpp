/*
  page_main.c
*/
#include <stdlib.h> // rand
#include <string.h> // strlen
#include "page_main.h"
#include "../main.h"
#include "../page.h"
#include "../win.h"
#include "../btns.h"

/**************/
/*   macros   */
/**************/
#define AUD_METER_MAX 65

/**************/
/*   locals   */
/**************/
static unsigned char s_l_audio_meter_1 = 40;
static unsigned char s_r_audio_meter_1 = 40;
static unsigned char s_l_audio_meter_2 = 40;
static unsigned char s_r_audio_meter_2 = 40;
static unsigned char s_rf_meter_1 = 3, s_rf_meter_2 = 2;
static unsigned char s_batt_meter_1 = 4, s_prev_batt_meter_1;
static unsigned char s_batt_meter_2 = 2, s_prev_batt_meter_2;

/*****************/
/*   functions   */
/*****************/

void page_main_on_active(void);
void page_main_on_refresh(void);
static void page_main_on_event(unsigned char btn);
static void page_main_draw_text(void);
static void page_main_draw_rf(void);
static void page_main_draw_audio(void);
static void page_main_draw_batt(void);
static void page_main_fake_meters(void);

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
  // clear the entire screen
  win_clear_screen();

  // force batt meter
  s_prev_batt_meter_1 = s_batt_meter_1 + 1;
  s_prev_batt_meter_2 = s_batt_meter_2 + 1;
  // update disp
  page_main_draw_text();
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
  page_main_fake_meters();
  page_main_draw_text();
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
      volume += 5;
      if (volume > VOL_MAX)
	volume = VOL_MAX;
      break;
    case BTN_DN:
      volume -= 5;
      if (volume > VOL_MAX) //unsigned rollover
	volume = VOL_MIN;
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
  page_main_draw_text

  draw text objects
***************************************************/
/*
static const unsigned char SONG_X = 0;
static const unsigned char SONG_Y = 24;
static const unsigned char SONG_MAX_WIDTH = 110;
static const unsigned char SCROLL_DELAY = 5; // 1/4 second
static const unsigned char EQ_X = 0;
static const unsigned char EQ_Y = 48;
static const unsigned char VOL_X = 69;
static const unsigned char VOL_Y = 48;
*/

static void page_main_draw_text(void)
{
  /*
  static const char *song_name = "Your Favorite Jam!  ";
  static unsigned char song_index = 0, str_width;
  static unsigned int scroll_count = 0;
  static const char *str_eq[] = {"Flat", "Party", "High", "Low", "Rock", "Surr", "Cust"};
  char vol_buf[4];
  unsigned int temp;
  */

  /*
  // draw song name, scroll if necessary
  win_set_transparent(TRANS_OFF);
  if (win_get_str_len(song_name) > SONG_MAX_WIDTH)
    {
      if (!(++scroll_count % SCROLL_DELAY))
	{
	  char song_str_len = strlen(song_name) - 1; // trim /n
	  char temp[song_str_len];
	  for (unsigned int i = 0; i < song_str_len; i++)
	    {
	      if ((song_index + i) > song_str_len)
		  temp[i] = song_name[i + song_index - (song_str_len + 1)];
	      else
		  temp[i] = song_name[i + song_index];
	    }
	  win_put_text_xy(temp, SONG_X, SONG_Y, SONG_MAX_WIDTH);
	  if (++song_index >= song_str_len)
	    song_index = 0;
	}
    }
  else
    {
      win_put_text_xy(song_name, SONG_X, SONG_Y, SONG_MAX_WIDTH);
    }
  */

  /*
  // put eq mode
  win_put_bmp_xy(EQ_X, EQ_Y, eq_slider);
  win_put_text_xy(str_eq[eq.mode], EQ_X + eq_slider.width, EQ_Y, MAX_COL - EQ_X + eq_slider.width);
  */
}

/**************************************************
  page_main_draw_rf_meters

  draw various RF meters: RSSI, Ant Diversity, etc
***************************************************/
static const unsigned char BT_X = 97;
static const unsigned char BT_Y_1 = 9;
static const unsigned char BT_Y_2 = 41;

static void page_main_draw_rf(void)
{
  win_set_inverse(INVERSE_OFF);
  win_set_transparent(TRANS_OFF);

  switch (s_rf_meter_1)
    {
    case 0:
      win_put_bmp_xy(BT_X, BT_Y_1, BTSignal0);
      break;
    case 1:
      win_put_bmp_xy(BT_X, BT_Y_1, BTSignal1);
      break;
    case 2:
      win_put_bmp_xy(BT_X, BT_Y_1, BTSignal2);
      break;
    case 3:
      win_put_bmp_xy(BT_X, BT_Y_1, BTSignal3);
      break;
    case 4:
      win_put_bmp_xy(BT_X, BT_Y_1, BTSignal4);
      break;
    case 5:
      win_put_bmp_xy(BT_X, BT_Y_1, BTSignal5);
      break;
    }
  switch (s_rf_meter_2)
    {
    case 0:
      win_put_bmp_xy(BT_X, BT_Y_2, BTSignal0);
      break;
    case 1:
      win_put_bmp_xy(BT_X, BT_Y_2, BTSignal1);
      break;
    case 2:
      win_put_bmp_xy(BT_X, BT_Y_2, BTSignal2);
      break;
    case 3:
      win_put_bmp_xy(BT_X, BT_Y_2, BTSignal3);
      break;
    case 4:
      win_put_bmp_xy(BT_X, BT_Y_2, BTSignal4);
      break;
    case 5:
      win_put_bmp_xy(BT_X, BT_Y_2, BTSignal5);
      break;
    }
  win_set_transparent(TRANS_ON);
}

/**************************************************
  page_main_draw_audio_meters

  draw audio meter boxes
***************************************************/
// audio meters
static const unsigned char LR_X = 0;
static const unsigned char L_1_Y = 0;
static const unsigned char R_1_Y = 8;
static const unsigned char L_2_Y = 32;
static const unsigned char R_2_Y = 40;
static const unsigned char AUD_X1 = LR_X + hp_l.width;
static const unsigned char AUD_X2 = AUD_X1 + AUD_METER_MAX;
static const unsigned char AUD_1_L_Y1 = L_1_Y;
static const unsigned char AUD_1_L_Y2 = L_1_Y + hp_l.height - 2;
static const unsigned char AUD_1_R_Y1 = R_1_Y;
static const unsigned char AUD_1_R_Y2 = R_1_Y + hp_r.height - 2;
static const unsigned char AUD_2_L_Y1 = L_2_Y;
static const unsigned char AUD_2_L_Y2 = L_2_Y + hp_l.height - 2;
static const unsigned char AUD_2_R_Y1 = R_2_Y;
static const unsigned char AUD_2_R_Y2 = R_2_Y + hp_r.height - 2;

void page_main_draw_audio(void)
{
  // draw icons
  win_put_bmp_xy(LR_X, L_1_Y, hp_l);
  win_put_bmp_xy(LR_X, R_1_Y, hp_r);
  win_put_bmp_xy(LR_X, L_2_Y, hp_l);
  win_put_bmp_xy(LR_X, R_2_Y, hp_r);

  // draw fake audio boxes
  win_put_box(AUD_X1, AUD_1_L_Y1, AUD_X1 + s_l_audio_meter_1, AUD_1_L_Y2);
  win_put_box(AUD_X1, AUD_1_R_Y1, AUD_X1 + s_r_audio_meter_1, AUD_1_R_Y2);
  win_put_box(AUD_X1, AUD_2_L_Y1, AUD_X1 + s_l_audio_meter_2, AUD_2_L_Y2);
  win_put_box(AUD_X1, AUD_2_R_Y1, AUD_X1 + s_r_audio_meter_2, AUD_2_R_Y2);
  // clear old meters
  win_set_inverse(INVERSE_ON);
  win_put_box(AUD_X1 + s_l_audio_meter_1 + 1, AUD_1_L_Y1, AUD_X2, AUD_1_L_Y2);
  win_put_box(AUD_X1 + s_r_audio_meter_1 + 1, AUD_1_R_Y1, AUD_X2, AUD_1_R_Y2);
  win_put_box(AUD_X1 + s_l_audio_meter_2 + 1, AUD_2_L_Y1, AUD_X2, AUD_2_L_Y2);
  win_put_box(AUD_X1 + s_r_audio_meter_2 + 1, AUD_2_R_Y1, AUD_X2, AUD_2_R_Y2);
  win_set_inverse(INVERSE_OFF);
}

/**************************************************
  page_main_draw_batt

  draw battery meter
***************************************************/
// battery meters
static const unsigned char BATT_TIP_X1 = 86;
static const unsigned char BATT_TIP_X2 = 90;
static const unsigned char BATT_X1 = 83;
static const unsigned char BATT_X2 = 93;
static const unsigned char BATT_1_TIP_Y1 = 0;
static const unsigned char BATT_1_TIP_Y2 = 2;
static const unsigned char BATT_1_TOP_Y1 = 4;
static const unsigned char BATT_1_TOP_Y2 = 9;
static const unsigned char BATT_1_MID_Y1 = 11;
static const unsigned char BATT_1_MID_Y2 = 16;
static const unsigned char BATT_1_BOT_Y1 = 18;
static const unsigned char BATT_1_BOT_Y2 = 23;
static const unsigned char BATT_2_TIP_Y1 = 32;
static const unsigned char BATT_2_TIP_Y2 = 34;
static const unsigned char BATT_2_TOP_Y1 = 36;
static const unsigned char BATT_2_TOP_Y2 = 41;
static const unsigned char BATT_2_MID_Y1 = 43;
static const unsigned char BATT_2_MID_Y2 = 48;
static const unsigned char BATT_2_BOT_Y1 = 50;
static const unsigned char BATT_2_BOT_Y2 = 55;

void page_main_draw_batt(void)
{
  // battery 1
  if (s_batt_meter_1 != s_prev_batt_meter_1)
    {
      // just the tip
      win_put_box_empty(BATT_TIP_X1, BATT_1_TIP_Y1, BATT_TIP_X2, BATT_1_TIP_Y2);

      // rest of meter
      if (s_batt_meter_1 > 0)
	{
	  win_put_box(BATT_X1, BATT_1_BOT_Y1, BATT_X2, BATT_1_BOT_Y2);
	}
      else
	{
	  // clear old meters
	  win_set_inverse(INVERSE_ON);
	  win_put_box(BATT_X1, BATT_1_BOT_Y1, BATT_X2, BATT_1_BOT_Y2);
	  win_set_inverse(INVERSE_OFF);
	  win_put_box_empty(BATT_X1, BATT_1_BOT_Y1, BATT_X2, BATT_1_BOT_Y2);
	}

      if (s_batt_meter_1 > 1)
	{
	  win_put_box(BATT_X1, BATT_1_MID_Y1, BATT_X2, BATT_1_MID_Y2);
	}
      else
	{
	  // clear old meters
	  win_set_inverse(INVERSE_ON);
	  win_put_box(BATT_X1, BATT_1_MID_Y1, BATT_X2, BATT_1_MID_Y2);
	  win_set_inverse(INVERSE_OFF);
	  win_put_box_empty(BATT_X1, BATT_1_MID_Y1, BATT_X2, BATT_1_MID_Y2);
	}

      if (s_batt_meter_1 > 2)
	{
	  win_put_box (BATT_X1, BATT_1_TOP_Y1, BATT_X2, BATT_1_TOP_Y2);
	}
      else
	{
	  // clear old meters
	  win_set_inverse(INVERSE_ON);
	  win_put_box(BATT_X1, BATT_1_TOP_Y1, BATT_X2, BATT_1_TOP_Y2);
	  win_set_inverse(INVERSE_OFF);
	  win_put_box_empty(BATT_X1, BATT_1_TOP_Y1, BATT_X2, BATT_1_TOP_Y2);
	}

      s_prev_batt_meter_1 = s_batt_meter_1;
    }
  // battery 2
  if (s_batt_meter_2 != s_prev_batt_meter_2)
    {
      // just the tip
      win_put_box_empty(BATT_TIP_X1, BATT_2_TIP_Y1, BATT_TIP_X2, BATT_2_TIP_Y2);

      // rest of meter
      if (s_batt_meter_2 > 0)
	{
	  win_put_box(BATT_X1, BATT_2_BOT_Y1, BATT_X2, BATT_2_BOT_Y2);
	}
      else
	{
	  // clear old meters
	  win_set_inverse(INVERSE_ON);
	  win_put_box(BATT_X1, BATT_2_BOT_Y1, BATT_X2, BATT_2_BOT_Y2);
	  win_set_inverse(INVERSE_OFF);
	  win_put_box_empty(BATT_X1, BATT_2_BOT_Y1, BATT_X2, BATT_2_BOT_Y2);
	}

      if (s_batt_meter_2 > 1)
	{
	  win_put_box(BATT_X1, BATT_2_MID_Y1, BATT_X2, BATT_2_MID_Y2);
	}
      else
	{
	  // clear old meters
	  win_set_inverse(INVERSE_ON);
	  win_put_box(BATT_X1, BATT_2_MID_Y1, BATT_X2, BATT_2_MID_Y2);
	  win_set_inverse(INVERSE_OFF);
	  win_put_box_empty(BATT_X1, BATT_2_MID_Y1, BATT_X2, BATT_2_MID_Y2);
	}

      if (s_batt_meter_2 > 2)
	{
	  win_put_box (BATT_X1, BATT_2_TOP_Y1, BATT_X2, BATT_2_TOP_Y2);
	}
      else
	{
	  // clear old meters
	  win_set_inverse(INVERSE_ON);
	  win_put_box(BATT_X1, BATT_2_TOP_Y1, BATT_X2, BATT_2_TOP_Y2);
	  win_set_inverse(INVERSE_OFF);
	  win_put_box_empty(BATT_X1, BATT_2_TOP_Y1, BATT_X2, BATT_2_TOP_Y2);
	}

      s_prev_batt_meter_2 = s_batt_meter_2;
    }
}

/**************************************************
  page_main_fake_meters

  update values for all the meters
***************************************************/
static const unsigned char RF_METER_MAX = 5; // 0 - 5
static const unsigned char BATT_METER_MAX = 3; // 0 - 3
static const unsigned char AUDIO_METER_DELTA = 40;
static const unsigned char RF_METER_DELTA = 5;

void page_main_fake_meters(void)
{
  static unsigned int s_count = 0;
  static unsigned int s_throttle = 0;
  unsigned int temp = 0;
  signed short offset;

  // audio L 1
  offset = (rand() % AUDIO_METER_DELTA) - (AUDIO_METER_DELTA / 2);
  offset = s_l_audio_meter_1 + offset;
  if (offset < 0)
    offset = 0;
  else if (offset > AUD_METER_MAX)
    offset = AUD_METER_MAX;
  s_l_audio_meter_1 = offset;
  srand(++s_count);
  // audio R 1
  offset = (rand() % AUDIO_METER_DELTA) - (AUDIO_METER_DELTA / 2);
  offset = s_r_audio_meter_1 + offset;
  if (offset < 0)
    offset = 0;
  else if (offset > AUD_METER_MAX)
    offset = AUD_METER_MAX;
  s_r_audio_meter_1 = offset;
  srand(++s_count);
  // audio L 2
  offset = (rand() % AUDIO_METER_DELTA) - (AUDIO_METER_DELTA / 2);
  offset = s_l_audio_meter_2 + offset;
  if (offset < 0)
    offset = 0;
  else if (offset > AUD_METER_MAX)
    offset = AUD_METER_MAX;
  s_l_audio_meter_2 = offset;
  srand(++s_count);
  // audio R 2
  offset = (rand() % AUDIO_METER_DELTA) - (AUDIO_METER_DELTA / 2);
  offset = s_r_audio_meter_2 + offset;
  if (offset < 0)
    offset = 0;
  else if (offset > AUD_METER_MAX)
    offset = AUD_METER_MAX;
  s_r_audio_meter_2 = offset;
  srand(++s_count);
  
  // rf meter, update less often
  if (!(s_throttle % 2))
    {
      offset = (rand() % 3) - 1;
      if ((offset < 0 && s_rf_meter_1 > 0) || offset > 0)
	s_rf_meter_1 += offset;
      if (s_rf_meter_1 > RF_METER_MAX)
	s_rf_meter_1 = RF_METER_MAX;
      srand(++s_count);
      offset = (rand() % 3) - 1;
      if ((offset < 0 && s_rf_meter_2 > 0) || offset > 0)
	s_rf_meter_2 += offset;
      if (s_rf_meter_2 > RF_METER_MAX)
	s_rf_meter_2 = RF_METER_MAX;
      srand(++s_count);
    }

  // battery meter, update very rarely.. decrease to zero, then repeat
  // 50ms page task, 10 min period for battery cycle,
  if (!(s_throttle % 1200))
    {
      s_batt_meter_1 -= 1;
      s_batt_meter_2 -= 1;
      if (s_batt_meter_1 > BATT_METER_MAX) //unsigned rollover
	s_batt_meter_1 = BATT_METER_MAX;
      if (s_batt_meter_2 > BATT_METER_MAX) //unsigned rollover
	s_batt_meter_2 = BATT_METER_MAX;
    }
  ++s_throttle;
}
