
#ifndef _UART_RS485
#define _UART_RS485

extern volatile U8 Memo_speed;
extern volatile U8 WaitingContor;
extern volatile U16 TimeOutForRS485;
extern volatile U8 PSCContorLowSpeed;

void init_uart (void);
void receive_command(void);
void interpret_comanda(void);
void sendchar(char c);
void com_with_master(void);

/* alocare port pentru controlul comunicatiei RS485 
==============================================================================*/
#define Set_Control_RS485()                       (DDRC |= (1<<PORTC1)) 
#define Set_RX_RS485()                            (PORTC &=~(1<<PORTC1))
#define Set_TX_RS485()                            (PORTC |= (1<<PORTC1))
/*============================================================================*/

#define WAIT_ADDRESS_FROM_MASTER                  1
#define WAIT_BEFORE_TO_SEND_STATUS                2
#define SEND_STATUS                               3
#define WAIT_DATA_FROM_MASTER                     4
#define INTERPRET_COMMAND                         5

#define NO_BYTES_TO_BE_RECEIVED                   2

#define WAIT_AT_LEAST_100us                       2
#define WAIT_AT_LEAST_500us                       10
#define WAIT_AT_LEAST_700us                       14+1
#define WAIT_AT_LEAST_1ms                         20+1
#define WAIT_AT_LEAST_2ms                         30+1
#define WAIT_AT_LEAST_5ms                         100+1
#define WAIT_AT_LEAST_10ms                        200+1
#define WAIT_AT_LEAST_20ms                        400+1
#define WAIT_AT_LEAST_50ms                        1000+1

#define MSK_ADRESA                                0xc0

/* Mod CTC + prescaler = 8 => Tclk = 0,5us */
#define init_timer0(time)                     (TCCR0A |= (1<<WGM01), \
                                               TCCR0B |= (1<<CS01), \
                                               OCR0A = 2*time, \
                                               TIMSK0 |=(1<<OCIE0A))
                                            
/* Mod CTC + prescaler = 8 => Tclk = 0,5us */
#define init_timer1(time)                     (TCCR1A = 0, \
                                               TCCR1B = (1<<WGM12) | (1<<CS11), \
                                               TCCR1C = 0, OCR1A = 2*time, \
                                               TIMSK1= (1<<OCIE1A))


#endif
