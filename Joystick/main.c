
#include "config.h"
#include "compiler.h"
#include "inavr.h"
#include <stdio.h>
#include "pc_com.h"
#include "uart_lib.h"
#include "adc_drv.h"
#include "rs485_com.h"

void main(void)
{   
   init_porturi();
   init_telegrame_RS485();
   init_telegram_RS232();
   init_timer1();
   init_UART0();
   init_UART1();
   init_ADC();
 
   SpeedAndDirection_H=0;
   SpeedAndDirection_V=0;
   
   Enable_interrupt();
   LED_OFF();
   
   while(1)
   { 
      if (Flag_EvenimenteButtonJoystick)
      {
        get_Buttons_Joystick();
        Flag_EvenimenteButtonJoystick = 0;
      }
      transmiteCatrePC();
      transmiteCatreSlaves();
      read_ADC();
   }     
}        

