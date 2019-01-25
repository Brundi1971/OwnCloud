
#ifndef _RS232_COM_H
#define _RS232_COM_H

extern volatile U16 TimeOutForRS232_Contor;
extern volatile U16 Contor_booting_cameras;
extern volatile U16 Contor_RepeatAGC;
extern volatile U8 VisibleCameraFunctionsSelect;
extern volatile U8 ThermalCameraFunctionsSelect;
extern volatile U8 VisibleDataZoom[4];
extern volatile U8 VisibleDataFocus[4];

void boot_cameras(void);
void create_booting_command(void);
void init_UART1(void);

void ResetTimeOut_Contor(void);
void ResetRS232Reception(void);
void ReceiveDataFromRS232(void);
U8 TransmiteDataToRS232(void);

void read_command_from_master(void);
void create_command_stack(void);
void change_camera_function(void);
void interpret_commands_from_stack(void);
void load_transmission_buffer(void);
void prepare_for_receiving(void);
void prepare_for_receiving_and_take_data(void);
void perform_command(void);
U8 compare_strings(void);
U8 compare_strings_and_take_data(void);
U8 compare_for_overflow_or_syn(void);
void receive_acknowledge(void);

void set_MasterCommandForCamera(U8 Val);
U8 get_MasterCommandForCamera(void);
void set_VisibleDataZoom(U8 Index, U8 Val);
U8 get_VisibleDataZoom(U8 Index);
void set_VisibleDataFocus(U8 Index, U8 Val);
U8 get_VisibleDataFocus(U8 Index);
void set_ThermalDataZoom(U8 Index, U8 Val);
U8 get_ThermalDataZoom(U8 Index);
void set_ThermalDataFocus(U8 Index, U8 Val);
U8 get_ThermalDataFocus(U8 Index);
void set_DataTemperatura(U8 Val);
U8 get_DataTemperatura(void);
void set_DataPresiune(U8 Val);
U8 get_DataPresiune(void);
void set_CameraErrorType(U8 Val);
U8 get_CameraErrorType(void);

void camera_command_increase_function(void);
void camera_command_decrease_function(void);
void camera_command_switch_on_off_function(void);
void camera_command_select_visible_or_thermal(void);
void camera_command_stop(void);
U8 verify_the_check_sum_for_visible(void);
U8 verify_the_check_sum_for_thermal(void);
void save_AGC_SETPOINT(void);


#define REPEAT_AGC_TIME                                     10000  /* 0,5s */

#define SELECT_ZOOM                                         0x00
#define SELECT_FOCUS                                        0x01
#define SELECT_AGC                                          0x02
#define SELECT_POLARITY                                     0x03
#define THERMAL_CAMERA_IS_SWITCHED_OFF                      0x00
#define THERMAL_CAMERA_IS_SWITCHED_ON                       0x01
#define VISIBLE_SELECTED                                    0x00
#define THERMAL_SELECTED                                    0x01
#define THERMAL_ON                                          0x01
#define THERMAL_OFF                                         0x00

#define MSK_STATUS_SELECT_FUNCTII                           0x07
#define MSK_STATUS_SELECT_SPEED                             0x18

/* alocarea bitilor de comanda in variabila MasterCommandForCamera */
/*====================================================================*/

#define BIT_CAMERA_COMMAND_INCREASE                         5
#define BIT_CAMERA_COMMAND_DECREASE                         4
#define BIT_CAMERA_COMMAND_SEL_ZOOM_FOCUS_AGC_POLARITY      3
#define BIT_CAMERA_COMMAND_SPEED                            2
#define BIT_CAMERA_COMMAND_SEL_CAMERA_TYPE                  1
#define BIT_CAMERA_COMMAND_CAMERA_ON_OFF                    0
/*====================================================================*/

#define TRANSMISSION_OK                                     0
#define TRANSMISSION_PENDING                                1
                                       
#define NB_OF_INCERCARI                                     3
#define NB_MAX_OF_BYTES_RECEIVED_FROM_RS232                 50

#define DO_NOTHING                                          1
#define TRANSMIT_COMMAND                                    2
#define WAIT_FOR_ACKNOWLEDGE                                3
#define COMPARE_STRINGS                                     4
#define SEND_SYN                                            5
#define INSERT_DELAY                                        6
#define WAIT_FOR_ACKNOWLEDGE_AND_TAKE_DATA                  7
#define COMPARE_STRINGS_AND_TAKE_DATA                       8

#define NO_ERROR                                            0
#define CAMERA_NO_ANSWER_ERROR                              1
#define CAMERA_IS_OUT_OF_SYNCHRONIZATION                    2
#define CAMERA_MIS_MATCHING_STRINGS                         3

/* constante SEMAFOR */
#define ROSU                                                0
#define VERDE                                               1

/* constante setare speed motoare */ 
#define SPEED_25                                            0x00
#define SPEED_50                                            0x01
#define SPEED_75                                            0x02
#define SPEED_100                                           0x03

/* constante CAMERA DE ZI */
#define SELECT_VISIBLE                                      0x01
#define VISIBLE_POWER_UP                                    0x03
#define VISIBLE_POWER_DOWN                                  0x04
#define VISIBLE_CAMERA_ON                                   0x05
#define VISIBLE_CAMERA_OFF                                  0x06
#define VISIBLE_STOP_MOTORS                                 0x07
#define VISIBLE_SET_PWM                                     0x08
#define VISIBLE_SET_SPEED_25                                0x09
#define VISIBLE_SET_SPEED_50                                0x0a
#define VISIBLE_SET_SPEED_75                                0x0b
#define VISIBLE_SET_SPEED_100                               0x0c
#define VISIBLE_ZOOM_NARROW                                 0x0d
#define VISIBLE_ZOOM_WIDE                                   0x0e
#define VISIBLE_FOCUS_NEAR                                  0x0f
#define VISIBLE_FOCUS_FAR                                   0x10
#define VISIBLE_AGC_SETPOINT_UP                             0x11
#define VISIBLE_AGC_SETPOINT_DOWN                           0x12
#define VISIBLE_AGC_SETPOINT_SAVE                           0x13
#define VISIBLE_QUERY_ZOOM                                  0x14
#define VISIBLE_QUERY_FOCUS                                 0x15

#define FINALIZE_THE_COMMAND                                0x16
#define REPEAT_COMMAND                                      0x17
#define READ_ZOOM_AND_FOCUS                                 0x18
#define BOTH_CAMERAS_POWER_UP                               0x19
#define BOTH_CAMERAS_POWER_DOWN                             0x1a

#define SELECT_VISIBLE_REPLY                                0x30
#define VISIBLE_POWER_UP_REPLY                              0x31
#define VISIBLE_POWER_DOWN_REPLY                            0x32
#define VISIBLE_CAMERA_ON_REPLY                             0x33
#define VISIBLE_CAMERA_OFF_REPLY                            0x34
#define VISIBLE_STOP_MOTORS_REPLY                           0x35
#define VISIBLE_SET_PWM_REPLY                               0x36
#define VISIBLE_SET_SPEED_25_REPLY                          0x37
#define VISIBLE_SET_SPEED_50_REPLY                          0x38
#define VISIBLE_SET_SPEED_75_REPLY                          0x39
#define VISIBLE_SET_SPEED_100_REPLY                         0x3a
#define VISIBLE_ZOOM_NARROW_REPLY                           0x3b
#define VISIBLE_ZOOM_WIDE_REPLY                             0x3c
#define VISIBLE_FOCUS_NEAR_REPLY                            0x3d
#define VISIBLE_FOCUS_FAR_REPLY                             0x3e
#define VISIBLE_AGC_SETPOINT_UP_REPLY                       0x3f
#define VISIBLE_AGC_SETPOINT_DOWN_REPLY                     0x40
#define VISIBLE_AGC_SETPOINT_SAVE_REPLY                     0x41
#define VISIBLE_QUERY_ZOOM_REPLY_HEADER                     0x42
#define VISIBLE_QUERY_FOCUS_REPLY_HEADER                    0x43

#define BOTH_CAMERAS_POWER_UP_REPLY                         0x44
#define BOTH_CAMERAS_POWER_DOWN_REPLY                       0x45


/* constante CAMERA TERMICA */
#define SELECT_THERMAL                                      0x50
#define THERMAL_POWER_UP                                    0x51
#define THERMAL_POWER_DOWN                                  0x52
#define THERMAL_STOP_MOTORS                                 0x53
#define THERMAL_SET_SPEED_25                                0x54
#define THERMAL_SET_SPEED_50                                0x55
#define THERMAL_SET_SPEED_75                                0x56
#define THERMAL_SET_SPEED_100                               0x57
#define THERMAL_ZOOM_IN                                     0x58
#define THERMAL_ZOOM_OUT                                    0x59
#define THERMAL_FOCUS_IN                                    0x5a
#define THERMAL_FOCUS_OUT                                   0x5b
#define THERMAL_AGC_SETPOINT_UP                             0x5c
#define THERMAL_AGC_SETPOINT_DOWN                           0x5d
#define THERMAL_AGC_SETPOINT_SAVE                           0x5e
#define THERMAL_SET_COLD_WHITE                              0x5f
#define THERMAL_SET_HOT_WHITE                               0x60
#define THERMAL_QUERY_ZOOM                                  0x61
#define THERMAL_QUERY_FOCUS                                 0x62

#define SELECT_THERMAL_REPLY                                0x70
#define THERMAL_POWER_UP_REPLY                              0x71
#define THERMAL_POWER_DOWN_REPLY                            0x72
#define THERMAL_STOP_MOTORS_REPLY                           0x73
#define THERMAL_SET_SPEED_25_REPLY                          0x74
#define THERMAL_SET_SPEED_50_REPLY                          0x75
#define THERMAL_SET_SPEED_75_REPLY                          0x76
#define THERMAL_SET_SPEED_100_REPLY                         0x77
#define THERMAL_ZOOM_IN_REPLY                               0x78
#define THERMAL_ZOOM_OUT_REPLY                              0x79
#define THERMAL_FOCUS_IN_REPLY                              0x7a
#define THERMAL_FOCUS_OUT_REPLY                             0x7b
#define THERMAL_AGC_SETPOINT_UP_REPLY                       0x7c
#define THERMAL_AGC_SETPOINT_DOWN_REPLY                     0x7d
#define THERMAL_AGC_SETPOINT_SAVE_REPLY                     0x7e
#define THERMAL_QUERY_ZOOM_REPLY_HEADER                     0x7f
#define THERMAL_QUERY_FOCUS_REPLY_HEADER                    0x80

#define EXECUTE_5ms_DELAY                                   0x81
#define EXECUTE_10ms_DELAY                                  0x82
#define EXECUTE_500ms_DELAY                                 0x83

#endif 
