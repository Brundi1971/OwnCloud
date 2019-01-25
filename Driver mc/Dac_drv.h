#ifndef DAC_DRV_H
#define DAC_DRV_H


#define DAC_100mV                             5
#define DAC_200mV                             10
#define DAC_300mV                             15
#define DAC_400mV                             20
#define DAC_500mV                             25
#define DAC_1V                                50
#define DAC_1500mV                            75
#define DAC_2V                                100
#define DAC_3V                                150
#define DAC_4V                                200
#define DAC_5V                                150


#define Enable_dac()                           (DACON |= (1<<DAEN))
#define Disable_dac()                          (DACON &= ~(1<<DAEN))

#define Right_adjust_dac_input()               (DACON  &= ~(1<<DALA))
#define Left_adjust_dac_input()                (DACON  |=  (1<<DALA))

#define Enable_dac_output()                    (DACON  |=  (1<<DAOE))
#define Disable_dac_output()                   (DACON  &= ~(1<<DAOE))

#define Set_dac_8_bits(dac_in)                 (DACH = (U8)(dac_in))
#define Set_dac_10_bits(dac_in)                (DACL = LOW((U16)(dac_in)), DACH = HIGH((U16)(dac_in)))

#define Right_adjust_dac_result()              (DACON  &= ~(1<<DALA))
#define Left_adjust_dac_result()               (DACON  |=  (1<<DALA))

#define Enable_internal_vref()                 (ADMUX  |=  ((1<<REFS1)|(1<<REFS0)) )
#define Enable_external_vref()                 (ADMUX  &= ~((1<<REFS1)|(1<<REFS0)) )
#define Enable_vcc_vref()                      (ADMUX  &= ~(1<<REFS1), ADMUX |= (1<<REFS0) )

#define Output_dac_enable()                    (DACON |= (1<<DAOE))
#define Output_dac_disabled()                  (DACON &= ~(1<<DAOE))

#define mc_set_Over_Current(Level)             (Set_dac_8_bits(Level))

//void mc_set_Over_Current(Level U8)

void init_dac(void);

#endif  // DAC_DRV_H
