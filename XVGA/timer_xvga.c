
#include "board.h"
#include "tc.h"
#include "pm.h"
#include "usart.h"
#include "timer_xvga.h"
#include "usart_xvga.h"
#include "acces_porturi_xvga.h"
#include "flashc.h"
#include "configuration_xvga.h"

volatile avr32_tc_t *tc = XVGA_TC;
volatile avr32_pm_t *pm = &AVR32_PM;

volatile U32 TimeOutForRS232;
volatile U32 TimerForLED;
volatile U32 TimerForReset;
volatile U32 clock_100us = 0;
volatile U32 TimerForClipping;


__attribute__((__interrupt__))
void Timer0_irq(void)
{
  // clock 100us
  TimerForLED++;
  TimerForReset++;
  clock_100us++;
  TimeOutForRS232++;
  TimerForClipping++;
  move_grid();
  //usart_write_line(XVGA_USART, "Goodbye!\n");
  tc->channel[0].sr;
}


__attribute__((__naked__))
void general_int_handler( void )
{
  __asm__ __volatile__ (
    "pushm   r0-r12, lr\n\t"  /* Save registers not saved upon NMI exception. */
  );
  //eic_clear_interrupt_line(&AVR32_EIC, EXT_INT_EXAMPLE_LINE1);
  __asm__ __volatile__ (
    "popm   r0-r12, lr\n\t"   /* Restore the registers. */
    "rete"                    /* Leaving the exception handler. */
  );
}

__attribute__((__naked__))
void Bus_Error_Instruction_Fetch_int_handler( void )
{
  __asm__ __volatile__ (
    "pushm   r0-r12, lr\n\t"  /* Save registers not saved upon NMI exception. */
  );
  //eic_clear_interrupt_line(&AVR32_EIC, EXT_INT_EXAMPLE_LINE1);
  __asm__ __volatile__ (
    "popm   r0-r12, lr\n\t"   /* Restore the registers. */
    "rete"                    /* Leaving the exception handler. */
  );
}

__attribute__((__naked__))
void Illegal_Opcode_int_handler( void )
{
  __asm__ __volatile__ (
    "pushm   r0-r12, lr\n\t"  /* Save registers not saved upon NMI exception. */
  );
  //eic_clear_interrupt_line(&AVR32_EIC, EXT_INT_EXAMPLE_LINE1);
  __asm__ __volatile__ (
    "popm   r0-r12, lr\n\t"   /* Restore the registers. */
    "rete"                    /* Leaving the exception handler. */
  );
}


void init_timer(void)
{
  gpio_enable_module_pin(XVGA_TC_CHANNEL_PIN, XVGA_TC_CHANNEL_FUNCTION);

  // Options for waveform genration.
   tc_waveform_opt_t waveform_opt =
   {
     .channel  = XVGA_TC_CHANNEL_ID,        // Channel selection.

     .bswtrg   = TC_EVT_EFFECT_NOOP,           // Software trigger effect on TIOB.
     .beevt    = TC_EVT_EFFECT_NOOP,           // External event effect on TIOB.
     .bcpc     = TC_EVT_EFFECT_NOOP,           // RC compare effect on TIOB.
     .bcpb     = TC_EVT_EFFECT_NOOP,           // RB compare effect on TIOB.

     .aswtrg   = TC_EVT_EFFECT_NOOP,           // Software trigger effect on TIOA.
     .aeevt    = TC_EVT_EFFECT_NOOP,           // External event effect on TIOA.
     .acpc     = TC_EVT_EFFECT_TOGGLE,         // RC compare effect on TIOA: toggle.
     .acpa     = TC_EVT_EFFECT_TOGGLE,         // RA compare effect on TIOA: toggle (other possibilities are none, set and clear).

     .wavsel   = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER,      //TC_WAVEFORM_SEL_UP_MODE,
                                               //TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER // Waveform selection: Up mode without automatic trigger on RC compare.
     .enetrg   = FALSE,                        // External event trigger enable.
     .eevt     = TC_EXT_EVENT_SEL_TIOB_INPUT,  // External event selection.
     .eevtedg  = TC_SEL_NO_EDGE,               // External event edge selection.
     .cpcdis   = FALSE,                        // Counter disable when RC compare.
     .cpcstop  = FALSE,                        // Counter clock stopped with RC compare.

     .burst    = TC_BURST_NOT_GATED,           // Burst signal selection.
     .clki     = TC_CLOCK_RISING_EDGE,         // Clock inversion.
     .tcclks   = TC_CLOCK_SOURCE_TC3           // Internal source clock 3, connected to fPBA / 8.
   };


   // Initialize the timer/counter.
   tc_init_waveform(XVGA_TC, &waveform_opt);  // Initialize the timer/counter waveform.

   // Set the compare triggers.
   tc_write_ra(XVGA_TC, XVGA_TC_CHANNEL_ID, 20);      // Set RA value.
   tc_write_rc(XVGA_TC, XVGA_TC_CHANNEL_ID, 400);     // 80 Set RC value.

   // Tclk = 32MHz
   // Ttimer = 32/8MHz = 4 MHz; Ttimer = 250ns; Tint = 250ns * 400 = 100us

   // Start the timer/counter.
   tc_start(XVGA_TC, XVGA_TC_CHANNEL_ID);
}

void init_timer_irq(void)
{
  INTC_init_interrupts();
  // Register the RTC interrupt handler to the interrupt controller.
  INTC_register_interrupt(&Timer0_irq, AVR32_TC_IRQ0, AVR32_INTC_INT0);

  static const tc_interrupt_t b_TCO_INT = {.cpcs = 1};//cpcs = 1, covfs = 1};

  tc_configure_interrupts(XVGA_TC, XVGA_TC_CHANNEL_ID, &b_TCO_INT);
}


void local_start_pll_clock(void)
{
  /* \note All calculations here suppose that the Osc0 frequency is 12MHz. */
  pm_switch_to_osc0(pm, FOSC0, OSC0_STARTUP);  // Switch main clock to Osc0.

  pm_pll_setup(pm,
               0,   // use PLL0
               15,  // MUL=7 in the formula
               3,   // DIV=1 in the formula
               0,   // Sel Osc0/PLL0 or Osc1/PLL1
               16); // lockcount in main clock for the PLL wait lock
  //fvco = 12MHz * (15+1)/3 = 4 * 16MHz = 64 MHz
  //This function will set a PLL option.
  pm_pll_set_option(pm,
                    0,  //pll PLL number 0
                    1,  //pll_freq Set to 1 for VCO frequency range 80-180MHz, set to 0 for VCO frequency range 160-240Mhz.
                    0,  //pll_div2 Divide the PLL output frequency by 2 (this settings does not change the FVCO value)
                    0); //pll_wbwdisable 1 Disable the Wide-Bandith Mode (Wide-Bandwith mode allow a faster startup time and out-of-lock time). 0 to enable the Wide-Bandith Mode.
  /*PLL output VCO frequency is 96MHz. We divide it by 2 with the pll_div2=1. This enable to get later main clock to 48MHz */
  /* Enable PLL0 */
  pm_pll_enable(pm,0);

  /* Wait for PLL0 locked */
  pm_wait_for_pll0_locked(pm) ;

  /* Pheripheral Bus A clock divisor enable = 1    32MHz
     Pheripheral Bus A select = 0
     Pheripheral Bus B clock divisor enable = 0    64MHz
     Pheripheral Bus B select = 0
     High Speed Bus clock divisor enable = 0       64MHz
     High Speed Bus select = 0 */

  pm_cksel(pm, 1, 0, 0, 0, 0, 0);

  // Set one wait-state (WS) for flash controller. 0 WS access is up to 30MHz for HSB/CPU clock.
  // As we want to have 48MHz on HSB/CPU clock, we need to set 1 WS on flash controller.
  flashc_set_wait_state(1);

  pm_switch_to_clock(pm, AVR32_PM_MCSEL_PLL0); /* Switch main clock to 48MHz */
}



//! Values to store in the interrupt priority registers for the various interrupt priority levels.
extern const unsigned int ipr_val[AVR32_INTC_NUM_INT_LEVELS];

//! Creates a table of interrupt line handlers per interrupt group in order to optimize RAM space.
//! Each line handler table contains a set of pointers to interrupt handlers.
#define DECL_INT_LINE_HANDLER_TABLE(GRP, unused) \
static volatile __int_handler _int_line_handler_table_##GRP[Max(AVR32_INTC_NUM_IRQS_PER_GRP##GRP, 1)];
MREPEAT(AVR32_INTC_NUM_INT_GRPS, DECL_INT_LINE_HANDLER_TABLE, ~);
#undef DECL_INT_LINE_HANDLER_TABLE

//! Table containing for each interrupt group the number of interrupt request
//! lines and a pointer to the table of interrupt line handlers.
static const struct
{
  unsigned int num_irqs;
  volatile __int_handler *_int_line_handler_table;
} _int_handler_table[AVR32_INTC_NUM_INT_GRPS] =
{
#define INSERT_INT_LINE_HANDLER_TABLE(GRP, unused) \
  {AVR32_INTC_NUM_IRQS_PER_GRP##GRP, _int_line_handler_table_##GRP},
  MREPEAT(AVR32_INTC_NUM_INT_GRPS, INSERT_INT_LINE_HANDLER_TABLE, ~)
#undef INSERT_INT_LINE_HANDLER_TABLE
};


/*! \brief Default interrupt handler.
 *
 * \note Taken and adapted from Newlib.
 */

__attribute__((__interrupt__))
static void _unhandled_interrupt(void)
{
  // Catch unregistered interrupts.
  while (TRUE);
}


__int_handler _get_interrupt_handler(unsigned int int_lev)
{
  unsigned int int_grp = AVR32_INTC.icr[AVR32_INTC_INT3 - int_lev];
  unsigned int int_req = AVR32_INTC.irr[int_grp];

  return (int_req) ? _int_handler_table[int_grp]._int_line_handler_table[32 - clz(int_req) - 1] : NULL;
}


void INTC_init_interrupts(void)
{
  unsigned int int_grp, int_req;

  for (int_grp = 0; int_grp < AVR32_INTC_NUM_INT_GRPS; int_grp++)
  {
    for (int_req = 0; int_req < _int_handler_table[int_grp].num_irqs; int_req++)
    {
      _int_handler_table[int_grp]._int_line_handler_table[int_req] = &_unhandled_interrupt;
    }
    AVR32_INTC.ipr[int_grp] = ipr_val[AVR32_INTC_INT0];
  }
}


void INTC_register_interrupt(__int_handler handler, unsigned int irq, unsigned int int_lev)
{
  unsigned int int_grp = irq / AVR32_INTC_MAX_NUM_IRQS_PER_GRP;

  _int_handler_table[int_grp]._int_line_handler_table[irq % AVR32_INTC_MAX_NUM_IRQS_PER_GRP] = handler;
  AVR32_INTC.ipr[int_grp] = ipr_val[int_lev & (AVR32_INTC_IPR_INTLEVEL_MASK >> AVR32_INTC_IPR_INTLEVEL_OFFSET)];
}
