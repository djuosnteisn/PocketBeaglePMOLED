/*
  page_contrast.c
*/

#include "page_contrast.h"
#include "../page.h"
#include "../win.h"
#include "../btns.h"

/**************/
/*   macros   */
/**************/

/**************/
/*   locals   */
/**************/

signed short s_con_lvl, s_prev_con_lvl;

/*****************/
/*   functions   */
/*****************/

void page_contrast_on_active(void);
void page_contrast_on_refresh(void);
static void page_contrast_on_event(unsigned char btn);
static void page_contrast_draw_text(void);
static void page_contrast_draw_bmp(void);

/**************************************************
  page_contrast_proc

  message processor for page
***************************************************/

char page_contrast_proc(events ev, unsigned char btn)
{
    switch (ev)
    {
    case EV_PAGE_ACTIVE:
        page_contrast_on_active();
        return 1;
    case EV_REFRESH:
        page_contrast_on_refresh();
        return 1;
    case EV_BTN:
        page_contrast_on_event(btn);
        break;
    }
    return 0;
}

/**************************************************
  page_contrast_on_active

  handle activation message for page
***************************************************/

void page_contrast_on_active(void)
{
  // clear the entire screen
  win_clear_screen();

  // update variables
  s_prev_con_lvl = s_con_lvl = win_get_contrast();
  // update disp
  page_contrast_draw_text();
  page_contrast_draw_bmp();
}

/**************************************************
  page_contrast_on_refresh

  handle refresh message for page
***************************************************/

void page_contrast_on_refresh(void)
{
  if (s_prev_con_lvl != s_con_lvl)
    {
      page_contrast_draw_text();
      // update prev variables
      s_prev_con_lvl = s_con_lvl;
    }
}

/**************************************************
  page_contrast_on_event

***************************************************/

void page_contrast_on_event(unsigned char btn)
{
  switch (btn)
    {
      /* up/down buttons */
    case BTN_UP:
      s_con_lvl += 10;
      if (s_con_lvl > MAX_CONTRAST)
	s_con_lvl = MAX_CONTRAST;
      win_set_contrast(s_con_lvl);
      break;
    case BTN_DN:
      s_con_lvl -= 10;
      if (s_con_lvl < MIN_CONTRAST)
	s_con_lvl = MIN_CONTRAST;
      win_set_contrast(s_con_lvl);
      break;
      /* menu & back buttons */
    case BTN_MENU:
      break;
    case BTN_BACK:
      page_show_page(PAGE_MENU);
      break;
    }
}

/**************************************************
  page_contrast_draw_text

  draw text at top of page
***************************************************/
static const unsigned char TEXT_X = 96;
static const unsigned char LVL_Y = 16;
static const unsigned char CON_Y = 32;
static const unsigned char CLEAR_X1 = TEXT_X - 30;
static const unsigned char CLEAR_X2 = TEXT_X + 30;

static void page_contrast_draw_text(void)
{
  const char *str_text = "LVL:";
  char lvl_buf[4];
  unsigned int temp;

  // "LVL:" text
  temp = win_get_str_len(str_text);
  win_put_text_xy(str_text, TEXT_X - (temp/2), LVL_Y, temp+10);
  // contrast value
  // clear region
  win_set_inverse(INVERSE_ON);
  win_put_box(CLEAR_X1, CON_Y, CLEAR_X2, CON_Y + win_get_font_height());
  win_set_inverse(INVERSE_OFF);
  if (s_con_lvl == 0)
    snprintf(lvl_buf, sizeof(lvl_buf), "MIN");
  else if (s_con_lvl == 0xFF)
    snprintf(lvl_buf, sizeof(lvl_buf), "MAX");
  else
    snprintf(lvl_buf, sizeof(lvl_buf), "%d", s_con_lvl);
  temp = win_get_str_len(lvl_buf);
  win_put_text_xy(lvl_buf, TEXT_X - (temp/2), CON_Y, temp+10);
}

/**************************************************
  page_contrast_draw_bmp

  draw bitmap image
***************************************************/
static const unsigned char BMP_X = 0;
static const unsigned char BMP_Y = 0;

static void page_contrast_draw_bmp(void)
{
  win_put_bmp_xy(BMP_X, BMP_Y, sc_circle);
}
