/*
  page_compat_setup.c
*/

#include "page_compat.h"
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

void page_compat_setup_on_active(void);
void page_compat_setup_on_refresh(void);
static void page_compat_setup_on_event(unsigned char btn);
static void page_compat_setup_draw_title(void);
static void page_compat_setup_draw_text(void);

/**************************************************
  page_compat_setup_proc

  message processor for page
***************************************************/

char page_compat_setup_proc(events ev, unsigned char btn)
{
    switch (ev)
    {
    case EV_PAGE_ACTIVE:
        page_compat_setup_on_active();
        return 1;
    case EV_REFRESH:
        page_compat_setup_on_refresh();
        return 1;
    case EV_BTN:
        page_compat_setup_on_event(btn);
        break;
    }
    return 0;
}

/**************************************************
  page_compat_setup_on_active

  handle activation message for page
***************************************************/

void page_compat_setup_on_active(void)
{
  // clear the entire screen
  win_clear_screen();

  // update variables
  s_prev_val = compat_mode;

  // update disp
  page_compat_setup_draw_title();
  page_compat_setup_draw_text();
}

/**************************************************
  page_compat_setup_on_refresh

  handle refresh message for page
***************************************************/

void page_compat_setup_on_refresh(void)
{
  if (s_prev_val != compat_mode)
    {
      page_compat_setup_draw_text();
      //update prev variables
      s_prev_val = compat_mode;
    }
}

/**************************************************
  page_compat_setup_on_event

***************************************************/

void page_compat_setup_on_event(unsigned char btn)
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
      if (++compat_mode > MAX_COMPAT_MODE)
	compat_mode = MIN_COMPAT_MODE;
      break;
    case BTN_DN:
      if (--compat_mode > MAX_COMPAT_MODE) //unsigned rollover
	compat_mode = MAX_COMPAT_MODE;
      break;
    }
}

/**************************************************
  page_compat_setup_draw_title

  draw title text
***************************************************/
static unsigned char TITLE_X = 64;
static unsigned char TITLE_Y = 0;
static unsigned char TITLE_Y2 = 16;

static void page_compat_setup_draw_title(void)
{
  const char *str_title = "Compatibility";
  const char *str_title2 = "Mode";
  unsigned char temp;

  /* configure window parameters */
  win_set_transparent(TRANS_OFF);
  win_set_inverse(INVERSE_OFF);

  // draw title string
  temp = win_get_str_len(str_title);
  win_put_text_xy(str_title, TITLE_X - temp/2, TITLE_Y, temp);
  temp = win_get_str_len(str_title2);
  win_put_text_xy(str_title2, TITLE_X - temp/2, TITLE_Y2, temp);
}

/**************************************************
  page_compat_setup_draw_text

  draw compat_setup item text
***************************************************/
static const unsigned char VAL_X = 64;
static const unsigned char VAL_Y  = 40;

static void page_compat_setup_draw_text(void)
{
  //NOTE string array must match definition in main.h
  const char *str_val[] =
    {"DSW", "DCH", "DUET", "HYBRID"};
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
  temp = win_get_str_len(str_val[compat_mode]);
  win_put_text_xy(str_val[compat_mode], VAL_X - temp/2, VAL_Y, temp);
}
