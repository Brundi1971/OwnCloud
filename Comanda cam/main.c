//main

#include "config.h"
#include "uart_lib.h"
#include "rs232_com.h"
#include "rs485_com.h"
#include "read_resolvers.h"

int main(void)
{   
   init_porturi_adresa();
   init_porturi_comenzi_si_senzori();
   init_resolver_ports();
   init_remainig_ports();
   init_timer1();
   init_UART0();
   init_UART1();
   init_LED();

   LED_ON();

   sei();

   if (PIN_PORT_SET_ADRESA & (1 << PIN_ADRESA_0)) ADRESA_MODUL_COMANDA_VIDEO = 0x03;
   else ADRESA_MODUL_COMANDA_VIDEO = 0x02;


   while(1)
   { 
     boot_cameras();
     com_with_master();
     perform_command();
	 receive_acknowledge();
     if (Flag_ActionCommand)
     {
       action_command();
       Flag_ActionCommand = 0;
     } 
   }  
   return(0);   
}        

