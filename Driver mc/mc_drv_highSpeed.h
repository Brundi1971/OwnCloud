
  #ifndef _MC_DRV_HIGH_H_
  #define _MC_DRV_HIGH_H_

//======================================================
  /* matricea 0 */

#define V02                100
#define V03                200
#define V04                300
#define V05                400
#define V06                500
#define V07                600
#define V08                700
#define V09                750

#define V010               100
#define V011               200
#define V012               300
#define V013               400
#define V014               500
#define V015               600
/* matricea 1 */
#define V12                100
#define V13                200
#define V14                300
#define V15                400
#define V16                500
#define V17                600
#define V18                700
#define V19                750

#define V110               100
#define V111               200
#define V112               300
#define V113               400
#define V114               500
#define V115               600
/* matricea 2 */
#define V22                100
#define V23                200
#define V24                300
#define V25                400
#define V26                500
#define V27                600
#define V28                700
#define V29                750

#define V210               250
#define V211               250
#define V212               250
#define V213               250
#define V214               250
#define V215               250
/* matricea 3 */
#define V32                100
#define V33                200
#define V34                300
#define V35                400
#define V36                500
#define V37                600
#define V38                700
#define V39                750

#define V310               250
#define V311               250
#define V312               250
#define V313               250
#define V314               250
#define V315               250
/* matricea 4 */
#define V42                100
#define V43                200
#define V44                300
#define V45                400
#define V46                500
#define V47                600
#define V48                700
#define V49                750

#define V410               250
#define V411               250
#define V412               250
#define V413               250
#define V414               250
#define V415               250

/* matricea 5 */
#define V52                100
#define V53                200
#define V54                300
#define V55                400
#define V56                500
#define V57                600
#define V58                700
#define V59                750

#define V510               250
#define V511               250
#define V512               250
#define V513               250
#define V514               250
#define V515               250
//======================================================

  // Output Pin configuration
  #define DDR_MC_DRIVER_B            DDRB
  #define DDR_MC_DRIVER_C            DDRC
  #define DDR_MC_DRIVER_D            DDRD

  #define PORT_MC_DRIVER_B           PORTB
  #define PORT_MC_DRIVER_C           PORTC
  #define PORT_MC_DRIVER_D           PORTD

  #define DDR_HALL_INPUT_C           DDRC
  #define DDR_HALL_INPUT_D           DDRD
  
  #define DDR_OVER_CURRENT_IN        DDRD
  #define PORT_OVER_CURRENT_IN       PORTD

  #define ACMP0                      PD7
  #define ACMP1                      PC6
  #define ACMP2                      PD5

  #define H_A        PD0
  #define L_A        PB7
  #define H_B        PC0
  #define L_B        PB6
  #define H_C        PB0
  #define L_C        PB1


  // Hardware initialization
  void mc_init_HW(void);
  void mc_init_port(void);
  void mc_init_IT(void);
  void PSC_Init (U16 OCRnRB,
                 U16 OCRnSB,
                 U16 OCRnRA,
                 U16 OCRnSA);
  
  void mc_prepare_for_highSpeed(void);
  
  // Phases commutation functions
  //U8 mc_get_hall(void);
  void mc_duty_cycle(U16 level);
  U8 mc_highSpeed(U8 position);
  U8 check_for_vertical_limit(void);

  // Sampling time configuration
  //void mc_config_sampling_period(void);

  // Over Current Detection
  //void mc_set_Over_Current(U8 Level);

  // output configuration
  //#define output_disconnected 0x01

 // #define Set_none() (TCCR1A=output_disconnected,TCCR3B=output_disconnected)
  //#define Set_timer_data_register_to_zero() (TCNT0=0x00)

  // MACRO for PSC initialization
  #define PSC_ONE_RAMP (0<<PMODE01)|(0<<PMODE00)
  #define PSC_TWO_RAMP (0<<PMODE01)|(1<<PMODE00)
  #define PSC_FOUR_RAMP (1<<PMODE01)|(0<<PMODE00)
  #define PSC_CENTERED (1<<PMODE01)|(1<<PMODE00)

  #define RAMP_MODE_NUMBER PSC_CENTERED

  #define SET_PLOCK (1<<PLOCK0)
  #define RELEASE_PLOCK (0<<PLOCK0)

  #define PRESC_NODIV     (0<<PPRE01)|(0<<PPRE00)
  #define PRESC_DIV_BY_4  (0<<PPRE01)|(1<<PPRE00)
  #define PRESC_DIV_BY_16 (1<<PPRE01)|(0<<PPRE00)
  #define PRESC_DIV_BY_64 (1<<PPRE01)|(1<<PPRE00)

  #define OUTPUT_ACTIVE_HIGH (1<<POP0)
  #define OUTPUT_ACTIVE_LOW  (0<<POP0)

  #define HALL_SENSOR_VALUE()        \
    ( (PIND & (1<<PIND7)) >> PIND7 ) \
  | ( (PINC & (1<<PINC6)) >> 5 )     \
  | ( (PIND & (1<<PIND5)) >> 3 )

  #define Clear_Port_Q1() (PORTB &= ( ~(1<<PORTB0)))
  #define Clear_Port_Q3() (PORTC &= ( ~(1<<PORTC0)))
  #define Clear_Port_Q5() (PORTD &= ( ~(1<<PORTD0)))
  #define Clear_Port_Q2() (PORTB &= ( ~(1<<PORTB1)))
  #define Clear_Port_Q4() (PORTB &= ( ~(1<<PORTB6)))
  #define Clear_Port_Q6() (PORTB &= ( ~(1<<PORTB7)))
  #define Set_Port_Q2()   (PORTB |=   (1<<PORTB1))
  #define Set_Port_Q4()   (PORTB |=   (1<<PORTB6))
  #define Set_Port_Q6()   (PORTB |=   (1<<PORTB7))

  // Six step commutation
  #define Set_none()                \
    PSOC0 = (0<<POEN0A)|(0<<POEN0B);\
    PSOC1 = (0<<POEN1A)|(0<<POEN1B);\
    PSOC2 = (0<<POEN2A)|(0<<POEN2B);\
    Set_Port_Q2();                \
    Set_Port_Q4();                \
    Set_Port_Q6();                \
    Clear_Port_Q1();                \
    Clear_Port_Q3();                \
    Clear_Port_Q5();

  #define Set_Q1Q4()                \
    PSOC0 = (0<<POEN0A)|(0<<POEN0B);\
    PSOC1 = (0<<POEN1A)|(1<<POEN1B);\
    PSOC2 = (1<<POEN2A)|(0<<POEN2B);

  #define Set_Q1Q6()                \
    PSOC0 = (0<<POEN0A)|(1<<POEN0B);\
    PSOC1 = (0<<POEN1A)|(0<<POEN1B);\
    PSOC2 = (1<<POEN2A)|(0<<POEN2B);

  #define Set_Q3Q2()                \
    PSOC0 = (0<<POEN0A)|(0<<POEN0B);\
    PSOC1 = (1<<POEN1A)|(0<<POEN1B);\
    PSOC2 = (0<<POEN2A)|(1<<POEN2B);

  #define Set_Q3Q6()                \
    PSOC0 = (0<<POEN0A)|(1<<POEN0B);\
    PSOC1 = (1<<POEN1A)|(0<<POEN1B);\
    PSOC2 = (0<<POEN2A)|(0<<POEN2B);

  #define Set_Q5Q2()                \
    PSOC0 = (1<<POEN0A)|(0<<POEN0B);\
    PSOC1 = (0<<POEN1A)|(0<<POEN1B);\
    PSOC2 = (0<<POEN2A)|(1<<POEN2B);

  #define Set_Q5Q4()                \
    PSOC0 = (1<<POEN0A)|(0<<POEN0B);\
    PSOC1 = (0<<POEN1A)|(1<<POEN1B);\
    PSOC2 = (0<<POEN2A)|(0<<POEN2B);

#endif
