/*
  page_menu.c
*/

#include "page_menu.h"
#include "../page.h"
#include "../win.h"
#include "../btns.h"

/**************/
/*   macros   */
/**************/
// menu constants
#define NUM_MENU_ITEMS 5
#define MENU_SEL_MIN 0
#define MENU_SEL_MAX (NUM_MENU_ITEMS - 1)
// cursor constants
#define NUM_TEXT_ROWS 4
#define CURSOR_MIN 0
#define CURSOR_MAX (NUM_TEXT_ROWS - 1)

/**************/
/*   locals   */
/**************/

static const struct tag_s_menu
{
  const char *str;
  pages page;
} s_menu[NUM_MENU_ITEMS] =
  {
    {"EQ SETUP", PAGE_EQ},
    {"VOLUME", PAGE_MAIN},
    {"CONTRAST", PAGE_CONTRAST},
    {"TEST PAGE", PAGE_MAIN},
    {"ABOUT", PAGE_ABOUT}
  };
static signed char s_sel, s_prev_sel;
static signed char s_cursor, s_offset;

/*****************/
/*   functions   */
/*****************/

void page_menu_on_active(void);
void page_menu_on_refresh(void);
static void page_menu_on_event(unsigned char btn);
static void page_menu_draw_text(void);
static void page_menu_draw_scroll(void);

/**************************************************
  page_menu_proc

  message processor for page
***************************************************/

char page_menu_proc(events ev, unsigned char btn)
{
    switch (ev)
    {
    case EV_PAGE_ACTIVE:
        page_menu_on_active();
        return 1;
    case EV_REFRESH:
        page_menu_on_refresh();
        return 1;
    case EV_BTN:
        page_menu_on_event(btn);
        break;
    }
    return 0;
}

/**************************************************
  page_menu_on_active

  handle activation message for page
***************************************************/

void page_menu_on_active(void)
{
  //clear the entire screen
  win_clear_screen();

  //update disp
  page_menu_draw_text();
  page_menu_draw_scroll();
}

/**************************************************
  page_menu_on_refresh

  handle refresh message for page
***************************************************/

void page_menu_on_refresh(void)
{
  if (s_prev_sel != s_sel)
    {
      page_menu_draw_text();
      page_menu_draw_scroll();
      //update prev variables
      s_prev_sel = s_sel;
    }
}

/**************************************************
  page_menu_on_event

***************************************************/

void page_menu_on_event(unsigned char btn)
{
  switch (btn)
    {
      /* menu & back buttons */
    case BTN_MENU:
      // show selected page
      page_show_page(s_menu[s_sel].page);
      break;
    case BTN_BACK:
      // return to main page
      page_show_page(PAGE_MAIN);
      break;
      /* up/down buttons */
    case BTN_UP:
      if (s_sel > MENU_SEL_MIN)
	{
	  --s_sel;
	  if (s_cursor > CURSOR_MIN)
	    --s_cursor;
	  s_offset = s_sel - s_cursor;
	}
      break;
    case BTN_DN:
      if (s_sel < MENU_SEL_MAX)
	{
	  ++s_sel;
	  if (s_cursor < CURSOR_MAX)
	    ++s_cursor;
	  s_offset = s_sel - s_cursor;
	}
      break;
    }
}

/**************************************************
  page_menu_draw_text

  draw menu item text
***************************************************/
static const unsigned char TEXT_X = 0;
static const unsigned char TEXT_Y_TOP = 0;
static const unsigned char TEXT_Y_SPACING = 16;

static void page_menu_draw_text(void)
{
  unsigned char i;
  unsigned char y;

  /* configure window parameters */
  win_set_invert(INVERSE_OFF);
  win_set_transparent(TRANS_OFF);

  for (i = 0; i < NUM_TEXT_ROWS; i++)
    {
      y = TEXT_Y_TOP + (i * TEXT_Y_SPACING);
      // clear the row
      win_set_invert(INVERSE_ON);
      win_put_box(TEXT_X, y, MAX_COL, y + TEXT_Y_SPACING - 1);
      // invert selected item
      if (i == s_sel - s_offset)
	win_set_invert(INVERSE_ON);
      else
	win_set_invert(INVERSE_OFF);
      win_put_text_xy(s_menu[i + s_offset].str, TEXT_X, y, FRAME_WIDTH_PIX);
    }
}

/**************************************************
  page_menu_draw_scroll

  draw scroll bar on the right
***************************************************/
static const unsigned char SCROLL_STOP_X1 = 110;
static const unsigned char SCROLL_STOP_X2 = 115;
static const unsigned char SCROLL_STOP_TOP_Y1 = 0;
static const unsigned char SCROLL_STOP_TOP_Y2 = 2;
static const unsigned char SCROLL_STOP_BOT_Y1 = 60;

static void page_menu_draw_scroll(void)
{
  /* configure window parameters */
  win_set_invert(INVERSE_OFF);
  win_set_transparent(TRANS_ON);
}
