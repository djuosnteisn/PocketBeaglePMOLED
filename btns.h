#ifndef BTNS_H
#define BTNS_H
#include "GPIO.h"

using namespace exploringBB;

#define BTN_EV_MENU 1
#define BTN_EV_BACK 2
#define BTN_EV_UP 4
#define BTN_EV_DN 8

typedef struct tagBTN_EV
{
  unsigned char event;
  unsigned int time;
} BTN_EV;

class BTNS
{
 private:
  GPIO *menu, *back, *up, *dn;

 public:
  BTNS(int menu_pin, int back_pin, int up_pin, int dn_pin);
  ~BTNS(); //destructor

  BTN_EV get_event(void);
  void reset_event(void);
};

#endif //BTNS_H
