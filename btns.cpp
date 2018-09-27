#include <iostream>
#include <stdio.h>
#include "btns.h"

using namespace std;
using namespace exploringBB;

BTNS::BTNS(int menu_pin, int back_pin, int up_pin, int dn_pin)
{
  BTN_EV event;
  //NOTE temp
  printf("creating GPIOs\n");
  menu = new GPIO(menu_pin);
  back = new GPIO(back_pin);
  up = new GPIO(up_pin);
  dn = new GPIO(dn_pin);

  // menu->setDirection(INPUT);
  // back->setDirection(INPUT);
  // up->setDirection(INPUT);
  // dn->setDirection(INPUT);
  //NOTE temp
  printf("setting GPIOs to OUTPUTs\n");
  menu->setDirection(OUTPUT);
  printf("menu done\t");
  back->setDirection(OUTPUT);
  printf("back done\t");
  up->setDirection(OUTPUT);
  printf("up done\t");
  dn->setDirection(OUTPUT);
  printf("down done\n");
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

void BTNS::set_state(unsigned char state)
{
  //NOTE temp
  printf("Setting GPIO state to %d\n", state);
  if (state)
    {
      menu->setValue(HIGH);
      back->setValue(HIGH);
      up->setValue(HIGH);
      dn->setValue(HIGH);
    }
  else
    {
      menu->setValue(LOW);
      back->setValue(LOW);
      up->setValue(LOW);
      dn->setValue(LOW);
    }
}
