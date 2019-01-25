//main

#include "acces_porturi_xvga.h"
#include "timer_xvga.h"
#include "usart_xvga.h"
#include "drawings_xvga.h"
#include "configuration_xvga.h"
#include "math.h"
#include "usart.h"
#include "flash_xvga.h"


int main(void)
{
    local_start_pll_clock();
    init_usart();
    init_timer();
    Disable_global_interrupt();
    init_timer_irq();
    Enable_global_interrupt();
    //usart_write_line(XVGA_USART, "Hello !!!\n");
    reset_SD15036();
    TimerForReset = 0;
    while (TimerForReset != 200);  //20ms

    //init_all_variables();

    read_stored_values_of_RAM();
    read_stored_values_of_RAE();
    read_stored_values_of_RAL();

    configure_control_signals_of_xvga_controller();
    configure_to_acces_REGISTERS();
    configure_general_regsiters();

    fill_all_memory_with_color(DISPLAY_BACKGROUND_COLOR);

    switch_on_display();
    configure_to_acces_DRAM();

    copy_LettersCaps_in_memory_90(ADDRESS_LETTER_CAPS_STORAGE_90);
    copy_Letters_in_memory_90(ADDRESS_LETTERS_STORAGE_90);
    copy_FiguresAndSymbols_in_memory_90(ADDRESS_FIGURES_AND_SYMBOLS_90);

    configure_to_acces_REGISTERS();
    while(!(clear_cursor_memory(MEMORY_ADDRESS_OF_CURSOR_1)));


    MainScheduler = MSP_PREPARE_THE_RECEIVER;

    while(1)
    {
        switch(MainScheduler)
        {
            case MSP_PREPARE_THE_RECEIVER:
                                                    RS232Scheduler = CHECK_THE_FIRST_CHARACTER;
                                                    ByteIndex_ReceivedFromRS232 = 0;
                                                    MainScheduler = MSP_RECEIVING_TELEGRAMA;
                                                    break;
            case MSP_RECEIVING_TELEGRAMA:
                                                    if (receive_data_from_RS232() == TRUE)
                                                    MainScheduler = MSP_CHECK_TELEGRAMA;
                                                    break;
            case MSP_CHECK_TELEGRAMA:
                                                    if (check_corectness_of_telegram())
                                                    {if (NewCmdCode == DISPLAY_STATUS) MainScheduler = MSP_SEND_STATUS_TELEGRAM;
                                                    else MainScheduler = MSP_LOAD_ACK_TELEGRAM;}
                                                    else MainScheduler = MSP_LOAD_ERROR_TELEGRAM;
                                                    break;
            case MSP_SEND_STATUS_TELEGRAM:
                                                    NewCmdCode = CmdCode;
                                                    load_transmission_buffer_with_status(CodEroare,bMODE);
                                                    TransmissionIndex = 0;
                                                    MainScheduler = MSP_TRANSMIT_ACK;
                                                    break;
            case MSP_LOAD_ACK_TELEGRAM:
                                                    load_transmission_buffer(CodEroare,NewCmdCode);
                                                    TransmissionIndex = 0;
                                                    MainScheduler = MSP_TRANSMIT_ACK_AND_DRAW_FIGURES;
                                                    break;
            case MSP_LOAD_ERROR_TELEGRAM:
                                                    load_transmission_buffer(CodEroare,NewCmdCode);
                                                    TransmissionIndex = 0;
                                                    MainScheduler = MSP_TRANSMIT_ERROR;
                                                    break;
            case MSP_TRANSMIT_ACK_AND_DRAW_FIGURES:
                                                    if (draw_figures()) MainScheduler = MSP_TRANSMIT_ACK;
                                                    if (send_data_to_RS232()) MainScheduler = MSP_DRAW_FIGURES;
                                                    break;
            case MSP_TRANSMIT_ACK:
                                                    if (send_data_to_RS232())
                                                    MainScheduler = MSP_PREPARE_THE_RECEIVER;
                                                    break;
            case MSP_DRAW_FIGURES:
                                                    if (draw_figures())
                                                    MainScheduler = MSP_PREPARE_THE_RECEIVER;
                                                    break;
            case MSP_TRANSMIT_ERROR:
                                                    if (send_data_to_RS232())
                                                    MainScheduler = MSP_PREPARE_THE_RECEIVER;
                                                    break;
            default:
                                                    MainScheduler = MSP_PREPARE_THE_RECEIVER;
                                                    break;
        }

        if ((NewCmdCode == DISPLAY_STANDARD) && (sNewRStandard.bFlags & (1 << BIT_DISPLAY_RO)) && (sNewRStandard.bFlags & (1 << BIT_FLASHING_RO)))
        {
            if ((ToggleOnOffCursor == CURSOR_ON) && (TimerForClipping > 10000))
            {set_start_memory_address_cursor_and_switch_on_the_cursor(LOCATION_NO_OF_CURSOR_1);ToggleOnOffCursor = CURSOR_OFF;TimerForClipping = 0;}
            if ((ToggleOnOffCursor == CURSOR_OFF) && (TimerForClipping > 10000))
            {set_start_memory_address_cursor_and_switch_off_the_cursor(LOCATION_NO_OF_CURSOR_1); ToggleOnOffCursor = CURSOR_ON;TimerForClipping = 0;}
        }
    }
}


