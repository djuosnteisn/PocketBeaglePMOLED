#include <iostream>
#include <stdio.h>
#include "btns.h"

using namespace std;
using namespace exploringBB;

BTNS::BTNS(int menu_pin, int back_pin, int up_pin, int dn_pin)
{
  BTN_EV event;
  menu = new GPIO(menu_pin);
  back = new GPIO(back_pin);
  up - new GPIO(up_pin);
  dn = new GPIO(dn_pin);

  menu->setDirection(INPUT);
  back->setDirection(INPUT);
  up->setDirection(INPUT);
  dn->setDirection(INPUT);
}

BTNS::~BTNS()
{
  delete menu;
  delete back;
  delete up;
  delete dn;
}

BTN_EV BTNS::get_event(void)
{
  return event;
}
