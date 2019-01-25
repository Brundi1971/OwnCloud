#ifndef _UART_SLAVES_H_
#define _UART_SLAVES_H_

extern volatile U8 SpeedAndDirection_H;
extern volatile U8 SpeedAndDirection_V;
extern volatile U8 Resolver_Pozitie;
extern volatile U16 Delay_Before_Reset_PINS;
extern volatile U8 ADRESA_MODUL_COMANDA_VIDEO;
extern volatile U8 Flag_ActionCommand;


void init_porturi_adresa(void);
void init_porturi_comenzi_si_senzori(void);
void init_UART0(void);
void init_timer1(void);
void com_with_master(void);
void saveMasterCommand(void);
void loadTelegramaStatus(void);
void action_command(void);
void comanda_detonare_capse(void);
void reset_command_ports(void);

/* functii de sincronizare variabile */
U8 get_Status_ModulConducereFoc(void);
void set_Status_ModulConducereFoc(U8 nb);
U8 get_MasterCommand(void);
void set_MasterCommand(U8 nb);
U8 get_SelectieTipFoc(void);
void set_SelectieTipFoc(U8 nb);
void set_Status_SystemCamere(U8 Index, U8 Val);
U8 get_Status_SystemCamere(U8 Index);

/* alocare port pentru controlul comunicatiei RS485 
=============================================================================*/
#define Set_TX_RS485()                            (PORTD |= (1<<PORTD4))
#define Set_RX_RS485()                            (PORTD &=~(1<<PORTD4))  
#define Set_Control_RS485()                       (DDRD |= (1<<DDD4)) 
/*===========================================================================*/

/* constante pentru comunicatia RS 485
=============================================================================*/
#define PREPARE_TELEGRAMA_STATUS                  1
#define WAIT_ADDRESS_FROM_MASTER                  2
#define WAIT_BEFORE_TO_SEND_STATUS_SLAVE          3
#define SEND_STATUS_SLAVE                         4
#define WAIT_DATA_FROM_MASTER                     5
#define LOAD_NEW_COMANDA                          6

#define TIME_WAITING_FOR_ACK_ADRESA               30
#define TIME_WAITING_FOR_ACK_DATA                 30
#define DELAY_BEFORE_TRANSMITE_DATA               5

#define DELAY_AT_LEAST_50us                       1+1
#define WAIT_AT_LEAST_100us                       2+1
#define WAIT_AT_LEAST_500us                       10+1
#define WAIT_AT_LEAST_750us                       15+1
#define WAIT_AT_LEAST_2ms                         40+1
#define WAIT_AT_LEAST_5ms                         100+1
#define WAIT_AT_LEAST_10ms                        200+1
#define WAIT_AT_LEAST_20ms                        400+1
#define WAIT_AT_LEAST_50ms                        1000+1
#define WAIT_AT_LEAST_350ms                       7000+1
#define WAIT_AT_LEAST_500ms                       2000+1

#define NO_BYTES_STATUS                           10
#define NO_BYTES_COMANDA                          4
/*===========================================================================*/

#define READ_RESOLVER_H                           11
#define READ_RESOLVER_V                           22

/*alocarea porturilor adresa
===================================================================*/
#define DDR_PORT_SET_ADRESA                       DDRD
#define PORT_PORT_SET_ADRESA                      PORTD
#define PIN_PORT_SET_ADRESA                       PIND

#define PIN_ADRESA_1                              PIND7
#define PIN_ADRESA_0                              PIND6
#define PIN_ADRESA_2                              PIND5
/*=================================================================*/

/*alocarea porturilor relee
===================================================================*/
#define DDR_PORT_RELEE_CAPSE                      DDRC
#define PORT_PORT_RELEE_CAPSE                     PORTC
#define PIN_PORT_RELEE_CAPE                       PINC

#define PIN_CAPSA_1                               PINC1
#define PIN_CAPSA_2                               PINC2
#define PIN_CAPSA_3                               PINC3

#define DDR_PORT_RELEU_TRIGGER                    DDRC
#define PORT_PORT_RELEU_TRIGGER                   PORTC
#define PIN_PORT_RELEU_TRIGGER                    PINC

#define PIN_TRIGGER                               PINC0
/*=================================================================*/

/*alocarea porturilor senzori
===================================================================*/
#define DDR_PORT_SENZORI_CAPSE                    DDRC
#define PORT_PORT_SENZORI_CAPSE                   PORTC
#define PIN_PORT_SENZORI_CAPSE                    PINC

#define PIN_SENZOR_CAPSA_1                        PINC5
#define PIN_SENZOR_CAPSA_2                        PINC6
#define PIN_SENZOR_CAPSA_3                        PINC7

#define PIN_INPUT_PINC4                           PINC4

#define DDR_PORT_SENZOR_ARMARE                    DDRA
#define PORT_PORT_SENZOR_ARMARE                   PORTA
#define PIN_PORT_SENZOR_ARMARE                    PINA
#define PIN_SENZOR_ARMARE                         PINA7

//*====================================================================*/
/* alocarea bitilor in secventa de comanda care soseste de la Master
======================================================================*/
/* alocare biti in comanda CAM [0] */
#define BIT_COMANDA_TRIGGER                           2
#define BIT_COMANDA_SELECTIE_FOC                      1
#define BIT_COMANDA_DETONARE_CAPSE                    0

/* alocare biti in comanda CAM [1] */
#define BIT_COMANDA_READ_SENSORS                      7
#define BIT_COMANDA_BOOTING                           6
#define BIT_COMANDA_INCREASE                          5
#define BIT_COMANDA_DECREASE                          4
#define BIT_COMANDA_SELECTIE_ZOOM_FOCUS_AGC           3
#define BIT_COMANDA_SELECTIE_SPEED                    2
#define BIT_COMANDA_SELECT_CAMERA_TYPE                1
#define BIT_COMANDA_CAMERA_ON_OFF                     0
/*====================================================================*/


/*====================================================================*/
#define SELECTIE_FOC_CU_FOC                           1
#define SELECTIE_SALVA                                2
#define SELECTIE_CONTINUU                             3

#define TRIGGER_FOC_CU_FOC_COMMAND                   33
#define TRIGGER_SALVA_COMMAND                        44
#define TRIGGER_CONTINUU_COMMAND                     55
#define DETONARE_CAPSE_COMMAND                       66
#define SELECTIE_FOC_COMMAND                         77
#define NO_COMMAND                                   88

#define SELECTED_ZOOM_FUNCTION                       0
#define SELECTED_FOCUS_FUNCTION                      1
#endif
