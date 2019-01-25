
#include "config.h"
#include "adc_drv.h"
#include "pc_com.h"
#include "buttons.h"

volatile U8 Flag_IT_ADC = 0;
volatile U8 Flag_Start_ADC = 0;
volatile U8 Potentiometru_H;
volatile U8 Potentiometru_V;
volatile U8 Memo_Speed_H;
volatile U8 Memo_Speed_V;
volatile U8 SpeedAndDirection_H = 0;
volatile U8 SpeedAndDirection_V = 0;
volatile U8 Status_MUX_ADC;

/* subrutina de tratare a intreruperii de ADC */
ISR(ADC_vect)
{
   Flag_IT_ADC = 1;
}

/* subrutina de initializare a convertorului A/D */
void init_ADC(void)
{ 
   /* Digital Input Disable Register 0 */
   DIDR0 = 0;
   DIDR0 |= (1<<ADC_H) | (1<<ADC_V);
   
   /* init ADC */
   Enable_avcc_vref();
   Left_adjust_adc_result();
   Free_adc_runing();
   Set_prescaler(ADC_PRESCALER_128); /* Tck = 62,5ns pentru f=16MHz => 
                                        perioada de esantionare a ADC-ului este de 8us
                                        perioada de conversie este de 13 * 8us = 104us 
                                        trebuie modificat PRESCALER-ul*/
   Enable_adc_it();
   Enable_adc();
   Start_conv_channel(ADC_H);
   Status_MUX_ADC = ADC_H;
   Memo_Speed_H = ZERO_SPEED;
   Memo_Speed_V = ZERO_SPEED;
}


/* subrutina de citire a potentiometrelor joystick-ului */
void read_ADC(void)
{ 
    if (Flag_IT_ADC)
    {
      switch(Status_MUX_ADC)
      {
          case ADC_H:
                          Potentiometru_H = Adc_get_8_bits_result(); 
                          if (Flag_PrezentaOperator == OPERATOR_ACTIV) set_SpeedAndDirection_H(speedHisterezis_H());
                          else set_SpeedAndDirection_H(0);
                          //Status_MUX_ADC=ADC_INPUT_ADC1;
                          //Start_conv_channel(ADC_INPUT_ADC1);
                          break;
          case ADC_V:
                          Potentiometru_V = Adc_get_8_bits_result();
                          if (Flag_PrezentaOperator == OPERATOR_ACTIV) set_SpeedAndDirection_V(speedHisterezis_V());
                          else set_SpeedAndDirection_V(0);
                          //Status_MUX_ADC=ADC_INPUT_ADC0;
                          //Start_conv_channel(ADC_INPUT_ADC0);
                           break;
      }
      Flag_Start_ADC = 1; /* fiecare conversie se realizeaza la 20ms*/
      Flag_IT_ADC=0;
    }
}


/* functia de histerezis pentru rezultatul citit de pe potentiometrul H*/
U8 speedHisterezis_H(void) 
{
  register U8 Direction, Speed;
  
  Speed = 0; 
     
  if (Potentiometru_H >= Memo_Speed_H )
  {
    if (Potentiometru_H >= UP_TOGGLE_SPEED_2_H) Memo_Speed_H = UP_TOGGLE_SPEED_2_H;
    if (Potentiometru_H >= UP_TOGGLE_SPEED_3_H) Memo_Speed_H = UP_TOGGLE_SPEED_3_H;
    if (Potentiometru_H >= UP_TOGGLE_SPEED_4_H) Memo_Speed_H = UP_TOGGLE_SPEED_4_H;
    if (Potentiometru_H >= UP_TOGGLE_SPEED_5_H) Memo_Speed_H = UP_TOGGLE_SPEED_5_H;
    if (Potentiometru_H >= UP_TOGGLE_SPEED_6_H) Memo_Speed_H = UP_TOGGLE_SPEED_6_H;
    if (Potentiometru_H >= UP_TOGGLE_SPEED_7_H) Memo_Speed_H = UP_TOGGLE_SPEED_7_H;
    if (Potentiometru_H >= UP_TOGGLE_SPEED_8_H) Memo_Speed_H = UP_TOGGLE_SPEED_8_H;
    if (Potentiometru_H >= UP_TOGGLE_SPEED_9_H) Memo_Speed_H = UP_TOGGLE_SPEED_9_H;
  }
  else
  {
    if (Potentiometru_H < DOWN_TOGGLE_SPEED_9_H) Memo_Speed_H = UP_TOGGLE_SPEED_8_H;
    if (Potentiometru_H < DOWN_TOGGLE_SPEED_8_H) Memo_Speed_H = UP_TOGGLE_SPEED_7_H;
    if (Potentiometru_H < DOWN_TOGGLE_SPEED_7_H) Memo_Speed_H = UP_TOGGLE_SPEED_6_H;
    if (Potentiometru_H < DOWN_TOGGLE_SPEED_6_H) Memo_Speed_H = UP_TOGGLE_SPEED_5_H;
    if (Potentiometru_H < DOWN_TOGGLE_SPEED_5_H) Memo_Speed_H = UP_TOGGLE_SPEED_4_H;
    if (Potentiometru_H < DOWN_TOGGLE_SPEED_4_H) Memo_Speed_H = UP_TOGGLE_SPEED_3_H;
    if (Potentiometru_H < DOWN_TOGGLE_SPEED_3_H) Memo_Speed_H = UP_TOGGLE_SPEED_2_H;
    if (Potentiometru_H < DOWN_TOGGLE_SPEED_2_H) Memo_Speed_H = UP_TOGGLE_SPEED_1_H;
  }
  
  switch(Memo_Speed_H)
  {
    case UP_TOGGLE_SPEED_9_H:   Speed = SPEED_9;break;
    case UP_TOGGLE_SPEED_8_H:   Speed = SPEED_8;break;
    case UP_TOGGLE_SPEED_7_H:   Speed = SPEED_7;break;
    case UP_TOGGLE_SPEED_6_H:   Speed = SPEED_6;break;
    case UP_TOGGLE_SPEED_5_H:   Speed = SPEED_5;break;
    case UP_TOGGLE_SPEED_4_H:   Speed = SPEED_4;break;
    case UP_TOGGLE_SPEED_3_H:   Speed = SPEED_3;break;
    case UP_TOGGLE_SPEED_2_H:   Speed = SPEED_2;break;
    case UP_TOGGLE_SPEED_1_H:   Speed = SPEED_1;break;
  }
  
   switch(get_DRH_direction())
  {
      case NO_DIRECTION:      Speed = SPEED_0; Direction = 0x00;break;
      case CCW:               Direction = 0x00; if (Speed == 0) Speed = SPEED_1; break;
      case CW:                Direction = 0x10; if (Speed == 0) Speed = SPEED_1; break;
      default:                return Speed = 0;
  }
  
  return (Speed | Direction | get_Flag_PrezentaOperator());
}

/* functia de histerezis pentru rezultatul citit de pe potentiometrul V*/
U8 speedHisterezis_V(void) 
{
 register U8 Direction, Speed;
  
  Speed = 0; 
     
  if (Potentiometru_V >= Memo_Speed_V)
  {
    if (Potentiometru_V >= UP_TOGGLE_SPEED_2_V) Memo_Speed_V = UP_TOGGLE_SPEED_2_V;
    if (Potentiometru_V >= UP_TOGGLE_SPEED_3_V) Memo_Speed_V = UP_TOGGLE_SPEED_3_V;
    if (Potentiometru_V >= UP_TOGGLE_SPEED_4_V) Memo_Speed_V = UP_TOGGLE_SPEED_4_V;
    if (Potentiometru_V >= UP_TOGGLE_SPEED_5_V) Memo_Speed_V = UP_TOGGLE_SPEED_5_V;
    if (Potentiometru_V >= UP_TOGGLE_SPEED_6_V) Memo_Speed_V = UP_TOGGLE_SPEED_6_V;
    if (Potentiometru_V >= UP_TOGGLE_SPEED_7_V) Memo_Speed_V = UP_TOGGLE_SPEED_7_V;
    if (Potentiometru_V >= UP_TOGGLE_SPEED_8_V) Memo_Speed_V = UP_TOGGLE_SPEED_8_V;
    if (Potentiometru_V >= UP_TOGGLE_SPEED_9_V) Memo_Speed_V = UP_TOGGLE_SPEED_9_V;
  }
  else
  {
    if (Potentiometru_V < DOWN_TOGGLE_SPEED_9_V) Memo_Speed_V = UP_TOGGLE_SPEED_8_V;
    if (Potentiometru_V < DOWN_TOGGLE_SPEED_8_V) Memo_Speed_V = UP_TOGGLE_SPEED_7_V;
    if (Potentiometru_V < DOWN_TOGGLE_SPEED_7_V) Memo_Speed_V = UP_TOGGLE_SPEED_6_V;
    if (Potentiometru_V < DOWN_TOGGLE_SPEED_6_V) Memo_Speed_V = UP_TOGGLE_SPEED_5_V;
    if (Potentiometru_V < DOWN_TOGGLE_SPEED_5_V) Memo_Speed_V = UP_TOGGLE_SPEED_4_V;
    if (Potentiometru_V < DOWN_TOGGLE_SPEED_4_V) Memo_Speed_V = UP_TOGGLE_SPEED_3_V;
    if (Potentiometru_V < DOWN_TOGGLE_SPEED_3_V) Memo_Speed_V = UP_TOGGLE_SPEED_2_V;
    if (Potentiometru_V < DOWN_TOGGLE_SPEED_2_V) Memo_Speed_V = UP_TOGGLE_SPEED_1_V;
  }
  
  switch(Memo_Speed_V)
  {
    case UP_TOGGLE_SPEED_9_V:   Speed = SPEED_9;break;
    case UP_TOGGLE_SPEED_8_V:   Speed = SPEED_8;break;
    case UP_TOGGLE_SPEED_7_V:   Speed = SPEED_7;break;
    case UP_TOGGLE_SPEED_6_V:   Speed = SPEED_6;break;
    case UP_TOGGLE_SPEED_5_V:   Speed = SPEED_5;break;
    case UP_TOGGLE_SPEED_4_V:   Speed = SPEED_4;break;
    case UP_TOGGLE_SPEED_3_V:   Speed = SPEED_3;break;
    case UP_TOGGLE_SPEED_2_V:   Speed = SPEED_2;break;
    case UP_TOGGLE_SPEED_1_V:   Speed = SPEED_1;break;
  }
  
   switch(get_DRV_direction())
  {
      case NO_DIRECTION:      Speed = SPEED_0; Direction = 0x00;break;
      case CCW:               Direction = 0x00; if (Speed == 0) Speed = SPEED_1; break;
      case CW:                Direction = 0x10; if (Speed == 0) Speed = SPEED_1; break;
      default:                return Speed = 0;
  }
  
  return (Speed | Direction | get_Flag_PrezentaOperator());
}


U8 get_SpeedAndDirection_H(void)        {return SpeedAndDirection_H;}
void set_SpeedAndDirection_H(U8 nb)     {SpeedAndDirection_H = nb;}
                        
U8 get_SpeedAndDirection_V(void)        {return SpeedAndDirection_V;}
void set_SpeedAndDirection_V(U8 nb)     {SpeedAndDirection_V = nb;}
