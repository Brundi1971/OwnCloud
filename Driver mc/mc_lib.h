
#ifndef _MC_LIB_H_
#define _MC_LIB_H_

extern Bool mc_run_stop;
extern U16 mc_cmd_highSpeed_V0;
extern U16 mc_cmd_highSpeed_V1;
extern U16 mc_cmd_lowSpeed_V0;
extern U16 mc_cmd_lowSpeed_V1;

void mc_motor_run_highSpeed(void);
void mc_motor_run_lowSpeed(void);
void mc_motor_stop(void);
void mc_motor_init(void);

void mc_set_motor_direction(U8 direction);
U8 mc_get_motor_direction(void);
void mc_forward(void);
void mc_backward(void); 

#endif
