#include <iostream>
#include <stdio.h>
#include "btns.h"
#include "gpio_setup.h"

using namespace std;
using namespace exploringBB;


//NOTE
/* bypassing DTOs by writing GPIO configuration
   memory directly
*/

int print_btn_pressed(int var)
{
  printf("Blafuck");
}

BTNS::BTNS(int menu_pin, int back_pin, int up_pin, int dn_pin)
{
  /* first call our memmap version of pin config for each pin,
     and make them all GPIO with pull-ups enabled, val: 0x37.
     Menu Pin = GPIO 89, addr = 0x44e10964
     Back Pin = GPIO 60, addr = 0x44e108f0
     Up Pin = GPIO 52, addr = 0x44e108d0
     Dn Pin = GPIO 23, addr = 0x44e1085c
  */
  conf_pinmux(0x44e10964, 0x0);
  conf_pinmux(0x44e108f0, 0x0);
  conf_pinmux(0x44e108d0, 0x0);
  conf_pinmux(0x44e1085c, 0x0);
  
  BTN_EV event;
  printf("attempting to create button gpios\n");
  menu = new GPIO(menu_pin);
  printf("menu created\n");
  back = new GPIO(back_pin);
  printf("back created\n");
  up = new GPIO(up_pin);
  printf("up created\n");
  dn = new GPIO(dn_pin);
  printf("dn created\n");

  /* buttons are inputs */
  menu->setDirection(INPUT);
  printf("menu direction set\n");
  back->setDirection(INPUT);
  printf("back direction set\n");
  up->setDirection(INPUT);
  printf("up direction set\n");
  dn->setDirection(INPUT);
  printf("dn direction set\n");
  /* edge-type = falling */
  menu->setEdgeType(FALLING);
  printf("menu edge set\n");
  back->setEdgeType(FALLING);
  printf("back edge set\n");
  up->setEdgeType(FALLING);
  printf("up edge set\n");
  dn->setEdgeType(FALLING);
  printf("dn edge set\n");
  /* wait for presses */
  menu->waitForEdge(print_btn_pressed);
  printf("menu wait set\n");
  back->waitForEdge(print_btn_pressed);
  printf("back wait set\n");
  up->waitForEdge(print_btn_pressed);
  printf("up wait set\n");
  dn->waitForEdge(print_btn_pressed);
  printf("dn wait set\n");
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
  BTN_EV ret = ui_event;
  //reset our event after a press
  ui_event.event = 0;
  //return the event
  return ret;
}
