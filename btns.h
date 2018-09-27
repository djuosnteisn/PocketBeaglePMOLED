#ifndef BTNS_H
#define BTNS_H
#include "GPIO.h"

using namespace exploringBB;

typedef struct tagBTN_EV
{
  unsigned char event;
  unsigned int time;
} BTN_EV;

class BTNS
{
 private:
  GPIO *menu, *back, *up, *dn;
  BTN_EV event;

 public:
  BTNS(int menu_pin, int back_pin, int up_pin, int dn_pin);
  ~BTNS(); //destructor

  BTN_EV get_event(void);
  //NOTE diag tests
  void set_state(unsigned char state);
};

#endif //BTNS_H
