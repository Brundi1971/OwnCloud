#ifndef ADC_DRV_H
#define ADC_DRV_H

#define ADHSM                                        7
#define ADASCR                                       4

#define CONV_VOLTAGE                                 11
#define CONV_TEMPERATURE                             22
#define CONV_CURRENT                                 33

#define CHANNEL_CONVERSIE_VOLTAGE                    5
#define CHANNEL_CONVERSIE_TEMPERATURE                3
#define CHANNEL_CONVERSIE_CURENT                     12

#define LEVEL_VOLATGE_0                              0
#define LEVEL_VOLATGE_1                              100
#define LEVEL_VOLATGE_2                              140
#define LEVEL_VOLATGE_3                              160
#define LEVEL_VOLATGE_4                              180
#define LEVEL_VOLATGE_5                              200
#define VOLTAGE_HIST                                 5

#define UP_TOGGLE_VOLTAGE_0                  LEVEL_VOLATGE_0
#define UP_TOGGLE_VOLTAGE_1                  LEVEL_VOLATGE_1 + VOLTAGE_HIST
#define UP_TOGGLE_VOLTAGE_2                  LEVEL_VOLATGE_2 + VOLTAGE_HIST
#define UP_TOGGLE_VOLTAGE_3                  LEVEL_VOLATGE_3 + VOLTAGE_HIST
#define UP_TOGGLE_VOLTAGE_4                  LEVEL_VOLATGE_4 + VOLTAGE_HIST
#define UP_TOGGLE_VOLTAGE_5                  LEVEL_VOLATGE_5 + VOLTAGE_HIST

#define DOWN_TOGGLE_VOLTAGE_1                LEVEL_VOLATGE_1 - VOLTAGE_HIST
#define DOWN_TOGGLE_VOLTAGE_2                LEVEL_VOLATGE_2 - VOLTAGE_HIST
#define DOWN_TOGGLE_VOLTAGE_3                LEVEL_VOLATGE_3 - VOLTAGE_HIST
#define DOWN_TOGGLE_VOLTAGE_4                LEVEL_VOLATGE_4 - VOLTAGE_HIST
#define DOWN_TOGGLE_VOLTAGE_5                LEVEL_VOLATGE_5 - VOLTAGE_HIST


#define Enable_adc()                         (ADCSRA |= (1<<ADEN))  
    
#define Right_adjust_adc_result()            (ADMUX  &= ~(1<<ADLAR)) 
#define Left_adjust_adc_result()             (ADMUX  |=  (1<<ADLAR))
      
#define Enable_adc_high_speed_mode()         (ADCSRB |=  (1<<ADHSM)) 
#define Disable_adc_high_speed_mode()        (ADCSRB &= ~(1<<ADHSM))
      
#define Enable_internal_vref()               (ADMUX  |=  ((1<<REFS1)|(1<<REFS0)) )
#define Enable_external_vref()               (ADMUX  &= ~((1<<REFS1)|(1<<REFS0)) )
#define Enable_vcc_vref()                    (ADMUX  &= ~(1<<REFS1),          \
                                              ADMUX  |=  (1<<REFS0) ) 
#define Enable_all_it()                      (SREG   |=  (0x80) ) 
#define Disable_all_it()                     (SREG   &= ~(0x80) )
#define Enable_adc_it()                      (ADCSRA |=  (1<<ADIE) )
#define Disable_adc_it()                     (ADCSRA &= ~(1<<ADIE) )
#define Clear_adc_flag()                     (ADCSRA |=  (1<<ADIF) ) 
     
#define Set_prescaler(prescaler)             (ADCSRA &= ~((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)),\
                                              ADCSRA |=  (prescaler) )
#define Clear_adc_mux()                      (ADMUX  &= ~((1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0)) )  
#define Select_adc_channel(channel)          (Clear_adc_mux(), ADMUX |= (channel) )
      
#define Start_conv()                          (ADCSRA |= (1<<ADSC) )
#define Start_conv_channel(channel)           (Select_adc_channel(channel), Start_conv() ) 
#define Start_amplified_conv()                (ADCSRB |= (1<<ADASCR) )
#define Stop_amplified_conv()                 (ADCSRB &= ~(1<<ADASCR) )
#define Start_amplified_conv_channel(channel) (Select_adc_channel(channel), Start_amplified_conv() )
        
#define Start_conv_idle()                    (SMCR   |=  (1<<SM0)|(1<<SE) )  
#define Start_conv_idle_channel(channel)     (Select_adc_channel(channel), Start_conv_idle() ) 
#define Clear_sleep_mode()                   (SMCR   &= ~(1<<SM0)|(1<<SE) )
        
#define Adc_get_8_bits_result()              ((U8)(ADCH))
#define Adc_get_10_bits_result()             ((U16)(ADCL+((U16)(ADCH<<8))))
      
#define Disable_adc()                         (ADCSRA &= ~(1<<ADEN))
     
void init_adc(void);
void init_all_ADC(void);
void voltageHisterezis(void);
void mc_ADC_Scheduler(void);

void mc_set_measured_voltage(U8 nb);
U8 mc_get_measured_voltage(void);
void mc_set_measured_temperature(U8 nb);
U8 mc_get_measured_temperature(void);
void mc_set_measured_current(U8 nb);
U8 mc_get_measured_current(void);
void set_VoltageIndex(U8 nb);
U8 get_VoltageIndex(void);


#endif  // ADC_DRV_H
