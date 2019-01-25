
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "compiler.h"

#define BAUDRATE_UART0                    38400
#define BAUDRATE_UART1                    76800
#define FOSC                              16000

#define LED                               PINA3
#define init_port_LED()                   (DDRA |= (1<<LED))
#define LED_ON()                          PORTA &= ~(1<<LED)
#define LED_OFF()                         PORTA |= (1<<LED)
#define Toggle_LED()                      PINA  |= (1<<LED)

#endif
