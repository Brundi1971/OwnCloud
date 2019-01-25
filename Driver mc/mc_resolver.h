
#ifndef _MC_RESOLVER_H_
#define _MC_RESOLVER_H_

#define TESTING                                      22
#define NO_TESTING                                   33

#define NO_MAX_BITI_TO_BE_READ                       10

#define NO_BYTES_WRITTN_IN_EEPROM                    2
#define NB_PERECHI_POLI                              3
#define NB_SECTOARE                                  6
#define NB_MAX_RESOLVER                              1024
#define STEP_MOTOR                                   (NB_MAX_RESOLVER/(NB_SECTOARE * NB_PERECHI_POLI)) + 1 //43     //57
#define INDEX_ZERO_HIGH_SPEED                        0
#define INDEX_ZERO_LOW_SPEED                         -18

#define NB_OF_TESTS                                  3
#define DELAY_OF_AMP_REFRESH_FOR_HIGH_SPEED          4 /* refresh speed la un interval de 200us */


extern volatile U8 Flag_CheckForTesting;
extern volatile U16 Data_Resolver;
extern volatile U8 Memo_SectorResolver;
extern volatile U8 Status_Motor;
extern volatile U8 IndexProgramed;

void mc_config_sampling_period(void);
void init_resolver(void);
void read_resolver(void);  
void check_for_test_resolver(void);
void read_resolver(void);
U16 get_Data_Resolver(void);
void set_Data_Resolver(U16 nb);
void put_motor_on_position_ZERO(void);
void calculate_toggle_position(void);
U16 Calculate_Index(void);
void write_eeprom_word(U16 adresa_eeprom, U16 date);
U16 read_eeprom_word(U16 adresa_eeprom);

U16 get_IndexZero_HighSpeed(void);
void set_IndexZero_HighSpeed(U16 nb);


#define Set_Sector_1()                        PSOC0 = (0<<POEN0A)|(1<<POEN0B);\
                                              PSOC1 = (1<<POEN1A)|(0<<POEN1B);\
                                              PSOC2 = (0<<POEN2A)|(1<<POEN2B);

#define Set_Sector_2()                        PSOC0 = (0<<POEN0A)|(1<<POEN0B);\
                                              PSOC1 = (1<<POEN1A)|(0<<POEN1B);\
                                              PSOC2 = (1<<POEN2A)|(0<<POEN2B);

#define Set_Sector_3()                        PSOC0 = (0<<POEN0A)|(1<<POEN0B);\
                                              PSOC1 = (0<<POEN1A)|(1<<POEN1B);\
                                              PSOC2 = (1<<POEN2A)|(0<<POEN2B);

#define Set_Sector_4()                        PSOC0 = (1<<POEN0A)|(0<<POEN0B);\
                                              PSOC1 = (0<<POEN1A)|(1<<POEN1B);\
                                              PSOC2 = (1<<POEN2A)|(0<<POEN2B);

#define Set_Sector_5()                        PSOC0 = (1<<POEN0A)|(0<<POEN0B);\
                                              PSOC1 = (0<<POEN1A)|(1<<POEN1B);\
                                              PSOC2 = (0<<POEN2A)|(1<<POEN2B);

#define Set_Sector_6()                        PSOC0 = (1<<POEN0A)|(0<<POEN0B);\
                                              PSOC1 = (1<<POEN1A)|(0<<POEN1B);\
                                              PSOC2 = (0<<POEN2A)|(1<<POEN2B);

/******************************************************************************/
#define Set_A_B()                             PSOC0 = (1<<POEN0A)|(0<<POEN0B);\
                                              PSOC1 = (0<<POEN1A)|(1<<POEN1B);\
                                              PSOC2 = (0<<POEN2A)|(0<<POEN2B);

#define Set_B_A()                             PSOC0 = (0<<POEN0A)|(1<<POEN0B);\
                                              PSOC1 = (1<<POEN1A)|(0<<POEN1B);\
                                              PSOC2 = (0<<POEN2A)|(0<<POEN2B);

#define Set_A_C()                             PSOC0 = (1<<POEN0A)|(0<<POEN0B);\
                                              PSOC1 = (0<<POEN1A)|(0<<POEN1B);\
                                              PSOC2 = (0<<POEN2A)|(1<<POEN2B);

#define Set_C_A()                             PSOC0 = (0<<POEN0A)|(1<<POEN0B);\
                                              PSOC1 = (0<<POEN1A)|(0<<POEN1B);\
                                              PSOC2 = (1<<POEN2A)|(0<<POEN2B);

#define Set_B_C()                             PSOC0 = (0<<POEN0A)|(0<<POEN0B);\
                                              PSOC1 = (1<<POEN1A)|(0<<POEN1B);\
                                              PSOC2 = (0<<POEN2A)|(1<<POEN2B);

#define Set_C_B()                             PSOC0 = (0<<POEN0A)|(0<<POEN0B);\
                                              PSOC1 = (0<<POEN1A)|(1<<POEN1B);\
                                              PSOC2 = (1<<POEN2A)|(0<<POEN2B);

/* port test index resolver
==============================================================================*/
#define BUTTON_DDR_PORT_TEST_RESOLVER         DDRB
#define BUTTON_PORT_TEST_RESOLVER             PORTB
#define PORT_TEST_RESOLVER                    PINB

#define PIN_TEST_RESOLVER                     PINB3

#define Set_BUTTON_TEST_RESOLVER()            (BUTTON_DDR_PORT_TEST_RESOLVER &= ~(1<<PIN_TEST_RESOLVER),\
                                              BUTTON_PORT_TEST_RESOLVER |= (1<<PIN_TEST_RESOLVER))
/* porturi citire resolver 
==============================================================================*/
#define DDR_PORT_RESOLVER_0                   DDRC
#define PORT_PORT_RESOLVER_0                  PORTC
#define PIN_PORT_RESOLVER_0                   PINC

#define DDR_PORT_RESOLVER_1                   DDRD
#define PORT_PORT_RESOLVER_1                  PORTD
#define PIN_PORT_RESOLVER_1                   PIND

#define CS_PORT                               PINC6
#define SCLK_PORT                             PIND7
#define DATA_PORT                             PIND5

#define Set_Ports_Resolver()                  (DDR_PORT_RESOLVER_0 |= (1<<CS_PORT), DDR_PORT_RESOLVER_1 |= (1<<SCLK_PORT),\
                                              DDR_PORT_RESOLVER_1 &= ~(1<<DATA_PORT), PORT_PORT_RESOLVER_1 |= (1<<DATA_PORT))

#define CS_DOWN()                             (PORT_PORT_RESOLVER_0 &= ~(1<<CS_PORT))  
#define CS_UP()                               (PORT_PORT_RESOLVER_0 |= (1<<CS_PORT))
#define SCLK_DOWN()                           (PORT_PORT_RESOLVER_1 &= ~(1<<SCLK_PORT)) 
#define SCLK_UP()                             (PORT_PORT_RESOLVER_1 |= (1<<SCLK_PORT)) 

//#define Is_DATA_PORT_RESOLVER_1               (PIN_PORT_RESOLVER & (1<<DATA_PORT) ? TRUE : FALSE)
/*============================================================================*/
//constante Timer 0
#define NO_SPEED                              0
#define LOW_SPEED                             1
#define HIGH_SPEED                            2
#define NO_INDEX_AND_FLASHING_LED             3
#define START_TEST_RESOLVER                   4  
#define WAIT_FOR_STABILIZATION                5
#define READ_INDEX_FROM_RESOLVER              6
#define STORE_INDEX                           7


#define NO_PROGRAMED                          0
#define PROGRAMED                             1

#define START_READING_PROCEDURE               0
#define WAIT_2s                               1
#define WAIT_05s                              2
#define START_READING_INDEX                   3
#define WAIT_05s_bis                          4

#define eeprom_ready()                        (!(EECR & (1<<EEWE)))
#define write_eeprom()                        (EECR |= (1<<EEMWE), EECR |= (1<<EEWE))
#define fill_eeprom(adr,data)                 (EEARH = 0, EEARL = adr, EEDR = data)
  
#endif
