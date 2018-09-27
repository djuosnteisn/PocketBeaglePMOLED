#include <iostream>
#include <stdio.h>
#include "btns.h"

using namespace std;
using namespace exploringBB;

int btn_callback(int var)
{
  printf("BTN pressed\n");
  return 0;
}

BTNS::BTNS(int menu_pin, int back_pin, int up_pin, int dn_pin)
{
  BTN_EV event;

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

  menu->waitForEdge(&btn_callback);
  back->waitForEdge(&btn_callback);
  up->waitForEdge(&btn_callback);
  dn->waitForEdge(&btn_callback);
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
