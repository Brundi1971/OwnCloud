
#ifndef _TIMER_H_
#define _TIMER_H_

void init_UART1(void);
void transmiteCatrePC(void);

#define TX_PC_PREPARE_DATA                      1
#define TX_PC_SEND_DATA                         2
#define TX_PC_DELAY                             3

#define NO_MAX_BYTES_TELEGRAMA                  11

#define BYTE0                                 0x10
#define BYTE1                                 0x20
#define BYTE2                                 0x30
#define BYTE3                                 0x40
#define BYTE4                                 0x50
#define BYTE5                                 0x60
#define BYTE6                                 0x70
#define BYTE7                                 0x80
#define BYTE8                                 0x90

//BYTE 4
#define BIT_ERROR_CONNECTION_WITH_DRH_OK      3
#define BIT_ERROR_CONNECTION_WITH_DRV_OK      2
#define BIT_ERROR_CONNECTION_WITH_CAM_OK      1
#define BIT_OPERATOR_PRESENCE                 0

//BYTE 5
#define BIT_TURELA_SUS                        3
#define BIT_TURELA_JOS                        2
#define BIT_TRIGGER_CARTUSE                   1
#define BIT_ARMARE                            0

//BYTE 6 bit3-bit2
#define CAPSE_0                               0x00
#define CAPSE_1                               0x04
#define CAPSE_2                               0x08
#define CAPSE_3                               0x0C
#define MASK_ARMARE                           0x0C

//BYTE 6 bi1-bit0
#define FOC_CU_FOC                            0x01
#define FOC_SALVA                             0x02
#define FOC_CONTINUU                          0x03
#define MASK_SELECTIE_FOC                     0x03

//BYTE 7
#define BIT_ERROR_HEADER_DRH_OK               3
#define BIT_ERROR_HEADER_DRV_OK               2
#define BIT_ERROR_HEADER_CAM_OK               1

#endif 
