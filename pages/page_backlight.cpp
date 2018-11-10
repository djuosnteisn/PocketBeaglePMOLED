/*
  page_backlight.c
*/

#include "page_backlight.h"
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

void page_backlight_on_active(void);
void page_backlight_on_refresh(void);
static void page_backlight_on_event(unsigned char btn);
static void page_backlight_draw_title(void);
static void page_backlight_draw_text(void);

/**************************************************
  page_backlight_proc

  message processor for page
***************************************************/

char page_backlight_proc(events ev, unsigned char btn)
{
    switch (ev)
    {
    case EV_PAGE_ACTIVE:
        page_backlight_on_active();
        return 1;
    case EV_REFRESH:
        page_backlight_on_refresh();
        return 1;
    case EV_BTN:
        page_backlight_on_event(btn);
        break;
    }
    return 0;
}

/**************************************************
  page_backlight_on_active

  handle activation message for page
***************************************************/

void page_backlight_on_active(void)
{
  // clear the entire screen
  win_clear_screen();

  // update variables
  s_prev_val = bl_mode;

  // update disp
  page_backlight_draw_title();
  page_backlight_draw_text();
}

/**************************************************
  page_backlight_on_refresh

  handle refresh message for page
***************************************************/

void page_backlight_on_refresh(void)
{
  if (s_prev_val != bl_mode)
    {
      page_backlight_draw_text();
      //update prev variables
      s_prev_val = bl_mode;
    }
}

/**************************************************
  page_backlight_on_event

***************************************************/

void page_backlight_on_event(unsigned char btn)
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
      if (++bl_mode > MAX_BACKLIGHT_MODE)
	bl_mode = MIN_BACKLIGHT_MODE;
      break;
    case BTN_DN:
      if (--bl_mode > MAX_BACKLIGHT_MODE) //unsigned rollover
	bl_mode = MAX_BACKLIGHT_MODE;
      break;
    }
}

/**************************************************
  page_backlight_draw_title

  draw title text
***************************************************/
static unsigned char TITLE_X = 64;
static unsigned char TITLE_Y = 8;

static void page_backlight_draw_title(void)
{
  const char *str_title = "LCD Timeout:";
  unsigned char temp;

  /* configure window parameters */
  win_set_transparent(TRANS_OFF);
  win_set_inverse(INVERSE_OFF);

  // draw title string
  temp = win_get_str_len(str_title);
  win_put_text_xy(str_title, TITLE_X - temp/2, TITLE_Y, temp);
}

/**************************************************
  page_backlight_draw_text

  draw backlight item text
***************************************************/
static const unsigned char VAL_X = 64;
static const unsigned char VAL_Y  = 40;

static void page_backlight_draw_text(void)
{
  //NOTE string array must match macros in main.h
  const char *str_val[] = {"Always On", "5 Minutes", "30 Seconds"};
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
  temp = win_get_str_len(str_val[bl_mode]);
  win_put_text_xy(str_val[bl_mode], VAL_X - temp/2, VAL_Y, temp);
}
