#ifndef BTNS_H
#define BTNS_H
#include "GPIO.h"

using namespace exploringBB;

#define BTN_CODE_MENU 1
#define BTN_CODE_BACK 2
#define BTN_CODE_UP 4
#define BTN_CODE_DN 8

typedef struct tagBTN_EV
{
  unsigned char event;
  unsigned int time;
} BTN_EV;

class BTNS
{
 private:
  GPIO *menu, *back, *up, *dn;
  BTN_EV ui_event;

 public:
  BTNS(int menu_pin, int back_pin, int up_pin, int dn_pin);
  ~BTNS(); //destructor

  BTN_EV get_event(void);
  int menu_press(int var);
  int back_press(int var);
  int up_press(int var);
  int dn_press(int var);
};

#endif //BTNS_H
