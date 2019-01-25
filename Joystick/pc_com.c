
#include "config.h"
#include "pc_com.h"
#include "uart_lib.h"
#include "rs485_com.h"
#include "adc_drv.h"

volatile U8 Telegrama_TX_PC[NO_MAX_BYTES_TELEGRAMA];
volatile U8 IndexNoBytes_TX_RS232;
volatile U8 Status_TX_RS232;
volatile U8 Flag_IT_T1_26us = 0;

/* initializarea serialei 0 */
void init_UART0(void)
{ 
    DDRD &=~(1<<PORTD0);
    PORTD |= (1<<PORTD0);
    
    Uart_set_baudrate(BAUDRATE_UART0);
    Uart_enable();
    Uart_enable_8bit();
    Status_TX_RS232 = TX_PC_PREPARE_DATA;
}
/*
Telegrama[0]    bit7 = 1
                bit6 = rezervat
                bit5 = DRV_SUS
                bit4 = DRV_JOS
                bit3 = DRV_H
                bit2 = DRV_V
                bit1 = Comanda_Rezolvere
                bit0 = Prezenta_Operator

Telegrama[1]    bit7 = 0
                bit6 = rezervat
                bit5 = rezervat
                bit4 = ARMARE
                bit3-bit2 = Capse                         01 - CP1
                                                          10 - CP2
                                                          11 - CP3
                bit1-bit 0 = Tip de FOC                   01 - FOC CU FOC
                                                          10 - FOC SALVA
                                                          11 - FOC CONTINUU

Telegrama[2]    bit7 = 0
                bit6 = camera on/off                      0 = camera off
                                                          1 = camera on
                bit5 = selectie tip camera                0 = camera de zi
                                                          1 = camera termica
                bit4 - bit3 = selectie viteza             00 = 10%
                                                          01 = 50%
                                                          10 = 100%
                bit2 - bit0 = selectie functie camera     000 = zoom
                                                          001 = focus
                                                          010 = AGC
                                                          011 = Polaritate

Telegrama[3]
                bit7 = 0
                bit6 - bit5 = Camera Error Type
                bit4 = bitul 7 de la resolver
                bit3 = Selectie resolvere
                bit2 - bit0 = selectie date de la camera

Telegrama[4]    bit7 = 0
                bit6 - bit0 = bitii 6 - 0 de la resolver
================================================================================
Telegrama[5]          bit7 = 0
                      bit6 - bit0 = date de la sistemul de camere

Telegrama[6]          bit7 = 0
                      bit6 - bit0 = date de la sistemul de camere

Telegrama[7]          bit7 = 0
                      bit6 - bit0 = date de la sistemul de camere

Telegrama[8]          bit7 = 0
                      bit6 - bit0 = date de la sistemul de camere

Telegrama[9]          bit7 = 0
                      bit6 - bit0 = date de la sistemul de camere

==============================================================================
Telegrama[10]         bit7 = 0
                      bit6 - bit0 = checksum

*/
/* subrutina de transmitere a datelor catre PC */
/* timpul de transmitere este de 11 octeti * 250us = aprox. 3ms + 10ms*/
void transmiteCatrePC(void)
{        
  register U8 Buffer_TX_RS232;
    
  switch (Status_TX_RS232)
  { 
    case TX_PC_PREPARE_DATA:
                             Telegrama_TX_PC[0] = (0x80 | get_Status_Comunicatie () | ((get_Status_DRV() & 0x03) << 4));
                             Telegrama_TX_PC[1] = 0x7f & (get_Status_ModulConducereFoc() | ((get_Status_DRH() & 0x04) << 3) | ((get_Status_DRV() & 0x04) << 4));
                             Telegrama_TX_PC[2] = 0x7f & (((get_Status_SystemCamere(0) & 0x03) << 5) | get_Status_SystemCamere(1));
                             Telegrama_TX_PC[3] = 0x7f & (((get_Status_SystemCamere(0) & 0x0c) << 3) | get_Status_Multiplexoare() | ((get_Resolver_Pozitie() >> 3) & 0x10));
                             Telegrama_TX_PC[4] = 0x7f & get_Resolver_Pozitie();
                             switch(get_Status_Multiplexoare() & 0x07)
                             {
                               case MUX_CHANNEL_0:
                                                   switch (get_Status_SystemCamere(0) & 0x01)
                                                   {
                                                     case VISIBLE_SELECTED:
                                                                            switch(get_Status_SystemCamere(1) & 0x07)
                                                                            {
                                                                              case ZOOM_FUNCTION_SELECTED:
                                                                                                           Telegrama_TX_PC[5] = (0x7f & ((VisibleDataZoom[0] << 1) | (VisibleDataZoom[1] >> 5)));
                                                                                                           Telegrama_TX_PC[6] = (0x7f & (((VisibleDataZoom[1] << 2) & 0x7c) | (VisibleDataZoom[2] >> 4)));
                                                                                                           Telegrama_TX_PC[7] = (0x7f & (((VisibleDataZoom[2] << 3) & 0x78) | (VisibleDataZoom[3] >> 3)));
                                                                                                           Telegrama_TX_PC[8] = (0x7f & ((VisibleDataZoom[3] << 4) & 0x70));
                                                                                                           break;
                                                                              case FOCUS_FUNCTION_SELECTED:
                                                                                                           Telegrama_TX_PC[5] = (0x7f & ((VisibleDataFocus[0] << 1) | (VisibleDataFocus[1] >> 5)));
                                                                                                           Telegrama_TX_PC[6] = (0x7f & (((VisibleDataFocus[1] << 2) & 0x7c) | (VisibleDataFocus[2] >> 4)));
                                                                                                           Telegrama_TX_PC[7] = (0x7f & (((VisibleDataFocus[2] << 3) & 0x78) | (VisibleDataFocus[3] >> 3)));
                                                                                                           Telegrama_TX_PC[8] = (0x7f & ((VisibleDataFocus[3] << 4) & 0x70));
                                                                                                           break;
                                                                            }
                                                                            break;
                                                     case THERMAL_SELECTED:
                                                                            switch(get_Status_SystemCamere(1) & 0x07)
                                                                            {
                                                                              case ZOOM_FUNCTION_SELECTED:
                                                                                                           Telegrama_TX_PC[5] = (0x7f & ((ThermalDataZoom[0] << 1) | (ThermalDataZoom[1] >> 5)));
                                                                                                           Telegrama_TX_PC[6] = (0x7f & (((ThermalDataZoom[1] << 2) & 0x7c) | (ThermalDataZoom[2] >> 4)));
                                                                                                           Telegrama_TX_PC[7] = (0x7f & (((ThermalDataZoom[2] << 3) & 0x78) | (ThermalDataZoom[3] >> 3)));
                                                                                                           Telegrama_TX_PC[8] = (0x7f & ((ThermalDataZoom[3] << 4) & 0x70));
                                                                                                           break;
                                                                              case FOCUS_FUNCTION_SELECTED:
                                                                                                           Telegrama_TX_PC[5] = (0x7f & ((ThermalDataFocus[0] << 1) | (ThermalDataFocus[1] >> 5)));
                                                                                                           Telegrama_TX_PC[6] = (0x7f & (((ThermalDataFocus[1] << 2) & 0x7c) | (ThermalDataFocus[2] >> 4)));
                                                                                                           Telegrama_TX_PC[7] = (0x7f & (((ThermalDataFocus[2] << 3) & 0x78) | (ThermalDataFocus[3] >> 3)));
                                                                                                           Telegrama_TX_PC[8] = (0x7f & ((ThermalDataFocus[3] << 4) & 0x70));
                                                                                                           break;
                                                                            }
                                                                            break;
                                                   }
                                                   break;
                               case MUX_CHANNEL_1:
                                                   Telegrama_TX_PC[5] = (0x7f & ((DataContainer[0] << 1) | (DataContainer[1] >> 5)));
                                                   Telegrama_TX_PC[6] = (0x7f & (((DataContainer[1] << 2) & 0x7c) | (DataContainer[2] >> 4)));
                                                   Telegrama_TX_PC[7] = (0x7f & (((DataContainer[2] << 3) & 0x78) | (DataContainer[3] >> 3)));
                                                   Telegrama_TX_PC[8] = (0x7f & ((DataContainer[3] << 4) & 0x70));
                                                   break;
                               case MUX_CHANNEL_2:
                                                   Telegrama_TX_PC[5] = (0x7f & (get_DataPresiune() >> 1));
                                                   Telegrama_TX_PC[6] = (0x7f & ((get_DataPresiune() << 6) & 0x40));
                                                   break;
                             }
                             Telegrama_TX_PC[9] = (0x7f & get_SpeedAndDirection_H());//get_Status_DRH();
                             Telegrama_TX_PC[10] = ((Telegrama_TX_PC[0] ^ Telegrama_TX_PC[1] ^ Telegrama_TX_PC[2] ^
                                                     Telegrama_TX_PC[3] ^ Telegrama_TX_PC[4] ^ Telegrama_TX_PC[5] ^
                                                     Telegrama_TX_PC[6] ^ Telegrama_TX_PC[7] ^ Telegrama_TX_PC[8] ^
                                                     Telegrama_TX_PC[9]) & 0x7f);

                             if (Uart_tx_ready())
                             {
                                IndexNoBytes_TX_RS232 = 0;
                                Buffer_TX_RS232 = Telegrama_TX_PC[IndexNoBytes_TX_RS232];
                                Uart_clear_flag_tx();
                                Uart_send_byte(Buffer_TX_RS232);
                                Status_TX_RS232 = TX_PC_SEND_DATA;
                             }
                             break;
    case TX_PC_SEND_DATA:
                             if (Uart_tx_ready())
                             {
                                IndexNoBytes_TX_RS232++;
                                if(IndexNoBytes_TX_RS232 != NO_MAX_BYTES_TELEGRAMA)
                                {
                                   Buffer_TX_RS232 = Telegrama_TX_PC[IndexNoBytes_TX_RS232];
                                   Uart_send_byte(Buffer_TX_RS232);
                                }
                                else
                                {
                                   Status_TX_RS232 = TX_PC_DELAY;
                                   DelayForRS232 = 0;
                                }
                              }
                              break;
    case TX_PC_DELAY:
                              if (DelayForRS232 > 200)  /* 10ms */
                              {
                                Status_TX_RS232 = TX_PC_PREPARE_DATA;
                              }
  } 
} 





