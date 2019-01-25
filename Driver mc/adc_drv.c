
#include "config.h"
#include "amplifier_drv.h"
#include "mc_inputuri.h"

#ifdef USE_ADC //!< this define is set on config.h file
#include "adc_drv.h"

volatile U8 ADC_State;
volatile U8 Scheduler_State;
volatile U8 VCC_voltage;
volatile U8 Temperature;
volatile U8 Shunt_current;
volatile U8 Channel_conversion;

volatile U8 VoltageIndex = 0;
volatile U8 Memo_VCC_voltage = UP_TOGGLE_VOLTAGE_0;

void init_adc(void)
{
    Enable_adc();
#   if (ADC_RIGHT_ADJUST_RESULT == 1)
       Right_adjust_adc_result();
#   elif (ADC_RIGHT_ADJUST_RESULT == 0)
       Left_adjust_adc_result();
#   else
#      error (ADC_RIGHT_ADJUST_RESULT should be 0 or 1... See config.h file)
#   endif

#   if (ADC_HIGH_SPEED_MODE == 1)
       Enable_adc_high_speed_mode();
#   elif (ADC_HIGH_SPEED_MODE == 0)
       Disable_adc_high_speed_mode();
#   else
#      error (ADC_HIGH_SPEED_MODE should be 0 or 1... See config.h file)
#   endif

#   if (ADC_INTERNAL_VREF == 2)
       Enable_vcc_vref();
#   elif (ADC_INTERNAL_VREF == 1)
       Enable_internal_vref();
#   elif (ADC_INTERNAL_VREF == 0)
       Enable_vcc_vref();
#   else
#      error (ADC_INTERNAL_VREF should be 0, 1 or 2... See config.h file)
#   endif

#   if (ADC_IT == 1)
       Enable_all_it();
       Enable_adc_it();
#   elif (ADC_IT == 0)
       Disable_adc_it();
#   else
#      error (ADC_IT should be 0 or 1... See config.h file)
#   endif

#   if (ADC_PRESCALER == 128)
       Set_prescaler(128);
#   elif (ADC_PRESCALER == 64)
       Set_prescaler(64);
#   elif (ADC_PRESCALER == 32)
       Set_prescaler(32);
#   elif (ADC_PRESCALER == 16)
       Set_prescaler(16);
#   elif (ADC_PRESCALER ==  8)
       Set_prescaler( 8);
#   elif (ADC_PRESCALER ==  4)
       Set_prescaler( 4);
#   elif (ADC_PRESCALER ==  2)
       Set_prescaler( 0);
#   else
#      error (ADC_PRESCALER should be 2, 4, 8, 16, 32, 64 or 128... See config.h file)
#   endif
}


void init_all_ADC(void)
{
    // Disable Digital Input for amplifier1
    // Digitals Inputs for comparators are not disable.
    DIDR0 |= (1<<ADC5D)|(1<<ADC3D);
    //DIDR1 |= (1<<ADC8D)|(1<<ADC9D);

    init_adc();
    //init_amp1();

    Scheduler_State = CONV_VOLTAGE;
    ADC_State = FREE;
}

ISR(ADC_vect)
{

    if(Channel_conversion == CHANNEL_CONVERSIE_VOLTAGE)
    {
        mc_set_measured_voltage(Adc_get_8_bits_result());
        voltageHisterezis();
//Toggle_LED();
    }
    if(Channel_conversion == CHANNEL_CONVERSIE_TEMPERATURE) mc_set_measured_temperature(Adc_get_8_bits_result());
    //if(Channel_conversion == CHANNEL_CONVERSIE_CURENT) mc_set_measured_current(Adc_get_8_bits_result());
    ADC_State = FREE;
    Clear_adc_flag();

}

void mc_ADC_Scheduler(void)
{
    switch(Scheduler_State)
    {
        case CONV_VOLTAGE:
                                    if (ADC_State == FREE)
                                    {
                                        ADC_State = BUSY;
                                        Scheduler_State = CONV_TEMPERATURE;
                                        //Left_adjust_adc_result();
                                        Start_conv_channel(CHANNEL_CONVERSIE_VOLTAGE);
                                        Channel_conversion = CHANNEL_CONVERSIE_VOLTAGE;
                                    }
                                    break;
        case CONV_TEMPERATURE:
                                    if(ADC_State == FREE)
                                    {
                                        ADC_State = BUSY;
                                        Scheduler_State = CONV_VOLTAGE; //;CONV_CURRENT; //
                                        //Left_adjust_adc_result();
                                        Start_conv_channel(CHANNEL_CONVERSIE_TEMPERATURE);
                                        Channel_conversion = CHANNEL_CONVERSIE_TEMPERATURE;
                                     }
                                     break;
        case CONV_CURRENT:
                                     if(ADC_State == FREE)
                                     {
                                        ADC_State = BUSY;
                                        Scheduler_State = CONV_VOLTAGE;
                                        //Left_adjust_adc_result();
                                        Start_amplified_conv_channel(CHANNEL_CONVERSIE_CURENT);
                                        Channel_conversion = CHANNEL_CONVERSIE_CURENT;
                                     }
                                     break;
    }
}


void mc_set_measured_voltage(U8 nb)                 {VCC_voltage = nb;}
U8 mc_get_measured_voltage(void)                    {return VCC_voltage;}
void mc_set_measured_temperature(U8 nb)             {Temperature = nb;}
U8 mc_get_measured_temperature(void)                {return Temperature;}
void mc_set_measured_current(U8 nb)                 {Shunt_current = nb;}
U8 mc_get_measured_current(void)                    {return Shunt_current;}
void set_VoltageIndex(U8 nb)                        {VoltageIndex = nb;}
U8 get_VoltageIndex(void)                           {return VoltageIndex;}



void voltageHisterezis(void) 
{
  register U8 measuredVoltage;
  
  measuredVoltage = mc_get_measured_voltage();
     
  if (measuredVoltage >= Memo_VCC_voltage )
  {
    if (measuredVoltage >= UP_TOGGLE_VOLTAGE_1) Memo_VCC_voltage = UP_TOGGLE_VOLTAGE_1;
    if (measuredVoltage >= UP_TOGGLE_VOLTAGE_2) Memo_VCC_voltage = UP_TOGGLE_VOLTAGE_2;
    if (measuredVoltage >= UP_TOGGLE_VOLTAGE_3) Memo_VCC_voltage = UP_TOGGLE_VOLTAGE_3;
    if (measuredVoltage >= UP_TOGGLE_VOLTAGE_4) Memo_VCC_voltage = UP_TOGGLE_VOLTAGE_4;
    if (measuredVoltage >= UP_TOGGLE_VOLTAGE_5) Memo_VCC_voltage = UP_TOGGLE_VOLTAGE_5;
  }
  else
  {
    if (measuredVoltage < DOWN_TOGGLE_VOLTAGE_5) Memo_VCC_voltage = UP_TOGGLE_VOLTAGE_4;
    if (measuredVoltage < DOWN_TOGGLE_VOLTAGE_4) Memo_VCC_voltage = UP_TOGGLE_VOLTAGE_3;
    if (measuredVoltage < DOWN_TOGGLE_VOLTAGE_3) Memo_VCC_voltage = UP_TOGGLE_VOLTAGE_2;
    if (measuredVoltage < DOWN_TOGGLE_VOLTAGE_2) Memo_VCC_voltage = UP_TOGGLE_VOLTAGE_1;
    if (measuredVoltage < DOWN_TOGGLE_VOLTAGE_1) Memo_VCC_voltage = UP_TOGGLE_VOLTAGE_0;
  }
  
  switch(Memo_VCC_voltage)
  {
    case UP_TOGGLE_VOLTAGE_0:   set_VoltageIndex(0);break;
    case UP_TOGGLE_VOLTAGE_1:   set_VoltageIndex(1);break;
    case UP_TOGGLE_VOLTAGE_2:   set_VoltageIndex(2);break;
    case UP_TOGGLE_VOLTAGE_3:   set_VoltageIndex(3);break;
    case UP_TOGGLE_VOLTAGE_4:   set_VoltageIndex(4);break;
    case UP_TOGGLE_VOLTAGE_5:   set_VoltageIndex(5);break;
  }
}


#endif // USE_ADC
