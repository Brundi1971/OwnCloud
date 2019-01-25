
#include "config.h"
#include "mc_lib.h"
#include "mc_drv_highSpeed.h"
#include "mc_drv_lowSpeed.h"
#include "mc_rs485.h"
#include "mc_resolver.h"

Bool mc_direction = CW;
U16 mc_cmd_highSpeed_V0 = 0;
U16 mc_cmd_highSpeed_V1 = 0;
U16 mc_cmd_lowSpeed_V0 = 0;
U16 mc_cmd_lowSpeed_V1 = 0;

void mc_motor_init()
{
  mc_init_HW();
  Enable_interrupt();
  mc_motor_stop();
  mc_set_motor_direction(CW);
  PSC_Init(MAX_PWM,0,MIN_PWM,0); 
  mc_config_sampling_period();
}

void mc_motor_run_highSpeed(void)
{
  mc_cmd_lowSpeed_V0 = 0;
  mc_cmd_lowSpeed_V1 = 0;
  PSC_Start();
  Status_Motor = HIGH_SPEED;
}

void mc_motor_run_lowSpeed(void)
{
  mc_cmd_highSpeed_V0 = 0;
  mc_cmd_highSpeed_V1 = 0;
  PSC_Load_lowSpeed (MAX_PWM/2, MAX_PWM/2 + DEAD_TIME, MAX_PWM/2, MAX_PWM/2 + DEAD_TIME, MAX_PWM/2, MAX_PWM/2 + DEAD_TIME);
  mc_connect_drivers();
  PSC_Start();
  Status_Motor = LOW_SPEED;
}

void mc_motor_stop(void)
{
  Set_none();
  Status_Motor = NO_SPEED;
  mc_cmd_highSpeed_V0 = 0;
  mc_cmd_highSpeed_V1 = 0;
  mc_cmd_lowSpeed_V0 = 0;
  mc_cmd_lowSpeed_V1 = 0;
  PSC_Stop();
}

U8 mc_get_motor_direction(void)                     {return mc_direction;}
void mc_set_motor_direction(U8 direction)           {mc_direction = direction;}

void mc_forward(void)                               {mc_direction = CW;}
void mc_backward(void)                              {mc_direction = CCW;}


