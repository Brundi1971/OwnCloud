
#ifndef TIMER_XVGA_H_
#define TIMER_XVGA_H_

#define XVGA_TC                                     (&AVR32_TC)
#define XVGA_TC_CHANNEL_ID                          0
#define XVGA_TC_CHANNEL_PIN                         AVR32_TC_A0_0_0_PIN
#define XVGA_TC_CHANNEL_FUNCTION                    AVR32_TC_A0_0_0_FUNCTION

#define AVR32_INTC_MAX_NUM_IRQS_PER_GRP             32
#define AVR32_INTC_NUM_INT_LEVELS                   (1 << AVR32_INTC_IPR_INTLEVEL_SIZE)

typedef void (*__int_handler)(void);

extern volatile U32 TimerForLED;
extern volatile U32 TimerForReset;
extern volatile U32 clock_100us;
extern volatile U32 TimeOutForRS232;
extern volatile U32 TimerForClipping;

void init_timer(void);
void init_timer_irq(void);
void local_start_pll_clock(void);

extern void INTC_init_interrupts(void);
extern void INTC_register_interrupt(__int_handler handler, unsigned int irq, unsigned int int_lev);

#endif
