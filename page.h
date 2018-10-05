#ifndef PAGE_H
#define PAGE_H

// button macros
/* bit  meaning
   6    slewctrl (0:Fast, 1:Slow
   5    rxactive (0:rx disabled, 1:enabled)
   4    pullupdn (0:pull down, 1:pull up)
   3    pullen   (0:enabled, 1:disabled)
   2-0  mux mode (0 thru 7, see table)
*/
#define PIN_MENU 89 //def mode: 0x00000034
#define PIN_BACK 60 //def mode: 0x00000030
#define PIN_UP 52 //def mode: 0x0x0000002F
#define PIN_DN 23 //def mode: 0x00000007

// event types
enum events
  {
    EV_BTN,
    EV_REFRESH,
    EV_PAGE_ACTIVE,
  };

// pages
enum pages
  {
    PAGE_MAIN,
    PAGE_MENU,
    // menu pages
    PAGE_EQ,
    PAGE_EQ_SETUP,
    PAGE_VOL,
    PAGE_CONTRAST,
    PAGE_ABOUT,
    PAGE_SHUTDOWN,
  };


/* get a btns handle and init page stuff */
void page_init(void);

/* handle user interface interactions */
void page_task(void);

/* change pages */
void page_show_page(pages page);

/* handle various events */
void page_on_event(pages page, events ev, unsigned char btn);

#endif // PAGE_H
