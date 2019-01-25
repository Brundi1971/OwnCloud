
#ifndef _MC_RESOLVER_H_
#define _MC_RESOLVER_H_

extern volatile U16 Contor_CalculateSpeed;
extern volatile U8 Speed_H;

#define TESTING                               22
#define NO_TESTING                            33

#define NO_MAX_BITI_TO_BE_READ_FROM_H         10   /* numarul maxim admis este 8*/
#define NO_MAX_BITI_TO_BE_READ_FROM_V         10   /* numarul maxim admis este 8*/
  
void init_resolver_ports(void);
void init_remainig_ports(void);
void read_resolver_H(void);
void read_resolver_V(void);
U16 get_Data_Resolver_H(void);
void set_Data_Resolver_H(U16 nb);
U16 get_Data_Resolver_V(void);
void set_Data_Resolver_V(U16 nb);

/*functii pentru calculul vitezei pe orizonatala*/
U16 calculate_speed_H(void);
U16 get_Speed_H(void);
void set_Speed_H(U16 nb);


/* constante Resolver H */
/******************************************************************************/
#define DDR_PORT_RESOLVER_H                   DDRB
#define PORT_PORT_RESOLVER_H                  PORTB
#define PIN_PORT_RESOLVER_H                   PINB

#define PIN_INPUT_PINB0                       PINB0        
#define PIN_INPUT_PINB1                       PINB1
             
#define CS_PORT_H                             PINB2
#define SCLK_PORT_H                           PINB3
#define DATA_PORT_H                           PINB4

#define PIN_INPUT_PINB5                       PINB5
#define PIN_INPUT_PINB6                       PINB6
#define PIN_INPUT_PINB7                       PINB7

#define Set_Ports_Resolver_H()                (DDR_PORT_RESOLVER_H |= ((1<<CS_PORT_H) | (1<<SCLK_PORT_H)),\
                                              DDR_PORT_RESOLVER_H &= ~(1<<DATA_PORT_H), PORT_PORT_RESOLVER_H |= (1<<DATA_PORT_H))
#define CS_DOWN_H()                           (PORT_PORT_RESOLVER_H &= ~(1<<CS_PORT_H))  
#define CS_UP_H()                             (PORT_PORT_RESOLVER_H |= (1<<CS_PORT_H))
#define SCLK_DOWN_H()                         (PORT_PORT_RESOLVER_H &= ~(1<<SCLK_PORT_H)) 
#define SCLK_UP_H()                           (PORT_PORT_RESOLVER_H |= (1<<SCLK_PORT_H)) 

/* constante Resolver V */
/******************************************************************************/
#define DDR_PORT_RESOLVER_V                   DDRA
#define PORT_PORT_RESOLVER_V                  PORTA
#define PIN_PORT_RESOLVER_V                   PINA

#define PIN_INPUT_PINA0                       PINA0
#define PIN_INPUT_PINA1                       PINA1
#define PIN_INPUT_PINA2                       PINA2
#define PIN_INPUT_PINA3                       PINA3

#define CS_PORT_V                             PINA4
#define SCLK_PORT_V                           PINA5
#define DATA_PORT_V                           PINA6

#define Set_Ports_Resolver_V()                (DDR_PORT_RESOLVER_V |= ((1<<CS_PORT_V) | (1<<SCLK_PORT_V)),\
                                              DDR_PORT_RESOLVER_V &= ~(1<<DATA_PORT_V), PORT_PORT_RESOLVER_V |= (1<<DATA_PORT_V))
#define CS_DOWN_V()                           (PORT_PORT_RESOLVER_V &= ~(1<<CS_PORT_V))  
#define CS_UP_V()                             (PORT_PORT_RESOLVER_V |= (1<<CS_PORT_V))
#define SCLK_DOWN_V()                         (PORT_PORT_RESOLVER_V &= ~(1<<SCLK_PORT_V)) 
#define SCLK_UP_V()                           (PORT_PORT_RESOLVER_V |= (1<<SCLK_PORT_V)) 

#endif

