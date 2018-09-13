#include <iostream>
#include <unistd.h> // required for usleep
#include "GPIO.h"

using namespace std;
using namespace exploringBB;

P25317::P25317(int pin)
{
  reset_pin = new GPIO(pin);
  dog = 0;
}

P25317::init_display(void)
{
  reset_pin->setDirection(OUTPUT);
  // here we should keep the reset asserted
  enable_display(0);
}

P25317::enable_display(int en)
{
  if (en)
    {
      //deassert reset line
      reset_pin->setValue(HIGH);
    }
  else
    {
      //assert reset line
      reset_pin->setValue(LOW);
    }
}

P25317::toggle_10_x(void)
{
  for (int i = 0; i < 10; i++)
    {
      enable_display(1);
      usleep(500000);
      enable_display(0);
      usleep(500000);
    }
}

P25317::~P25317()
{
  delete reset_pin;
}

int main()
{
  //  GPIO outGPIO(49), inGPIO(115);
  P25317 my_disp(59);

  my_disp.toggle_10_x();

  /*
  // basic output example
  outGPIO.setDirection(OUTPUT);
  for (int i = 0; i < 10; i++)
    {
      outGPIO.setValue(HIGH);
      usleep(500000);
      outGPIO.setValue(LOW);
      usleep(500000);
    }

  // basic input example
  inGPIO.setDirection(INPUT);
  cout << "The Value of the input is: "<< inGPIO.getValue() << endl;

  // fast write 100 times
  outGPIO.streamOpen();
    for (int i = 0; i < 100; i++)
      {
	outGPIO.streamWrite(HIGH);
	outGPIO.streamWrite(LOW);
      }
  outGPIO.streamClose();
  */
  return 0;
}
