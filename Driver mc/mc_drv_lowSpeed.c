
#include "config.h"
#include "table_sin.h"
#include "mc_lib.h"
#include "mc_drv_highSpeed.h"
#include "mc_drv_lowSpeed.h"
#include "mc_rs485.h"
#include "mc_resolver.h"
#include "dac_drv.h"
#include "adc_drv.h"

const U16 Cmd_Speed_lowSpeed[6] PROGMEM = {VL0,VL1,VL2,VL3,VL4,VL5};

volatile U16  PWM_A, PWM_B, PWM_C;
volatile U8   Theta;
volatile U8   Sector_lowSpeed;
volatile U16  IndexZero_LowSpeed = 0;
volatile U8 Contor_Refresh_lowSpeed = 0;

void PSC_Load_lowSpeed ( U16 dt0a, U16 dt1a, 
                         U16 dt0b, U16 dt1b, 
                         U16 dt0c, U16 dt1c)
{
   PCNF0 = (1<<PLOCK0)|(1<<PMODE01) | (1<<PMODE00) | (1<<POP0) | (1<<PCLKSEL0);
   PCNF1 = (1<<PLOCK1)|(1<<PMODE11) | (1<<PMODE10) | (1<<POP1) | (1<<PCLKSEL1);
   PCNF2 = (1<<PLOCK2)|(1<<PMODE21) | (1<<PMODE20) | (1<<POP2) | (1<<PCLKSEL2);

   OCR0SAH = MSB(dt0a);
   OCR0SAL = LSB(dt0a);
   OCR0SBH = MSB(dt1a);
   OCR0SBL = LSB(dt1a);

   OCR1SAH = MSB(dt0b);
   OCR1SAL = LSB(dt0b);
   OCR1SBH = MSB(dt1b);
   OCR1SBL = LSB(dt1b);

   OCR2SAH = MSB(dt0c);
   OCR2SAL = LSB(dt0c);
   OCR2SBH = MSB(dt1c);
   OCR2SBL = LSB(dt1c);
    
   //Enable_interrupt();
   PCNF0 = (1<<PMODE01) | (1<<PMODE00) | (1<<POP0) | (1<<PCLKSEL0);
   PCNF1 = (1<<PMODE11) | (1<<PMODE10) | (1<<POP1) | (1<<PCLKSEL1);
   PCNF2 = (1<<PMODE21) | (1<<PMODE20) | (1<<POP2) | (1<<PCLKSEL2);
   //Disable_interrupt();
}

U16 get_IndexZero_LowSpeed(void)       {return IndexZero_LowSpeed;}
void set_IndexZero_LowSpeed(U16 nb)    {IndexZero_LowSpeed = nb;}


void mc_prepare_for_lowSpeed(void)
{
  register S16 tempPosition;
  
  Set_none();
  read_resolver();
  tempPosition = get_Data_Resolver() - get_IndexZero_LowSpeed();  //INDEX_ZERO_LOW_SPEED;//  get_IndexZero_LowSpeed();

  if (tempPosition < 0)                 tempPosition += NB_MAX_RESOLVER;
  //if (tempPosition > NB_MAX_RESOLVER)   tempPosition -= NB_MAX_RESOLVER;

  while (tempPosition >= NB_MAX_RESOLVER / NB_PERECHI_POLI)
  {
     tempPosition = tempPosition - NB_MAX_RESOLVER / NB_PERECHI_POLI;
  }

  if(tempPosition >= 0*STEP_MOTOR) {Sector_lowSpeed = 1;}
  if(tempPosition >= 1*STEP_MOTOR) {Sector_lowSpeed = 2;}
  if(tempPosition >= 2*STEP_MOTOR) {Sector_lowSpeed = 3;}
  if(tempPosition >= 3*STEP_MOTOR) {Sector_lowSpeed = 4;}
  if(tempPosition >= 4*STEP_MOTOR) {Sector_lowSpeed = 5;}
  if(tempPosition >= 5*STEP_MOTOR) {Sector_lowSpeed = 6;}

  Theta = (tempPosition - (U16) ((Sector_lowSpeed - 1) * STEP_MOTOR));  
  Theta = (U8)((U16) (Theta * MAX_THETA) / STEP_MOTOR);

  if (Theta == 0)           Theta = 1;
  if (Theta >= MAX_THETA)   Theta = MAX_THETA - 1;

  mc_set_Over_Current(DAC_3V);
  PFRC0A = (1<<PFLTE0A)|(0<<PRFM0A3)|(1<<PRFM0A2)|(1<<PRFM0A1)|(1<<PRFM0A0); /* overcurrent */
  PFRC0A = 0;
}


U8 mc_lowSpeed(void)
{
  U16    tau1, tau2 ;
  U8     theta1 , theta2 ;

  if (get_VoltageIndex() > 5) {mc_cmd_lowSpeed_V1 = pgm_read_word(&Cmd_Speed_lowSpeed[5]);}
  else {mc_cmd_lowSpeed_V1 = pgm_read_word(&Cmd_Speed_lowSpeed[get_VoltageIndex()]);}
  
  if (++Contor_Refresh_lowSpeed >= DELAY_OF_AMP_REFRESH_FOR_LOW_SPEED)
  {
    Contor_Refresh_lowSpeed = 0;
    if (mc_cmd_lowSpeed_V1 > mc_cmd_lowSpeed_V0)          {mc_cmd_lowSpeed_V0 += 2;}
    else if(mc_cmd_lowSpeed_V1 < mc_cmd_lowSpeed_V0)      {mc_cmd_lowSpeed_V0 -= 2;}
  } 

  //if (check_for_vertical_limit() == TRUE) {mc_motor_stop(); return FALSE;}

  if (PSCContorLowSpeed++ > DELAY_OF_THETA_REFRESH) 
  {
    PSCContorLowSpeed = 0;
  
    if (mc_get_motor_direction() == CCW)
    {
      Theta++;
      if(Theta == (MAX_THETA/2 - CLEARENCE_FOR_LOW_SPEED)) Theta = (MAX_THETA/2 + CLEARENCE_FOR_LOW_SPEED);
      if (Theta == MAX_THETA) 
      {
          Theta = 1;
          Sector_lowSpeed++;
          if (Sector_lowSpeed == 7) Sector_lowSpeed = 1;
      }
    }
    else
    {
      Theta--;
      if(Theta == (MAX_THETA/2 + CLEARENCE_FOR_LOW_SPEED)) Theta = (MAX_THETA/2 - CLEARENCE_FOR_LOW_SPEED);
      if (Theta == 0) 
      {
          Theta = MAX_THETA-1;
          Sector_lowSpeed--;
          if (Sector_lowSpeed == 0) Sector_lowSpeed = 6;
      }
    }
   
    theta2 = (U8) Theta ;
    theta1 = (U8)(MAX_THETA - theta2) ;
  
    tau1 = ((U32)mc_cmd_lowSpeed_V0 * pgm_read_byte(&tab_sin[theta1])) / 256 ;
    tau2 = ((U32)mc_cmd_lowSpeed_V0 * pgm_read_byte(&tab_sin[theta2])) / 256 ;

    //tau1 = (U32)mc_cmd_lowSpeed_V0 * theta1 / MAX_THETA ;
    //tau2 = (U32)mc_cmd_lowSpeed_V0 * theta2 / MAX_THETA ;

    switch (Sector_lowSpeed)
    {
      case 1 :
               PWM_A = (U16) (MAX_PWM/2 - tau1 - tau2) ;
               PWM_B = (U16) (MAX_PWM/2 + tau1 + tau2) ;
               PWM_C = (U16) (MAX_PWM/2 - tau1 + tau2) ; break ;
      case 2 :
               PWM_A = (U16) (MAX_PWM/2 - tau1 - tau2) ;
               PWM_B = (U16) (MAX_PWM/2 + tau1 - tau2) ;
               PWM_C = (U16) (MAX_PWM/2 + tau1 + tau2) ; break ;
      case 3 :
               PWM_A = (U16) (MAX_PWM/2 - tau1 + tau2) ;
               PWM_B = (U16) (MAX_PWM/2 - tau1 - tau2) ;
               PWM_C = (U16) (MAX_PWM/2 + tau1 + tau2) ; break ;
      case 4 :
               PWM_A = (U16) (MAX_PWM/2 + tau1 + tau2) ;
               PWM_B = (U16) (MAX_PWM/2 - tau1 - tau2) ;
               PWM_C = (U16) (MAX_PWM/2 + tau1 - tau2) ; break ;
      case 5 :
               PWM_A = (U16) (MAX_PWM/2 + tau1 + tau2) ;
               PWM_B = (U16) (MAX_PWM/2 - tau1 + tau2) ;
               PWM_C = (U16) (MAX_PWM/2 - tau1 - tau2) ; break ;
      case 6 :
               PWM_A = (U16) (MAX_PWM/2 + tau1 - tau2) ;
               PWM_B = (U16) (MAX_PWM/2 + tau1 + tau2) ;
               PWM_C = (U16) (MAX_PWM/2 - tau1 - tau2) ; break ;
      default :
               PWM_A = (U16) (MAX_PWM/2) ;
               PWM_B = (U16) (MAX_PWM/2) ;
               PWM_C = (U16) (MAX_PWM/2) ; break ;
    }
    PSC_Load_lowSpeed (PWM_A, PWM_A+DEAD_TIME, PWM_B, PWM_B+DEAD_TIME, PWM_C, PWM_C+DEAD_TIME);
  }
  return TRUE;
}
