
#include "config.h"
#include "mc_drv_highSpeed.h"
#include "mc_drv_lowSpeed.h"
#include "mc_lib.h"
#include "adc_drv.h"
#include "dac_drv.h"
#include "pll_drv.h"
#include "mc_rs485.h"
#include "mc_resolver.h"
#include "mc_inputuri.h"

/********************************************************************************************/
/*                            Hardware Initialization                                       */
/********************************************************************************************/
void mc_init_HW(void)
{
  mc_init_port();
  init_dac();
  mc_set_Over_Current(20); 
                                       // 10 - referinta comparator = 200mV
                                       // 25 - referinta comparator = 500mV
                                       // 51 - referinta comparator = 1V
                                       // 102 - refeinta comparator = 2V
  Start_pll_64_mega();
  Wait_pll_ready();
}

void mc_init_port(void)
{

  /*PSCOUT Configuration */
  DDR_MC_DRIVER_B |= ((1<<L_A) | (1<<L_B) | (1<<H_C) | (1<<L_C));
  DDR_MC_DRIVER_C |= (1<<H_B);
  DDR_MC_DRIVER_D |= (1<<H_A);

  /* Activation of MOSFET Drivers */
  PORT_MC_DRIVER_B &= ~(1<<H_C);
  PORT_MC_DRIVER_C &= ~(1<<H_B);
  PORT_MC_DRIVER_D &= ~(1<<H_A);
  PORT_MC_DRIVER_B |= ((1<<L_A) | (1<<L_B) | (1<<L_C));
}

// PSC initialization depend on the PSC mode
//  0- One ramp Mode
//  1- Two ramp Mode
//  2- Four ramp Mode
//  3- Center Aligned Mode

void PSC_Init ( U16 OCRnRB,
                U16 OCRnSB,
                U16 OCRnRA,
                U16 OCRnSA)
{
   Set_none();
  //PSC0
  OCR0SAH = MSB(OCRnSA);
  OCR0SAL = LSB(OCRnSA);
  OCR0RAH = MSB(OCRnRA);
  OCR0RAL = LSB(OCRnRA);
  OCR0SBH = MSB(OCRnSB);
  OCR0SBL = LSB(OCRnSB);
  OCR0RBH = MSB(OCRnRB);
  OCR0RBL = LSB(OCRnRB);

  PCNF0 =  RAMP_MODE_NUMBER | (1<<PCLKSEL0) | OUTPUT_ACTIVE_HIGH ;
  PFRC0A = 0;
  //PFRC0A = (1<<PELEV0A)|(1<<PFLTE0A)|(0<<PRFM0A3)|(1<<PRFM0A2)|(1<<PRFM0A1)|(1<<PRFM0A0); /* overcurrent*/
  PFRC0B = 0;
  PSOC0 = (1<<PSYNC00); //Send signal on match with OCRnSA (during counting up of PSC)
  PCTL0 = (0<<PAOC0A)|(1<<PARUN0)|PRESC_NODIV; /* AUTORUN !! */
  //PSC1
  OCR1SAH = MSB(OCRnSA);
  OCR1SAL = LSB(OCRnSA);
  OCR1RAH = MSB(OCRnRA);
  OCR1RAL = LSB(OCRnRA);
  OCR1SBH = MSB(OCRnSB);
  OCR1SBL = LSB(OCRnSB);
  OCR1RBH = MSB(OCRnRB);
  OCR1RBL = LSB(OCRnRB);

  PCNF1 =  RAMP_MODE_NUMBER | (1<<PCLKSEL1) | OUTPUT_ACTIVE_HIGH ;
  PFRC1A = 0;
  PFRC1B = 0;
  PCTL1 = (0<<PAOC1A)|(1<<PARUN1)|PRESC_NODIV; /* AUTORUN !! */
  //PSC2
  OCR2SAH = MSB(OCRnSA);
  OCR2SAL = LSB(OCRnSA);
  OCR2RAH = MSB(OCRnRA);
  OCR2RAL = LSB(OCRnRA);
  OCR2SBH = MSB(OCRnSB);
  OCR2SBL = LSB(OCRnSB);
  OCR2RBH = MSB(OCRnRB);
  OCR2RBL = LSB(OCRnRB);

  PCNF2 =  RAMP_MODE_NUMBER | (1<<PCLKSEL2) | OUTPUT_ACTIVE_HIGH ;
  PFRC2A = 0;
  PFRC2B = 0;
  //PCTL2 = (0<<PAOC2A)|(1<<PRUN2)|PRESC_NODIV; /* RUN !! */
  
}

/* functia de incarcare a modulatoarelor PWM cu noua valoare */ 
void mc_duty_cycle(U16 level)
{
  U16 duty;
  duty = level;

  PCNF0 = SET_PLOCK | RAMP_MODE_NUMBER |(1<<PCLKSEL0)|(1<<POP0); /* set plock */
  PCNF1 = SET_PLOCK | RAMP_MODE_NUMBER |(1<<PCLKSEL1)|(1<<POP1); /* set plock */
  PCNF2 = SET_PLOCK | RAMP_MODE_NUMBER |(1<<PCLKSEL2)|(1<<POP2); /* set plock */
  
  OCR0SAH = MSB(duty);
  OCR0SAL = LSB(duty);
  OCR1SAH = MSB(duty);
  OCR1SAL = LSB(duty);
  OCR2SAH = MSB(duty);
  OCR2SAL = LSB(duty);
  /*
  OCR0SAH = 0;
  OCR0SAL = duty;
  OCR1SAH = 0;
  OCR1SAL = duty;
  OCR2SAH = 0;
  OCR2SAL = duty;
  */
  OCR0SBH = 0;
  OCR0SBL = 0;
  OCR1SBH = 0;
  OCR1SBL = 0;
  OCR2SBH = 0;
  OCR2SBL = 0;

  //Disable_interrupt();
  PCNF0 = RELEASE_PLOCK | RAMP_MODE_NUMBER |(1<<PCLKSEL0)|(1<<POP0); /* release plock */
  PCNF1 = RELEASE_PLOCK | RAMP_MODE_NUMBER |(1<<PCLKSEL1)|(1<<POP1); /* release plock */
  PCNF2 = RELEASE_PLOCK | RAMP_MODE_NUMBER |(1<<PCLKSEL2)|(1<<POP2); /* release plock */
  //Enable_interrupt();
}

/* functia de preparare a modulatoarelor PSC pentru viteaza mare
   si setarea protectiei de curent */ 
void mc_prepare_for_highSpeed(void)
{
  //set_IndexZero_HighSpeed(INDEX_ZERO_HIGH_SPEED);
  Set_none();
  mc_set_Over_Current(DAC_400mV);
  PFRC0A = (1<<PELEV0A)|(1<<PFLTE0A)|(0<<PRFM0A3)|(1<<PRFM0A2)|(1<<PRFM0A1)|(1<<PRFM0A0); /* overcurrent*/
  PFRC0A = 0;
}

/**************************************************************************************/
/*                       Sampling time configuration                                  */
/**************************************************************************************/

U8 mc_highSpeed(U8 position)
{
  register U8 direction = mc_get_motor_direction();

  //if (check_for_vertical_limit() == TRUE) {mc_motor_stop(); return FALSE;}
  
    Set_none();
    asm("nop");  /* intarziere de 500ns */
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    switch(position)
    {
      case 1:     if (direction)  {Set_B_A();}
                  else            {Set_A_B();}
                  break;
      case 2:     if (direction)  {Set_C_A();}
                  else            {Set_A_C();}
                  break;
      case 3:     if (direction)  {Set_C_B();}
                  else            {Set_B_C();}
                  break;
      case 4:     if (direction)  {Set_A_B();}
                  else            {Set_B_A();}
                  break;
      case 5:     if (direction)  {Set_A_C();}
                  else            {Set_C_A();}
                  break;
      case 6:     if (direction)  {Set_B_C();}
                  else            {Set_C_B();}
                  break;
      default:    Set_none();break;
    }
	return TRUE;
}

U8 check_for_vertical_limit(void)
{
  register U8 direction = mc_get_motor_direction();

  if (((direction == CW) && ((PIN_PORT_SENZORI) && (1 << PIN_SENZOR_LIMITA_V_SUPERIOARA))) ||
     ((direction == CCW) && ((PIN_PORT_SENZORI) && (1 << PIN_SENZOR_LIMITA_V_INFERIOARA))))
	 {return FALSE;}
  else {return TRUE;}
}



