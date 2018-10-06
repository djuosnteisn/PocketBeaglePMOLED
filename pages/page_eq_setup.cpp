/*
  page_eq_setup.c
*/

#include "page_eq_setup.h"
#include "../main.h"
#include "../page.h"
#include "../win.h"
#include "../btns.h"

/**************/
/*   macros   */
/**************/
// constants
#define NUM_SEL_ITEMS 3
#define EQ_SETUP_SEL_MIN 0
#define EQ_SETUP_SEL_MAX (NUM_SEL_ITEMS - 1)
#define EQ_SETUP_SEL_BASS 0
#define EQ_SETUP_SEL_MID 1
#define EQ_SETUP_SEL_TREB 2

/**************/
/*   locals   */
/**************/

static signed char s_sel, s_prev_sel;
static signed char s_prev_bass, s_prev_mid, s_prev_treb;

/*****************/
/*   functions   */
/*****************/

void page_eq_setup_on_active(void);
void page_eq_setup_on_refresh(void);
static void page_eq_setup_on_event(unsigned char btn);
static void page_eq_setup_draw_text(void);

/**************************************************
  page_eq_setup_proc

  message processor for page
***************************************************/

char page_eq_setup_proc(events ev, unsigned char btn)
{
    switch (ev)
    {
    case EV_PAGE_ACTIVE:
        page_eq_setup_on_active();
        return 1;
    case EV_REFRESH:
        page_eq_setup_on_refresh();
        return 1;
    case EV_BTN:
        page_eq_setup_on_event(btn);
        break;
    }
    return 0;
}

/**************************************************
  page_eq_setup_on_active

  handle activation message for page
***************************************************/

void page_eq_setup_on_active(void)
{
  // clear the entire screen
  win_clear_screen();

  // update variables
  s_prev_bass = eq.bass_lvl + 1;
  s_prev_mid = eq.mid_lvl + 1;
  s_prev_treb = eq.treb_lvl + 1;
  s_prev_sel = s_sel;

  // update disp
  page_eq_setup_draw_text();
}

/**************************************************
  page_eq_setup_on_refresh

  handle refresh message for page
***************************************************/

void page_eq_setup_on_refresh(void)
{
  if (s_prev_sel != s_sel || s_prev_bass != eq.bass_lvl ||
      s_prev_mid != eq.mid_lvl || s_prev_treb != eq.treb_lvl)
    {
      page_eq_setup_draw_text();

      //update prev variables
      s_prev_sel = s_sel;
      s_prev_bass = eq.bass_lvl;
      s_prev_mid = eq.mid_lvl;
      s_prev_treb = eq.treb_lvl;
    }
}

/**************************************************
  page_eq_setup_on_event

***************************************************/

void page_eq_setup_on_event(unsigned char btn)
{
  switch (btn)
    {
      /* menu & back buttons */
    case BTN_MENU:
      ++s_sel;
      if (s_sel > EQ_SETUP_SEL_MAX)
	s_sel = EQ_SETUP_SEL_MIN;
      break;
    case BTN_BACK:
      // return to menu page
      page_show_page(PAGE_MENU);
      break;
      /* up/down buttons */
    case BTN_UP:
      switch (s_sel)
	{
	case EQ_SETUP_SEL_BASS:
	  ++eq.bass_lvl;
	  if (eq.bass_lvl > MAX_LVL)
	    eq.bass_lvl = MAX_LVL;
	  break;
	case EQ_SETUP_SEL_MID:
	  ++eq.mid_lvl;
	  if (eq.mid_lvl > MAX_LVL)
	    eq.mid_lvl = MAX_LVL;
	  break;
	case EQ_SETUP_SEL_TREB:
	  ++eq.treb_lvl;
	  if (eq.treb_lvl > MAX_LVL)
	    eq.treb_lvl = MAX_LVL;
	  break;
	}
      break;
    case BTN_DN:
      switch (s_sel)
	{
	case EQ_SETUP_SEL_BASS:
	  --eq.bass_lvl;
	  if (eq.bass_lvl < MIN_LVL)
	    eq.bass_lvl = MIN_LVL;
	  break;
	case EQ_SETUP_SEL_MID:
	  --eq.mid_lvl;
	  if (eq.mid_lvl < MIN_LVL)
	    eq.mid_lvl = MIN_LVL;
	  break;
	case EQ_SETUP_SEL_TREB:
	  --eq.treb_lvl;
	  if (eq.treb_lvl < MIN_LVL)
	    eq.treb_lvl = MIN_LVL;
	  break;
	}
      break;
    }
}

/**************************************************
  page_eq_setup_draw_text

  draw eq_setup item text
***************************************************/
static const unsigned char PARAM_X = 0;
static const unsigned char BASS_Y = 8;
static const unsigned char MID_Y = 24;
static const unsigned char TREB_Y = 40;
static const unsigned char CLEAR_WIDTH = 40;

static void page_eq_setup_draw_text(void)
{
  const char *str_bass = "BASS:";
  const char *str_mid = "MID:";
  const char *str_treb = "TREB:";
  char temp_buf[4];
  
  unsigned char temp;

  /* configure window parameters */
  win_set_transparent(TRANS_OFF);
  win_set_inverse(INVERSE_OFF);

  // draw param strings
  win_put_text_xy(str_bass, PARAM_X, BASS_Y, win_get_str_len(str_bass));
  win_put_text_xy(str_mid, PARAM_X, MID_Y, win_get_str_len(str_mid));
  win_put_text_xy(str_treb, PARAM_X, TREB_Y, win_get_str_len(str_treb));

  // draw parameter values, invert selection
  // clear prev text
  win_set_inverse(INVERSE_ON);
  temp = win_get_str_len(str_bass);
  win_put_box(PARAM_X + temp, BASS_Y, PARAM_X + temp + CLEAR_WIDTH, BASS_Y + 15);
  temp = win_get_str_len(str_mid);
  win_put_box(PARAM_X + temp, MID_Y, PARAM_X + temp + CLEAR_WIDTH, MID_Y + 15);
  temp = win_get_str_len(str_treb);
  win_put_box(PARAM_X + temp, TREB_Y, PARAM_X + temp + CLEAR_WIDTH, TREB_Y + 15);

  // draw new text
  if (s_sel == EQ_SETUP_SEL_BASS)
    win_set_inverse(INVERSE_ON);
  else
    win_set_inverse(INVERSE_OFF);
  temp = win_get_str_len(str_bass);
  snprintf(temp_buf, sizeof(temp_buf), "%+02d", eq.bass_lvl);
  win_put_text_xy(temp_buf, PARAM_X + temp, BASS_Y, win_get_str_len(temp_buf));

  if (s_sel == EQ_SETUP_SEL_MID)
    win_set_inverse(INVERSE_ON);
  else
    win_set_inverse(INVERSE_OFF);
  temp = win_get_str_len(str_mid);
  snprintf(temp_buf, sizeof(temp_buf), "%+02d", eq.mid_lvl);
  win_put_text_xy(temp_buf, PARAM_X + temp, MID_Y, win_get_str_len(temp_buf));

  if (s_sel == EQ_SETUP_SEL_TREB)
    win_set_inverse(INVERSE_ON);
  else
    win_set_inverse(INVERSE_OFF);
  temp = win_get_str_len(str_treb);
  snprintf(temp_buf, sizeof(temp_buf), "%+02d", eq.treb_lvl);
  win_put_text_xy(temp_buf, PARAM_X + temp, TREB_Y, win_get_str_len(temp_buf));
}
