
#include "config.h"
#include "mc_rs485.h"
#include "uart_drv.h"
#include "mc_lib.h"
#include "mc_drv_highSpeed.h"
#include "mc_drv_lowSpeed.h"
#include "mc_resolver.h"
#include "mc_inputuri.h"
#include "serial.h"
#include "adc_drv.h"

volatile U8 Status_RS485;
volatile U8 DATA_IN[NO_BYTES_TO_BE_RECEIVED];
volatile U8 DATA_OUT;

volatile U8 Memo_speed = 0;
volatile U8 WaitingContor = 0;
volatile U16 TimeOutForRS485 = 0;
volatile U8 PSCContorLowSpeed;
volatile U8 ContorNoBytesReceived;

void init_uart (void)
{
  Uart_set_baudrate(BAUDRATE);
  Uart_enable_9bit(); 
  Uart_enable_1stop();
  Uart_clear_TXB8();
  Uart_enable_mpcm();
  Uart_enable();
  Status_RS485 = WAIT_ADDRESS_FROM_MASTER;
  Set_Control_RS485();
  TimeOutForRS485 = 0;
  Memo_speed = 0;
}


void interpret_comanda(void)
{ 
 if ((DATA_IN[0] == DATA_IN[1]) && ((DATA_IN[0] & MSK_ADRESA ) == (ADRESA_MODUL_DRIVER_MOTOR << 6)))
  {
    if((DATA_IN[0] & 0x1f) != Memo_speed)
    {
         Memo_speed = (DATA_IN[0] & 0x1f);
         if (Memo_speed & 0x10)mc_forward();
         else mc_backward();
         Disable_interrupt();
         switch (Memo_speed & 0x0f)
         {
              case 0:       mc_motor_stop();
                            break;
              case 1:   
                            mc_prepare_for_lowSpeed();
                            mc_motor_run_lowSpeed();
                            break;
              case 2:
              case 3:
              case 4:
              case 5:
              case 6:
              case 7:
              case 8:
              case 9:
              case 10:
              case 11:
              case 12:
              case 13:
              case 14:
              case 15:
                            Memo_SectorResolver = 0;
                            if ((Status_Motor == NO_SPEED) || (Status_Motor == LOW_SPEED))
                            {
                                mc_prepare_for_highSpeed();
                                mc_motor_run_highSpeed();
                            }
                            break;      
         }
         Enable_interrupt();
	}
  }
}


/* functie comunicatie cu Masterul */
void com_with_master(void)
{
  register char c, TempBufferRX8;
  
  switch(Status_RS485)
  {
      case WAIT_ADDRESS_FROM_MASTER:      /* adresare pe 9 biti - asteapta adresa de la Master */
                                          if (TimeOutForRS485 > WAIT_AT_LEAST_20ms) 
                                          {
                                              TimeOutForRS485 = 0;
                                              if (Flag_CheckForTesting == NO_TESTING) mc_motor_stop();
                                              Memo_speed = 0;
                                          }
                                          else
                                          {
                                              Set_RX_RS485();
                                              if (Uart_rx_ready())
                                              {
                                                  TempBufferRX8 = UCSRB;
                                                  c = Uart_get_byte();
                                                  if ((c == ADRESA_MODUL_DRIVER_MOTOR) && (TempBufferRX8 & 0x02))    /*este adresa slave-ului*/
                                                  {
                                                      pollingSenzoriExterni();
                                                      //DATA_OUT = ((ADRESA_MODUL_DRIVER_MOTOR << 6 ) | get_Status_Senzori());
                                                      //DATA_OUT = ((ADRESA_MODUL_DRIVER_MOTOR << 6 ) | (get_Data_Resolver()>>5));
                                                      DATA_OUT = ((ADRESA_MODUL_DRIVER_MOTOR << 6 ) | get_Status_Senzori() | ((get_VoltageIndex() & 0xf) << 2));
                                                      Uart_disable_rx();
                                                      WaitingContor = 0;
                                                      Status_RS485 = WAIT_BEFORE_TO_SEND_STATUS;
                                                      TimeOutForRS485 = 0;
                                                  }
                                              }
                                          }
                                          break;
      case WAIT_BEFORE_TO_SEND_STATUS:    /* asteapta inainte sa transmiti confirmare pentru receptie adresa*/
                                          if (WaitingContor > WAIT_AT_LEAST_100us)
                                          {
                                             Set_TX_RS485();
                                             Uart_disable_mpcm(); /* dezactiveaza facilitatea multiprocesor pentru a receptiona date */
                                             Uart_clear_TXB8();   /* transmite date */
                                             Uart_clear_flag_tx();
                                             while(!Uart_tx_ready());Uart_send_byte(DATA_OUT);
                                             Status_RS485 = SEND_STATUS;
                                           }
                                           break;
       case SEND_STATUS:                   /* adresare pe 9 biti - asteapta sa se transmita Status-ul */
                                           if (Uart_tx_complet())
                                           {
                                              //Uart_disable_mpcm(); /* dezactiveaza facilitatea multiprocesor pentru a receptiona date */
                                              Uart_enable_rx();
                                              Set_RX_RS485();
                                              WaitingContor = 0;
                                              ContorNoBytesReceived = 0;
                                              Status_RS485 = WAIT_DATA_FROM_MASTER;
                                            }
                                            break;
      case WAIT_DATA_FROM_MASTER:           /* adresare pe 9 biti - asteapta Data de la Master */
                                            if (WaitingContor > WAIT_AT_LEAST_1ms) /* asteapa de 3 ori mai mult: 250us + 3*250us */
                                            {
                                                c = Uart_get_byte();
                                                //Uart_set_TXB8();
                                                Uart_enable_mpcm();  /* activeaza facilitatea multiprocesor pentru a receptiona adrese */
                                                TimeOutForRS485 = 0;
                                                Status_RS485 = WAIT_ADDRESS_FROM_MASTER;
                                            }
                                             else
                                            {
                                                if (Uart_rx_ready())
                                                { 
                                                    c = Uart_get_byte();
                                                    DATA_IN[ContorNoBytesReceived] = c;
                                                    WaitingContor = 0;
                                                    //ContorNoBytesReceived++;
                                                    if (++ContorNoBytesReceived == NO_BYTES_TO_BE_RECEIVED)
                                                    {
                                                        Status_RS485 = INTERPRET_COMMAND; 
                                                    }
                                                }
                                            }
                                            break;
      case INTERPRET_COMMAND:               /* zona de interpretare a comenzii sosite de la master */
                                            if (Flag_CheckForTesting == NO_TESTING) interpret_comanda();
                                            TimeOutForRS485 = 0;
                                            //Uart_set_TXB8();
                                            Uart_enable_mpcm();  /* activeaza facilitatea multiprocesor pentru a receptiona adrese */
                                            Status_RS485 = WAIT_ADDRESS_FROM_MASTER;
                                            break;
  }
}


