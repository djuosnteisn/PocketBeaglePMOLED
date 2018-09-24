#ifndef PAGE_H
#define PAGE_H

// button macros
#define PIN_MENU 55 //NOTE wrong value
#define PIN_BACK 55 //NOTE wrong value
#define PIN_UP 55 //NOTE wrong value
#define PIN_DN 55 //NOTE wrong value

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
    PAGE_EQ,
    PAGE_ABOUT
  };

/* get a btns handle and init page stuff */
void page_init(void);

/* handle user interface interactions */
void page_task(void);

/* change pages */
void page_show_page(pages page);

/* handle various events */
void page_on_event(pages page, events ev);

#endif // PAGE_H
