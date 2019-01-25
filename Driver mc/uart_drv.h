/*H**************************************************************************
* NAME:         uart_drv.h
*----------------------------------------------------------------------------
* Copyright (c) 2004 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      mc100_bldc_sinus_0_0_5
* REVISION:     1.1.2.3
*----------------------------------------------------------------------------
* PURPOSE:
* Provide Uart driver
*****************************************************************************/

#ifndef _UART_DRV_H_
#define _UART_DRV_H_

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

#ifdef USE_UART1
    #define UCSRC   (UCSR0C)
    #define UCSRB   (UCSR0B)
    #define UCSRA   (UCSR0A)
    #define UDR     (UDR0)
    #define UBRRL   (UBRR0L)
    #define UBRRH   (UBRR0H)
    #define UBRR    (UBRR0)
#endif
#ifdef USE_UART2
    #define UCSRC    (UCSR1C)
    #define UCSRB    (UCSR1B)
    #define UCSRA    (UCSR1A)
    #define UDR      (UDR1)
    #define UBRRL    (UBRR1L)
    #define UBRRH    (UBRR1H)
    #define UBRR     (UBRR1)

#endif


#define Uart_hw_init(config)    (UCSRC=config)
#define Uart_set_baudrate(bdr)  ( UBRRH = (U8)((((U32)FOSC*1000)/((U32)bdr*16)-1)>>8),\
                                  UBRRL = (U8)(((U32)FOSC*1000)/((U32)bdr*16)-1)    )
#define Uart_enable()           (UCSRB|=MSK_UART_ENABLE_RX|MSK_UART_ENABLE_TX)
#define Uart_tx_ready()         (UCSRA&MSK_UART_DRE)
#define Uart_set_tx_busy()
#define Uart_send_byte(ch)      (UDR=ch)
#define Uart_rx_ready()         (UCSRA&MSK_UART_RX_DONE)
#define Uart_get_byte()         (UDR)
#define Uart_ack_rx_byte()

#define Uart_enable_it_rx()    (UCSRB|=MSK_UART_ENABLE_IT_RX)
#define Uart_enable_it_tx()    (UCSRB|=MSK_UART_ENABLE_IT_TX)
#define Uart_disable_it_rx()   (UCSRB&=~MSK_UART_ENABLE_IT_RX)


#define Uart_disable_tx()           (UCSRB &= ~(1<<TXEN))
#define Uart_disable_rx()           (UCSRB &= ~(1<<RXEN))
#define Uart_enable_tx()            (UCSRB |= (1<<TXEN))
#define Uart_enable_rx()            (UCSRB |= (1<<RXEN))
#define Uart_tx_complet()           (UCSRA&MSK_UART_TX_COMPLET)
#define Uart_clear_flag_tx()        (UCSRA |= (1<<TXC))

#define Uart_enable_mpcm()          (UCSRA |= (1<<MPCM))
#define Uart_disable_mpcm()         (UCSRA &= ~(1<<MPCM))
#define Uart_set_TXB8()             (UCSRB |= (1<<TXB8))
#define Uart_clear_TXB8()           (UCSRB &= ~(1<<TXB8))
#define Uart_enable_9bit()          (UCSRB |= (1<<UCSZ2), UCSRC |= ((1<<UCSZ1) | (1<<UCSZ0)))
#define Uart_enable_8bit()          (UCSRB &= ~(1<<UCSZ2), UCSRC |= ((1<<UCSZ1) | (1<<UCSZ0))))
#define Uart_enable_2stop()         (UCSRC |= (1<USBS))
#define Uart_enable_1stop()         (UCSRC &= ~(1<USBS))
#define Uart_disable_parity()       (UCSRC &= ~((1<UMP1) | (1<<UMP0)))

#endif
