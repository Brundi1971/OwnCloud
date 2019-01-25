
#include "config.h"
#include "uart_lib.h"
#include "rs232_com.h"
#include "rs485_com.h"
#include "read_resolvers.h"

/* declarare variabile comunicatie */ 
volatile U8 ADRESA_MODUL_COMANDA_VIDEO;
volatile U8 Status_RS485;
volatile U16 TimeOutForRS485_Contor = 0;
volatile U16 TimeOutForErrorHeaderMasterData = 0;
volatile U16 TimeOutForErrorMasterData = 0;
volatile U8 ContorBytesSent;
volatile U8 ContorBytesReceived;
volatile U8 WaitingContor;
volatile U8 Telegrama_Status_Modul[NO_BYTES_STATUS];
volatile U8 Telegrama_Comanda[NO_BYTES_COMANDA];

/* declarare variabile pentru citirea rezolverelor */
volatile U8 Switch_between_Reading_Resolvers;

/* declarare variabile pentru procesarea comenzilor
	sosite de la master*/
volatile U8 MasterCommand;                       /* variabila care poarta comanda primita de la master */
volatile U8 ActionCommand = NO_COMMAND;
volatile U8 ActionCommandForReset = NO_COMMAND;
volatile U16 Contor_ActionComanda;
volatile U16 Contor_ResetComanda;
volatile U8 Flag_ActionCommand;
volatile U8 Directie_detonare = 0;
volatile U8 Multiplexor_Resolvere = 0;
volatile U8 Multiplexor_DateCamera = 0;

volatile U16 Contor_AsynchronousReading;

/* declarare variabile pentru procesarea starii senzorilor*/
volatile U8 Status_ModulConducereFoc;
                                                 /*
                                                 alocare biti in variabila Status_ModulConducereFoc
                                                 =====================================================
                                                 bit4 = Armare
                                                 bit3 - bit2 = stare capse
                                                 bit1 - bit0 = selectie tip foc
                                                 */

volatile U8 Status_SystemCamere[2];
                                                 /*
                                                 alocare biti in variabila Status_Camera[0]
                                                 =====================================================
                                                 bit 4 - bit 3 => selectie speed 00 = 25%
                                                                                 01 = 50%
                                                                                 10 = 75%
                                                                                 11 = 100%
                                                 bit 2 - bit 0 => functii camera 000 = zoom
                                                                                 001 = focus
                                                                                 010 = AGC
                                                                                 011 = Polarity
                                                */
                                                /*
                                                alocare biti in variabila Status_Camera[1]
                                                ======================================================
                                                bit 1 = Camera Type
                                                bit 0 = Camera On/Off
                                                */


volatile U8 SelectieTipFoc = 1;                 /* variabila in care se stocheaza starea Selectie Foc */



/* ALOCARE BITI IN COMANDA
===================================================================
  COMMAND_0
===================================================================
  bit 2 => trigger  
  bit 1 => selectie foc
  bit 0 => detonare capse      
===================================================================
  COMMAND_1
===================================================================
  bit 4 = select comanda zoom, focus sau AGC
  bit 3 = incrementeaza (+)
  bit 2 = decrementeaza (-)
  bit 1 = camera type
  bit 0 = camera on/off
===================================================================
*/

/* functii de sincronizare variabile */
U8 get_MasterCommand(void)                                {return MasterCommand;}
void set_MasterCommand(U8 nb)                             {MasterCommand = nb;}
U8 get_Status_ModulConducereFoc(void)                     {return Status_ModulConducereFoc;}
void set_Status_ModulConducereFoc(U8 nb)                  {Status_ModulConducereFoc = nb;}
U8 get_SelectieTipFoc(void)                               {return SelectieTipFoc;}
void set_SelectieTipFoc(U8 nb)                            {SelectieTipFoc = nb;}
void set_Status_SystemCamere(U8 Index, U8 Val)            {Status_SystemCamere[Index] = Val;}
U8 get_Status_SystemCamere(U8 Index)                      {return Status_SystemCamere[Index];}


/* subrutina care trateaza intreruperea de Timer 1 */
ISR (TIMER1_COMPA_vect)
{
  /* Timer setat pentru 50us*/
  TimeOutForRS485_Contor++;
  TimeOutForRS232_Contor++;
  TimeOutForErrorHeaderMasterData++;
  TimeOutForErrorMasterData++;
  Contor_ActionComanda++;
  Contor_ResetComanda++;
  WaitingContor++;
  Contor_booting_cameras++;
  Contor_RepeatAGC++;
  
  
//  if(Contor_AsynchronousReading++ > 1000)
//  {
//    Contor_AsynchronousReading = 0;
//  }
  
  if (Contor_ActionComanda > WAIT_AT_LEAST_10ms)
  {
    Flag_ActionCommand = 1;
    Contor_ActionComanda = 0;
  }
  reset_command_ports();
  
  switch(Switch_between_Reading_Resolvers)
  {
    case READ_RESOLVER_H:   read_resolver_H();
                            Switch_between_Reading_Resolvers = READ_RESOLVER_V;
                            break;
    case READ_RESOLVER_V:   read_resolver_V();
                            Switch_between_Reading_Resolvers = READ_RESOLVER_H;
                            break;
    default:                Switch_between_Reading_Resolvers = READ_RESOLVER_H;
  }
  
  /*calculeaza viteza turelei la intervale de 1s */
  Contor_CalculateSpeed++;
  if (Contor_CalculateSpeed >= 20000)
  {
    Contor_CalculateSpeed = 0;
    set_Speed_H(calculate_speed_H());  
  }
}

/* subrutina care reseteaza porturile comandate */
void reset_command_ports(void)
{
  switch (ActionCommand)
  {
    case TRIGGER_FOC_CU_FOC_COMMAND:      if (Contor_ResetComanda > WAIT_AT_LEAST_50ms)
                                          {
                                            PORT_PORT_RELEU_TRIGGER &= ~(1<<PIN_TRIGGER);
                                            ActionCommandForReset = NO_COMMAND;
                                          }
                                          break;
      
    case TRIGGER_SALVA_COMMAND:           if (Contor_ResetComanda > WAIT_AT_LEAST_350ms)
                                          {
                                            PORT_PORT_RELEU_TRIGGER &= ~(1<<PIN_TRIGGER);
                                            ActionCommandForReset = NO_COMMAND;
                                          }
                                          break;
    
    case DETONARE_CAPSE_COMMAND:          if (Contor_ResetComanda > WAIT_AT_LEAST_50ms)
                                          {
                                            PORT_PORT_RELEE_CAPSE &= ~((1<<PIN_CAPSA_1) | (1<<PIN_CAPSA_2) | (1<<PIN_CAPSA_3));
                                            ActionCommandForReset = NO_COMMAND;
                                          }
                                          break;
    default:                              ActionCommandForReset = NO_COMMAND;
                                          break;       
  } 
}


/* subrutina care trateaza comenzile de la master */
void action_command(void)
{
    register U8 c;
  
  /* load comanda */
    if (ActionCommand == NO_COMMAND)
    {
      switch(MasterCommand & ((1<<BIT_COMANDA_DETONARE_CAPSE) | (1<<BIT_COMANDA_SELECTIE_FOC) | (1<<BIT_COMANDA_TRIGGER)))
      {
        case (1<<BIT_COMANDA_DETONARE_CAPSE):
                                              if (PIN_PORT_SENZOR_ARMARE & (1<<PIN_SENZOR_ARMARE)) 
                                              {
                                                 comanda_detonare_capse();
                                                 ActionCommand = DETONARE_CAPSE_COMMAND;
                                                 ActionCommandForReset = DETONARE_CAPSE_COMMAND;
                                                 Contor_ResetComanda = 0;
                                              }
                                              break;
        case (1<<BIT_COMANDA_SELECTIE_FOC):
                                              /* actualizare variabila Selectie Foc */
                                              if (get_SelectieTipFoc() == 0x03) {set_SelectieTipFoc(0x01);}
                                              else {c = get_SelectieTipFoc(); c++; set_SelectieTipFoc(c);}  
                                              ActionCommand = SELECTIE_FOC_COMMAND;
                                              ActionCommandForReset = SELECTIE_FOC_COMMAND;
                                              break;
        case (1<<BIT_COMANDA_TRIGGER):
//                                              if ((get_Data_Resolver_H() <= 62) || (get_Data_Resolver_H() >= 191))
  //                                            {
                                                PORT_PORT_RELEU_TRIGGER |= (1<<PIN_TRIGGER);
                                                switch(SelectieTipFoc)
                                                {
                                                   case SELECTIE_FOC_CU_FOC:  ActionCommand = TRIGGER_FOC_CU_FOC_COMMAND;
                                                                              ActionCommandForReset = TRIGGER_FOC_CU_FOC_COMMAND;break;
                                                   case SELECTIE_SALVA:       ActionCommand = TRIGGER_SALVA_COMMAND;
                                                                              ActionCommandForReset = TRIGGER_SALVA_COMMAND;break;
                                                   case SELECTIE_CONTINUU:    ActionCommand = TRIGGER_CONTINUU_COMMAND;
                                                                              ActionCommandForReset = TRIGGER_CONTINUU_COMMAND;break;
                                                   default:                   ActionCommand = TRIGGER_FOC_CU_FOC_COMMAND;break;
                                                }
                                                Contor_ResetComanda = 0;
    //                                          }
                                              break;
        }
    }
 /* reset comanda */
    switch (ActionCommand)
    {
        case DETONARE_CAPSE_COMMAND:              
                                              if ((!(MasterCommand & (1<<BIT_COMANDA_DETONARE_CAPSE))) && (ActionCommandForReset == NO_COMMAND))
                                              {ActionCommand = NO_COMMAND;}
                                              break;
        case SELECTIE_FOC_COMMAND:                
                                              if (!(MasterCommand & (1<<BIT_COMANDA_SELECTIE_FOC)))
                                              {ActionCommand = NO_COMMAND;}
                                              break;
        case TRIGGER_FOC_CU_FOC_COMMAND:          
        case TRIGGER_SALVA_COMMAND:           if ((!(MasterCommand & (1<<BIT_COMANDA_TRIGGER))) && (ActionCommandForReset == NO_COMMAND))
                                              {ActionCommand = NO_COMMAND;}
                                              break;
                                                  
        case TRIGGER_CONTINUU_COMMAND:        if (!(MasterCommand & (1<<BIT_COMANDA_TRIGGER)))
                                              {PORT_PORT_RELEU_TRIGGER &= ~(1<<PIN_TRIGGER);
                                              ActionCommand = NO_COMMAND;}
                                              break;
    }
}


/* subrutina care alege ordinea de detonare a capselor */
void comanda_detonare_capse(void)
{
      if (Directie_detonare)
      {
          Directie_detonare = 0;
          if ((PIN_PORT_SENZORI_CAPSE  & (1<<PIN_SENZOR_CAPSA_1))) {PORT_PORT_RELEE_CAPSE |= (1<<PIN_CAPSA_1);}
          else if ((PIN_PORT_SENZORI_CAPSE  & (1<<PIN_SENZOR_CAPSA_2))) {PORT_PORT_RELEE_CAPSE |= (1<<PIN_CAPSA_2);}
          else if ((PIN_PORT_SENZORI_CAPSE  & (1<<PIN_SENZOR_CAPSA_3))) {PORT_PORT_RELEE_CAPSE |= (1<<PIN_CAPSA_3);}
      }
      else 
      {
          Directie_detonare = 1;
          if ((PIN_PORT_SENZORI_CAPSE  & (1<<PIN_SENZOR_CAPSA_3))) {PORT_PORT_RELEE_CAPSE |= (1<<PIN_CAPSA_3);}
          else if ((PIN_PORT_SENZORI_CAPSE  & (1<<PIN_SENZOR_CAPSA_2))) {PORT_PORT_RELEE_CAPSE |= (1<<PIN_CAPSA_2);}
          else if ((PIN_PORT_SENZORI_CAPSE  & (1<<PIN_SENZOR_CAPSA_1))) {PORT_PORT_RELEE_CAPSE |= (1<<PIN_CAPSA_1);}
      }
}


void init_porturi_adresa(void)
{
   /* set porturi adresa */
  DDR_PORT_SET_ADRESA &= ~((1<<PIN_ADRESA_0) | (1<<PIN_ADRESA_1) | (1<<PIN_ADRESA_2));
  PORT_PORT_SET_ADRESA |= ((1<<PIN_ADRESA_0) | (1<<PIN_ADRESA_1) | (1<<PIN_ADRESA_2)); /*pull-up resitor*/
}
  

void init_porturi_comenzi_si_senzori(void)
{ 
  /* set port releu trigger*/
  DDR_PORT_RELEU_TRIGGER |= ((1<<PIN_TRIGGER));
  PORT_PORT_RELEU_TRIGGER &= ~((1<<PIN_TRIGGER)); 
  //PORT_PORT_RELEU_TRIGGER |= ((1<<PIN_TRIGGER));
  
  /* set porturi relee capse*/
  DDR_PORT_RELEE_CAPSE |= ((1<<PIN_CAPSA_1) | (1<<PIN_CAPSA_2) | (1<<PIN_CAPSA_3));
  PORT_PORT_RELEE_CAPSE &= ~((1<<PIN_CAPSA_1) | (1<<PIN_CAPSA_2) | (1<<PIN_CAPSA_3)); 
  
  /* set porturi senzori capse */
  DDR_PORT_SENZORI_CAPSE &= ~((1<<PIN_SENZOR_CAPSA_1) | (1<<PIN_SENZOR_CAPSA_2) | (1<<PIN_SENZOR_CAPSA_3));
  PORT_PORT_SENZORI_CAPSE |= ((1<<PIN_SENZOR_CAPSA_1) | (1<<PIN_SENZOR_CAPSA_2) | (1<<PIN_SENZOR_CAPSA_3));
  
  /* set porturi senzori armare */
  DDR_PORT_SENZOR_ARMARE &= ~(1<<PIN_SENZOR_ARMARE);
  PORT_PORT_SENZOR_ARMARE |= (1<<PIN_SENZOR_ARMARE);
}


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


void init_UART0(void)
{ 
    Set_Control_RS485();
    Uart0_set_baudrate(BAUDRATE_UART0);
    Uart0_enable_9bit();
    Uart0_enable_1stop();
    Uart0_enable_mpcm();
    //Uart0_enable_even_parity();
    Uart0_enable();
    Status_RS485 = PREPARE_TELEGRAMA_STATUS;
}

   
/* verifica cosistenta datelor sosite de la Master si salveaza comanda respectiva */
void saveMasterCommand(void)
{ 
/* verifica Header-ul datelor  pentru toate capurile - pentru toti 4 octeti si pentru primii 3 biti din octet - 
   care trebuie sa fie egale cu adresa modulului) */
   if (((Telegrama_Comanda[0] >> 6) | (Telegrama_Comanda[1] >> 6) |
        (Telegrama_Comanda[2] >> 6) | (Telegrama_Comanda[3] >> 6)) == ADRESA_MODUL_COMANDA_VIDEO)
        {
          TimeOutForErrorHeaderMasterData = 0;
          if (((Telegrama_Comanda[0] & 0x3f) == (Telegrama_Comanda[1] & 0x3f)) &&
             ((Telegrama_Comanda[2] & 0x3f) == (Telegrama_Comanda[3] & 0x3f)))
          {
              TimeOutForErrorMasterData = 0;
              set_MasterCommand(Telegrama_Comanda[0] & 0x3f);
              set_MasterCommandForCamera(Telegrama_Comanda[2] & 0x3f);
              //if ((Telegrama_Comanda[2] &0x1f) != 0) LED_OFF();
              read_command_from_master();
              //set_Data_Resolver_H(get_MasterCommand());
          }
          else { if (TimeOutForErrorMasterData > WAIT_AT_LEAST_20ms) set_MasterCommand(0);}
        }  
        else { if (TimeOutForErrorHeaderMasterData > WAIT_AT_LEAST_20ms) set_MasterCommand(0);}
}


/* subrutina care citeste starea senzorilor - capselor si a armarii si citeste variabila selectie foc */
void loadTelegramaStatus(void)
{
  register U8 c,c1;
  
  /*load stare senzori capse*/
  //=============================================================================================================
  switch (PIN_PORT_SENZORI_CAPSE & ((1<<PIN_SENZOR_CAPSA_1) | (1<<PIN_SENZOR_CAPSA_2) | (1<<PIN_SENZOR_CAPSA_3)))
  {
      case 0:                                                                             c = get_Status_ModulConducereFoc();
      /* 0 capse */                                                                       c &= 0xf3;
                                                                                          //c |= 0x0c;
                                                                                          set_Status_ModulConducereFoc(c);
                                                                                          break;
      case (1<<PIN_SENZOR_CAPSA_1):
      case (1<<PIN_SENZOR_CAPSA_2):
      case (1<<PIN_SENZOR_CAPSA_3):
      /* 1 capsa */                                                                       c = get_Status_ModulConducereFoc();
                                                                                          c &= 0xf3;
                                                                                          c |= 0x04;
                                                                                          set_Status_ModulConducereFoc(c);
                                                                                          break;                                                       
      case ((1<<PIN_SENZOR_CAPSA_1) | (1<<PIN_SENZOR_CAPSA_2)): 
      case ((1<<PIN_SENZOR_CAPSA_2) | (1<<PIN_SENZOR_CAPSA_3)): 
      case ((1<<PIN_SENZOR_CAPSA_1) | (1<<PIN_SENZOR_CAPSA_3)):
      /* 2 capse */                                                                       c = get_Status_ModulConducereFoc();
                                                                                          c &= 0xf3;
                                                                                          c |= 0x08;
                                                                                          set_Status_ModulConducereFoc(c);
                                                                                          break; 
                                                                                          
      case ((1<<PIN_SENZOR_CAPSA_1) | (1<<PIN_SENZOR_CAPSA_2) | (1<<PIN_SENZOR_CAPSA_3)):
     /* 3 capse */                                                                        c = get_Status_ModulConducereFoc();
                                                                                          c &= 0xf3;
                                                                                          c |= 0x0c;
                                                                                          set_Status_ModulConducereFoc(c);
                                                                                          break;  
  }
  
  /*load stare selectie tip foc*/
  //=============================================================================================================
  c = get_SelectieTipFoc();
  c1 = get_Status_ModulConducereFoc();
  c1 = c1 & 0xfc;
  c1 = c1 | c;
  set_Status_ModulConducereFoc(c1);
  
  /*load stare armare*/
  //=============================================================================================================
  if (PIN_PORT_SENZOR_ARMARE & (1<<PIN_SENZOR_ARMARE))
  {
     c = get_Status_ModulConducereFoc();
     c &= ~0x10;
     set_Status_ModulConducereFoc(c);
  }
  else
  {
     c = get_Status_ModulConducereFoc();
     c |= 0x10;
     set_Status_ModulConducereFoc(c);
  }
  
  //get_Speed_H();
  /* incarca telegrama cu datele de stare ale senzorilor, respectiv pozitia rezolverelor*/
  //============================================================================================================

  Telegrama_Status_Modul[0] = (ADRESA_MODUL_COMANDA_VIDEO << 6);
  Telegrama_Status_Modul[1] = (ADRESA_MODUL_COMANDA_VIDEO << 6);
  Telegrama_Status_Modul[2] = (ADRESA_MODUL_COMANDA_VIDEO << 6);
  Telegrama_Status_Modul[3] = (ADRESA_MODUL_COMANDA_VIDEO << 6);
  Telegrama_Status_Modul[4] = (ADRESA_MODUL_COMANDA_VIDEO << 6);
  Telegrama_Status_Modul[5] = (ADRESA_MODUL_COMANDA_VIDEO << 6);
  Telegrama_Status_Modul[6] = (ADRESA_MODUL_COMANDA_VIDEO << 6);
  Telegrama_Status_Modul[7] = (ADRESA_MODUL_COMANDA_VIDEO << 6);
  Telegrama_Status_Modul[8] = (ADRESA_MODUL_COMANDA_VIDEO << 6);
  Telegrama_Status_Modul[9] = (ADRESA_MODUL_COMANDA_VIDEO << 6);


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
  BYTE 0 => valoare date de la camera - MSB
  BYTE 1 => B2
  BYTE 2 => B3
  BYTE 4 => LSB
=============================================================================
  STATUS_9
=============================================================================
  bit 5 - bit 0 => checksum
============================================================================*/


  /*  
  incarcarea datelor colectate in telegrama de raspuns catre master 
  =============================================================================
  */
  Telegrama_Status_Modul[0] |= (get_Status_ModulConducereFoc() & 0x3f);
  Telegrama_Status_Modul[1] |= ((get_Status_SystemCamere(0) | (get_CameraErrorType() << 2)) & 0x3f);
  switch (get_Status_SystemCamere(0) & 0x01) 
  {
  	case VISIBLE_SELECTED:
                                 c = get_Status_SystemCamere(1);
                                 c &= ~MSK_STATUS_SELECT_FUNCTII;
                                 c |= VisibleCameraFunctionsSelect;
                                 set_Status_SystemCamere(1,c);
								 break;
	case THERMAL_SELECTED:
	                             c = get_Status_SystemCamere(1);
                                 c &= ~MSK_STATUS_SELECT_FUNCTII;
                                 c |= ThermalCameraFunctionsSelect;
                                 set_Status_SystemCamere(1,c);
								 break;
	
  }
  Telegrama_Status_Modul[2] |= (get_Status_SystemCamere(1) & 0x3f);

  if (Multiplexor_Resolvere == 0)
  {
    Telegrama_Status_Modul[3] |= (0x3f & ((U8)(get_Data_Resolver_H() >> 4) & 0x30));
    Telegrama_Status_Modul[4] |= (0x3f & (U8)(get_Data_Resolver_H() >> 2));
    Multiplexor_Resolvere = 1;
  }
  else 
  {
    Telegrama_Status_Modul[3] |= (0x3f & (((U8)(get_Data_Resolver_V() >> 4) & 0x30) | (Multiplexor_Resolvere << 3)));
    Telegrama_Status_Modul[4] |= (0x3f & (U8)(get_Data_Resolver_V() >> 2)); 
    Multiplexor_Resolvere = 0;
  }

    Telegrama_Status_Modul[3] |= (Multiplexor_DateCamera & 0x3f);

//set_DataZoom(3,0x33);
//set_DataZoom(2,0x22);
//set_DataZoom(1,0x11);

//set_DataFocus(3,0x11);
//set_DataFocus(2,0x22);
//set_DataFocus(1,0x33);
//set_Speed_H(0xffff);

  switch(Multiplexor_DateCamera)
  {
    case 0:
               Multiplexor_DateCamera = 1; 
               switch (get_Status_SystemCamere(0) & 0x01) 
               {
                  case VISIBLE_SELECTED:
                                           switch((get_Status_SystemCamere(1) & 0x07))
                                           {
                                             case SELECTED_ZOOM_FUNCTION:
                                                   Telegrama_Status_Modul[5] |= (0x3f & (get_VisibleDataZoom(2) >> 2));
                                                   Telegrama_Status_Modul[6] |= (0x3f & (((get_VisibleDataZoom(2) << 4) & 0x30) | (get_VisibleDataZoom(1) >> 4)));
                                                   Telegrama_Status_Modul[7] |= (0x3f & (((get_VisibleDataZoom(1) << 2) & 0x3c) | (get_VisibleDataZoom(0) >> 6)));
                                                   Telegrama_Status_Modul[8] |= (0x3f & get_VisibleDataZoom(0));
                                                   break;
                                             case SELECTED_FOCUS_FUNCTION:
                                                   Telegrama_Status_Modul[5] |= (0x3f & (get_VisibleDataFocus(2) >> 2));
                                                   Telegrama_Status_Modul[6] |= (0x3f & (((get_VisibleDataFocus(2) << 4) & 0x30) | (get_VisibleDataFocus(1) >> 4)));
                                                   Telegrama_Status_Modul[7] |= (0x3f & (((get_VisibleDataFocus(1) << 2) & 0x3c) | (get_VisibleDataFocus(0) >> 6)));
                                                   Telegrama_Status_Modul[8] |= (0x3f & get_VisibleDataFocus(0));
                                                   break;
                                           }
                                           break;
                  case THERMAL_SELECTED:
                                           switch((get_Status_SystemCamere(1) & 0x07))
                                           {
                                             case SELECTED_ZOOM_FUNCTION:
                                                   Telegrama_Status_Modul[5] |= (0x3f & (get_ThermalDataZoom(2) >> 2));
                                                   Telegrama_Status_Modul[6] |= (0x3f & (((get_ThermalDataZoom(2) << 4) & 0x30) | (get_ThermalDataZoom(1) >> 4)));
                                                   Telegrama_Status_Modul[7] |= (0x3f & (((get_ThermalDataZoom(1) << 2) & 0x3c) | (get_ThermalDataZoom(0) >> 6)));
                                                   Telegrama_Status_Modul[8] |= (0x3f & get_ThermalDataZoom(0));
                                                   break;
                                             case SELECTED_FOCUS_FUNCTION:
                                                   Telegrama_Status_Modul[5] |= (0x3f & (get_ThermalDataFocus(3) >> 2));
                                                   Telegrama_Status_Modul[6] |= (0x3f & (((get_ThermalDataFocus(3) << 4) & 0x30) | (get_ThermalDataFocus(2) >> 4)));
                                                   Telegrama_Status_Modul[7] |= (0x3f & (((get_ThermalDataFocus(2) << 2) & 0x3c) | (get_ThermalDataFocus(1) >> 6)));
                                                   Telegrama_Status_Modul[8] |= (0x3f & get_ThermalDataFocus(1));
                                                   break;
                                           }
                                           break;
               }
               break;
    case 1:
               Multiplexor_DateCamera = 2;
			   Telegrama_Status_Modul[6] |= (0x3f & (get_Speed_H() >> 12));
               Telegrama_Status_Modul[7] |= (0x3f & (get_Speed_H() >> 6));
               Telegrama_Status_Modul[8] |= (0x3f & get_Speed_H()); 
               break;
    case 2:
               Multiplexor_DateCamera = 0; 
               Telegrama_Status_Modul[7] |= (0x3f & (get_DataPresiune() >> 6));
               Telegrama_Status_Modul[8] |= (0x3f & get_DataPresiune()); 
               break;
  }


  /* octetul de checksum */
  Telegrama_Status_Modul[9] |= ((Telegrama_Status_Modul[0] ^ Telegrama_Status_Modul[1] ^ Telegrama_Status_Modul[2] ^ 
                                 Telegrama_Status_Modul[3] ^ Telegrama_Status_Modul[4] ^ Telegrama_Status_Modul[5] ^
                                 Telegrama_Status_Modul[6] ^ Telegrama_Status_Modul[7] ^ Telegrama_Status_Modul[8]) & 0x3f);
}

/* subrutina care comunica cu masterul */
void com_with_master(void)
{
  register char c, TempBufferRX8;
  
  switch(Status_RS485)
  {
      case PREPARE_TELEGRAMA_STATUS:        loadTelegramaStatus();
                                            Status_RS485 = WAIT_ADDRESS_FROM_MASTER; 
                                            TimeOutForRS485_Contor = 0;
                                            c = Uart0_get_byte();
                                            c ^= c;
                                            break;
      case WAIT_ADDRESS_FROM_MASTER:        /* adresare pe 9 biti - asteapta adresa de la Master */
                                            if (TimeOutForRS485_Contor >= WAIT_AT_LEAST_20ms) 
                                            {
                                              TimeOutForRS485_Contor = 0;
                                              set_MasterCommand(0);
                                            }
                                            Set_RX_RS485();
                                            if (Uart0_rx_ready())
                                            {
                                                TempBufferRX8 = UCSR0B;
                                                c = Uart0_get_byte();
                                                if ((c == ADRESA_MODUL_COMANDA_VIDEO) && (TempBufferRX8 & 0x02))    /*este adresa slave-ului*/
                                                {
                                                    Uart0_disable_rx();
                                                    WaitingContor = 0;
                                                    Uart0_disable_mpcm();
                                                    Status_RS485 = WAIT_BEFORE_TO_SEND_STATUS_SLAVE;
                                                }
                                             }                                    
                                             break;
      case WAIT_BEFORE_TO_SEND_STATUS_SLAVE: /* intarziere inainte sa transmita status-ul*/
                                             if (WaitingContor > DELAY_AT_LEAST_50us)  
                                             {
                                                Set_TX_RS485();
                                                Uart0_clear_TXB8();
                                                Uart0_clear_flag_tx();
                                                ContorBytesSent = 0;
                                                c = Telegrama_Status_Modul[ContorBytesSent];
                                                while(!Uart0_tx_ready());Uart0_send_byte(c);
                                                Status_RS485 = SEND_STATUS_SLAVE;
                                             }
                                             break;
       case SEND_STATUS_SLAVE:                 /* adresare pe 9 biti - transmite Status-ul */
                                             if (Uart0_tx_complet())
                                             {
                                                Uart0_clear_flag_tx();
                                                ContorBytesSent++;
                                                if (ContorBytesSent == NO_BYTES_STATUS)
                                                {
                                                    c = Uart0_get_byte();
                                                    c ^= c;
                                                    Uart0_disable_mpcm();
                                                    Uart0_enable_rx();
                                                    Set_RX_RS485();
                                                    WaitingContor = 0;
                                                    Status_RS485 = WAIT_DATA_FROM_MASTER;
                                                    ContorBytesReceived = 0;
                                                }
                                                else
                                                {
                                                    c = Telegrama_Status_Modul[ContorBytesSent];
                                                    while(!Uart0_tx_ready());Uart0_send_byte(c);
                                                }
                                             }
                                             break;
      case WAIT_DATA_FROM_MASTER:             /*adresare pe 9 biti - asteapta Data de la Master */
                                             if (WaitingContor < WAIT_AT_LEAST_750us)
                                             {
                                                if (Uart0_rx_ready())
                                                {
                                                    c = Uart0_get_byte();
                                                    WaitingContor = 0;
                                                    Telegrama_Comanda[ContorBytesReceived] = c;
                                                    ContorBytesReceived++;
                                                    if (ContorBytesReceived == NO_BYTES_COMANDA)
                                                    {
                                                       Uart0_set_TXB8();
                                                       Uart0_enable_mpcm();
                                                       Status_RS485 = LOAD_NEW_COMANDA;
                                                    }
                                                }
                                              }
                                              else
                                              {
                                                  Uart0_set_TXB8();
                                                  Uart0_enable_mpcm();
                                                  Status_RS485 = PREPARE_TELEGRAMA_STATUS;
                                              }
                                              break;
      case LOAD_NEW_COMANDA:                  saveMasterCommand();
                                              Status_RS485 = PREPARE_TELEGRAMA_STATUS; 
                                              break;
    default:                                  Status_RS485 = PREPARE_TELEGRAMA_STATUS;
  }
}

