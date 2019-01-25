
#ifndef ACCES_PORTURI_XVGA_H_
#define ACCES_PORTURI_XVGA_H_

#include <avr32/io.h>
#include "gpio.h"

#define GPIO                                          AVR32_GPIO

#define BS_CONTROLLER_XVGA                            17         //81
#define RESET_CONTROLLER_XVGA                         16         //80

#define CS_CONTOLLER_XVGA                             14         //78
#define MR_CONTROLLER_XVGA                            13         //77
#define WE1_CONTROLLER_XVGA                           12         //76
#define WE0_CONTROLLER_XVGA                           11         //75
#define RD1_CONTROLLER_XVGA                           10         //74
#define RD0_CONTROLLER_XVGA                           9          //73
#define WAIT_CONTROLLER_XVGA                          8          //72

#define LED00                                          0
#define LED01                                          1
#define LED02                                          2
#define LED03                                          3
#define LED04                                          4
#define LED05                                          5
#define LED06                                          6
#define LED07                                          7

#define PB00                                           10
#define PB01                                           9
#define PB02                                           11
#define PB03                                           12
#define PB04                                           13

/*
#define gpio_set_xvga_control(pin)     (gpio_port_CONTROL->ovr  |= (1 << ((unsigned int)pin & 0x1F)))
#define gpio_clr_xvga_control(pin)     (gpio_port_CONTROL->ovr  &= ~(1 << ((unsigned int)pin & 0x1F)))
*/

#define CS_RD0_RD1_UP             (gpio_port_CONTROL->ovr  |= ((1 << CS_CONTOLLER_XVGA) |\
                                                               (1 << RD0_CONTROLLER_XVGA) |\
                                                               (1 << RD1_CONTROLLER_XVGA)))

#define CS_RD0_RD1_DOWN           (gpio_port_CONTROL->ovr  &= ~((1 << CS_CONTOLLER_XVGA) |\
                                                                (1 << RD0_CONTROLLER_XVGA) |\
                                                                (1 << RD1_CONTROLLER_XVGA)))

#define CS_WR0_WR1_UP             (gpio_port_CONTROL->ovr  |= ((1 << CS_CONTOLLER_XVGA) |\
                                                               (1 << WE0_CONTROLLER_XVGA) |\
                                                               (1 << WE1_CONTROLLER_XVGA)))

#define CS_WR0_WR1_DOWN           (gpio_port_CONTROL->ovr  &= ~((1 << CS_CONTOLLER_XVGA) |\
                                                                (1 << WE0_CONTROLLER_XVGA) |\
                                                                (1 << WE1_CONTROLLER_XVGA)))

#define RD0_RD1_UP                (gpio_port_CONTROL->ovr  |= ((1 << RD0_CONTROLLER_XVGA) |\
                                                               (1 << RD1_CONTROLLER_XVGA)))

#define RD0_RD1_DOWN              (gpio_port_CONTROL->ovr  &= ~((1 << RD0_CONTROLLER_XVGA) |\
                                                                (1 << RD1_CONTROLLER_XVGA)))

#define WR0_WR1_UP                (gpio_port_CONTROL->ovr  |= ((1 << WE0_CONTROLLER_XVGA) |\
                                                               (1 << WE1_CONTROLLER_XVGA)))

#define WR0_WR1_DOWN              (gpio_port_CONTROL->ovr  &= ~((1 << WE0_CONTROLLER_XVGA) |\
                                                                (1 << WE1_CONTROLLER_XVGA)))


void light_the_led(void);
void configure_control_signals_of_xvga_controller(void);
void write_to_S1D13506(U32 address, U32 data);
U16 read_from_S1D13506(U32 address);
void toggle_the_led(void);
void reset_SD15036(void);
void configure_LEDs(void);
void toggle_LED(U8 led);
void switch_on_LED(int led);
void switch_off_LED(int led);
void show_data_by_LED(U8 dt);
void configure_to_acces_DRAM(void);
void configure_to_acces_REGISTERS(void);
inline void configure_general_regsiters();
inline void set_display_resolution_to_800_600(void);
inline void switch_on_display(void);
inline void switch_off_display(void);
inline void set_start_address_of_image(U32 address);
inline void set_width_of_the_display(U32 width);
inline void set_start_memory_address_cursor_and_switch_on_the_cursor(U8 address);
inline void set_start_memory_address_cursor_and_switch_off_the_cursor(U8 address);
inline void set_watchdog(U8 time);
inline void set_CRT_mode(void);
inline void set_position_of_cursor(signed short int x, signed short int y);
inline void set_swivel_view_mode(void);
inline void clear_swivel_view_mode(void);
inline void set_BitBLT_Source_Start_Address(U32 address);
inline void set_BitBLT_Destination_Start_Address(U32 address);
inline void set_BitBLT_Memory_Address_Offset(U32 offset);
inline void set_BitBLT_Width(U32 width);
inline void set_BitBLT_Height(U32 height);
inline void set_BitBLT_Background_Color(U16 bkColor);
inline void set_BitBLT_Foreground_Color(U16 frColor);
inline void set_BitBLT_Operation(U8 operation, U8 codeROP);
inline void start_BitBLT_Operation_with_liniar_destination(void);
inline void start_BitBLT_Operation(void);
inline void wait_to_complete_BitBLT_engine(void);
inline U8 check_if_complete_BitBLT_engine(void);
U8 query_PB0(void);
U8 query_PB1(void);
U8 query_PB2(void);
U8 query_PB3(void);
U8 query_PB4(void);


#endif /* ACCES_PORTURI_H_ */
