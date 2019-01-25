
#include "config.h"

#ifdef USE_DAC //!< this define is set on config.h file
#include "dac_drv.h"

void init_dac(void){

    Enable_dac();

#   if (DAC_INPUT_RIGHT_ADJUST == 1)
       Right_adjust_dac_input();
#   elif (DAC_INPUT_RIGHT_ADJUST == 0)
       Left_adjust_dac_input();
#   else
#      error (ADC_RIGHT_ADJUST_RESULT should be 0 or 1... See config.h file)
#   endif

#   if (DAC_INTERNAL_VREF == 2)
       Enable_vcc_vref();
#   elif (DAC_INTERNAL_VREF == 1)
       Enable_internal_vref();
#   elif (DAC_INTERNAL_VREF == 0)
       Enable_vcc_vref();
#   else
#      error (DAC_INTERNAL_VREF should be 0, 1 or 2... See config.h file)
#   endif

#   if (DAC_OUTPUT_DRIVER == 1)
       Enable_dac_output();
#   elif (DAC_OUTPUT_DRIVER == 0)
       Disable_dac_output();
#   else
#      error (DAC_OUTPUT_DRIVER should be 0 or 1... See config.h file)
#   endif
}

#endif // USE_DAC
