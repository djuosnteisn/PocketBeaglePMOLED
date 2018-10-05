/*
  page_eq.c
*/

#include "page_eq.h"
#include "../main.h"
#include "../page.h"
#include "../win.h"
#include "../btns.h"

/**************/
/*   macros   */
/**************/

/**************/
/*   locals   */
/**************/

static unsigned char s_prev_val;

/*****************/
/*   functions   */
/*****************/

void page_eq_on_active(void);
void page_eq_on_refresh(void);
static void page_eq_on_event(unsigned char btn);
static void page_eq_draw_title(void);
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
  // clear the entire screen
  win_clear_screen();

  // update variables
  s_prev_val = eq.mode;

  // update disp
  page_eq_draw_title();
  page_eq_draw_text();
}

/**************************************************
  page_eq_on_refresh

  handle refresh message for page
***************************************************/

void page_eq_on_refresh(void)
{
  if (s_prev_val != eq.mode)
    {
      page_eq_draw_text();
      //update prev variables
      s_prev_val = eq.mode;
    }
}

/**************************************************
  page_eq_on_event

***************************************************/

void page_eq_on_event(unsigned char btn)
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
      if (++eq.mode > MAX_EQ_MODE)
	eq.mode = MIN_EQ_MODE;
      break;
    case BTN_DN:
      if (--eq.mode > MAX_EQ_MODE) //unsigned rollover
	eq.mode = MAX_EQ_MODE;
      break;
    }
}

/**************************************************
  page_eq_draw_title

  draw title text
***************************************************/
static unsigned char TITLE_X = 64;
static unsigned char TITLE_Y = 8;

static void page_eq_draw_title(void)
{
  const char *str_title = "EQ Mode:";
  unsigned char temp;

  /* configure window parameters */
  win_set_transparent(TRANS_OFF);
  win_set_inverse(INVERSE_OFF);

  // draw title string
  temp = win_get_str_len(str_title);
  win_put_text_xy(str_title, TITLE_X - temp/2, TITLE_Y, temp);
}

/**************************************************
  page_eq_draw_text

  draw eq item text
***************************************************/
static const unsigned char VAL_X = 64;
static const unsigned char VAL_Y  = 40;

static void page_eq_draw_text(void)
{
  //NOTE string array must match enum definition in main.h
  const char *str_val[] =
    {"Flat Mode", "Party Mode", "High Mode", "Low Mode", "Rock Mode", "Surround", "Custom"};
  unsigned char temp;

  /* configure window parameters */
  win_set_transparent(TRANS_OFF);
  win_set_inverse(INVERSE_OFF);

  // clear previous text
  win_set_inverse(INVERSE_ON);
  temp = win_get_str_len(str_val[s_prev_val]);
  win_put_box(VAL_X - temp/2 - 1, VAL_Y, VAL_X + temp/2 +1, VAL_Y + 15);

  // draw value text inverted
  win_set_inverse(INVERSE_ON);
  temp = win_get_str_len(str_val[eq.mode]);
  win_put_text_xy(str_val[eq.mode], VAL_X - temp/2, VAL_Y, temp);
}
