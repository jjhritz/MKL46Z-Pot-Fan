//************************************************************************************************************
// FILE: motor.c
//
// DESCRIPTION
//
// AUTHOR
// John J. Hritz (John.Hritz@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#include "global.h"
#include "motor.h"
#include "tpm_pwm.h"

void motor_cfg_speed(motor_speed_t p_speed)
{
	if(p_speed == motor_speed_0)
	{
		//pwm_disable(g_motor_tpm_timer, g_motor_tpm_chan);
		TPM0_C0SC = 0;
	}
	else
	{
		//pwm_enable(g_motor_tpm_timer, g_motor_tpm_chan, g_motor_pwm_polarity);
		TPM0_C0SC = 0b00101000;
		
		//pwm_cfg_duty_TPM0(100);
		TPM0_C0V = (40 * 100) / 100;
		
		if(p_speed == motor_speed_50)
		{
			tpm_busy_delay_ms(1);
			
			//pwm_cfg_duty_TPM0(50);
			TPM0_C0V = (40 * 50) / 100;
		}
	}
}
