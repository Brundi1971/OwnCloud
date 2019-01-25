
#include "config.h"
#include "serial.h"
#include "mc_rs485.h"


void putint(int number)
{
  char value[6]={0,0,0,0,0,0};

   while((number - 10000)>=0)
  {
    number -= 10000;
    value[5]++;
  }
  value[5] += '0';

  while((number - 1000)>=0)
  {
    number -= 1000;
    value[4]++;
  }
  value[4] += '0';

  while((number - 100)>=0)
  {
    number -= 100;
    value[3]++;
  }
  value[3] += '0';

  while((number - 10)>=0)
  {
    number -= 10;
    value[2]++;
  }
  value[2] += '0';

  value[1] = number + '0';
  value[0] = '\0';

  sendchar(value[5]);
  sendchar(value[4]);
  sendchar(value[3]);
  sendchar(value[2]);
  sendchar(value[1]);
}

/**
* @brief send an unsigned char on the UART
* @param Unsigned char (U8) number
* @pre UART initialization
* @post Unsigned char send to the UART
*/
void putU8(U8 number)
{
  char value[3]={0,0,0};

  while((number - 100)>=0)
  {
    number -= 100;
    value[2]++;
  }
  value[2] += '0';

  while((number - 10)>=0)
  {
    number -= 10;
    value[1]++;
  }
  value[1] += '0';

  value[0] = number + '0';

  sendchar(value[2]);
  sendchar(value[1]);
  sendchar(value[0]);
}

/**
* @brief send a integer signed to the UART
* @param Signed Integer number
* @pre UART initialization
* @post Signed integer send to the UART
*/
void putS16(S16 number)
{
  char value[6]={0,0,0,0};

  if(number >= 0)
  {
    value[5]='+';
  }
  else
  {
    value[5]='-';
    number *= -1;
  }

  while((number - 10000)>=0)
  {
    number -= 10000;
    value[4]++;
  }
  value[4] += '0';

  while((number - 1000)>=0)
  {
    number -= 1000;
    value[3]++;
  }
  value[3] += '0';

  while((number - 100)>=0)
  {
    number -= 100;
    value[2]++;
  }
  value[2] += '0';

  while((number - 10)>=0)
  {
    number -= 10;
    value[1]++;
  }
  value[1] += '0';

  value[0] = number + '0';

  sendchar(value[5]);
  sendchar(value[4]);
  sendchar(value[3]);
  sendchar(value[2]);
  sendchar(value[1]);
  sendchar(value[0]);
}
