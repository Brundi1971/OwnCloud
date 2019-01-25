
#ifndef CONFIGURATION_XVGA_H_
#define CONFIGURATION_XVGA_H_

//constante display
//======================================================================================================
#define WORD_MEMORY_SIZE                                   0x100000        //1MWord
#define BYTE_MEMORY_SIZE                                   (WORD_MEMORY_SIZE << 1)
#define DISPLAY_RESOLUTION_X                               800
#define DISPLAY_RESOLUTION_Y                               600
#define DISPLAY_OFFSET                                     DISPLAY_RESOLUTION_X/2
#define DISPLAY_RESOLUTION_X_90                            600
#define DISPLAY_RESOLUTION_Y_90                            800
#define DISPLAY_OFFSET_90                                  DISPLAY_RESOLUTION_Y_90/2
#define START_ADDRESS_MEMORY                               0
#define DISPLAY_BACKGROUND_COLOR                           0xFF
#define DISPLAY_FOREGROUND_COLOR                           0
#define DISPLAY_ORIGIN_X_90                                300
#define DISPLAY_ORIGIN_Y_90                                400

#define START_ADDRESS_PANEL1                               0
#define START_ADDRESS_PANEL2                               0x080000
#define START_ADDRESS_PANEL3                               0x100000
#define START_ADDRESS_PANEL4                               0x180000
#define PANEL_OFFSET                                       0x080000

//constante pentru semnalele de sincronizare
//======================================================================================================
#define HORIZONTAL_DISPLAY_WIDTH                           800/8-1
#define HORIZONTAL_NON_DISPLAY_WIDTH                       256/8-1
#define HRTC_START_POSITION                                40/8-1
#define HRTC_PULSE_WIDTH                                   0x80 | (128/8-1)
#define VERTICAL_DISPLAY_HEIGHT                            600-1
#define VERTICAL_NON_DISPLAY_HEIGHT                        28-1
#define VRTC_START_POSITION                                1-1
#define VRTC_PULSE_WIDTH                                   0x80 | (4-1)

//constante pentru configurare
//======================================================================================================
#define COLOR_DEPTH_4BPP                                   2
#define COLOR_DEPTH_8BPP                                   3
#define COLOR_DEPTH_15BPP                                  4
#define COLOR_DEPTH_16BPP                                  5
#define BIT_CONTROL_DISPLAY                                7

//constante cursor
//======================================================================================================
#define MAX_CURSOR_RESOLUTION                              64
#define CROSS_ARM                                          20
#define HALF_BIG_FRAME_X                                   30
#define HALF_BIG_FRAME_Y                                   20
#define HALF_SMALL_FRAME_X                                 3
#define HALF_SMALL_FRAME_Y                                 3
#define CURSOR_FOREGROUND_RED_COLOR                        0
#define CURSOR_FOREGROUND_GREEN_COLOR                      0
#define CURSOR_FOREGROUND_BLUE_COLOR                       0
#define CURSOR_GROUND_RED_COLOR                            0x1f
#define CURSOR_GROUND_GREEN_COLOR                          0x3f
#define CURSOR_GROUND_BLUE_COLOR                           0x1f
#define CURSOR_OFF                                         0
#define CURSOR_ON                                          1
#define CENTERED_CURSOR_POSITION_X                        (DISPLAY_RESOLUTION_X - MAX_CURSOR_RESOLUTION)/2 // 368
#define CENTERED_CURSOR_POSITION_Y                        (DISPLAY_RESOLUTION_Y - MAX_CURSOR_RESOLUTION)/2  //268

#define LOCATION_NO_OF_CURSOR_1                            170 //192 //180  // 0x76000
#define MEMORY_ADDRESS_OF_CURSOR_1                         (BYTE_MEMORY_SIZE - (LOCATION_NO_OF_CURSOR_1*8192))
#define LOCATION_NO_OF_CURSOR_2                            4//133  // 0xF6000
#define MEMORY_ADDRESS_OF_CURSOR_2                         (BYTE_MEMORY_SIZE - (LOCATION_NO_OF_CURSOR_2*8192))
#define LOCATION_NO_OF_CURSOR_3                            3//69  //0x176000
#define MEMORY_ADDRESS_OF_CURSOR_3                         (BYTE_MEMORY_SIZE - (LOCATION_NO_OF_CURSOR_3*8192))
#define LOCATION_NO_OF_CURSOR_4                            0
#define MEMORY_ADDRESS_OF_CURSOR_4                         0x1FFC00

//constante caractere
//======================================================================================================
#define CHARACTER_WIDTH                                    13
#define CHARACTER_HEIGHT                                   17
#define CHARACTER_WIDTH_90                                 20
#define CHARACTER_HEIGHT_90                                13
#define NO_OF_CAPS_LETTERS_CHARACTERS                      26
#define NO_OF_LETTERS_CHARACTERS                           26
#define NO_OF_FIGURES_AND_SYMBOLS                          33
#define MATRIX_LETTERS_CAPS_WIDTH                          NO_OF_CAPS_LETTERS_CHARACTERS*CHARACTER_WIDTH   //338
#define MATRIX_LETTERS_WIDTH                               NO_OF_LETTERS_CHARACTERS*CHARACTER_WIDTH         //338
#define MATRIX_FIGURURES_AND_SYMBOLS_WIDTH                 NO_OF_FIGURES_AND_SYMBOLS*CHARACTER_WIDTH       //416
#define MATRIX_LETTERS_CAPS_WIDTH_90                       NO_OF_CAPS_LETTERS_CHARACTERS*CHARACTER_WIDTH_90
#define MATRIX_LETTERS_WIDTH_90                            NO_OF_LETTERS_CHARACTERS*CHARACTER_WIDTH_90
#define MATRIX_FIGURURES_AND_SYMBOLS_WIDTH_90              NO_OF_FIGURES_AND_SYMBOLS*CHARACTER_WIDTH_90
#define ADDRESS_CHARACTERS_STORAGE                         0x76400 // 0x1FAC00 //
#define ADDRESS_LETTER_CAPS_STORAGE_90                     0x100000
#define ADDRESS_LETTERS_STORAGE_90                         ADDRESS_LETTER_CAPS_STORAGE_90 + (CHARACTER_HEIGHT_90 * DISPLAY_RESOLUTION_X)
#define ADDRESS_FIGURES_AND_SYMBOLS_90                     ADDRESS_LETTER_CAPS_STORAGE_90 + 2*(CHARACTER_HEIGHT_90 * DISPLAY_RESOLUTION_X)

//constante configurare DRIVER S1D13506
//======================================================================================================
#define BIT_SWIVEL_VIEW                                               6
#define MODE_CRT                                                      2
#define CONFIGURE_CRT_MODE                                            2

//constante pentru BitBLT engine
//======================================================================================================
#define BITBLT_OPERATION_WRITE                                        0
#define BITBLT_OPERATION_READ                                         1
#define BITBLT_OPERATION_MOVE_POSITIVE                                2
#define BITBLT_OPERATION_MOVE_NEGATIVE                                3
#define BITBLT_OPERATION_TRANSPARENT_WRITE                            4
#define BITBLT_OPERATION_TRANSPARENT_MOVE                             5
#define BITBLT_OPERATION_PETTERN_FILL_WITH_ROP                        6
#define BITBLT_OPERATION_PATTERN_FILL_WITH_TRANSPARENCY               7
#define BITBLT_OPERATION_COLOR_EXPANSION                              8
#define BITBLT_OPERATION_COLOR_EXPANSION_WITH_TRANSPARENCY            9
#define BITBLT_OPERATION_MOVE_WITH_COLOR_EXPANSION                   10
#define BITBLT_OPERATION_MOVE_WITH_COLOR_EXPANSION_1                 11
#define BITBLT_OPERATION_SOLID_FILL                                  12

#define CODE_ROP_COLOR_EXPANSION_0                                    5
#define CODE_ROP_COLOR_EXPANSION_1                                    1
#define CODE_ROP_COLOR_EXPANSION_2                                    2

//constante pentru functia CROSS
//======================================================================================================
#define CROSS_WIDTH                                                   2
#define FRAME_WIDTH                                                   2

//constante pentru fucntia STANDARD
//======================================================================================================
#define WIDTH_TM                                                      3
#define WIDTH_ERR                                                     6
#define WIDTH_X                                                       4
#define WIDTH_Y                                                       4

#define NO_REFRESH                                                    0
#define BIT_REFRESH_OF_TM                                             0
#define BIT_REFRESH_OF_ERR                                            1
#define BIT_REFRESH_OF_X                                              2
#define BIT_REFRESH_OF_Y                                              3
#define BIT_REFRESH_OF_FRAME_POSITION                                 4
#define BIT_REFRESH_OF_FLAGS                                          5

//constante FLAGS
//================================
#define BIT_DISPLAY_READY                                             0
#define BIT_DISPLAY_RO                                                1
#define BIT_FLASHING_RO                                               2
#define BIT_DISPLAY_ERR                                               3

#define MSK_READY_AND_ERR_FLAGS                                       9

//constante pentru functia GRID
//======================================================================================================
#define GRID_POSITION_X                                               0
#define GRID_POSITION_Y                                               150
#define GRID_CLEARANCE                                                10
#define GRID_LONG_Y                                                   500
#define MAX_ADDRESS                                                   DISPLAY_RESOLUTION_Y_90 * GRID_CLEARANCE / 2

#define WIDTH_GRID                                                    75
#define HEIGHT_GRID                                                   500

#define STOP_MOVING                                                   0
#define START_MOVING                                                  1

//constante pentru program principal
//======================================================================================================
#define MSP_PREPARE_THE_RECEIVER                                      1
#define MSP_RECEIVING_TELEGRAMA                                       2
#define MSP_CHECK_TELEGRAMA                                           3
#define MSP_LOAD_ACK_TELEGRAM                                         4
#define MSP_LOAD_ERROR_TELEGRAM                                       5
#define MSP_TRANSMIT_ACK_AND_DRAW_FIGURES                             6
#define MSP_TRANSMIT_ERROR                                            7
#define MSP_TRANSMIT_ACK                                              8
#define MSP_DRAW_FIGURES                                              9
#define MSP_SEND_STATUS_TELEGRAM                                     10
//======================================================================================================
#define BRAT_RAL                                                     6
#define BRAT_RAE_X                                                   6
#define BRAT_RAE_Y                                                   14
#define RO_LG_X                                                      6
#define RO_LG_Y                                                      4
//======================================================================================================
#define DEFAULT_RAM_LGX                                              334
#define DEFAULT_RAM_MINIM_LGX                                        300
#define DEFAULT_RAM_MAXIMUM_X                                        552
#define DEFAULT_RAM_MAXIMUM_Y                                        768

#define DEFAULT_RAM_OFFSET_X                                         0
#define DEFAULT_RAM_OFFSET_Y                                         0
#define DEFAULT_RAE_POZ_X                                            300
#define DEFAULT_RAE_POZ_Y                                            400-DEFAULT_RAM_LGX/2
#define DEFAULT_RAL_POZ_X                                            300
#define DEFAULT_RAL_POZ_Y                                            400
#define MAXIMUM_WORD_NUMBER                                          65535
#define FACTOR_DE_SCALARE_RPM                                        10000
#define FACTOR_LIMITA_RAE                                            14
#define FACTOR_LIMITA_RAL                                            10


#endif
