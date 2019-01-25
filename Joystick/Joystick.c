
#include "config.h"
#include "pc_com.h"
#include "uart_lib.h"
#include "adc_drv.h"
#include "rs485_com.h"
#include "buttons.h"

int main(void)
{   
   init_porturi_joystick();
   init_timer1();
   init_UART0();
   init_UART1();
   init_ADC();

   init_port_LED();

   LED_ON();
 
   SpeedAndDirection_H = 0;
   SpeedAndDirection_V = 0;
   
   Enable_interrupt();

   while(1)
   { 
      if (Flag_ForReadingButtonEvents)
      {
        get_Buttons_Joystick();
        Flag_ForReadingButtonEvents = 0;
      }
      transmiteCatrePC();
      transmiteCatreSlaves();
      read_ADC();
   }  
   
   return(0);   
}        
