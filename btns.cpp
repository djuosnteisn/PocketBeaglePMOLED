#include <iostream>
#include <stdio.h>
#include "btns.h"
#include "main.h"

using namespace std;
using namespace exploringBB;

static BTN_EV s_event;
static const unsigned int DEBOUNCE = 25; // refrenced to 5ms, 1/8 second

int btn_callback_menu(int var)
{
  static unsigned int debounce = 0;

  if ((tick - debounce) > DEBOUNCE)
    {
      s_event.btn |= BTN_MENU;
      debounce = tick;
    }
  return 0;
}

int btn_callback_back(int var)
{
  static unsigned int debounce = 0;

  if ((tick - debounce) > DEBOUNCE)
    {
      s_event.btn |= BTN_BACK;
      debounce = tick;
    }
  return 0;
}

int btn_callback_up(int var)
{
  static unsigned int debounce = 0;

  if ((tick - debounce) > DEBOUNCE)
    {
      s_event.btn |= BTN_UP;
      debounce = tick;
    }
  return 0;
}

int btn_callback_dn(int var)
{
  static unsigned int debounce = 0;

  if ((tick - debounce) > DEBOUNCE)
    {
      s_event.btn |= BTN_DN;
      debounce = tick;
    }
  return 0;
}

BTNS::BTNS(int menu_pin, int back_pin, int up_pin, int dn_pin)
{
  /* call linux config-pin utility to set
     our button pins as inputs with pull-ups
     enabled
  */
  system("config-pin P2_08 gpio_pu");
  system("config-pin P1_04 gpio_pu");
  system("config-pin P2_10 gpio_pu");
  system("config-pin P2_03 gpio_pu");
  
  /* assign GPIO objects to each pin */
  menu = new GPIO(menu_pin);
  back = new GPIO(back_pin);
  up = new GPIO(up_pin);
  dn = new GPIO(dn_pin);

  /* set directions, edge triggers and callbacks */
  menu->setDirection(INPUT);
  back->setDirection(INPUT);
  up->setDirection(INPUT);
  dn->setDirection(INPUT);

  menu->setEdgeType(FALLING);
  back->setEdgeType(FALLING);
  up->setEdgeType(FALLING);
  dn->setEdgeType(FALLING);

  menu->waitForEdge(&btn_callback_menu);
  back->waitForEdge(&btn_callback_back);
  up->waitForEdge(&btn_callback_up);
  dn->waitForEdge(&btn_callback_dn);
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
  return s_event;
}

void BTNS::reset_event(void)
{
  s_event.btn = 0;
}
