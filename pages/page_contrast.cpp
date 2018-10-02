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

/*****************/
/*   functions   */
/*****************/

void page_contrast_on_active(void);
void page_contrast_on_refresh(void);
static void page_contrast_on_event(unsigned char btn);
static void page_contrast_draw_title(void);

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
  //clear the entire screen
  win_clear_screen();

  //update disp
  page_contrast_draw_title();
}

/**************************************************
  page_contrast_on_refresh

  handle refresh message for page
***************************************************/

void page_contrast_on_refresh(void)
{
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
      //NOTE make vol increase
      break;
    case BTN_DN:
      //NOTE make vol decrease
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
  page_contrast_draw_title

  draw title at top of page
***************************************************/
static const unsigned char TITLE_X = 64;
static const unsigned char TITLE_Y = 0;

static void page_contrast_draw_title(void)
{
  const char *str_title = "CONTRAST";
  unsigned int temp = win_get_str_len(str_title) / 2;

  win_put_text_xy(str_title, TITLE_X - temp, TITLE_Y, FRAME_WIDTH_PIX);
}
