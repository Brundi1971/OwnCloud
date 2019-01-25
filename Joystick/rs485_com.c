
#include "config.h"
#include "uart_lib.h"
#include "rs485_com.h"
#include "pc_com.h"
#include "adc_drv.h"
#include "buttons.h"

#define NB_OF_CHECKING_FOR_RS485_COMMUNICATION_ERRORS           3
#define NB_OF_CHECKING_FOR_RS485_HEADER_ERRORS                  3


volatile U8 Status_RS485;
volatile U8 TimeOutForRS485;
volatile U16 DelayForRS232;
volatile U8 Comanda_DRH;                 /* variabila de stovare a comezii catre DRV H */
volatile U8 Comanda_DRV;                 /* variabila de stocare a comenzii catre DRV V */
volatile U8 ContorBytesSent;
volatile U8 ContorBytesReceived;
volatile U8 Status_DRH = 0;                  /* variabila de stocare a starii SRV H */
volatile U8 Status_DRV = 0;                  /* variabila de stocare a starii SRV V */
/* 
   bit 4 = 0 rezervat
   bit 3 = 0 rezervat
   bit 2 = 0 rezervat
   bit 1 = Limita superioara pe verticala
   bit 0 = Limita inferioara pe verticala
*/

volatile U8 Status_Comunicatie;            /* variabila pentru indicarea starii comunicatiei pe seriala RS485 */
/*
   bit 3 = Comunicatie cu DRV_V
   bit 2 = Comunicatie cu DRV_V
   bit 1 = Comunicatie cu sistemul de camere
   bit 0 = Prezenta Operator
*/
volatile U8 Status_Header_Error;           /* variabila pentru indicarea consistentei datelor pe comunicatia seriala RS485 */


volatile U8 Resolver_Pozitie;
volatile U8 Status_ModulConducereFoc;
volatile U8 Status_SystemCamere[2];        /* variabila stocare senzorii de stare ai modulului CAM */
/*
================================= 
byte 0
=================================
   bit 4 = Armare
   bit 3 - bit 2 = Stare Capse 
   bit 1 - bit 0 = Selectie Foc
=================================
byte 1
=================================
  bit 4 = 
*/
volatile U8 Comanda_CAM[3];               /* variabila de stocare a comenzilor catre CAM */
volatile U16 Contor_Start_ADC;
volatile U16 Delay_Before_Reset_PINS;
volatile U8 Telegrama_Status_CAM[NO_BYTES_RECEIVED_FROM_CAM];
volatile U8 Telegrama_Comanda_CAM[NO_BYTES_SENT_TO_CAM];
volatile U16 Contor_PoolingButtons;
volatile U8 CameraErrorType;

volatile U8 ContorTheConnectionErrorsFor_DRH = 0;
volatile U8 ContorTheConnectionErrorsFor_DRV = 0;
volatile U8 ContorTheConnectionErrorsFor_CAM = 0;

volatile U8 ContorTheHeaderErrorsFor_DRH = 0;
volatile U8 ContorTheHeaderErrorsFor_DRV = 0;
volatile U8 ContorTheHeaderErrorsFor_CAM = 0;

volatile U8 Status_Multiplexoare;

volatile U8 VisibleDataZoom[4];                 /* se incarca numai cate 6 biti */
volatile U8 VisibleDataFocus[4];                /* se incarca numai cate 6 biti */
volatile U8 ThermalDataZoom[4];                 /* se incarca numai cate 6 biti */
volatile U8 ThermalDataFocus[4];                /* se incarca numai cate 6 biti */ 
volatile U8 DataContainer[4];                   /* se incarca numai cate 6 biti */
volatile U8 DataTemperatura;
volatile U8 DataPresiune;


/*************************************************/
/* subrutina de tratare a intreruperii de Timer 1*/
/*************************************************/
ISR(TIMER1_COMPA_vect)
{
  /* intrerupere realizata la 50 us */
  //=====================================================
  TimeOutForRS485++;
  Contor_PoolingButtons++;
  DelayForRS232++;
  /* citire stare butoane la un interval de 10 ms*/
  //=====================================================
  if (Contor_PoolingButtons >= 200)
  {
    Flag_ForReadingButtonEvents = 1;
    Contor_PoolingButtons = 0;
  }
  
  /* citire stare potentiometrii la un interval de 2 ms*/
  //=====================================================
  if (Flag_Start_ADC == 1)
  { 
    Contor_Start_ADC++;
    if (Contor_Start_ADC >= 40)      /* aprox 2ms */
    {
      Contor_Start_ADC = 0;
      switch(Status_MUX_ADC)
      {
            case ADC_H:
                                  Status_MUX_ADC = ADC_V;
                                  Start_conv_channel(ADC_V);
                                  break;
            case ADC_V:
                                  Status_MUX_ADC = ADC_H;
                                  Start_conv_channel(ADC_H);
                                  break;
      }
     Flag_Start_ADC = 0;
    }
  }
}

/****************************/
/*initializarea Timer-ului 1*/
/****************************/
void init_timer1(void)
{
/*
    TCCR0A = 0;                         //Normal port operation + Mode CTC
    TCCR0B = (1<<WGM12) | (1<<CS10) ;   // Mode CTC + no presscaler = 100ns
    OCR0A = 255;                         //15ms; 1312 pentru sin 80
    TIMSK0=(1<<OCIE0A);                 // Output compare A Match interrupt Enable
*/
    
    TCCR1A = 0;
    TCCR1B = (1<<WGM12) | (1<<CS11) ;   /*  Mode CTC + prescaler = 8 => Tck = 0,5us */
    TCCR1C = 0;
    OCR1A = 100;                        /*  50us */
    TIMSK1=(1<<OCIE1A);                 
}

/***************************/
/*initializarea UART-ului 1*/
/***************************/
void init_UART1(void)
{ 
  
    DDRD &=~(1<<PORTD2);
    PORTD |= (1<<PORTD2);
    
    Set_Control_RS485();
    Uart1_set_baudrate(BAUDRATE_UART1);
    Uart1_enable_9bit();
    Uart1_enable_1stop();
    Uart1_enable();
    Status_RS485=PREPARE_TELEGRAMA_FOR_DRH;
    TimeOutForRS485 = 0;
}

U8 get_Resolver_Pozitie(void)                          {return Resolver_Pozitie;}
void set_Resolver_Pozitie(U8 nb)                       {Resolver_Pozitie = nb;}

U8 get_Status_DRH(void)                                {return Status_DRH;}
void set_Status_DRH(U8 nb)                             {Status_DRH = nb;}
U8 get_Status_DRV(void)                                {return Status_DRV;}
void set_Status_DRV(U8 nb)                             {Status_DRV = nb;}

U8 get_Status_SystemCamere(U8 Index)                   {return Status_SystemCamere[Index];}
void set_Status_SystemCamere(U8 Index, U8 Val)         {Status_SystemCamere[Index] = Val;}

U8 get_Comanda_CAM(U8 Index)                           {return Comanda_CAM[Index];}
void set_Comanda_CAM(U8 Index, U8 Val)                 {Comanda_CAM[Index] = Val;}

U8 get_Status_Comunicatie(void)                        {return Status_Comunicatie;}
void set_Status_Comunicatie(U8 nb)                     {Status_Comunicatie = nb;}

U8 get_Status_Header_Error(void)                       {return Status_Header_Error;}
void set_Status_Header_Error(U8 nb)                    {Status_Header_Error = nb;}

U8 get_Status_Multiplexoare(void)                      {return Status_Multiplexoare;}
void set_Status_Multiplexoare(U8 nb)                   {Status_Multiplexoare = nb;}

U8 get_Status_ModulConducereFoc(void)                  {return Status_ModulConducereFoc;}
void set_Status_ModulConducereFoc(U8 nb)               {Status_ModulConducereFoc = nb;}

void set_DataTemperatura(U8 Val)                       {DataTemperatura = Val;}
U8 get_DataTemperatura(void)                           {return DataTemperatura;}
void set_DataPresiune(U8 Val)                          {DataPresiune = Val;}
U8 get_DataPresiune(void)                              {return DataPresiune;}

void set_CameraErrorType(U8 Val)                       {CameraErrorType = Val;}
U8 get_CameraErrorType(void)                           {return CameraErrorType;}

/* interpreteaza starea sistemului de camere formata din 5 octeti */
/*

BYTE 0 => bit 7 - bit 5 = adresa modul sistem camere; bit 4 - bit 0 = informatie resolver H;
BYTE 1 => bit 7 - bit 5 = adresa modul sistem camere; bit 4 - bit 0 = informatie resolvere H + V;
BYTE 2 => bit 7 - bit 5 = adresa modul sistem camere; bit 4 - bit 0 = informatie resolver V;
BYTE 3 => bit 7 - bit 5 = adresa modul sistem camere; bit 4 = armare
                                                      bit 1 = selectie foc
                                                      bit 2 = trigger

BYTE 4 => bit 7 - bit 5 = adresa modul sistem camere; bit 0 = select camera type
                                                      bit 1 = camera on/off
                                                      bit 2 = selectie functii zoom/focus/AGC/polarity
                                                      bit 3 = (-)
                                                      bit 4 = (+)
*/

/*============================================================================
  ALOCARE BITI IN STATUS
==============================================================================
  STATUS_0
==============================================================================
  bit 5          => rezervat
  bit 4          => stare armare
  bit 3 - bit 2  => stare capse
  bit 1 - bit 0  => stare selectie foc
==============================================================================
  STATUS_1
==============================================================================
  bit 5 - bit 4 => rezervat
  bit 3 - bit 2 => ErrorType 
  bit 1         => Camera Termala On/Off
  bit 0         => Camera Type
==============================================================================
  SRATUS_2
==============================================================================
  bit 5         => rezervat
  bit 4 - bit 3 => selectie speed 00 = 25%
                                  01 = 50%
                                  10 = 75%
                                  11 = 100%
  bit 2 - bit 0 => functii camera 000 = zoom
                                  001 = focus
                                  010 = AGC
                                  011 = Polarity
=============================================================================
  STATUS_3
=============================================================================
  bit 5 - bit 4 => Valoare biti 7 si 6 de la Resolvere
  bit 3 =>         bit selectie Resolver transmis
  bit 2 - bit 0 => biti selectie date transmise de la camera
=============================================================================
  STATUS_4
=============================================================================
  bit 5 - bit 0 => bitii de la 5 la 0 de la Resolvere
=============================================================================
  STATUS_5 - STATUS_8
=============================================================================
  BYTE 0 => valoare date de la camera
  BYTE 1 =>
  BYTE 2 =>
  BYTE 4 =>
=============================================================================
  STATUS_9
=============================================================================
  bit 5 - bit 0 => checksum
============================================================================*/

void interpret_Status_CAM(void)
{
  if (((Telegrama_Status_CAM[0] & 0xc0) == ADRESA_MODUL_CAM <<6 ) && ((Telegrama_Status_CAM[1] & 0xc0) == ADRESA_MODUL_CAM <<6 ) &&
      ((Telegrama_Status_CAM[2] & 0xc0) == ADRESA_MODUL_CAM <<6 ) && ((Telegrama_Status_CAM[3] & 0xc0) == ADRESA_MODUL_CAM <<6 ) &&
      ((Telegrama_Status_CAM[4] & 0xc0) == ADRESA_MODUL_CAM <<6 ) && ((Telegrama_Status_CAM[5] & 0xc0) == ADRESA_MODUL_CAM <<6 ) &&
      ((Telegrama_Status_CAM[6] & 0xc0) == ADRESA_MODUL_CAM <<6 ) && ((Telegrama_Status_CAM[7] & 0xc0) == ADRESA_MODUL_CAM <<6 ) &&
      ((Telegrama_Status_CAM[8] & 0xc0) == ADRESA_MODUL_CAM <<6 ) && ((Telegrama_Status_CAM[9] & 0xc0) == ADRESA_MODUL_CAM <<6 ))
      {
        if ((Telegrama_Status_CAM[9] & 0x3f) == ((Telegrama_Status_CAM[0] ^ Telegrama_Status_CAM[1] ^ 
                                                  Telegrama_Status_CAM[2] ^ Telegrama_Status_CAM[3] ^ 
                                                  Telegrama_Status_CAM[4] ^ Telegrama_Status_CAM[5] ^ 
                                                  Telegrama_Status_CAM[6] ^ Telegrama_Status_CAM[7] ^
                                                  Telegrama_Status_CAM[8]) & 0x3f))
            {
              /* informatii de stare a modulul de conducere a focului */
              set_Status_ModulConducereFoc(Telegrama_Status_CAM[0] & 0x3f);

              /* informatii de stare a sistemului de camere */
              set_Status_SystemCamere(0, (Telegrama_Status_CAM[1] & 0x0f));
              set_Status_SystemCamere(1, (Telegrama_Status_CAM[2] & 0x1f));

              /* datele receptionate de la resolvere */
              set_Status_Multiplexoare(Telegrama_Status_CAM[3] & 0x0f);
              set_Resolver_Pozitie ((Telegrama_Status_CAM[4] & 0x3f) | ((Telegrama_Status_CAM[3] << 2) & 0xc0));
              
              /* datele receptionate de la pozitie zoom, focus, presiune, temperatura, etc. */

              switch(get_Status_Multiplexoare() & 0x07)
              {
                case MUX_CHANNEL_0:
                                     switch (get_Status_SystemCamere(0) & 0x01)
                                     {
                                       case VISIBLE_SELECTED:
                                                              switch(get_Status_SystemCamere(1) & 0x07)
                                                              {
                                                                case ZOOM_FUNCTION_SELECTED:
                                                                                             VisibleDataZoom[0] = (Telegrama_Status_CAM[5] & 0x3f);
                                                                                             VisibleDataZoom[1] = (Telegrama_Status_CAM[6] & 0x3f);
                                                                                             VisibleDataZoom[2] = (Telegrama_Status_CAM[7] & 0x3f);
                                                                                             VisibleDataZoom[3] = (Telegrama_Status_CAM[8] & 0x3f);
                                                                                             break;
                                                                case FOCUS_FUNCTION_SELECTED:
                                                                                             VisibleDataFocus[0] = (Telegrama_Status_CAM[5] & 0x3f);
                                                                                             VisibleDataFocus[1] = (Telegrama_Status_CAM[6] & 0x3f);
                                                                                             VisibleDataFocus[2] = (Telegrama_Status_CAM[7] & 0x3f);
                                                                                             VisibleDataFocus[3] = (Telegrama_Status_CAM[8] & 0x3f);
                                                                                             break;
                                                              }
                                                              break;
                                       case THERMAL_SELECTED:
                                                              switch(get_Status_SystemCamere(0) & 0x07)
                                                              {
                                                                case ZOOM_FUNCTION_SELECTED:
                                                                                             ThermalDataZoom[0] = (Telegrama_Status_CAM[5] & 0x3f);
                                                                                             ThermalDataZoom[1] = (Telegrama_Status_CAM[6] & 0x3f);
                                                                                             ThermalDataZoom[2] = (Telegrama_Status_CAM[7] & 0x3f);
                                                                                             ThermalDataZoom[3] = (Telegrama_Status_CAM[8] & 0x3f);
                                                                                             break;
                                                                case FOCUS_FUNCTION_SELECTED:
                                                                                             ThermalDataFocus[0] = (Telegrama_Status_CAM[5] & 0x3f);
                                                                                             ThermalDataFocus[1] = (Telegrama_Status_CAM[6] & 0x3f);
                                                                                             ThermalDataFocus[2] = (Telegrama_Status_CAM[7] & 0x3f);
                                                                                             ThermalDataFocus[3] = (Telegrama_Status_CAM[8] & 0x3f);
                                                                                             break;
                                                              }
                                                              break;
									 }
                                     break;
                case MUX_CHANNEL_1:
                                     DataContainer[0] = (Telegrama_Status_CAM[5] & 0x3f);
                                     DataContainer[1] = (Telegrama_Status_CAM[6] & 0x3f);
                                     DataContainer[2] = (Telegrama_Status_CAM[7] & 0x3f);
                                     DataContainer[3] = (Telegrama_Status_CAM[8] & 0x3f);
                                     break;
                case MUX_CHANNEL_2:
                                     set_DataPresiune(((Telegrama_Status_CAM[7] >> 4) & 0x03) | (Telegrama_Status_CAM[8] << 2));
                                     break;
              }
              
              //Toggle_LED();
              ContorTheHeaderErrorsFor_CAM = 0;
              Status_Comunicatie |= (1 << BIT_ERROR_HEADER_CAM_OK);
            }
      }
      else 
      {
        if (ContorTheHeaderErrorsFor_CAM == NB_OF_CHECKING_FOR_RS485_HEADER_ERRORS) 
        {Status_Comunicatie &= ~(1 << BIT_ERROR_HEADER_CAM_OK);}
        else {ContorTheHeaderErrorsFor_CAM++;}
      }
}


/* creeaza comanda pentru sistemul de camere format din 4 octeti in sistem redundant 2+2 */
void create_comanda_for_CAM(void)
{
    Telegrama_Comanda_CAM[0] = (ADRESA_MODUL_CAM << 6) | get_Comanda_CAM(0);
    Telegrama_Comanda_CAM[1] = (ADRESA_MODUL_CAM << 6) | get_Comanda_CAM(0);
    Telegrama_Comanda_CAM[2] = (ADRESA_MODUL_CAM << 6) | get_Comanda_CAM(1);
    Telegrama_Comanda_CAM[3] = (ADRESA_MODUL_CAM << 6) | get_Comanda_CAM(1);
}

void transmiteCatreSlaves(void)
{
  register char c;
  
  switch (Status_RS485)
  {
    //***************************************************************************************
    //
    //  Comunicatia cu DRH
    //
    //***************************************************************************************
    case PREPARE_TELEGRAMA_FOR_DRH:               /* adresare pe 9 biti */
                                                  if (TimeOutForRS485 > WAIT_AT_LEAST_100us)
                                                  {
                                                    Comanda_DRH = ((ADRESA_DRH << 6) | get_SpeedAndDirection_H());
                                                    Uart1_set_TXB8();
                                                    Uart1_disable_rx();
                                                    Set_TX_RS485();
                                                    c = Uart1_get_byte();
                                                    c ^= c;
                                                    Uart1_clear_flag_tx();
                                                    while(!Uart1_tx_ready());Uart1_send_byte(ADRESA_DRH);
                                                    Status_RS485 = TRANSMITE_ADRESA_TO_DRH;
                                                  }
                                                  break;
    case TRANSMITE_ADRESA_TO_DRH:                 /* asteapta sa se transmita ADRESA catre Slave-ul DRH */
                                                  if (Uart1_tx_complete())
                                                  {
                                                      Set_RX_RS485();
                                                      Uart1_enable_rx();
                                                      TimeOutForRS485 = 0;
                                                      Status_RS485 = WAIT_STATUS_FROM_DRH;
                                                  }
                                                  break;
    case WAIT_STATUS_FROM_DRH:                    /* asteapta transmiterea Status-ului de catre Slave-ul DRH */
                                                  if (TimeOutForRS485 > WAIT_AT_LEAST_750us)
                                                  {
                                                      Uart1_disable_rx();
                                                      if (ContorTheConnectionErrorsFor_DRH == NB_OF_CHECKING_FOR_RS485_COMMUNICATION_ERRORS) 
                                                      {Status_Comunicatie &= ~(1 << BIT_ERROR_CONNECTION_WITH_DRH_OK);}
                                                      else {ContorTheConnectionErrorsFor_DRH++;}
                                                      Status_RS485 = PREPARE_TELEGRAMA_FOR_DRV;
                                                  }
                                                  else
                                                  {
                                                      if (Uart1_rx_ready())
                                                      {
                                                          c = Uart1_get_byte();
                                                          Uart1_disable_rx();
                                                          TimeOutForRS485 = 0;
                                                          ContorTheConnectionErrorsFor_DRH = 0;
                                                          Status_Comunicatie |= (1 << BIT_ERROR_CONNECTION_WITH_DRH_OK);
                                                          Status_RS485 = WAIT_BEFORE_TRANSMITE_COMANDA_TO_DRH;
                                                          if ((c & MSK_BITI_ADRESA ) == (ADRESA_DRH << 6)) 
                                                          {
                                                              Status_Header_Error |= (1 << BIT_ERROR_HEADER_DRH_OK);
                                                              Status_DRH = c & 0x1f;
                                                          }
                                                          else 
                                                          {
                                                            if (ContorTheHeaderErrorsFor_DRH == NB_OF_CHECKING_FOR_RS485_HEADER_ERRORS) 
                                                            {Status_Header_Error &= ~(1 << BIT_ERROR_HEADER_DRH_OK);}
                                                            else {ContorTheHeaderErrorsFor_DRH++;}
                                                          }
                                                      }
                                                  }
                                                  break;
    case WAIT_BEFORE_TRANSMITE_COMANDA_TO_DRH:    /* asteapta 100us inainte sa transmita comanda catre DRH */
                                                  if (TimeOutForRS485 > WAIT_AT_LEAST_100us)
                                                  {
                                                      Uart1_clear_TXB8();   /* transmitere de date */
                                                      Set_TX_RS485();
                                                      ContorBytesSent = 0;
                                                      c = Uart1_get_byte();
                                                      Uart1_clear_flag_tx();
                                                      TimeOutForRS485 = 0;
                                                      while(!Uart1_tx_ready());Uart1_send_byte(Comanda_DRH);
                                                      Status_RS485 = TRANSMITE_COMANDA_TO_DRH;
                                                  }
                                                  break;
    case TRANSMITE_COMANDA_TO_DRH:                /* asteapta sa se transmita datele catre DRH */
                                                  if (Uart1_tx_complete())
                                                  { /* introduce o intarziere de 100us intre octeti */
                                                    if (TimeOutForRS485 > WAIT_AT_LEAST_100us)
                                                    {
                                                      ContorBytesSent++;
                                                      if (ContorBytesSent == NO_BYTES_SENT_TO_DRH)
                                                      {
                                                         Set_RX_RS485();
                                                         Uart1_enable_rx();
                                                         Status_RS485 = PREPARE_TELEGRAMA_FOR_DRV;
                                                         TimeOutForRS485 = 0;
                                                      }
                                                      else
                                                      {
                                                          Uart1_clear_flag_tx();
                                                          while(!Uart1_tx_ready());Uart1_send_byte(Comanda_DRH);
                                                      }
                                                    }
                                                  }
                                                  break;
    //***************************************************************************************
    //
    //  Comunicatia cu DRV
    //
    //***************************************************************************************
    case PREPARE_TELEGRAMA_FOR_DRV:               /* adresare pe 9 biti */
                                                  if (TimeOutForRS485 > WAIT_AT_LEAST_100us)
                                                  {
                                                    Comanda_DRV = ((ADRESA_DRV << 6) | get_SpeedAndDirection_V());
                                                    Uart1_set_TXB8();
                                                    Uart1_disable_rx();
                                                    Set_TX_RS485();
                                                    c = Uart1_get_byte();
                                                    c ^= c;
                                                    Uart1_clear_flag_tx();
                                                    while(!Uart1_tx_ready());Uart1_send_byte(ADRESA_DRV);
                                                    Status_RS485 = TRANSMITE_ADRESA_TO_DRV;
                                                  }
                                                  break;
    case TRANSMITE_ADRESA_TO_DRV:                 /* asteapta sa transmita ADRESA catre Slave DRH */
                                                  if (Uart1_tx_complete())
                                                  {
                                                      Set_RX_RS485();
                                                      Uart1_enable_rx();
                                                      TimeOutForRS485 = 0;
                                                      Status_RS485 = WAIT_STATUS_FROM_DRV;
                                                  }
                                                  break;                               
    case WAIT_STATUS_FROM_DRV:                    /* asteapta confirmarea de la Slave-ul DRH */
                                                  if (TimeOutForRS485 > WAIT_AT_LEAST_750us)
                                                  {
                                                      Uart1_disable_rx();
                                                      Status_Comunicatie &= ~(1 << BIT_ERROR_CONNECTION_WITH_DRV_OK);
                                                      Status_RS485 = PREPARE_TELEGRAMA_FOR_CAM;
                                                  }
                                                  else
                                                  {
                                                      if (Uart1_rx_ready())
                                                      {
                                                          c = Uart1_get_byte();
                                                          Uart1_disable_rx();
                                                          TimeOutForRS485 = 0;
                                                          Status_RS485 = WAIT_BEFORE_TRANSMITE_COMANDA_TO_DRV;
                                                          if ((c & MSK_BITI_ADRESA ) == (ADRESA_DRV << 6)) 
                                                          {
                                                              Status_Comunicatie |= (1 << BIT_ERROR_CONNECTION_WITH_DRV_OK);
                                                              Status_DRV = c & 0x1f;
                                                          }
                                                          else Status_Comunicatie &= ~(1 << BIT_ERROR_CONNECTION_WITH_DRV_OK);
                                                      }
                                                  }
                                                  break;
    case WAIT_BEFORE_TRANSMITE_COMANDA_TO_DRV:        
                                                  if (TimeOutForRS485 > WAIT_AT_LEAST_100us)
                                                  {
                                                      Uart1_clear_TXB8();
                                                      Set_TX_RS485();
                                                      ContorBytesSent = 0;
                                                      c = Uart1_get_byte();
                                                      Uart1_clear_flag_tx();
                                                      while(!Uart1_tx_ready());Uart1_send_byte(Comanda_DRV);
                                                      Status_RS485 = TRANSMITE_COMANDA_TO_DRV;
                                                  }
                                                  break;                                                 
   case TRANSMITE_COMANDA_TO_DRV:                     
                                                  if (Uart1_tx_complete())
                                                  {
                                                    ContorBytesSent++;
                                                    if (ContorBytesSent == NO_BYTES_SENT_TO_DRV)
                                                      {
                                                         Set_RX_RS485();
                                                         Uart1_enable_rx();
                                                         Status_RS485 = PREPARE_TELEGRAMA_FOR_CAM;
                                                         TimeOutForRS485 = 0;
                                                      }
                                                      else
                                                      {
                                                          Uart1_clear_flag_tx();
                                                          while(!Uart1_tx_ready());Uart1_send_byte(Comanda_DRV);
                                                      }
                                                  }
                                                  break;                             
    //***************************************************************************************
    //
    //  Comunicatia cu CAM
    //
    //***************************************************************************************
    case PREPARE_TELEGRAMA_FOR_CAM:  
                                                  if (TimeOutForRS485 > WAIT_AT_LEAST_100us)
                                                  {
                                                    create_comanda_for_CAM();
                                                    Uart1_set_TXB8(); 
                                                    Uart1_disable_rx();                                            
                                                    Set_TX_RS485();
                                                    c = Uart1_get_byte();
                                                    Uart1_clear_flag_tx();
                                                    while(!Uart1_tx_ready());Uart1_send_byte(ADRESA_MODUL_CAM);
                                                    Status_RS485 = TRANSMITE_ADRESA_TO_CAM;
                                                  }
                                                  break;
    case TRANSMITE_ADRESA_TO_CAM:         
                                                  if (Uart1_tx_complete())
                                                  {
                                                      c = Uart1_get_byte();
                                                      Set_RX_RS485();
                                                      Uart1_enable_rx();
                                                      TimeOutForRS485 = 0;
                                                      Status_RS485 = WAIT_STATUS_FROM_CAM;
                                                      ContorBytesReceived = 0;
                                                  }
                                                  break;                               
    case WAIT_STATUS_FROM_CAM:
                                                  if (TimeOutForRS485 > WAIT_AT_LEAST_500us)
                                                  {
                                                        Uart1_disable_rx();
                                                        Status_Comunicatie &= ~(1 << BIT_ERROR_CONNECTION_WITH_CAM_OK);
                                                        Status_RS485 = PREPARE_TELEGRAMA_FOR_DRH;
                                                  }
                                                  else
                                                  {
                                                      if (Uart1_rx_ready())
                                                      {
                                                          c = Uart1_get_byte();
                                                          TimeOutForRS485 = 0;
                                                          Telegrama_Status_CAM[ContorBytesReceived] = c;
                                                          ContorBytesReceived++;
                                                          if (ContorBytesReceived == NO_BYTES_RECEIVED_FROM_CAM)
                                                          {
                                                              interpret_Status_CAM();
                                                              Uart1_disable_rx();
                                                              Status_RS485 = WAIT_BEFORE_TRANSMITE_COMANDA_TO_CAM;
                                                          }
                                                      }
                                                  }
                                                  break;
   case WAIT_BEFORE_TRANSMITE_COMANDA_TO_CAM:        
                                                  if (TimeOutForRS485 > WAIT_AT_LEAST_100us)
                                                  {
                                                      Uart1_clear_TXB8();
                                                      Set_TX_RS485();
                                                      ContorBytesSent = 0;
                                                      c = Uart1_get_byte();
                                                      Uart1_clear_flag_tx();
                                                      c = Telegrama_Comanda_CAM[0];
                                                      while(!Uart1_tx_ready());Uart1_send_byte(c);
                                                      Status_RS485 = TRANSMITE_COMANDA_TO_CAM;
                                                  }
                                                  break;  
   case TRANSMITE_COMANDA_TO_CAM:                     
                                                  if (Uart1_tx_complete())
                                                  {
                                                      ContorBytesSent++;
                                                      if (ContorBytesSent == NO_BYTES_SENT_TO_CAM)
                                                      {
                                                         Set_RX_RS485();
                                                         Uart1_enable_rx();
                                                         Status_RS485 = PREPARE_TELEGRAMA_FOR_DRH;
                                                         TimeOutForRS485 = 0;
                                                      }
                                                      else
                                                      {
                                                          Uart1_clear_flag_tx();
                                                          c = Telegrama_Comanda_CAM[ContorBytesSent];
                                                          while(!Uart1_tx_ready());Uart1_send_byte(c);
                                                      }
                                                  }
                                                  break;
                                                                 
  }
}
