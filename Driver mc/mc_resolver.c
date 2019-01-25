
#include "config.h"
#include "mc_resolver.h"
#include "mc_rs485.h"
#include "mc_drv_highSpeed.h"
#include "mc_drv_lowSpeed.h"
#include "mc_lib.h"
#include "mc_inputuri.h"
#include "adc_drv.h"
#include <avr\eeprom.h>
#include <string.h>


const U16 Cmd_Speed[6][14] PROGMEM = 
{
    {V02,V03,V04,V05,V06,V07,V08,V09,V010,V011,V012,V013,V014,V015},
    {V12,V13,V14,V15,V16,V17,V18,V19,V110,V111,V112,V113,V114,V115},
    {V22,V23,V24,V25,V26,V27,V28,V29,V210,V211,V212,V213,V214,V215},
    {V32,V33,V34,V35,V36,V37,V38,V39,V310,V311,V312,V313,V314,V315},
    {V42,V43,V44,V45,V46,V47,V48,V49,V410,V411,V412,V413,V414,V415},
    {V52,V53,V54,V55,V56,V57,V58,V59,V510,V511,V512,V513,V514,V515},
};


volatile U8 Flag_CheckForTesting;
volatile U16 WaitingContorForTestResolver;

volatile U16 Data_Resolver;
volatile U16 AdresaEEPROM;
volatile U16 DataToBeStoredInEEPROM; 
volatile U16 IndexZero_HighSpeed;
volatile U8 Memo_SectorResolver = 0;
volatile U8 nbOfTest;
volatile U8 Contor_Refresh_highSpeed = 0;
volatile U8 Status_Motor = 0;
volatile U16 Contor_flashing_led = 0;
volatile U8 IndexProgramed = NO_PROGRAMED; 
volatile U8 Switch_OnOff_LED = OK;
volatile U8 Flag_ForStartReadingIndex;
volatile U16 Cronometru_ForReadingIndex = START_READING_PROCEDURE;


void mc_config_sampling_period(void)
{
  TCCR1A = 0;                                   //Normal port operation + Mode CTC
  TCCR1B = (1 << WGM12) | (1 << CS11) ;         // Mode CTC + prescaler 64 (4us)
  TCCR1C = 0;
  OCR1AH = 0;
  OCR1AL = 100;                                 //25
  TIMSK1 = (1<<OCIE1A);                         // Output compare B Match interrupt Enable
}

void init_resolver(void)
{
  Set_BUTTON_TEST_RESOLVER();
  Flag_CheckForTesting = NO_TESTING;
  Set_Ports_Resolver();
  CS_UP();
  SCLK_UP();
  set_IndexZero_LowSpeed(0);
  set_IndexZero_HighSpeed(0);
}


void check_for_test_resolver(void)
{ 
  /* pushbutton apasat */
  if (!(PORT_TEST_RESOLVER & (1<<PIN_TEST_RESOLVER)) && (Flag_CheckForTesting == NO_TESTING))
  {
     switch(Flag_ForStartReadingIndex)
     {
        case START_READING_PROCEDURE:
                                              Cronometru_ForReadingIndex = 0;
                                              Flag_ForStartReadingIndex = WAIT_2s;
                                              LED_OFF();
                                              break;
        case WAIT_2s:
                                              if (Cronometru_ForReadingIndex > 60000)  /* 2s*/
                                              {
                                                 Cronometru_ForReadingIndex = 0;
                                                 Flag_ForStartReadingIndex = WAIT_05s;
                                              }
                                              break;
        case WAIT_05s:
                                              Cronometru_ForReadingIndex = 0;
                                              break;
        case WAIT_05s_bis:
                                              Flag_ForStartReadingIndex = START_READING_INDEX;
                                              break;
        case START_READING_INDEX:
                                              //Set_BUTTON_TEST_RESOLVER();
                                              Flag_CheckForTesting = TESTING;
                                              Status_Motor = START_TEST_RESOLVER;
                                              //No_Sector = 1;
                                              AdresaEEPROM = 0;
                                              nbOfTest = 0;
                                              LED_ON();
                                              break;
        default:
                                              Flag_ForStartReadingIndex = START_READING_PROCEDURE;
                                              break;
      }
  }
  else  /* pushbutton eliberat */
  {
     switch(Flag_ForStartReadingIndex)
     {
        case START_READING_PROCEDURE:
                                              break;
        case WAIT_2s:
                                              Flag_ForStartReadingIndex = START_READING_PROCEDURE;
                                              break;
        case WAIT_05s:
        case WAIT_05s_bis:
                                              if (Cronometru_ForReadingIndex > 20000)  /* 1s */
                                              {
                                                 Flag_ForStartReadingIndex = START_READING_PROCEDURE;
                                              }
                                              else
                                              {
                                                 Flag_ForStartReadingIndex = WAIT_05s_bis;
                                              }
                                              break;
        case START_READING_INDEX:
                                              Flag_ForStartReadingIndex = START_READING_PROCEDURE;
                                              break;
        default:
                                              Flag_ForStartReadingIndex = START_READING_PROCEDURE;
                                              break;
     }
  }
}


void read_resolver(void)
{
  register U8 i;
  register U16 tempData;
 
  CS_DOWN();
  tempData = 0;
  i = 0;

  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  
  do{
      SCLK_DOWN();
      tempData = tempData << 1;
      if (PIN_PORT_RESOLVER_1 & (1<<DATA_PORT)) tempData |= 0x0001;
      //asm("nop");
      SCLK_UP();
      i++;
  }while(i!=NO_MAX_BITI_TO_BE_READ);

  CS_UP();  
  set_Data_Resolver(tempData);
}

U16 get_Data_Resolver(void)       {return Data_Resolver;}
void set_Data_Resolver(U16 nb)    {Data_Resolver = nb;}

void write_eeprom_word(U16 adresa_eeprom, U16 date)
{
   while(!eeprom_is_ready());
   EEAR = adresa_eeprom, EEDR = MSB(date), EECR |= (1<<EEMWE), EECR |= (1<<EEWE);
   adresa_eeprom++;
   while(!eeprom_is_ready());
   EEAR = adresa_eeprom, EEDR = LSB(date), EECR |= (1<<EEMWE), EECR |= (1<<EEWE);
}

U16 read_eeprom_word(U16 adresa_eeprom)
{
  register U16 c;

  EEAR = adresa_eeprom, EECR |= (1<<EERE), c = (EEDR << 8);
  adresa_eeprom++;
  EEAR = adresa_eeprom, EECR |= (1<<EERE), c = (c | EEDR);
  return c;
}

ISR(TIMER1_COMPA_vect)
{
  /* intreruperea se realizeaza la un interval de 100us */
  //g_tic = TRUE;
  //char direction = mc_get_motor_direction();
  
  //=========== consuma 37us pe LOW_SPEED
  //=========== consuma 26us pe HIGH_SPEED

  //LED_OFF();
  WaitingContor++;
  TimeOutForRS485++;
  Cronometru_ForReadingIndex++;

  //calculate_toggle_position();
  //Toggle_LED();

  switch (Status_Motor)
  {

    case NO_SPEED:
                                        if (IndexProgramed == NO_PROGRAMED)
                                        {
                                          if (Switch_OnOff_LED == OK)
                                          {
                                            if (Contor_flashing_led++ >= 20000) /* 1s */
                                            {
                                               Switch_OnOff_LED = KO;
                                               Contor_flashing_led = 0;
                                               LED_ON();
                                            }
                                          }
                                          else
                                          {
                                            if (Contor_flashing_led++ >= 4000) /* 0,2s */
                                            {
                                               Switch_OnOff_LED = OK;
                                               Contor_flashing_led = 0;
                                               LED_OFF();
                                            }
                                          }
                                        }
                                        else LED_ON();
    case HIGH_SPEED:
                                        calculate_toggle_position();
                                        break;
    case LOW_SPEED:
                                        mc_lowSpeed();
                                        break;    
    case START_TEST_RESOLVER:
                                        put_motor_on_position_ZERO();
                                        WaitingContorForTestResolver = 0;
                                        Status_Motor = WAIT_FOR_STABILIZATION;
                                        break;
    case WAIT_FOR_STABILIZATION:  
                                        if (WaitingContorForTestResolver++ == 40000) /* 2s */
                                        Status_Motor = READ_INDEX_FROM_RESOLVER;
                                        break;
    case READ_INDEX_FROM_RESOLVER:
                                        read_resolver();
                                        Set_none();
                                        PSC_Stop();
                                        DataToBeStoredInEEPROM = Calculate_Index();
                                        Status_Motor = STORE_INDEX;
                                        break;
    case STORE_INDEX:
                                        write_eeprom_word(AdresaEEPROM, DataToBeStoredInEEPROM);
                                        AdresaEEPROM = AdresaEEPROM + 8;
                                        if(++nbOfTest == NB_OF_TESTS)
                                        {
                                              write_eeprom_word(AdresaEEPROM, 0x4f4b);
                                              Flag_CheckForTesting = NO_TESTING;
                                              Status_Motor = NO_SPEED;
                                        }
                                        else
                                        {
                                              Status_Motor = START_TEST_RESOLVER;
                                        }
                                        break;
  } 
  //=========== consuma 26 us pe HIGH_SPEED 
  //=========== consuma 37 us pe LOW_SPEED
  //LED_ON();
}


/* subrutina pentru determinarea indexului 0*/
void put_motor_on_position_ZERO(void)
{
    Set_none();
    PSC_Stop();
    mc_duty_cycle(INDEX_AMPLITUDE);
    switch(nbOfTest)
    {
       case 0:  Set_Sector_1();
                break;
       case 1:  Set_B_C();
                break;
       case 2:  Set_C_A();
                break;
    }
    PSC_Start();
}

U16 Calculate_Index(void)
{
   U16 tempDataResolver;
   
   tempDataResolver = get_Data_Resolver();
   while (tempDataResolver > NB_MAX_RESOLVER / NB_PERECHI_POLI)
   {
     tempDataResolver = tempDataResolver - NB_MAX_RESOLVER / NB_PERECHI_POLI;
   }
   return tempDataResolver;
}


void calculate_toggle_position(void)
{
  register S16 tempPosition;
  register U8 Sector_Calculat;
  
  // ============= consuma 9us
  read_resolver();
  // ============= consuma 9 us
  tempPosition = get_Data_Resolver() - get_IndexZero_HighSpeed(); //INDEX_ZERO;


  // ============== consuma 4us
  if (tempPosition < 0)                 tempPosition += NB_MAX_RESOLVER;
  //if (tempPosition > NB_MAX_RESOLVER)   tempPosition -= NB_MAX_RESOLVER;

  while (tempPosition > NB_MAX_RESOLVER / NB_PERECHI_POLI)
  {
     tempPosition = tempPosition - NB_MAX_RESOLVER / NB_PERECHI_POLI;
  }

  if(tempPosition >= 0*STEP_MOTOR) {Sector_Calculat = 5;}
  if(tempPosition >= 1*STEP_MOTOR) {Sector_Calculat = 6;}
  if(tempPosition >= 2*STEP_MOTOR) {Sector_Calculat = 1;}
  if(tempPosition >= 3*STEP_MOTOR) {Sector_Calculat = 2;}
  if(tempPosition >= 4*STEP_MOTOR) {Sector_Calculat = 3;}
  if(tempPosition >= 5*STEP_MOTOR) {Sector_Calculat = 4;}
  // ============== consuma 4 us

  mc_cmd_highSpeed_V1 = pgm_read_word(&Cmd_Speed[get_VoltageIndex()][(Memo_speed & 0x0f) - 2]);

  if (Memo_SectorResolver != Sector_Calculat)
  {
    Memo_SectorResolver = Sector_Calculat;
    mc_highSpeed(Memo_SectorResolver);
    //Toggle_LED();
  }
  else
  {
    if (++Contor_Refresh_highSpeed >= DELAY_OF_AMP_REFRESH_FOR_HIGH_SPEED)
    {
      Contor_Refresh_highSpeed = 0;
      if (mc_cmd_highSpeed_V1 > mc_cmd_highSpeed_V0)
      {
         mc_cmd_highSpeed_V0 += 10;
         mc_duty_cycle(mc_cmd_highSpeed_V0);
      }
      else if(mc_cmd_highSpeed_V1 < mc_cmd_highSpeed_V0)
      {
         mc_cmd_highSpeed_V0 -= 10;
         mc_duty_cycle(mc_cmd_highSpeed_V0);
      }
    } 
  }
}


U16 get_IndexZero_HighSpeed(void)       {return IndexZero_HighSpeed;}
void set_IndexZero_HighSpeed(U16 nb)    {IndexZero_HighSpeed = nb;}
