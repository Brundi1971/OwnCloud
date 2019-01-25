#include "compiler.h"

#ifndef _CONFIG_H_
#define _CONFIG_H_
#endif

#define BAUDRATE                                76800
#define FOSC                                    16000

#define CCW                                     0
#define CW                                      1

#define USE_ADC
#define ADC_RIGHT_ADJUST_RESULT                 0 //!< 0: Result left adjusted  1: Result right adjusted
#define ADC_HIGH_SPEED_MODE                     0 //!< 0: low speed mode        1: high speed mode
#define ADC_INTERNAL_VREF                       2 //!< 0: External Vref         1: Internal Vref  2: Vref is connected to Vcc
#define ADC_IT                                  1 //!< 0: No ADC End of Conv IT 1: ADC End of conversion generates an IT
#define ADC_PRESCALER                           4 //!< 2, 4, 8, 16, 32, 64, 128  : The input ADC frequency is the system clock frequency divided by the const value

#define USE_DAC
#define DAC_INPUT_RIGHT_ADJUST                  0 //!< 0: Result left adjusted  1: Result right adjusted
#define DAC_INTERNAL_VREF                       0 //!< 0: VCC ref  1: Vref
#define DAC_OUTPUT_DRIVER                       1 //!<

#define USE_AMP1
#define AMP1_INPUT_SHUNT                        0 //!< 0: Disable Input Shunt   1: Enable Input Shunt
#define AMP1_GAIN                               5 //!< 5: Gain 5    10: Gain 10     20: Gain 20     40: Gain 40
#define AMP1_CLOCK                              0 //!< 0: Internal Clock            1: PSC0 Clock     2: PSC1 Clock     3: PSC2 Clock

#define USE_COMP0
#define COMPARATOR0_IT                          1 //!< 0: Disable Comparator Interrupt    1: Enable Comparator Interrupt
#define COMPARATOR0_IT_EVENT                    0 //!< Comparator Interrupt on outut => 0: toggle     2: falling edge     3: rising edge
#define COMPARATOR0_NEGATIVE_INPUT              3 //!< Comparator negative input selection => 0: vref/6.40  1: vref/3.20  2: vref/2.13  3: vref/1.60  4: ACMPM  5: DAC result

#define USE_COMP1
#define COMPARATOR1_IT                          1 //!< 0: Disable Comparator Interrupt    1: Enable Comparator Interrupt
#define COMPARATOR1_IT_EVENT                    0 //!< Comparator Interrupt on outut => 0: toggle     2: falling edge     3: rising edge
#define COMPARATOR1_NEGATIVE_INPUT              3 //!< Comparator negative input selection => 0: vref/6.40  1: vref/3.20  2: vref/2.13  3: vref/1.60  4: ACMPM  5: DAC result

#define USE_COMP2
#define COMPARATOR2_IT                          1 //!< 0: Disable Comparator Interrupt    1: Enable Comparator Interrupt
#define COMPARATOR2_IT_EVENT                    0 //!< Comparator Interrupt on outut => 0: toggle     2: falling edge     3: rising edge
#define COMPARATOR2_NEGATIVE_INPUT              3 //!< Comparator negative input selection => 0: vref/6.40  1: vref/3.20  2: vref/2.13  3: vref/1.60  4: ACMPM  5: DAC result

