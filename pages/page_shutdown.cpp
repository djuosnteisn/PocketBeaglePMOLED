/*
  page_shutdown.c
*/

#include "page_shutdown.h"
#include "../page.h"
#include "../win.h"
#include "../btns.h"

/**************/
/*   macros   */
/**************/
// constants
#define NUM_SEL_ITEMS 2
#define SHUTDOWN_SEL_MIN 0
#define SHUTDOWN_SEL_MAX (NUM_SHUTDOWN_ITEMS - 1)
#define SHUTDOWN_SEL_YES 1
#define SHUTDOWN_SEL_NO 0

/**************/
/*   locals   */
/**************/

static signed char s_sel, s_prev_sel;

/*****************/
/*   functions   */
/*****************/

void page_shutdown_on_active(void);
void page_shutdown_on_refresh(void);
static void page_shutdown_on_event(unsigned char btn);
static void page_shutdown_draw_text(void);

/**************************************************
  page_shutdown_proc

  message processor for page
***************************************************/

char page_shutdown_proc(events ev, unsigned char btn)
{
    switch (ev)
    {
    case EV_PAGE_ACTIVE:
        page_shutdown_on_active();
        return 1;
    case EV_REFRESH:
        page_shutdown_on_refresh();
        return 1;
    case EV_BTN:
        page_shutdown_on_event(btn);
        break;
    }
    return 0;
}

/**************************************************
  page_shutdown_on_active

  handle activation message for page
***************************************************/

void page_shutdown_on_active(void)
{
  // clear the entire screen
  win_clear_screen();

  // update variables
  s_sel = SHUTDOWN_SEL_NO;

  // update disp
  page_shutdown_draw_text();
}

/**************************************************
  page_shutdown_on_refresh

  handle refresh message for page
***************************************************/

void page_shutdown_on_refresh(void)
{
  if (s_prev_sel != s_sel)
    {
      page_shutdown_draw_text();
      //update prev variables
      s_prev_sel = s_sel;
    }
}

/**************************************************
  page_shutdown_on_event

***************************************************/

void page_shutdown_on_event(unsigned char btn)
{
  switch (btn)
    {
      /* menu & back buttons */
    case BTN_MENU:
      // see if we're shutting down or not
      switch (s_sel)
	{
	case SHUTDOWN_SEL_YES:
	  printf("SHUTTING DOWN...\n");
	  // turn off peripherals
	  win_close();
	  // good bye!
	  system("shutdown -h now");
	  break;
	case SHUTDOWN_SEL_NO:
	  // return to menu page
	  page_show_page(PAGE_MENU);
	  break;
	}
      break;
    case BTN_BACK:
      // return to menu page
      page_show_page(PAGE_MENU);
      break;
      /* up/down buttons */
    case BTN_UP: // fall thru
    case BTN_DN:
      if (s_sel == SHUTDOWN_SEL_YES)
	s_sel = SHUTDOWN_SEL_NO;
      else
	s_sel = SHUTDOWN_SEL_YES;
      break;
    }
}

/**************************************************
  page_shutdown_draw_text

  draw shutdown item text
***************************************************/
static const unsigned char DESC_X = 64;
static const unsigned char DESC_Y = 8;
static const unsigned char YES_X  = 32;
static const unsigned char NO_X = 96;
static const unsigned char YES_NO_Y = 40;

static void page_shutdown_draw_text(void)
{
  const char *str_desc = "Shutdown?";
  const char *str_yes = "YES", *str_no = "NO";
  unsigned char temp;

  /* configure window parameters */
  win_set_transparent(TRANS_OFF);
  win_set_inverse(INVERSE_OFF);

  // draw desc string
  temp = win_get_str_len(str_desc);
  win_put_text_xy(str_desc, DESC_X - temp/2, DESC_Y, temp + 5);

  // draw yes and no, invert selection
  if (s_sel == SHUTDOWN_SEL_NO)
    win_set_inverse(INVERSE_ON);
  else
    win_set_inverse(INVERSE_OFF);
  temp = win_get_str_len(str_no);
  win_put_text_xy(str_no, NO_X - temp/2, YES_NO_Y, temp);

  if (s_sel == SHUTDOWN_SEL_YES)
    win_set_inverse(INVERSE_ON);
  else
    win_set_inverse(INVERSE_OFF);
  temp = win_get_str_len(str_yes);
  win_put_text_xy(str_yes, YES_X - temp/2, YES_NO_Y, temp);
}
