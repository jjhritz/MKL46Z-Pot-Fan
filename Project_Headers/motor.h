//************************************************************************************************************
// FILE: motor.h
//
// DESCRIPTION
//
// AUTHOR
// John J. Hritz (John.Hritz@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#ifndef MOTOR_H_
#define MOTOR_H_

#include "tpm.h"

typedef enum
{
	motor_speed_0	=	0,
	motor_speed_50	=	50,
	motor_speed_100	=	100
}motor_speed_t;

extern tpm_timer_t g_motor_tpm_timer;
extern tpm_channel_t g_motor_tpm_chan;

void motor_cfg_speed(motor_speed_t p_speed);

#endif
