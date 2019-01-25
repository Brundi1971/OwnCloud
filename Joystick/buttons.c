
#include "config.h"
#include "buttons.h"
#include "rs485_com.h"
#include "pc_com.h"

volatile U8 DRH_direction = NO_DIRECTION;
volatile U8 DRV_direction = NO_DIRECTION;
volatile U8 Flag_ForReadingButtonEvents;
volatile U8 Flag_PrezentaOperator = OPERATOR_INACTIV;          /* stocheaza prezenta operator */


                        //      o - gri                   o - mov
                        //      detonare capse            selectie tip foc
                        //
                        //      o - negru                 o - verde
                        //      trigger                   camera on/off
                        //
                        //
                        //      o - albastru              o - rosu
                        //      selectie tip camera       selectie functii

void init_porturi_joystick(void)
{
  DDR_PORT_DIRECTIE = 0xf0;         /* porturi de intrare */
  PORT_PORT_DIRECTIE = 0x0f;        /* pull-up resitor  */
  
  DDR_PORT_BUTOANE = 0x00;          /* porturi de intrare */
  PORT_PORT_BUTOANE = 0xff;         /* pull-up resitor  */ 
  
  DDR_PORT_BUTOANE_1 &= ~0xc0;      /* porturi de intrare */
  PORT_PORT_BUTOANE_1 |= 0xc0;      /* pull-up resitor  */ 
}


/* subrutina de citire a butoanelor joystick-ului si de creare a comenzilor pentru slave-uri*/
void get_Buttons_Joystick(void)
{
  register U8 c, c1, valPort;
  
  /* citirea starii comutatoarelor de directie */
  //==================================================================================================
  switch (PIN_PORT_DIRECTIE & ((1<<BUTTON_DRH_FORWARD) | (1<<BUTTON_DRH_BACKWARD)))
  {
      case ((1<<BUTTON_DRH_FORWARD) | (1<<BUTTON_DRH_BACKWARD)):  DRH_direction = NO_DIRECTION; break;
      case ((0<<BUTTON_DRH_FORWARD) | (1<<BUTTON_DRH_BACKWARD)):  DRH_direction = CW; break;
      case ((1<<BUTTON_DRH_FORWARD) | (0<<BUTTON_DRH_BACKWARD)):  DRH_direction = CCW; break;
      default:                                                    DRH_direction = NO_DIRECTION;
  }
  
   switch (PIN_PORT_DIRECTIE & ((1<<BUTTON_DRV_FORWARD) | (1<<BUTTON_DRV_BACKWARD)))
  {
      case ((1<<BUTTON_DRV_FORWARD) | (1<<BUTTON_DRV_BACKWARD)):  DRV_direction = NO_DIRECTION; break;
      case ((0<<BUTTON_DRV_FORWARD) | (1<<BUTTON_DRV_BACKWARD)):  DRV_direction = CW; break;
      case ((1<<BUTTON_DRV_FORWARD) | (0<<BUTTON_DRV_BACKWARD)):  DRV_direction = CCW; break;
      default:                                                    DRV_direction = NO_DIRECTION;
  }


  /* citire stare butoane joystick */
  //==================================================================================================
  c = 0;  c1 = 0;

  valPort = PIN_PORT_BUTOANE_1;
  
  if (!(valPort & (1<<BUTTON_OPERATOR))) 
  {
    Status_Comunicatie |= (1<<BIT_OPERATOR_PRESENCE);
    Flag_PrezentaOperator = OPERATOR_ACTIV;
  }
  else 
  {
    Status_Comunicatie &= ~(1<<BIT_OPERATOR_PRESENCE);
    Flag_PrezentaOperator = OPERATOR_INACTIV;
  }

  valPort = PIN_PORT_BUTOANE;

  if (!(valPort & (1<<BUTTON_TRIGGER)))                    c |= (1<<BIT_COMANDA_TRIGGER);
  if (!(valPort & (1<<BUTTON_SELECTIE_FOC)))               c |= (1<<BIT_COMANDA_SELECTIE_FOC);
  if (!(valPort & (1<<BUTTON_DETONARE_CAPSE)))             c |= (1<<BIT_COMANDA_DETONARE_CAPSE);

  if (!(valPort & (1<<BUTTON_INCREASE)))                  c1 |= (1<<BIT_COMANDA_INCREASE);
  if (!(valPort & (1<<BUTTON_DECREASE)))                  c1 |= (1<<BIT_COMANDA_DECREASE);
  if (!(valPort & (1<<BUTTON_SELECTIE_FUNCTII_CAMERA)))   c1 |= (1<<BIT_COMANDA_SELECTIE_ZOOM_FOCUS_AGC);
  if (!(valPort & (1<<BUTTON_SELECTIE_SPEED)))            c1 |= (1<<BIT_COMANDA_SELECTIE_SPEED);
  if (!(valPort & (1<<BUTTON_SELECTIE_TIP_CAMERA)))       c1 |= (1<<BIT_COMANDA_SELECT_CAMERA_TYPE);

  valPort = PIN_PORT_BUTOANE_1;

  if (!(valPort & (1<<BUTTON_CAMERA_ON_OFF)))             c1 |= (1<<BIT_COMANDA_CAMERA_ON_OFF);


  switch(c)
  {
    case (1<<BIT_COMANDA_TRIGGER):
    case (1<<BIT_COMANDA_SELECTIE_FOC):
    case (1<<BIT_COMANDA_DETONARE_CAPSE):            break;
    default:                                         c = 0;
  }

  switch (c1)
  {
    case (1<<BIT_COMANDA_INCREASE):
    case (1<<BIT_COMANDA_DECREASE):
    case (1<<BIT_COMANDA_SELECTIE_ZOOM_FOCUS_AGC):
    case (1<<BIT_COMANDA_SELECTIE_SPEED):
    case (1<<BIT_COMANDA_SELECT_CAMERA_TYPE):
    case (1<<BIT_COMANDA_CAMERA_ON_OFF):             break;
    default:                                         c1 = 0;
  }
  
  //if (Flag_PrezentaOperator == OPERATOR_INACTIV)          {c = 0; c1 = 0;}

  //if (c1 != 0) LED_OFF();

  set_Comanda_CAM(0,c);
  set_Comanda_CAM(1,c1);
}


U8 get_DRH_direction(void)               {return DRH_direction;}
U8 get_DRV_direction(void)               {return DRV_direction;}
U8 get_Flag_PrezentaOperator(void)       {return Flag_PrezentaOperator;}
