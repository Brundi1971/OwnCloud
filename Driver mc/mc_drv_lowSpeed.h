
#ifndef _MC_DRV_LOW_H_
#define _MC_DRV_LOW_H_

extern volatile U8   Theta;
extern volatile U8   Sector_lowSpeed;

#define PSC_Start()                             PCTL2 |= (1<<PCCYC2)|(1<<PRUN2);
#define PSC_Stop()                              PCTL2 |= (1<<PCCYC2);

#define  mc_connect_drivers()                   PSOC0 = (1<<POEN0B)|(1<<POEN0A);\
                                                PSOC1 = (1<<POEN1B)|(1<<POEN1A);\
                                                PSOC2 = (1<<POEN2B)|(1<<POEN2A);

#define PWM0_OFF()                              PSOC0 = (0<<POEN0A)|(0<<POEN0B);\
                                                PSOC1 = (1<<POEN1A)|(1<<POEN1B);\
                                                PSOC2 = (1<<POEN2A)|(1<<POEN2B);

#define PWM1_OFF()                              PSOC0 = (1<<POEN0A)|(1<<POEN0B);\
                                                PSOC1 = (0<<POEN1A)|(0<<POEN1B);\
                                                PSOC2 = (1<<POEN2A)|(1<<POEN2B);

#define PWM2_OFF()                              PSOC0 = (1<<POEN0A)|(1<<POEN0B);\
                                                PSOC1 = (1<<POEN1A)|(1<<POEN1B);\
                                                PSOC2 = (0<<POEN2A)|(0<<POEN2B);

#define MAX_PWM                                 802
#define MIN_PWM                                 2

#define MARJA_SIGURANTA                         0

#define DEAD_TIME                               8
#define MAX_AMPLITUDE                           (((MAX_PWM - MIN_PWM) / 2) - DEAD_TIME - MARJA_SIGURANTA)

#define LOW_SPEED_APMPLITUDE                    10

#define MAX_THETA                               240
#define CLEARENCE_FOR_LOW_SPEED                 5   /* aceste numere se sar fata de valoarea de 120 */
#define ADVANCE                                 0

#define DELAY_OF_AMP_REFRESH_FOR_LOW_SPEED      4   /* valoarea factorului de unplere creste progresiv cu 2 la fiecare 200us, adica 4x50us */
#define DELAY_OF_THETA_REFRESH                  80  /* acest parametru determina viteza mica */

/* determina curentul prin motor in fuctie de tensiunea de alimenatre a motorului
de rugula acesti parametrii se seteaza astfel incat curentul prin motor sa ramana constant 
la orice tensiuena de alimentare */

#define VL0                                     50+LOW_SPEED_APMPLITUDE
#define VL1                                     50+LOW_SPEED_APMPLITUDE
#define VL2                                     50+LOW_SPEED_APMPLITUDE
#define VL3                                     50+LOW_SPEED_APMPLITUDE
#define VL4                                     50+LOW_SPEED_APMPLITUDE
#define VL5                                     50+LOW_SPEED_APMPLITUDE

#define INDEX_AMPLITUDE                         100

void PSC_Load_lowSpeed( U16 dt0a,  U16 dt1a,
                        U16 dt0b,  U16 dt1b,
                        U16 dt0c,  U16 dt1c);

void mc_prepare_for_lowSpeed(void);
U8 mc_lowSpeed(void);
U16 get_IndexZero_LowSpeed(void);
void set_IndexZero_LowSpeed(U16 nb);

#endif
