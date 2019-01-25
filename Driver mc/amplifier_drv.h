
#ifndef AMPLIFIER_DRV_H
#define AMPLIFIER_DRV_H

   //! *** CONFIGURE THE AMPLIFIER 0 ***
   //! ****** AMP0CSR
   //! 7   AMP0EN    : 1 Enable the amplifier
   //! 6   AMP0IS    : 1 Shunt the amplifier input
   //! 5:4 AMP0G1:0  : Amplifier 0 gain selction bits
   //!               : 00 gain  5
   //!               : 01 gain 10 
   //!               : 10 gain 20 
   //!               : 11 gain 40 
   //! 3:2 Reserved  : Not used
   //! 1:0 AMP0TS1:0 : Amplifier 0 Trigger Source Bits
   //!               : 00 ADCclk/8
   //!               : 01 PSC0 
   //!               : 10 PSC1
   //!               : 11 PSC2
   //! ****** END AMP0CSR


/* Amplifier 0 Control and Status register */
//#define AMP0CSR _SFR_MEM8(0x76)

#define AMP0EN          7
#define AMP0IS          6
#define AMP0G1          5
#define AMP0G0          4
#define AMP0TS2         2
#define AMP0TS1         1
#define AMP0TS0         0

/* Amplifier 1 Control and Status register */
//#define AMP1CSR _SFR_MEM8(0x77)
#define AMP1EN          7
#define AMP1IS          6
#define AMP1G1          5
#define AMP1G0          4
#define AMP1TS2         2
#define AMP1TS1         1
#define AMP1TS0         0


#define Enable_amp0()                         (AMP0CSR |= (1<<AMP0EN))  
#define Is_amp0_enabled()                     (AMP0CSR &  (1<<AMP0EN))

#define Enable_amp0_input_shunt()             (AMP0CSR |=  (1<<AMP0IS))
#define Disable_amp0_input_shunt()            (AMP0CSR &= ~(1<<AMP0IS)) 

//! Amplifier 0 Gain  5 Configuration
#define Set_amp0_gain05()                                                      \
           (AMP0CSR &= ~(  (1<<AMP0G1) | (1<<AMP0G0) ) ) 
//! Amplifier 0 Gain 10 Configuration
#define Set_amp0_gain10()                                                     \
           (AMP0CSR &= ~(1<<AMP0G1),                                          \
            AMP0CSR |=  (1<<AMP0G0) )
//! Amplifier 0 Gain 20 Configuration
#define Set_amp0_gain20()                                                     \
           (AMP0CSR |=  (1<<AMP0G1),                                          \
            AMP0CSR &= ~(1<<AMP0G0) )
//! Amplifier 0 Gain 40 Configuration
#define Set_amp0_gain40()                                                     \
           (AMP0CSR |=  ((1<<AMP0G1) | (1<<AMP0G0)) ) 

//! Amplifier 0 clocked by ADCclk/8
#define Amp0_internal_clock()                                                 \
           (AMP0CSR &= ~(  (1<<AMP0TS1) | (1<<AMP0TS0) ) ) 
//! Amplifier 0 clocked by PSC0 trig signal
#define Amp0_psc0_clock()                                                     \
           (AMP0CSR &= ~(1<<AMP0TS1),                                         \
            AMP0CSR |=  (1<<AMP0TS0) )
//! Amplifier 0 clocked by PSC1 trig signal
#define Amp0_psc1_clock()                                                     \
           (AMP0CSR |=  (1<<AMP0TS1),                                         \
            AMP0CSR &= ~(1<<AMP0TS0) )
//! Amplifier 0 clocked by PSC2 trig signal
#define Amp0_psc2_clock()                                                     \
           (AMP0CSR |=  ((1<<AMP0TS1) | (1<<AMP0TS0)) ) 

#define Disable_amp0()                         (AMP0CSR &= ~(1<<AMP0EN) )  


   //! *** CONFIGURE THE AMPLIFIER 1 ***
   //! ****** AMP1CSR
   //! 7   AMP1EN    : 1 Enable the amplifier
   //! 6   AMP1IS    : 1 Shunt the amplifier input
   //! 5:4 AMP1G1:0  : Amplifier 1 gain selction bits
   //!               : 00 gain  5
   //!               : 01 gain 10 
   //!               : 10 gain 20 
   //!               : 11 gain 40 
   //! 3:2 Reserved  : Not used
   //! 1:0 AMP1TS1:0 : Amplifier 1 Trigger Source Bits
   //!               : 00 ADCclk/8
   //!               : 01 PSC0 
   //!               : 10 PSC1
   //!               : 11 PSC2
   //! ****** END AMP1CSR


#define Enable_amp1()                         (AMP1CSR |= (1<<AMP1EN))  
#define Is_amp1_enabled()                     (AMP1CSR &  (1<<AMP1EN))

#define Enable_amp1_input_shunt()             (AMP1CSR |=  (1<<AMP1IS))
#define Disable_amp1_input_shunt()            (AMP1CSR &= ~(1<<AMP1IS)) 

//! Amplifier 1 Gain  5 Configuration
#define Set_amp1_gain05()                                                      \
           (AMP1CSR &= ~(  (1<<AMP1G1) | (1<<AMP1G0) ) ) 
//! Amplifier 1 Gain 10 Configuration
#define Set_amp1_gain10()                                                     \
           (AMP1CSR &= ~(1<<AMP1G1),                                          \
            AMP1CSR |=  (1<<AMP1G0) )
//! Amplifier 1 Gain 20 Configuration
#define Set_amp1_gain20()                                                     \
           (AMP1CSR |=  (1<<AMP1G1),                                          \
            AMP1CSR &= ~(1<<AMP1G0) )
//! Amplifier 0 Gain 40 Configuration
#define Set_amp1_gain40()                                                     \
           (AMP1CSR |=  ((1<<AMP1G1) | (1<<AMP1G0)) ) 

//! Amplifier 1 clocked by ADCclk/8
#define Amp1_internal_clock()                                                 \
           (AMP1CSR &= ~(  (1<<AMP1TS1) | (1<<AMP1TS0) ) ) 
//! Amplifier 1 clocked by PSC0 trig signal
#define Amp1_psc0_clock()                                                     \
           (AMP1CSR &= ~(1<<AMP1TS1),                                         \
            AMP1CSR |=  (1<<AMP1TS0) )
//! Amplifier 1 clocked by PSC1 trig signal
#define Amp1_psc1_clock()                                                     \
           (AMP1CSR |=  (1<<AMP1TS1),                                         \
            AMP1CSR &= ~(1<<AMP1TS0) )
//! Amplifier 1 clocked by PSC2 trig signal
#define Amp1_psc2_clock()                                                     \
           (AMP1CSR |=  ((1<<AMP1TS1) | (1<<AMP1TS0)) ) 

#define Disable_amp1()                         (AMP1CSR &= ~(1<<AMP1EN) )  



#ifdef USE_AMP0 //!< this define is set on config.h file
    
void init_amp0(void);

#endif //USE_AMP0



#ifdef USE_AMP1 //!< this define is set on config.h file
    
void init_amp1(void);

#endif //USE_AMP1

#endif  // AMPLIFIER_DRV_H

