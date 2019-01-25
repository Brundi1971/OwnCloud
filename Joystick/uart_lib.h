
#ifndef _UART_LIB_H_
#define _UART_LIB_H_



#ifndef UART_CONFIG
#define UART_CONFIG     MSK_UART_8BIT
#endif


#define MSK_UART_5BIT           0x00 /* UCSRnC register */
#define MSK_UART_6BIT           0x02
#define MSK_UART_7BIT           0x04
#define MSK_UART_8BIT           0x06
#define MSK_UART_9BIT           0x06

#define MSK_UART_RX_DONE        0x80 /* UCSRnA register */
#define MSK_UART_TX_COMPLET     0x40
#define MSK_UART_DRE            0x20

#define MSK_UART_ENABLE_IT_RX   0x80
#define MSK_UART_ENABLE_IT_TX   0x40
#define MSK_UART_ENABLE_RX      0x10 /* UCSRnB register */
#define MSK_UART_ENABLE_TX      0x08
#define MSK_UART_TX_BIT9        0x01
#define MSK_UART_RX_BIT9        0x02


/*    Driver for UART0    */

#define Uart_hw_init(config)          (UCSR0C=config)
#define Uart_set_baudrate(bdr)        ( UBRR0H = (U8)((((U32)FOSC*1000L)/((U32)bdr*16)-1)>>8),\
                                        UBRR0L = (U8)(((U32)FOSC*1000 )/((U32)bdr*16)-1)    )
#define Uart_set_TXB8()               (UCSR0B |= (1<<TXB8))
#define Uart_clear_TXB8()             (UCSR0B &= ~(1<<TXB8))
#define Uart_enable_tx()              (UCSR0B |= (1<<TXEN0))
#define Uart_enable_rx()              (UCSR0B |= (1<<RXEN0))
#define Uart_disable_tx()             (UCSR0B &= ~(1<<TXEN0))
#define Uart_disable_rx()             (UCSR0B &= ~(1<<RXEN0))
#define Uart_enable_mpcm()            (UCSR0A |= (1<<MPCM))
#define Uart_disable_mpcm()           (UCSR0A &= ~(1<<MPCM))
#define Uart_tx_complete()            (UCSR0A & MSK_UART_TX_COMPLET)

#define Uart_clear_flag_tx()          (UCSR0A |= (1<<TXC0))
#define Uart_enable_9bit()            (UCSR0B |= (1<<UCSZ02), UCSR0C |= ((1<<UCSZ01) | (1<<UCSZ00)))
#define Uart_enable_8bit()            (UCSR0B &= ~(1<<UCSZ02), UCSR0C |= ((1<<UCSZ01) | (1<<UCSZ00))) 
#define Uart_enable_2stop()           (UCSR0C |= (1<USBS0))
#define Uart_enable_1stop()           (UCSR0C &= ~(1<USBS0))
#define Uart_enable_even_parity()     (UCSR0C |= (1<<UPM01), UCSR0C &= ~(1<<UPM00))
#define Uart_enable_odd_parity()      (UCSR0C |= (1<<UPM01) | (1<<UPM00))
#define Uart_disable_parity()         (UCSR0C &= ~((1<<UPM01) | (1<<UPM00)))


#define Uart_enable()                 (UCSR0B|=MSK_UART_ENABLE_RX|MSK_UART_ENABLE_TX)
#define Uart_tx_ready()               (UCSR0A&MSK_UART_DRE)
#define Uart_set_tx_busy()
#define Uart_send_byte(ch)            (UDR0=ch)
#define Uart_rx_ready()               (UCSR0A&MSK_UART_RX_DONE)
#define Uart_get_byte()               (UDR0)
#define Uart_ack_rx_byte()

#define Uart_enable_it_rx()           (UCSR0B|=MSK_UART_ENABLE_IT_RX)
#define Uart_enable_it_tx()           (UCSR0B|=MSK_UART_ENABLE_IT_TX)
#define Uart_disable_it_rx()          (UCSR0B&=~MSK_UART_ENABLE_IT_RX)

/*    Driver for UART1    */

#define Uart1_hw_init(config)          (UCSR1C=config)
#define Uart1_set_baudrate(bdr)        ( UBRR1H = (U8)((((U32)FOSC*1000L)/((U32)bdr*16)-1)>>8),\
                                        UBRR1L = (U8)(((U32)FOSC*1000 )/((U32)bdr*16)-1)    )
#define Uart1_set_TXB8()               (UCSR1B |= (1<<TXB81))
#define Uart1_clear_TXB8()             (UCSR1B &= ~(1<<TXB81))
#define Uart1_enable_tx()              (UCSR1B |= (1<<TXEN1))
#define Uart1_enable_rx()              (UCSR1B |= (1<<RXEN1))
#define Uart1_disable_tx()             (UCSR1B &= ~(1<<TXEN1))
#define Uart1_disable_rx()             (UCSR1B &= ~(1<<RXEN1))
#define Uart1_enable_mpcm()            (UCSR1A |= (1<<MPCM))
#define Uart1_disable_mpcm()           (UCSR1A &= ~(1<<MPCM))

#define Uart1_clear_flag_tx()          (UCSR1A |= (1<<TXC1))
#define Uart1_enable_9bit()            (UCSR1B |= (1<<UCSZ12), UCSR1C |= ((1<<UCSZ11) | (1<<UCSZ10)))
#define Uart1_enable_8bit()            (UCSR1B &= ~(1<<UCSZ12), UCSR1C |= ((1<<UCSZ11) | (1<<UCSZ10))) 
#define Uart1_enable_2stop()           (UCSR1C |= (1<USBS1))
#define Uart1_enable_1stop()           (UCSR1C &= ~(1<USBS1))
#define Uart1_enable_even_parity()     (UCSR1C |= (1<<UPM11), UCSR1C &= ~(1<<UPM10))
#define Uart1_enable_odd_parity()      (UCSR1C |= (1<<UPM11) | (1<<UPM10))
#define Uart1_disable_parity()         (UCSR1C &= ~((1<UPM11) | (1<<UPM10)))


#define Uart1_enable()                 (UCSR1B|=MSK_UART_ENABLE_RX|MSK_UART_ENABLE_TX)
#define Uart1_tx_ready()               (UCSR1A&MSK_UART_DRE)
#define Uart1_rx_ready()               (UCSR1A&MSK_UART_RX_DONE)
#define Uart1_tx_complete()            (UCSR1A & MSK_UART_TX_COMPLET)
#define Uart1_set_tx_busy()
#define Uart1_send_byte(ch)            (UDR1=ch)
#define Uart1_get_byte()               (UDR1)
#define Uart1_ack_rx_byte()

#define Uart1_enable_it_rx()           (UCSR1B|=MSK_UART_ENABLE_IT_RX)
#define Uart1_enable_it_tx()           (UCSR1B|=MSK_UART_ENABLE_IT_TX)
#define Uart1_disable_it_rx()          (UCSR1B&=~MSK_UART_ENABLE_IT_RX)

#endif /* _UART_LIB_H_ */
