
#ifndef USART_XVGA_H_
#define USART_XVGA_H_

#include "drawings_xvga.h"

//constante USART
//=================================================================================================
#define XVGA_USART                                         (&AVR32_USART2)
#define XVGA_USART_RX_PIN                                  AVR32_USART2_RXD_0_0_PIN
#define XVGA_USART_RX_FUNCTION                             AVR32_USART2_RXD_0_0_FUNCTION
#define XVGA_USART_TX_PIN                                  AVR32_USART2_TXD_0_0_PIN
#define XVGA_USART_TX_FUNCTION                             AVR32_USART2_TXD_0_0_FUNCTION

#define PBA_HZ                                             32000000
#define USART_BAUDRATE                                     19200

//constante buffere receptie si transmisie
//=================================================================================================
#define DIMENSIUNE_RS232_RECEIVER_BUFFER                   50
#define DIMENSIUNE_RS232_TRANSMISSON_BUFFER                50

//constante subrutina receive_data_from_RS232
//=================================================================================================
#define CHECK_THE_FIRST_CHARACTER                          1
#define CHECK_THE_TIME_OUT                                 2

//constante pentru mesajul de eroare
//=================================================================================================
#define BIT_STATUS_OK                                      0
#define BIT_CKS_ERROR                                      1
#define BIT_CMD_CODE_ERROR                                 2
#define BIT_LG_ERROR                                       3

#define DISPLAY_STATUS                                     0
#define DISPLAY_OFF                                        1
#define DISPLAY_RAE                                        2
#define DISPLAY_RAL                                        3
#define DISPLAY_RAM                                        4
#define DISPLAY_STANDARD                                   5
#define DISPLAY_GRID                                       6
#define DISPLAY_TEST                                       7


#define TRANSMISSION_OK                                    1
#define TRANSMISSION_PENDING                               2

#define TIME_OUT_FOR_RS232                                 200         //20ms - 32caractere

//variabile
//=================================================================================================
extern U8 CodEroare;
extern U8 CmdCode;
extern U8 NewCmdCode;;
extern U8 TransmissionBufferRS232[DIMENSIUNE_RS232_TRANSMISSON_BUFFER];
extern U8 TransmissionIndex;
extern U8 RS232Scheduler;
extern U8 ByteIndex_ReceivedFromRS232;
extern U8 MainScheduler;
extern U8 ToggleOnOffCursor;
extern DRAM sRAM, sNewRAM;
extern DRAL sRAL, sNewRAL;
extern DRAE sRAE, sNewRAE;
extern DRStandard sRStandard, sNewRStandard;
extern DRGrid sNewRGrid, sRGrid;
//extern U16 RPM_value;
extern U8 bMODE;

void init_usart(void);
U8 receive_data_from_RS232(void);
U8 send_data_to_RS232(void);
U8 check_corectness_of_telegram(void);
U8 draw_figures(void);
U8 display_RAL(void);
U8 display_RAE(void);
U8 display_RAM(void);
U8 display_standard(void);
U8 display_grid(void);
U8 display_test(void);
void move_grid(void);
void load_transmission_buffer(U8 os, U8 code);
void load_transmission_buffer_with_status(U8 os, U8 mode);
void reset_rs232_errors(volatile avr32_usart_t *usart);
void init_all_variables(void);

#endif
