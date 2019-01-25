
#ifndef _BUTTONS_H_
#define _BUTTONS_H_

extern volatile U8 Flaguri_Buttons;
extern volatile U8 DRH_direction;
extern volatile U8 DRV_direction;
extern volatile U8 Flag_PrezentaOperator;
extern volatile U8 Flag_ForReadingButtonEvents;

void init_porturi_joystick(void);
void get_Buttons_Joystick(void);
U8 get_DRH_direction(void);
U8 get_DRV_direction(void);
U8 get_Flag_PrezentaOperator(void);

/* port comutatoare directie ADC */
#define PORT_PORT_DIRECTIE                    PORTB
#define DDR_PORT_DIRECTIE                     DDRB
#define PIN_PORT_DIRECTIE                     PINB

#define BUTTON_DRH_FORWARD                    PINB0
#define BUTTON_DRH_BACKWARD                   PINB1

/*ATENTIE TREBUIE REDEFINITE SEMNALELE DE FORWARD SI BACKWARD PENTRU DRV*/
#define BUTTON_DRV_FORWARD                    PINB2
#define BUTTON_DRV_BACKWARD                   PINB3

/* port butoane */
//===========================================================================================
#define PORT_PORT_BUTOANE                     PORTC
#define DDR_PORT_BUTOANE                      DDRC
#define PIN_PORT_BUTOANE                      PINC

#define BUTTON_TRIGGER                        PINC0     /* negru trigger */ 
#define BUTTON_SELECTIE_TIP_CAMERA            PINC1     /* albastru - selectie tip camera*/
#define BUTTON_SELECTIE_FUNCTII_CAMERA        PINC2     /* rosu - selectie functii camera*/ 
#define BUTTON_DETONARE_CAPSE                 PINC3     /* gri - detonare capse */
#define BUTTON_SELECTIE_FOC                   PINC4     /* mov - selectie tip foc*/
#define BUTTON_SELECTIE_SPEED                 PINC5     /* verde - camera on/off */
#define BUTTON_INCREASE                       PINC6     /* (+) zoom, focus, AGC */
#define BUTTON_DECREASE                       PINC7     /* (-) zoom, focus, AGC */

#define PORT_PORT_BUTOANE_1                   PORTD
#define DDR_PORT_BUTOANE_1                    DDRD
#define PIN_PORT_BUTOANE_1                    PIND

#define BUTTON_OPERATOR                       PIND7     /* negru - operator */
#define BUTTON_CAMERA_ON_OFF                  PIND6

#define NO_DIRECTION                          0x77    
#define CW                                    0x01
#define CCW                                   0x00

#define OPERATOR_ACTIV                        0x20
#define OPERATOR_INACTIV                      0x00

#endif
