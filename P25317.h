#ifndef P25317_H_
#define P25317_H_

#include "GPIO.h"

class P25317
{
 private:
  GPIO* reset_pin;
  int dog;
 public:

  P25317(int pin); //constructor
  void init_display(void);
  void toggle_10_x(void);

  ~P25317(); //destructor
}
