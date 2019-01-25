
#include "compiler.h"
#include <avr/interrupt.h>

#ifndef _CONFIG_H_
#define _CONFIG_H_
#endif

#define BAUDRATE_UART0              76800
#define BAUDRATE_UART1              19200
#define FOSC                        16000

#define LED                         PIND5
#define init_LED()                  DDRD |= (1<<LED)
#define LED_ON()                    PORTD &= ~(1<<LED)
#define LED_OFF()                   PORTD |= (1<<LED)
#define Toggle_LED()                PIND  |= (1<<LED)



