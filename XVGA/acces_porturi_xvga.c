

#include "acces_porturi_xvga.h"
#include "configuration_xvga.h"

volatile avr32_gpio_port_t *gpio_port_ADDRESS = &GPIO.port[0];
volatile avr32_gpio_port_t *gpio_port_DATA = &GPIO.port[1];
volatile avr32_gpio_port_t *gpio_port_CONTROL = &GPIO.port[2];
volatile avr32_gpio_port_t *gpio_port_LED_AND_PB = &GPIO.port[3];

volatile U32 DATA, ADDRESS;

void reset_SD15036(void)
{
    gpio_port_ADDRESS->ovr  =  0x00000000;
    gpio_port_ADDRESS->oder =  0x00FFFFFF;
    gpio_port_ADDRESS->gper |= 0x00FFFFFF;
    //gpio_port_ADDRESS->ierc =  0xFFFFFFFF;
    //gpio_port_ADDRESS->ifrc =  0xFFFFFFFF;

    gpio_port_DATA->ovr  =  0x00000000;
    gpio_port_DATA->oder =  0x00000000;//0x0000ffff;
    gpio_port_DATA->gper |= 0x0000FFFF;
    //gpio_port_DATA->ierc =  0xFFFFFFFF;
    //gpio_port_DATA->ifrc =  0xFFFFFFFF;

    gpio_port_CONTROL->ovr  = ((1 << BS_CONTROLLER_XVGA) |
                               (0 << RESET_CONTROLLER_XVGA) |
                               (1 << CS_CONTOLLER_XVGA) |
                               (0 << MR_CONTROLLER_XVGA) |
                               (1 << WE1_CONTROLLER_XVGA) |
                               (1 << WE0_CONTROLLER_XVGA) |
                               (1 << RD1_CONTROLLER_XVGA) |
                               (1 << RD0_CONTROLLER_XVGA) |
                               (1 << WAIT_CONTROLLER_XVGA));

    gpio_port_CONTROL->oder = ((1 << BS_CONTROLLER_XVGA) |
                               (1 << RESET_CONTROLLER_XVGA) |
                               (1 << CS_CONTOLLER_XVGA) |
                               (1 << MR_CONTROLLER_XVGA) |
                               (1 << WE1_CONTROLLER_XVGA) |
                               (1 << WE0_CONTROLLER_XVGA) |
                               (1 << RD1_CONTROLLER_XVGA) |
                               (1 << RD0_CONTROLLER_XVGA) |
                               (0 << WAIT_CONTROLLER_XVGA));

    gpio_port_CONTROL->gper |= ((1 << BS_CONTROLLER_XVGA) |
                               (1 << RESET_CONTROLLER_XVGA) |
                               (1 << CS_CONTOLLER_XVGA) |
                               (1 << MR_CONTROLLER_XVGA) |
                               (1 << WE1_CONTROLLER_XVGA) |
                               (1 << WE0_CONTROLLER_XVGA) |
                               (1 << RD1_CONTROLLER_XVGA) |
                               (1 << RD0_CONTROLLER_XVGA) |
                               (1 << WAIT_CONTROLLER_XVGA));

    //gpio_port_CONTROL->ierc = 0xFFFFFFFF;
    //gpio_port_CONTROL->ifrc = 0xFFFFFFFF;

    gpio_port_LED_AND_PB->ovr  =  0x00000000;
    gpio_port_LED_AND_PB->oder =  0x000000FF;
    gpio_port_LED_AND_PB->gper |= 0x0000FFFF;
    //gpio_port_LED_AND_PB->ierc =  0xFFFFFFFF;
    //gpio_port_LED_AND_PB->ifrc =  0xFFFFFFFF;
}

void configure_control_signals_of_xvga_controller(void)
{
    gpio_port_CONTROL->ovr  = ((1 << BS_CONTROLLER_XVGA) |
                               (1 << RESET_CONTROLLER_XVGA) |
                               (1 << CS_CONTOLLER_XVGA) |
                               (0 << MR_CONTROLLER_XVGA) |
                               (1 << WE1_CONTROLLER_XVGA) |
                               (1 << WE0_CONTROLLER_XVGA) |
                               (1 << RD1_CONTROLLER_XVGA) |
                               (1 << RD0_CONTROLLER_XVGA) |
                               (1 << WAIT_CONTROLLER_XVGA));
}

void reset_all_ports(void)
{
    gpio_port_ADDRESS->ovr  &= ~0x00ffffff;
    gpio_port_ADDRESS->oder |= 0x00ffffff;
    gpio_port_ADDRESS->gper |= 0x00ffffff;

    gpio_port_DATA->ovr  &= ~0x0000ffff;
    gpio_port_DATA->oder = 0x0000ffff;
    gpio_port_DATA->gper = 0x0000ffff;

    gpio_port_CONTROL->ovr  &= ~0x0000ffff;
    gpio_port_CONTROL->oder = 0x0000ffff;
    gpio_port_CONTROL->gper = 0x0000ffff;

    gpio_port_LED_AND_PB->ovr  &= ~0x00000000;
    gpio_port_LED_AND_PB->oder |= 0x000000FF;
    gpio_port_LED_AND_PB->gper |= 0x0000FFFF;
}

void configure_to_acces_DRAM(void)
{
    gpio_port_CONTROL->ovr  |= (1 << MR_CONTROLLER_XVGA);
    //gpio_port_CONTROL->ovr  |= (1 << MR_CONTROLLER_XVGA);
}

void configure_to_acces_REGISTERS(void)
{
    gpio_port_CONTROL->ovr  &= ~(1 << MR_CONTROLLER_XVGA);
    //gpio_port_CONTROL->ovr  &= ~(1 << MR_CONTROLLER_XVGA);
}

void write_to_S1D13506(U32 address, U32 data)
{
    gpio_port_ADDRESS->ovr = address;
    gpio_port_DATA->ovr = data;
    //gpio_port_DATA->ovr = data;
    gpio_port_DATA->oder |= 0x0000ffff;
    //gpio_port_DATA->ovr = data;
    //gpio_port_DATA->ovr = data;
    //gpio_port_DATA->ovr = data;

    CS_WR0_WR1_DOWN;
    CS_WR0_WR1_UP;
    //CS_WR0_WR1_UP;
}

U16 read_from_S1D13506(U32 address)
{
volatile U32 data;

    gpio_port_ADDRESS->ovr = address;
    gpio_port_DATA->ovr &= ~0x0000FFFF;
    gpio_port_DATA->oder &= ~0x0000FFFF;
    //gpio_port_DATA->ovr &= ~0x0000FFFF;

    CS_RD0_RD1_DOWN;
    CS_RD0_RD1_DOWN;
    data = gpio_port_DATA->pvr;
    //CS_RD0_RD1_DOWN;
    CS_RD0_RD1_UP;
    //CS_RD0_RD1_UP;

    return (U16)data;
}

inline void configure_general_regsiters()
{
    write_to_S1D13506(0,0);                    //Revision Code/Miscellaneous

    write_to_S1D13506(0x1FC,2);                //Display Mode

    write_to_S1D13506(4,0);                    //General IO Pins Configuration
    //write_to_S1D13506(8,0);                  //General IO Pins Control

    write_to_S1D13506(0x10,1);                 //Memory Clock Configuration - MCLK = BUSCLK
    write_to_S1D13506(0x14,2);                 //LCD Pixel Clock Configuration - CLKI2
    write_to_S1D13506(0x18,0);                 //CRT Pixel Clock Configuration - CLKI
    write_to_S1D13506(0x1C,2);                 //MediaPlug Pixel Clock Configuration - CLKI2
    write_to_S1D13506(0x1E,1);                 //Memory WAIT State Select
    write_to_S1D13506(0x20,0x600);             //Memory Configuration/DRAM Refresh Rate
                                               //EDO-DRAM with 2-CAS#
                                               //CBR Refresh
                                               //26,2ms
    write_to_S1D13506(0x2A,0x101);             //DRAM Timing Control
                                               //50ns

    write_to_S1D13506(0x50,HORIZONTAL_DISPLAY_WIDTH);            //CRT Horizontal Display Width
    write_to_S1D13506(0x52,((HRTC_START_POSITION)<<8) | (HORIZONTAL_NON_DISPLAY_WIDTH));
                                                                 //CRT Horizontal Non-Display Period //HRTC Start Position
    write_to_S1D13506(0x54,HRTC_PULSE_WIDTH);                    //HRTC Pulse Width

    write_to_S1D13506(0x56,VERTICAL_DISPLAY_HEIGHT);             //CRT Vertical Display Height
    write_to_S1D13506(0x58,((VRTC_START_POSITION) << 8) | (VERTICAL_NON_DISPLAY_HEIGHT));  //CRT Vertical Non-Display Period/VRTC Start Position
    write_to_S1D13506(0x5A,VRTC_PULSE_WIDTH);                    //VRTC Pulse Width/CRT Output Control


    write_to_S1D13506(0x60,COLOR_DEPTH_8BPP | (1 << BIT_CONTROL_DISPLAY));  //CRT Display Mode - 8bpp


    write_to_S1D13506(0x62,START_ADDRESS_MEMORY & 0xFFFF);       //CRT Display Start Address
	write_to_S1D13506(0x64,START_ADDRESS_MEMORY >> 16);

	write_to_S1D13506(0x66,DISPLAY_OFFSET);                      //CRT Memory Address Offset

    write_to_S1D13506(0x68,0);                                   //CRT Pixel Panning
    write_to_S1D13506(0x6A,0);                                   //CRT Display FIFO High Threshold

    write_to_S1D13506(0x80, (LOCATION_NO_OF_CURSOR_1 << 8 ) | CURSOR_OFF);//CRT Ink/Cursor Control/CRT Ink/Cursor Start Address


    write_to_S1D13506(0x82,CENTERED_CURSOR_POSITION_X);          //CRT Cursor X Position
    write_to_S1D13506(0x84,CENTERED_CURSOR_POSITION_Y);          //CRT Cursor Y Position

    write_to_S1D13506(0x86,(CURSOR_FOREGROUND_GREEN_COLOR << 8) | CURSOR_FOREGROUND_BLUE_COLOR);  //CRT Ink/Cursor Blue Color 0/CRT Ink/Cursor Green Color 0
    write_to_S1D13506(0x88,CURSOR_FOREGROUND_RED_COLOR);         //CRT Ink/Cursor Red Color 0
    write_to_S1D13506(0x8A,(CURSOR_GROUND_GREEN_COLOR << 8) | CURSOR_GROUND_BLUE_COLOR);  //CRT Ink/Cursor Blue Color 1/CRT Ink/Cursor Green Color 1
    write_to_S1D13506(0x8C,CURSOR_GROUND_RED_COLOR);             //CRT Ink/Cursor Red Color 1

    write_to_S1D13506(0x1F0,0);
    write_to_S1D13506(0x1F4,0);
    write_to_S1D13506(0x1FC,CONFIGURE_CRT_MODE);
}


inline void set_display_resolution_to_800_600(void)
{
    write_to_S1D13506(0x50,HORIZONTAL_DISPLAY_WIDTH);  //CRT Horizontal Display Width
    write_to_S1D13506(0x52,((HRTC_START_POSITION)<<8) | (HORIZONTAL_NON_DISPLAY_WIDTH));
                                                              //CRT Horizontal Non-Display Period //HRTC Start Position
    write_to_S1D13506(0x54,HRTC_PULSE_WIDTH);  //HRTC Pulse Width

    write_to_S1D13506(0x56,VERTICAL_DISPLAY_HEIGHT);  //CRT Vertical Display Height
    write_to_S1D13506(0x58,((VRTC_START_POSITION) << 8) | (VERTICAL_NON_DISPLAY_HEIGHT));  //CRT Vertical Non-Display Period/VRTC Start Position
    write_to_S1D13506(0x5A,VRTC_PULSE_WIDTH);  //VRTC Pulse Width/CRT Output Control
}

inline void switch_off_display(void)
{
    write_to_S1D13506(0x60,COLOR_DEPTH_8BPP | (1 << BIT_CONTROL_DISPLAY));
}

inline void switch_on_display(void)
{
    write_to_S1D13506(0x60,COLOR_DEPTH_8BPP & ~(1 << BIT_CONTROL_DISPLAY));
}

inline void set_start_address_of_image(U32 address)
{
    write_to_S1D13506(0x62,address & 0xFFFF);        //CRT Display Start Address 0 1
    write_to_S1D13506(0x64,address >> 16);           //CRT Display Start Address 2
}


inline void set_width_of_the_display(U32 width)
{
	write_to_S1D13506(0x66,width);
}

inline void set_start_memory_address_cursor_and_switch_on_the_cursor(U8 address)
{
   write_to_S1D13506(0x80, address << 8 | 1);
}

inline void set_start_memory_address_cursor_and_switch_off_the_cursor(U8 address)
{
   write_to_S1D13506(0x80, address<<8);
}

inline void set_watchdog(U8 time)
{
    write_to_S1D13506(0x1F4,time);
}

inline void set_CRT_mode(void)
{
    write_to_S1D13506(0x1FC,2);
}

inline void set_position_of_cursor(signed short int x, signed short int y)
{
    if (x < 0) x = ((-x) | 0x8000);
    if (y < 0) y = ((-y) | 0x8000);

    write_to_S1D13506(0x82,x);
    write_to_S1D13506(0x84,y);
}

inline void set_swivel_view_mode(void)
{
    write_to_S1D13506(0x1FC,MODE_CRT | (1 << BIT_SWIVEL_VIEW));
    write_to_S1D13506(0x40,0);
}

inline void clear_swivel_view_mode(void)
{
    write_to_S1D13506(0x40,0);
    write_to_S1D13506(0x1FC,MODE_CRT | (0 << BIT_SWIVEL_VIEW));
}
//===================================================================
//BitBLT functions
//===================================================================
inline void set_BitBLT_Source_Start_Address(U32 address)
{
    write_to_S1D13506(0x104,address);
    write_to_S1D13506(0x106,(address >> 16));
}

inline void set_BitBLT_Destination_Start_Address(U32 address)
{
    write_to_S1D13506(0x108,address);
    write_to_S1D13506(0x10A,(address >> 16));
}

inline void set_BitBLT_Memory_Address_Offset(U32 offset)
{
    write_to_S1D13506(0x10C,offset);
}

inline void set_BitBLT_Width(U32 width)
{
    write_to_S1D13506(0x110,width);
}

inline void set_BitBLT_Height(U32 height)
{
    write_to_S1D13506(0x112,height);
}

inline void set_BitBLT_Background_Color(U16 bkColor)
{
    write_to_S1D13506(0x114,bkColor);
}

inline void set_BitBLT_Foreground_Color(U16 frColor)
{
    write_to_S1D13506(0x118,frColor);
}

inline void set_BitBLT_Operation(U8 operation, U8 codeROP)
{
    write_to_S1D13506(0x102,(operation << 8) | codeROP);
}

inline void start_BitBLT_Operation(void)
{
    write_to_S1D13506(0x100,0x80);
}

inline void wait_to_complete_BitBLT_engine(void)
{
    while ((read_from_S1D13506(0x100) & 0x0080) == 0x0080);
}

inline U8 check_if_complete_BitBLT_engine(void)
{
    if ((read_from_S1D13506(0x100) & 0x0080) == 0x0080) return FALSE;
    else return TRUE;
}

inline void start_BitBLT_Operation_with_liniar_destination(void)
{
    write_to_S1D13506(0x100,0x82);
}



U8 query_PB0(void)
{
    if (((gpio_port_LED_AND_PB->pvr) & (1<<PB00)) != (U32)(1<<PB00)) return TRUE;
    else return FALSE;
}

U8 query_PB1(void)
{
    if (((gpio_port_LED_AND_PB->pvr) & (1<<PB01)) != (U32)(1<<PB01)) return TRUE;
    else return FALSE;
}

U8 query_PB2(void)
{
    if (((gpio_port_LED_AND_PB->pvr) & (1<<PB02)) != (U32)(1<<PB02)) return TRUE;
    else return FALSE;
}

U8 query_PB3(void)
{
    if (((gpio_port_LED_AND_PB->pvr) & (1<<PB04)) != (U32)(1<<PB04)) return TRUE;
    else return FALSE;
}

U8 query_PB4(void)
{
    if (((gpio_port_LED_AND_PB->pvr) & (1<<PB03)) != (U32)(1<<PB03)) return TRUE;
    else return FALSE;
}

void toggle_LED(U8 led)
{
    volatile U32 temp;

    temp = gpio_port_LED_AND_PB->ovr;
    temp = temp ^ (1<<led);
    gpio_port_LED_AND_PB->ovr = temp;
}

void switch_on_LED(int led)
{
    gpio_port_LED_AND_PB->ovr |= 1 << led;
}

void switch_off_LED(int led)
{
    gpio_port_LED_AND_PB->ovr &= ~(1 << led);
}

void show_data_by_LED(U8 dt)
{
    //volatile U32 temp = 0;
    gpio_port_LED_AND_PB->ovr &= 0xffffff00;
    gpio_port_LED_AND_PB->ovr |= (U32)dt;
}

