
#ifndef _MC_INPUTURI_H_
#define _MC_INPUTURI_H_

extern volatile U8 ADRESA_MODUL_DRIVER_MOTOR;

void init_porturi_adresa(void);
void init_senzori_externi(void);
void pollingSenzoriExterni(void);
void init_port_led(void);
void read_ADDRESS(void);

U8 get_Status_Senzori(void);
void set_Status_Senzori(U8 nb);

U8 get_temporar(void);
void set_temporar(U8 temp);

/* alocare porturi pentru setarea adresei driver-ului 
=================================================================================*/
#define DDR_PORT_SET_ADRESA                       DDRB
#define PORT_PORT_SET_ADRESA                      PORTB
#define PIN_PORT_SET_ADRESA                       PINB

#define PIN_ADRESA_0                              PINB5
#define PIN_ADRESA_1                              PINB4
/*===============================================================================*/
/* alocare porturi pentru senzorii externi 
=================================================================================*/
#define DDR_PORT_SENZORI                          DDRC
#define PORT_PORT_SENZORI                         PORTC
#define PIN_PORT_SENZORI                          PINC

#define PIN_SENZOR_LIMITA_V_SUPERIOARA            PINC2
#define PIN_SENZOR_LIMITA_V_INFERIOARA            PINC3

/* alocarea bitilor in telegrama transmisa catre Master */
#define BIT_SENZOR_LIMITA_V_SUPERIOARA            1
#define BIT_SENZOR_LIMITA_V_INFERIOARA            0
/*===============================================================================*/
/* port LED */
/*===============================================================================*/
#define DDR_PORT_LED                               DDRB
#define PORT_PORT_LED                              PORTB
#define PIN_PORT_LED                               PINB

#define PIN_LED                                    PINB2

#define LED_ON()                                   (PORT_PORT_LED &= ~( 1<< PIN_LED))
#define LED_OFF()                                  (PORT_PORT_LED |= (1 << PIN_LED))
#define Toggle_LED()                               (PIN_PORT_LED  |= (1 << PIN_LED))
/*================================================================================*/

#endif
