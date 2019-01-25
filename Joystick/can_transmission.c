 
#include "config.h"
#include "uart_drv.h"
#include "can_lib.h"
#include "can_drv.h"
#include "can_transmission.h"
#include "com_drivere.h"
#include "adc_drv.h"
#include "TWI_Master.h"

U8 data_cmd_drivere[8];
U8 data_status_drivere[8];

st_cmd_t MOb_ReceiveCommand;
st_cmd_t MOb_TransmitStatus;

volatile U8 Temperatura_Motor_H = 0;
volatile U8 Temperatura_Motor_V = 0;
volatile U8 Temperatura_Radiator_H = 0;
volatile U8 Temperatura_Radiator_V = 0;
volatile U8 Comanda_DRH = 0;
volatile U8 Comanda_DRV = 0;
volatile U16 TimeOutForCAN;
volatile U8 StatusConnectionWithMaster;
volatile U8 StatusErrorConnection;
volatile U8 StatusDrivere = 0;
volatile U8 StatusRezolvereAndPS;
volatile U8 MultiplexorDate;
volatile U8 StatusPS = 0;
volatile U8 ValidMOb[2];

/*=================================================================
 alocare biti in telegrama STATUS DRIVERE PE BUS-UL CAN
 ==================================================================
BYTE 0:  bit7        = Status Termistor H
         bit6        = Status Termistor V
         bit5        = Status Senzor Temp A2
         bit4        = Status Senzor Temp A0
         bit3        = Status Limita Verticala Superioara
         bit2        = Status Limita Verticala Inferioara
         bit1        = Status Connection With DRV
         bit0        = Status Connection With DRH
BYTE 1:  bit7 - bit6 = Multiplexor Drivere
                                       00 = Temperatura Motor DRH
                                       01 = Temperatura Motor DRV
                                       10 = Temperatura Radiator DRH
                                       11 = Temperatura Radiator DRV
         bit5        = Status PSV
         bit4        = Status PSH
         bit3        = Status DOS V
         bit2        = Status LOT V
         bit1        = Status DOS H
         bit0        = Status LOT H

BYTE 2:  bit7 - bit0 = Container date 
==================================================================*/

void config_T1(void)
{
  TCCR1A = 0;                                    //Normal port operation + Mode CTC
  TCCR1B = (1 << WGM12) | (1 << CS11);           //Mode CTC + prescaler 8 (0,5us)
  TCCR1C = 0;
  OCR1AH = 0;
  OCR1AL = 200;                                  //100us
  TIMSK1 = (1<<OCIE1A);                          //Output compare B Match interrupt Enable
}


void init_senzori(void)
{
  DDR_PORT_SENZORI = 0x00;      /* porturi de intrare */
  PORT_PORT_SENZORI = 0xff;     /* pull-up rezitor */
  DDR_PORT_COMENZI = 0xff;      /* porturi de iesire */
  PORT_PORT_COMENZI = 0x00;
}


/********************************************************/
/* subrutina de tratare a intreruperii de Timer 1       */
/********************************************************/
ISR(TIMER1_COMPA_vect)
{
/* intreruperea se genereaza la 100us */
/*====================================*/

  TimeOutForCAN++;
  TimeOutForRS232_DRH++;
  TimeOutForRS232_DRV++;
  DelayCommandForRS232_DRH++;
  DelayCommandForRS232_DRV++;
  Contor_Start_ADC++;

  Contor_flashing_led++;
}


void init_can(void)
{
  //CLKPR = 0x80;  CLKPR = 0x00;

  //- Pull-up on TxCAN & RxCAN one by one to use bit-addressing
  CAN_PORT_DIR &= ~(1<<CAN_INPUT_PIN );
  CAN_PORT_DIR &= ~(1<<CAN_OUTPUT_PIN);
  CAN_PORT_OUT |=  (1<<CAN_INPUT_PIN );
  CAN_PORT_OUT |=  (1<<CAN_OUTPUT_PIN);

  Can_reset();
  can_init();

  data_status_drivere[0] = StatusDrivere;
  MultiplexorDate = MUX_CHANNEL_0;
  data_status_drivere[1] = (StatusRezolvereAndPS |  MultiplexorDate);
  data_status_drivere[2] = Temperatura_Motor_H;


  MOb_ReceiveCommand.id.std = ID_CAN_DRIVERE;
  MOb_ReceiveCommand.ctrl.ide = 0;
  MOb_ReceiveCommand.dlc = NB_OF_BYTES_RECEIVE_FROM_MASTER;
  MOb_ReceiveCommand.pt_data = &data_cmd_drivere[0];
  MOb_ReceiveCommand.cmd = CMD_RX_DATA_MASKED;
  can_cmd(&MOb_ReceiveCommand);// != CAN_CMD_ACCEPTED);
  ValidMOb[0] = MOb_ReceiveCommand.handle;


  MOb_TransmitStatus.id.std = ID_CAN_DRIVERE;
  MOb_TransmitStatus.ctrl.ide = 0;
  MOb_TransmitStatus.dlc = NB_OF_BYTES_SENT_TO_MASTER;
  MOb_TransmitStatus.pt_data = &data_status_drivere[0];
  MOb_TransmitStatus.cmd = CMD_REPLY_MASKED;
  can_cmd(&MOb_TransmitStatus);// != CAN_CMD_ACCEPTED);
  ValidMOb[1] = MOb_TransmitStatus.handle;


  StatusConnectionWithMaster = CONNECTION_WITH_MASTER_IS_ON_LINE;
  StatusErrorConnection = NO_ERRORS;
  TimeOutForCAN = 0;
}


void can_transmission (void)
{
  U8 u8_temp;

  /* receive the command from master */
  /*=================================*/
  if(TimeOutForCAN >= WAIT_AT_LEAST_25ms)
  {
    TimeOutForCAN = 0;
    StatusConnectionWithMaster = CONNECTION_WITH_MASTER_IS_OFF_LINE;
	Comanda_DRH = 0;
	Comanda_DRV = 0;
	//LED_ON();
  }
  else
  { 
    u8_temp = can_get_status(&MOb_ReceiveCommand);
    if (u8_temp == CAN_STATUS_COMPLETED) 
    {
      StatusConnectionWithMaster = CONNECTION_WITH_MASTER_IS_ON_LINE;
      StatusErrorConnection = NO_ERRORS;
      TimeOutForCAN = 0;
	  Comanda_DRH = data_cmd_drivere[0];
	  Comanda_DRV = data_cmd_drivere[1];
      MOb_ReceiveCommand.id.std = ID_CAN_DRIVERE;
      MOb_ReceiveCommand.ctrl.ide = 0;
      MOb_ReceiveCommand.dlc = NB_OF_BYTES_RECEIVE_FROM_MASTER;
      MOb_ReceiveCommand.pt_data = &data_cmd_drivere[0];
      MOb_ReceiveCommand.cmd = CMD_RX_DATA_MASKED;
      can_cmd(&MOb_ReceiveCommand);// != CAN_CMD_ACCEPTED);
      ValidMOb[0] = MOb_ReceiveCommand.handle;
    }
    else if (u8_temp == CAN_STATUS_ERROR) 
    {
      StatusErrorConnection = ERRORS;
      TimeOutForCAN = 0;
      MOb_ReceiveCommand.id.std = ID_CAN_DRIVERE;
      MOb_ReceiveCommand.cmd = CMD_RX_DATA_MASKED;
      MOb_ReceiveCommand.ctrl.ide = 0;
      MOb_ReceiveCommand.dlc = NB_OF_BYTES_RECEIVE_FROM_MASTER;
      MOb_ReceiveCommand.pt_data = &data_cmd_drivere[0];
      can_cmd(&MOb_ReceiveCommand);// != CAN_CMD_ACCEPTED);
      ValidMOb[0] = MOb_ReceiveCommand.handle;
    }
  }

  /* auto reply with status to master */
  /*==================================*/
  u8_temp = can_get_status(&MOb_TransmitStatus);
  if (u8_temp == CAN_STATUS_COMPLETED) 
  {
    StatusErrorConnection = NO_ERRORS;
	prepare_status_data();
    MOb_TransmitStatus.id.std = ID_CAN_DRIVERE;
    MOb_TransmitStatus.cmd = CMD_REPLY_MASKED;
    MOb_TransmitStatus.ctrl.ide = 0;
    MOb_TransmitStatus.dlc = NB_OF_BYTES_SENT_TO_MASTER;
    MOb_TransmitStatus.pt_data = &data_status_drivere[0];
    can_cmd(&MOb_TransmitStatus);// != CAN_CMD_ACCEPTED);
    ValidMOb[1] = MOb_TransmitStatus.handle;
  }
  else if (u8_temp == CAN_STATUS_ERROR) 
  {
    StatusErrorConnection = ERRORS;
	prepare_status_data();

    MOb_TransmitStatus.id.std = ID_CAN_DRIVERE;
    MOb_TransmitStatus.cmd = CMD_REPLY_MASKED;
    MOb_TransmitStatus.ctrl.ide = 0;
    MOb_TransmitStatus.dlc = NB_OF_BYTES_SENT_TO_MASTER;
    MOb_TransmitStatus.pt_data = &data_status_drivere[0];
    can_cmd(&MOb_TransmitStatus);// != CAN_CMD_ACCEPTED);
    ValidMOb[1] = MOb_TransmitStatus.handle;
  }
  verifica_AutoMObGeneration();
}

void verifica_AutoMObGeneration(void)
{
  U8 mob_number;

  for (mob_number = 0; mob_number < 15; mob_number++)
  {
    if(!((mob_number == ValidMOb[0]) || (mob_number == ValidMOb[1])))
    {
      Can_set_mob(mob_number);
      Can_mob_abort();
      Can_clear_status_mob();       // To be sure !
    }
  }
}


void prepare_status_data(void)
{
  StatusRezolvereAndPS = 0;
  StatusPS = 0;

  if (!((PIN_PORT_SENZORI & (1 << SZ_PS_H)) == (1 << SZ_PS_H))) StatusPS |= 1 << BIT_SZ_PS_H;
  if (!((PIN_PORT_SENZORI & (1 << SZ_PS_V)) == (1 << SZ_PS_V))) StatusPS |= 1 << BIT_SZ_PS_V;

  if (StatusDRH & MSK_TX_CONNECTION) StatusDrivere &= ~(1 << BIT_CONNECTION_WITH_DRH);
  if (StatusDRV & MSK_TX_CONNECTION) StatusDrivere &= ~(1 << BIT_CONNECTION_WITH_DRV);


  //if (StatusConnectionWithDRV == CONNECTION_WITH_DRV_ON_LINE) LED_ON(); else LED_OFF();
  StatusDrivere &= ~MSK_LIMITE_SUPERIOARA_AND_INFERIOARA;
  StatusDrivere |= (StatusDRV & MSK_LIMITE_SUPERIOARA_AND_INFERIOARA);
  //StatusDrivere |= (1 << BIT_STATUS_SENZOR_TEMP_A0);
  //StatusDrivere |= (1 << BIT_STATUS_SENZOR_TEMP_A2);

  data_status_drivere[0] = StatusDrivere; 
  StatusRezolvereAndPS = StatusPS | ((StatusDRH & MSK_BITI_STATUS_REZOLVER_H) >> 4) | ((StatusDRV & MSK_BITI_STATUS_REZOLVER_V) >> 2);

  switch (MultiplexorDate & MSK_BITI_MULTIPLEXOR)
  {
    case MUX_CHANNEL_0:     data_status_drivere[1] = (StatusRezolvereAndPS |  MultiplexorDate);
                            data_status_drivere[2] = Temperatura_Motor_H;
                            MultiplexorDate = MUX_CHANNEL_1;
                            break;
    case MUX_CHANNEL_1:     data_status_drivere[1] = (StatusRezolvereAndPS |  MultiplexorDate);
                            data_status_drivere[2] = Temperatura_Motor_V;
                            MultiplexorDate = MUX_CHANNEL_2;
                            break;
    case MUX_CHANNEL_2:     data_status_drivere[1] = (StatusRezolvereAndPS |  MultiplexorDate);
                            data_status_drivere[2] = Temperatura_Radiator_H;
                            MultiplexorDate = MUX_CHANNEL_3;
                            break;
    case MUX_CHANNEL_3:     data_status_drivere[1] = (StatusRezolvereAndPS |  MultiplexorDate);
                            data_status_drivere[2] = Temperatura_Radiator_V;
                            MultiplexorDate = MUX_CHANNEL_0;
                            break;
    default:
                            MultiplexorDate = MUX_CHANNEL_0;
                            data_status_drivere[1] = (StatusRezolvereAndPS |  MultiplexorDate);
                            data_status_drivere[2] = Temperatura_Motor_H;
                            break;
  }
/*
  data_status_drivere[0] = 0x31;
  data_status_drivere[1] = 0x32;
  data_status_drivere[2] = 0x33;
*/

}
