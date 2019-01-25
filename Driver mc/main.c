//main

#include "config.h"
#include "mc_lib.h"
#include "mc_control.h"
#include "mc_drv_highSpeed.h"
#include "mc_drv_lowSpeed.h"
#include "adc_drv.h"
#include "mc_rs485.h"
#include "mc_resolver.h"

extern Bool g_tic;    

/****************************************************************************************************************/
/****************************************************************************************************************/


void main(void)
{
  mc_motor_init();
  init_porturi_adresa();
  init_senzori_externi();
  init_port_led();
  //mc_prepare_for_lowSpeed();
  //mc_prepare_for_highSpeed();
  init_uart();
  init_resolver();
 
  //mc_set_motor_highSpeed(150);
  //mc_motor_run_highSpeed();
  
  switch(PIN_PORT_SET_ADRESA & ((1 << PIN_ADRESA_1) | (1 << PIN_ADRESA_0)))
  {
    case (0 << PIN_ADRESA_1) | (0 << PIN_ADRESA_0):     ADRESA_MODUL_DRIVER_MOTOR = 0; break;
    case (0 << PIN_ADRESA_1) | (1 << PIN_ADRESA_0):     ADRESA_MODUL_DRIVER_MOTOR = 1; break;
    case (1 << PIN_ADRESA_1) | (0 << PIN_ADRESA_0):     ADRESA_MODUL_DRIVER_MOTOR = 2; break;
    case (1 << PIN_ADRESA_1) | (1 << PIN_ADRESA_0):     ADRESA_MODUL_DRIVER_MOTOR = 3; break;
  }
  
  while(1)
  {
     com_with_master();  
     check_for_test_resolver();
    
    // Show PSC state according to the Over Current information
    //if(PCTL2 & (1<<PRUN2))  switch_OFF_LED();// PSC ON
    //else  switch_ON_LED();//PSC OFF => Over_Current
   

    // Launch regulation loop
    // Timer 1 generate an IT (g_tic) all 250us
    // Sampling period = n * 250us
    if (g_tic == TRUE)
    {
      g_tic = FALSE;
      //Toggle_PC3();
      // Get Current and potentiometer value
      //mc_ADC_Scheduler();

      //g_regulation_period += 1;
      //if ( g_regulation_period >= 320 ) //n * 250us = Te
      //{
        //g_regulation_period = 0;
        //mc_set_motor_speed(motor_speed); // Set User Speed Command for an UART control
        //mc_regulation_loop(); // launch regulation loop
      //}
    }
  }
}
