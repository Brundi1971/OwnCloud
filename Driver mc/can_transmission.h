#ifndef _CAN_TRANSMISSION_H_
#define _CAN_TRANSMISSION_H_

extern volatile U8 Comanda_DRH;
extern volatile U8 Comanda_DRV;
extern volatile U8 StatusDrivere;
extern volatile U8 Temperatura_Motor_H;
extern volatile U8 Temperatura_Motor_V;
extern volatile U8 Temperatura_Radiator_H;
extern volatile U8 Temperatura_Radiator_V;

void config_T1(void);
void init_can(void);
void can_transmission (void);
void prepare_status_data(void);
void init_senzori(void);
void verifica_AutoMObGeneration(void);

#define ID_CAN_DRIVERE                                               0x727
#define ID_CAN_CAMERA                                                0x728
#define NB_OF_BYTES_RECEIVE_FROM_MASTER                              2
#define NB_OF_BYTES_SENT_TO_MASTER                                   3
#define NO_ERRORS                                                    0x00
#define ERRORS                                                       0x10

#define DDR_PORT_SENZORI                                             DDRB
#define PORT_PORT_SENZORI                                            PORTB
#define PIN_PORT_SENZORI                                             PINB

#define SZ_PS_H                                                      PINB2
#define SZ_PS_V                                                      PINB3

#define DDR_PORT_COMENZI                                             DDRC
#define PORT_PORT_COMENZI                                            PORTC
#define PIN_PORT_COMENZI                                             PINC

#define CMD_FAN_H                                                    PINC7
#define CMD_FAN_V                                                    PINC6

#define START_FAN_H()                                                (PORT_PORT_COMENZI |= (1 << CMD_FAN_H))
#define START_FAN_V()                                                (PORT_PORT_COMENZI |= (1 << CMD_FAN_V))
#define STOP_FAN_H()                                                 (PORT_PORT_COMENZI &= ~(1 << CMD_FAN_H))
#define STOP_FAN_V()                                                 (PORT_PORT_COMENZI &= ~(1 << CMD_FAN_V))



#define BIT_FAN_V_ON                                                 5
#define BIT_FAN_H_ON                                                 4
#define BIT_LOSE_CONNECTION_WITH_DRV                                 3
#define BIT_LOSE_CONNECTION_WITH_DRH                                 2
#define BIT_LIMITA_VERTICALA_LOW                                     1
#define BIT_LIMITA_VERTICALA_HIGH                                    0

#define CONNECTION_WITH_MASTER_IS_ON_LINE                            33
#define CONNECTION_WITH_MASTER_IS_OFF_LINE                           55


#define WAIT_AT_LEAST_200us                                          2+1
#define WAIT_AT_LEAST_300us                                          3+1
#define WAIT_AT_LEAST_500us                                          5+1
#define WAIT_AT_LEAST_1ms                                            10+1
#define WAIT_AT_LEAST_10ms                                           100+1
#define WAIT_AT_LEAST_20ms                                           200+1
#define WAIT_AT_LEAST_25ms                                           250+1
#define WAIT_AT_LEAST_30ms                                           300+1
#define WAIT_AT_LEAST_40ms                                           400+1
#define WAIT_AT_LEAST_50ms                                           500+1
#define WAIT_AT_LEAST_100ms                                          1000+1

#define MUX_CHANNEL_0                                                0x00
#define MUX_CHANNEL_1                                                0x40
#define MUX_CHANNEL_2                                                0x80
#define MUX_CHANNEL_3                                                0xc0

/* mastile se aplica pentru variabilele MultiplexorDate StatusPS si Statusrezolvere */
#define MSK_BITI_MULTIPLEXOR                                         0xc0
#define MSK_BITI_PS                                                  0x30
#define MSK_BITI_STATUS_REZOLVERE                                    0x0f
#define MSK_TX_CONNECTION                                            0x80

#define BIT_SZ_PS_V                                                  5
#define BIT_SZ_PS_H                                                  4

#define BIT_STATUS_SENZOR_TEMP_A2                                    5
#define BIT_STATUS_SENZOR_TEMP_A0                                    4
#endif
