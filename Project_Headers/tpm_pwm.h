//************************************************************************************************************
// FILE: tpm_pwm.h
//
// DESCRIPTION
//
// AUTHOR
// John J. Hritz (John.Hritz@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#ifndef TPM_PWM_H_
#define TPM_PWM_H_

#include "global.h"
#include "tpm.h"

typedef enum
{
	pwm_polarity_low	=	0b01,
	pwm_polarity_high	=	0b10
}pwm_polarity_t;

extern pwm_polarity_t g_motor_pwm_polarity;

void tpm_init_pwm(tpm_timer_t p_timer, tpm_channel_t p_channel, 
					tpm_prescale_t p_ps, pwm_polarity_t p_polarity, 
					uint32_t p_period);

void tpm_cfg_period(tpm_timer_t p_timer, uint32_t p_period);

void pwm_enable(tpm_timer_t p_timer, tpm_channel_t p_channel, 
				pwm_polarity_t p_polarity);

void pwm_disable(tpm_timer_t p_timer, tpm_channel_t p_channel);

void pwm_cfg_duty(tpm_timer_t p_timer, tpm_channel_t p_channel, uint32_t p_cycle);

void pwm_cfg_duty_TPM0(uint32_t p_cycle);

void pwm_cfg_polarity(tpm_timer_t p_timer, tpm_channel_t p_channel, 
						pwm_polarity_t p_polarity);

#endif
