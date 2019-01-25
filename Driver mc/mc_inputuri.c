
#include "config.h"
#include "mc_inputuri.h"

volatile U8 ADRESA_MODUL_DRIVER_MOTOR;
volatile U8 Status_Senzori = 0;
volatile U8 temporar;

/*
                              ========================
                      H_A   ||1 - PD0         PB7 - 32||   L_A
                      H_B   ||2 - PC0         PB6 - 31||   L_B
                    RESET   ||3 - PE0         PB5 - 30||   ADRESA_0
              OverCurrent   ||4 - PD1         PC7 - 29||   DAC
                     MISO   ||5 - PD2         PB4 - 28||   ADRESA_1
                 MOSI-TXD   ||6 - PD3         PB3 - 27||   Test resolver
                CTL RS485   ||7 - PC1         PC6 - 26||   CS_RESOLVER
                            ||8 - VCC        AREF - 25||
                            ||9 - GND        AGND - 24||
             Limit_V-High   ||10 - PC2       AVCC - 23||
              Limit_V-Low   ||11 - PC3        PC5 - 22||   V_Shunt+ (ADC12)
                      H_C   ||12 - PB0        PC4 - 21||   V_Shunt- (ADC12)
                      L_C   ||13 - PB1        PB2 - 20||   Termistor (AD5)
                    XTAL1   ||14 - PE1        PD7 - 19||   CLOCK_RESOLVER
                    XTAL2   ||15 - PE2        PD6 - 18||   Divizor rezitiv - citire tensiune de alimentare sistem (ADC3)
                  SCK-RXD   ||16 - PD4        PD5 - 17||   DATA_RESOLVER
                              ========================
*/

/* functia de initializare a portului prin care se citeste adresa modulului */
void init_porturi_adresa(void)
{
  DDR_PORT_SET_ADRESA &= ~((1 << PIN_ADRESA_0) | (1 << PIN_ADRESA_1)); /* porturi de intrare */
  PORT_PORT_SET_ADRESA |= ((1 << PIN_ADRESA_0) | (1 << PIN_ADRESA_1)); /* pull-up resitor */
}

void read_ADDRESS(void)
{
 if(PIN_PORT_SET_ADRESA & (1 << PIN_ADRESA_0)) ADRESA_MODUL_DRIVER_MOTOR = 1;
 else ADRESA_MODUL_DRIVER_MOTOR = 0;
} 

/* functia de initializare a senzorilor externi */
void init_senzori_externi(void)
{
  DDR_PORT_SENZORI &= ~((1 << PIN_SENZOR_LIMITA_V_SUPERIOARA) | (1 << PIN_SENZOR_LIMITA_V_INFERIOARA));
  PORT_PORT_SENZORI |= ((1 << PIN_SENZOR_LIMITA_V_SUPERIOARA) | (1 << PIN_SENZOR_LIMITA_V_INFERIOARA));
}

/* functia de citire a senzorilor externi */
void pollingSenzoriExterni(void)
{
    register U8 c;
    
    c = (PIN_PORT_SENZORI & ((1 << PIN_SENZOR_LIMITA_V_SUPERIOARA) | (1 << PIN_SENZOR_LIMITA_V_INFERIOARA)));
    switch (c)
    {
      case (1 << PIN_SENZOR_LIMITA_V_SUPERIOARA): Status_Senzori |= (1 << BIT_SENZOR_LIMITA_V_SUPERIOARA);break; 
      case (1 << PIN_SENZOR_LIMITA_V_INFERIOARA): Status_Senzori |= (1 << BIT_SENZOR_LIMITA_V_INFERIOARA);break; 
      default: Status_Senzori = 0;
    }
}

void init_port_led(void)
{
   DDR_PORT_LED |= (1 << PIN_LED);
   PORT_PORT_LED |= (1 << PIN_LED); 
}

/* functii de sincronizare a variabilelor */
U8 get_Status_Senzori(void)             {return Status_Senzori;}
void set_Status_Senzori(U8 nb)          {Status_Senzori = nb;}

U8 get_temporar(void)                   {return temporar;}
void set_temporar(U8 temp)              {temporar = temp;}
