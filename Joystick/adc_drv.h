
#ifndef ADC_DRV_H
#define ADC_DRV_H

#ifndef  ADHSM
#define  ADHSM   7
#endif

extern volatile U8 Flag_IT_ADC;
extern volatile U8 Flag_Start_ADC;
extern volatile U8 SpeedAndDirection_H;
extern volatile U8 SpeedAndDirection_V;
extern volatile U8 Potentiometru_H;
extern volatile U8 Potentiometru_V;
extern volatile U8 Status_MUX_ADC;
extern volatile U8 Memo_Speed_H;
extern volatile U8 Memo_Speed_V;

void init_ADC(void);
void read_ADC(void);
U16 read_acquisition(void);
U8 speedHisterezis_H(void);
U8 speedHisterezis_V(void);
U8 get_SpeedAndDirection_H(void);
U8 get_SpeedAndDirection_V(void);
void set_SpeedAndDirection_H(U8 nb);
void set_SpeedAndDirection_V(U8 nb);


#define ADC_INPUT_ADC0                       0
#define ADC_INPUT_ADC1                       1
#define ADC_INPUT_ADC2                       2
#define ADC_INPUT_ADC3                       3
#define ADC_INPUT_ADC4                       4
#define ADC_INPUT_ADC5                       5
#define ADC_INPUT_ADC6                       6
#define ADC_INPUT_ADC7                       7
#define ADC_INPUT_BANDGAP                    14

#define ADC_PRESCALER_2                      0 
#define ADC_PRESCALER_2b                     1
#define ADC_PRESCALER_4                      2
#define ADC_PRESCALER_8                      3
#define ADC_PRESCALER_16                     4
#define ADC_PRESCALER_32                     5
#define ADC_PRESCALER_64                     6
#define ADC_PRESCALER_128                    7

#define ADC_VREF_AREF                        0x00
#define ADC_VREF_AVCC                        0x40
#define ADC_VREF_INTERNAL                    0xc0

#define ADC_TRIG_SRC_FREE_RUNNING            0
#define ADC_TRIG_SRC_ANA_COMP0               1
#define ADC_TRIG_SRC_EXT_IT_0                2
#define ADC_TRIG_SRC_TIM0_COMP_MATCH         3
#define ADC_TRIG_SRC_TIM0_COMP_OVF           4
#define ADC_TRIG_SRC_TIM1_COMP_MATCH         5
#define ADC_TRIG_SRC_TIM1_COMP_OVF           6
#define ADC_TRIG_SRC_TIM1_CAPTURE            7

#define ADC_H                                ADC_INPUT_ADC6
#define ADC_V                                ADC_INPUT_ADC7


// conversia tensiunii de pe cursorul potentiometrelor se va realiza pe 8 biti
#define ZERO_SPEED                           0
#define SECOND_SPEED_H                       70                   
#define STEP_SPEED_H                         25
#define HIST_H                               5

#define SECOND_SPEED_V                       70                   
#define STEP_SPEED_V                         25
#define HIST_V                               5

#define SPEED_0                              0
#define SPEED_1                              1
#define SPEED_2                              2
#define SPEED_3                              3
#define SPEED_4                              4
#define SPEED_5                              5
#define SPEED_6                              6
#define SPEED_7                              7
#define SPEED_8                              8
#define SPEED_9                              9
#define SPEED_10                             10
#define SPEED_11                             11
#define SPEED_12                             12
#define SPEED_13                             13
#define SPEED_14                             14
#define SPEED_15                             15
#define SPEED_16                             16

// constante pentru conversia potentiometrului pentru orizontala
#define UP_TOGGLE_SPEED_1_H                  ZERO_SPEED + SECOND_SPEED_H - HIST_H
#define UP_TOGGLE_SPEED_2_H                  ZERO_SPEED + SECOND_SPEED_H + HIST_H
#define UP_TOGGLE_SPEED_3_H                  ZERO_SPEED + SECOND_SPEED_H + 1*STEP_SPEED_H + HIST_H
#define UP_TOGGLE_SPEED_4_H                  ZERO_SPEED + SECOND_SPEED_H + 2*STEP_SPEED_H + HIST_H
#define UP_TOGGLE_SPEED_5_H                  ZERO_SPEED + SECOND_SPEED_H + 3*STEP_SPEED_H + HIST_H
#define UP_TOGGLE_SPEED_6_H                  ZERO_SPEED + SECOND_SPEED_H + 4*STEP_SPEED_H + HIST_H
#define UP_TOGGLE_SPEED_7_H                  ZERO_SPEED + SECOND_SPEED_H + 5*STEP_SPEED_H + HIST_H
#define UP_TOGGLE_SPEED_8_H                  ZERO_SPEED + SECOND_SPEED_H + 6*STEP_SPEED_H + HIST_H
#define UP_TOGGLE_SPEED_9_H                  ZERO_SPEED + SECOND_SPEED_H + 7*STEP_SPEED_H + HIST_H
#define UP_TOGGLE_SPEED_10_H                 ZERO_SPEED + SECOND_SPEED_H + 8*STEP_SPEED_H + HIST_H

#define DOWN_TOGGLE_SPEED_2_H                ZERO_SPEED + SECOND_SPEED_H - HIST_H
#define DOWN_TOGGLE_SPEED_3_H                ZERO_SPEED + SECOND_SPEED_H + 1*STEP_SPEED_H - HIST_H
#define DOWN_TOGGLE_SPEED_4_H                ZERO_SPEED + SECOND_SPEED_H + 2*STEP_SPEED_H - HIST_H
#define DOWN_TOGGLE_SPEED_5_H                ZERO_SPEED + SECOND_SPEED_H + 3*STEP_SPEED_H - HIST_H
#define DOWN_TOGGLE_SPEED_6_H                ZERO_SPEED + SECOND_SPEED_H + 4*STEP_SPEED_H - HIST_H
#define DOWN_TOGGLE_SPEED_7_H                ZERO_SPEED + SECOND_SPEED_H + 5*STEP_SPEED_H - HIST_H
#define DOWN_TOGGLE_SPEED_8_H                ZERO_SPEED + SECOND_SPEED_H + 6*STEP_SPEED_H - HIST_H
#define DOWN_TOGGLE_SPEED_9_H                ZERO_SPEED + SECOND_SPEED_H + 7*STEP_SPEED_H - HIST_H
#define DOWN_TOGGLE_SPEED_10_H               ZERO_SPEED + SECOND_SPEED_H + 8*STEP_SPEED_H - HIST_H

// constante pentru conversia potentiometrului pentru verticala
#define UP_TOGGLE_SPEED_1_V                  ZERO_SPEED + SECOND_SPEED_V - HIST_V
#define UP_TOGGLE_SPEED_2_V                  ZERO_SPEED + SECOND_SPEED_V + HIST_V
#define UP_TOGGLE_SPEED_3_V                  ZERO_SPEED + SECOND_SPEED_V + 1*STEP_SPEED_V + HIST_V
#define UP_TOGGLE_SPEED_4_V                  ZERO_SPEED + SECOND_SPEED_V + 2*STEP_SPEED_V + HIST_V
#define UP_TOGGLE_SPEED_5_V                  ZERO_SPEED + SECOND_SPEED_V + 3*STEP_SPEED_V + HIST_V
#define UP_TOGGLE_SPEED_6_V                  ZERO_SPEED + SECOND_SPEED_V + 4*STEP_SPEED_V + HIST_V
#define UP_TOGGLE_SPEED_7_V                  ZERO_SPEED + SECOND_SPEED_V + 5*STEP_SPEED_V + HIST_V
#define UP_TOGGLE_SPEED_8_V                  ZERO_SPEED + SECOND_SPEED_V + 6*STEP_SPEED_V + HIST_V
#define UP_TOGGLE_SPEED_9_V                  ZERO_SPEED + SECOND_SPEED_V + 7*STEP_SPEED_V + HIST_V
#define UP_TOGGLE_SPEED_10_V                 ZERO_SPEED + SECOND_SPEED_V + 8*STEP_SPEED_V + HIST_V

#define DOWN_TOGGLE_SPEED_2_V                ZERO_SPEED + SECOND_SPEED_V -HIST_V
#define DOWN_TOGGLE_SPEED_3_V                ZERO_SPEED + SECOND_SPEED_V + 1*STEP_SPEED_V - HIST_V
#define DOWN_TOGGLE_SPEED_4_V                ZERO_SPEED + SECOND_SPEED_V + 2*STEP_SPEED_V - HIST_V
#define DOWN_TOGGLE_SPEED_5_V                ZERO_SPEED + SECOND_SPEED_V + 3*STEP_SPEED_V - HIST_V
#define DOWN_TOGGLE_SPEED_6_V                ZERO_SPEED + SECOND_SPEED_V + 4*STEP_SPEED_V - HIST_V
#define DOWN_TOGGLE_SPEED_7_V                ZERO_SPEED + SECOND_SPEED_V + 5*STEP_SPEED_V - HIST_V
#define DOWN_TOGGLE_SPEED_8_V                ZERO_SPEED + SECOND_SPEED_V + 6*STEP_SPEED_V - HIST_V
#define DOWN_TOGGLE_SPEED_9_V                ZERO_SPEED + SECOND_SPEED_V + 7*STEP_SPEED_V - HIST_V
#define DOWN_TOGGLE_SPEED_10_V               ZERO_SPEED + SECOND_SPEED_V + 8*STEP_SPEED_V - HIST_V


#define Enable_adc()                         (ADCSRA |= (1<<ADEN))

#define Right_adjust_adc_result()            (ADMUX  &= ~(1<<ADLAR))
#define Left_adjust_adc_result()             (ADMUX  |=  (1<<ADLAR))

#define Enable_adc_high_speed_mode()         (ADCSRB |=  (1<<ADHSM))
#define Disable_adc_high_speed_mode()        (ADCSRB &= ~(1<<ADHSM))

#define Enable_internal_vref()               (ADMUX  |=  ((1<<REFS1)|(1<<REFS0)) )
#define Enable_external_vref()               (ADMUX  &= ~((1<<REFS1)|(1<<REFS0)) )
#define Enable_avcc_vref()                    (ADMUX  &= ~(1<<REFS1), ADMUX |= (1<<REFS0))         

#define Enable_all_it()                      (SREG   |=  (0x80) )
#define Disable_all_it()                     (SREG   &= ~(0x80) )
#define Enable_adc_it()                      (ADCSRA |=  (1<<ADIE) )
#define Disable_adc_it()                     (ADCSRA &= ~(1<<ADIE) )
#define Clear_adc_flag()                     (ADCSRA &=  (1<<ADIF) )

#define Set_prescaler(prescaler)             (ADCSRA &= ~((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)),\
                                             (ADCSRA |= prescaler))

#define Free_adc_runing()                    (ADCSRB &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0)))

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

#define Adc_get_8_bits_result()               ((U8)(ADCH))
#define Adc_get_10_bits_result()              ((U16)(ADCL+((U16)(ADCH<<8))))

#define Disable_adc()                        (ADCSRA &= ~(1<<ADEN))

#define Is_adc_conv_finished()               ((ADCSRA &  (1<<ADIF)) ? TRUE : FALSE)
#define Is_adc_conv_not_finished()           ((ADCSRA | ~(1<<ADIF)) ? TRUE : FALSE)

#endif  // ADC_DRV_H
