#include "config.h"
#include "mc_lib.h"
#include "mc_drv_highSpeed.h"
#include "mc_drv_lowSpeed.h"
#include "adc_drv.h"
#include "mc_rs485.h"
#include "mc_resolver.h"
#include "mc_inputuri.h"
#include <string.h>

volatile U16 temp;


/****************************************************************************************************************/
int main(void)
{
  mc_motor_init();

  init_porturi_adresa();
  init_senzori_externi();
  init_port_led();

  init_resolver();
  init_uart();

  //init_all_ADC();

  LED_OFF();

  read_ADDRESS();

  temp = read_eeprom_word(0x0018);
  if (temp == 0x4f4b)
  {
     IndexProgramed = PROGRAMED;
     temp = read_eeprom_word(0x0000);
     set_IndexZero_LowSpeed(temp);
     temp = read_eeprom_word(0x0008);
     set_IndexZero_HighSpeed(temp);
  }

  //mc_config_sampling_period();

  while(1)
  {
     check_for_test_resolver();
     com_with_master();
    
    //Show PSC state according to the Over Current information
    //if(PCTL2 & (1<<PRUN2))  LED_OFF();// PSC ON
    //else  LED_ON();//PSC OFF => Over_Current

     mc_ADC_Scheduler();
  }
  return(0);
}
