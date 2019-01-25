#ifndef _UART_SLAVES_H_
#define _UART_SLAVES_H_

extern volatile U8 SpeedAndDirection_H;
extern volatile U8 SpeedAndDirection_V;
extern volatile U8 Resolver_Pozitie;
extern volatile U16 Delay_Before_Reset_PINS;
extern volatile U16 DelayForRS232;

extern volatile U8 Status_Comunicatie;
extern volatile U8 Status_Header_Error;

extern volatile U8 VisibleDataZoom[4];
extern volatile U8 VisibleDataFocus[4];
extern volatile U8 ThermalDataZoom[4];
extern volatile U8 ThermalDataFocus[4];
extern volatile U8 DataContainer[4];

void init_UART0(void);
void transmiteCatreSlaves(void);
void delay(U8 no_clk);
void init_timer1(void);
U8 get_Resolver_Pozitie(void);
void set_Resolver_Pozitie(U8 nb);
U8 get_Status_DRH(void);
void set_Status_DRH(U8 nb);
U8 get_Status_DRV(void);
void set_Status_DRV(U8 nb);

void interpret_Status_CAM(void);
void create_comanda_for_CAM(void);
//U8 get_Comanda_CAM(void);
//void set_Comanda_CAM(U8 nb);

U8 get_Status_SystemCamere(U8 Index);
void set_Status_SystemCamere(U8 Index, U8 Val);

U8 get_Comanda_CAM(U8 Index);
void set_Comanda_CAM(U8 Index, U8 Val);

U8 get_Status_Comunicatie(void);
void set_Status_Comunicatie(U8 nb);
U8 get_Status_Header_Error(void);
void set_Status_Header_Error(U8 nb);

U8 get_Status_Multiplexoare(void);
void set_Status_Multiplexoare(U8 nb);

U8 get_Status_ModulConducereFoc(void);
void set_Status_ModulConducereFoc(U8 nb);

void set_DataTemperatura(U8 Val);
U8 get_DataTemperatura(void);
void set_DataPresiune(U8 Val);
U8 get_DataPresiune(void);

void set_CameraErrorType(U8 Val);
U8 get_CameraErrorType(void);

#define VISIBLE_SELECTED                              0
#define THERMAL_SELECTED                              1
#define ZOOM_FUNCTION_SELECTED                        0
#define FOCUS_FUNCTION_SELECTED                       1
#define MUX_CHANNEL_0                                 0
#define MUX_CHANNEL_1                                 1
#define MUX_CHANNEL_2                                 2

#define ADRESA_DRH                                    0x00
#define ADRESA_DRV                                    0x01
#define ADRESA_MODUL_CAM                              0x02

#define MSK_BITI_ADRESA                               0xc0
//#define MSK_BITI_DATE                                 0x1f

//DRH
#define PREPARE_TELEGRAMA_FOR_DRH                     0x01
#define TRANSMITE_ADRESA_TO_DRH                       0x02
#define WAIT_STATUS_FROM_DRH                          0x03
#define WAIT_BEFORE_TRANSMITE_COMANDA_TO_DRH          0x04
#define TRANSMITE_COMANDA_TO_DRH                      0x05

#define NO_BYTES_SENT_TO_DRH                           2
#define NO_BYTES_RECEIVED_FROM_DRH                     2

// DRV
#define PREPARE_TELEGRAMA_FOR_DRV                     0x10
#define TRANSMITE_ADRESA_TO_DRV                       0x11
#define WAIT_STATUS_FROM_DRV                          0x12
#define WAIT_BEFORE_TRANSMITE_COMANDA_TO_DRV          0x13
#define TRANSMITE_COMANDA_TO_DRV                      0x14

#define NO_BYTES_SENT_TO_DRV                           2
#define NO_BYTES_RECEIVED_FROM_DRV                     2

// CAM
#define PREPARE_TELEGRAMA_FOR_CAM                     0x20
#define TRANSMITE_ADRESA_TO_CAM                       0x21
#define WAIT_STATUS_FROM_CAM                          0x22
#define WAIT_BEFORE_TRANSMITE_COMANDA_TO_CAM          0x23
#define TRANSMITE_COMANDA_TO_CAM                      0x24

#define NO_BYTES_SENT_TO_CAM                           4
#define NO_BYTES_RECEIVED_FROM_CAM                     10

#define TIME_WAITING_FOR_ACK_ADRESA                   30
#define TIME_WAITING_FOR_ACK_DATA                     30
#define DELAY_BEFORE_TRANSMITE_DATA                   5

#define WAIT_AT_LEAST_100us                           2+1
#define WAIT_AT_LEAST_500us                           10+1
#define WAIT_AT_LEAST_750us                           15+1
#define WAIT_AT_LEAST_1ms                             20+1
#define WAIT_AT_LEAST_5ms                             100+1

#define Set_TX_RS485()                                (PORTD |= (1<<PORTD4))
#define Set_RX_RS485()                                (PORTD &=~(1<<PORTD4))  
#define Set_Control_RS485()                           (DDRD |= (1<<DDD4)) 

/*====================================================================*/
/* alocarea bitilor in secventa de comanda pentru sistemul de camere
======================================================================*/
/* alocare biti in comanda CAM [0] */
#define BIT_COMANDA_TRIGGER                           2
#define BIT_COMANDA_SELECTIE_FOC                      1
#define BIT_COMANDA_DETONARE_CAPSE                    0


/* alocare biti in comanda CAM [1] */
#define BIT_COMANDA_INCREASE                          5
#define BIT_COMANDA_DECREASE                          4
#define BIT_COMANDA_SELECTIE_ZOOM_FOCUS_AGC           3
#define BIT_COMANDA_SELECTIE_SPEED                    2
#define BIT_COMANDA_SELECT_CAMERA_TYPE                1
#define BIT_COMANDA_CAMERA_ON_OFF                     0
/*====================================================================*/


/*alocare biti in comanda driverelor de motor 
bit 4           => directie
bit 3 - bit 0   => viteza
*/

/*alocare biti in Status_DRH si Status_DRV */
#define BIT_DRV_PROTECTIE_TERMICA_MAJORA              4
#define BIT_DRV_PROTECTIE_TERMICA_MINORA              3
#define BIT_DRV_PROTECTIE_CURENT                      2
#define BIT_DRV_LIMITA_INFERIOARA                     1
#define BIT_DRV_LIMITA_SUPERIOAR                      0

#endif
