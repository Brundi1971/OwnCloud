
#include "config.h"
#include "amplifier_drv.h"

#ifdef USE_AMP0 //!< this define is set on config.h file

void init_amp0(void)
{
    Enable_amp0();

#   if (AMP0_INPUT_SHUNT == 1)
       Enable_amp0_input_shunt();
#   elif (AMP0_INPUT_SHUNT == 0)
       Disable_amp0_input_shunt();
#   else
#      error (AMP0_INPUT_SHUNT should be 0 or 1... See config.h file)
#   endif

#   if (AMP0_GAIN == 40)
       Set_amp0_gain40();
#   elif (AMP0_GAIN == 20)
       Set_amp0_gain20();
#   elif (AMP0_GAIN == 10)
       Set_amp0_gain10();
#   elif (AMP0_GAIN ==  5)
       Set_amp0_gain05();
#   else
#      error (AMP0_GAIN should be 5, 10, 20 or 40... See config.h file)
#   endif

#   if (AMP0_CLOCK == 0)
       Amp0_internal_clock();
#   elif (AMP0_CLOCK == 1)
       Amp0_psc0_clock();
#   elif (AMP0_CLOCK == 2)
       Amp0_psc1_clock();
#   elif (AMP0_CLOCK == 3)
       Amp0_psc2_clock();
#   else
#      error (AMP0_CLOCK should be 0, 1, 2 or 3... See config.h file)
#   endif
}
#endif //USE_AMP0

#ifdef USE_AMP1 //!< this define is set on config.h file
//! Configures the Amplifier 1 accordingly to the ADC Define Configuration
//! values.
//! Take care that you have to select the ports which will be converted as
//! analog inputs thanks to the DIDR0 and DIDR1 registers.
//!
void init_amp1(void)
{
    Enable_amp1();

#   if (AMP1_INPUT_SHUNT == 1)
       Enable_amp1_input_shunt();
#   elif (AMP1_INPUT_SHUNT == 0)
       Disable_amp1_input_shunt();
#   else
#      error (AMP1_INPUT_SHUNT should be 0 or 1... See config.h file)
#   endif

#   if (AMP1_GAIN == 40)
       Set_amp1_gain40();
#   elif (AMP1_GAIN == 20)
       Set_amp1_gain20();
#   elif (AMP1_GAIN == 10)
       Set_amp1_gain10();
#   elif (AMP1_GAIN ==  5)
       Set_amp1_gain05();
#   else
#      error (AMP1_GAIN should be 5, 10, 20 or 40... See config.h file)
#   endif

#   if (AMP1_CLOCK == 0)
       Amp1_internal_clock();
#   elif (AMP1_CLOCK == 1)
       Amp1_psc0_clock();
#   elif (AMP1_CLOCK == 2)
       Amp1_psc1_clock();
#   elif (AMP1_CLOCK == 3)
       Amp1_psc2_clock();
#   else
#      error (AMP1_CLOCK should be 0, 1, 2 or 3... See config.h file)
#   endif
}
#endif // USE_AMP1
