
#ifndef _MC_CONTROL_H_
#define _MC_CONTROL_H_

U8 mc_control_speed(U8 speed_cmd);

void mc_regulation_loop();
void mc_set_Open_Loop();
void mc_set_Speed_Loop();

U16 mc_get_Duty_Cycle();

#endif
