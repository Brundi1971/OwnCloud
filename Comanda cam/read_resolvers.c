
#include "config.h"
#include "read_resolvers.h"
#include "rs485_com.h"
#include "rs232_com.h"

volatile U16 Data_Resolver_H;
volatile U16 Data_Resolver_V;
/* variabile pentru calculul vitezei*/
volatile U16 Memo_Data_Resolver_H = 0;
volatile U16 Memo_Data_Resolver_V = 0;
volatile U8 Speed_H;
volatile U8 Speed_V;
volatile U16 Contor_CalculateSpeed;

/* functii de sincronizare a variabilelor Data_Resolver_H si Data_Resolver_V */ 
U16 get_Data_Resolver_H(void)                 {return Data_Resolver_H;}
void set_Data_Resolver_H(U16 nb)              {Data_Resolver_H = nb;}
U16 get_Data_Resolver_V(void)                 {return Data_Resolver_V;}
void set_Data_Resolver_V(U16 nb)              {Data_Resolver_V = nb;}

/* functie initializare porturi resolvere */
void init_resolver_ports(void)
{
  Set_Ports_Resolver_H();
  Set_Ports_Resolver_V();
  CS_UP_H();
  SCLK_UP_H();
  CS_UP_V();
  SCLK_UP_V();
  //DDRA = 0x0f;
  //PORTA |= 0xf0;
}

void init_remainig_ports(void)
{
   DDR_PORT_RESOLVER_H &= ~((1 << PIN_INPUT_PINB0) | (1 << PIN_INPUT_PINB1) | (1 << PIN_INPUT_PINB5) | (1 << PIN_INPUT_PINB6) | (1 << PIN_INPUT_PINB7));
   PORT_PORT_RESOLVER_H |= ((1 << PIN_INPUT_PINB0) | (1 << PIN_INPUT_PINB1) | (1 << PIN_INPUT_PINB5) | (1 << PIN_INPUT_PINB6) | (1 << PIN_INPUT_PINB7));
  
   DDR_PORT_RESOLVER_V &= ~((1 << PIN_INPUT_PINA0) | (1 << PIN_INPUT_PINA1) | (1 << PIN_INPUT_PINA2) | (1 << PIN_INPUT_PINA3));
   PORT_PORT_RESOLVER_V |= ((1 << PIN_INPUT_PINA0) | (1 << PIN_INPUT_PINA1) | (1 << PIN_INPUT_PINA2) | (1 << PIN_INPUT_PINA3));
}

/* functie citire rezolver pozitie orizontala */
void read_resolver_H(void)
{
  register U8 i, c,tempData;
 
  CS_DOWN_H();
  tempData = 0;
  i = 0;
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  
  do{
      SCLK_DOWN_H();
      asm("nop");
      asm("nop");
      tempData = tempData << 1;
      c = PIN_PORT_RESOLVER_H;
      if (c & (1<<DATA_PORT_H)) tempData |= 0x01;
      asm("nop");
      SCLK_UP_H();
      asm("nop");
      asm("nop");
      i++;
  }while(i!=NO_MAX_BITI_TO_BE_READ_FROM_H);

  CS_UP_H();  
  set_Data_Resolver_H(tempData);
}

/* functie citire rezolver pozitie verticala */
void read_resolver_V(void)
{
  register U8 i, c, tempData;
 
  CS_DOWN_V();
  tempData = 0;
  i = 0;
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  
  do{
      SCLK_DOWN_V();
      asm("nop");
      asm("nop");
      tempData = tempData << 1;
      c = PIN_PORT_RESOLVER_V;
      if (c & (1<<DATA_PORT_V)) tempData |= 0x01;
      asm("nop");
      SCLK_UP_V();
      asm("nop");
      asm("nop");
      i++;
  }while(i!=NO_MAX_BITI_TO_BE_READ_FROM_V);

  CS_UP_V();  
  set_Data_Resolver_V(tempData);
}

/* subrutina pentru calculul vitezei unghiulare a turelei */
U16 calculate_speed_H(void)
{
  register U16 c;
  
  if (get_Data_Resolver_H() >= Memo_Data_Resolver_H)
        {c = get_Data_Resolver_H() - Memo_Data_Resolver_H;}
  else  {c = Memo_Data_Resolver_H - get_Data_Resolver_H();}
 
  Memo_Data_Resolver_H = get_Data_Resolver_H();
  
  if (c > 200) {c = 1024 - c;}
  
  return (U16)((U32)(c*60)/1024);
}

U16 get_Speed_H(void)          {return Speed_H;}
void set_Speed_H(U16 nb)       {Speed_H = nb;}
