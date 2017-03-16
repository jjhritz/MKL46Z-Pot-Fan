//************************************************************************************************************
// FILE: tpm_pwm.c
//
// DESCRIPTION
//
// AUTHOR
// John J. Hritz (John.Hritz@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************

#include "global.h"
#include "tpm_pwm.h"
#include "tpm.h"

void tpm_init_pwm(tpm_timer_t p_timer, tpm_channel_t p_channel, 
					tpm_prescale_t p_ps, pwm_polarity_t p_polarity, 
					uint32_t p_period)
{
	tpm_init(p_timer, p_ps, tpm_state_disabled);
	
	TPM_CNT(p_timer) = 0;
	
	tpm_cfg_period(p_timer, p_period);
	
	TPM_CSC(p_timer, p_channel) = 0;
	
	pwm_enable(p_timer, p_channel, p_polarity);
	
	TPM_CONF(p_timer) = 0;
}

void tpm_cfg_period(tpm_timer_t p_timer, uint32_t p_period)
{
	TPM_MOD(p_timer) = (p_period - 1);
}

void pwm_enable(tpm_timer_t p_timer, tpm_channel_t p_channel, pwm_polarity_t p_polarity)
{
	TPM_CSC(p_timer, p_channel) |= (0b10 << 4);
	TPM0_C0SC |= (0b10 << 4);
	pwm_cfg_polarity(p_timer, p_channel, p_polarity);
}

void pwm_disable(tpm_timer_t p_timer, tpm_channel_t p_channel)
{
	TPM_CSC(p_timer, p_channel) &= ~(0b11 << 4); //
	TPM_CSC(p_timer, p_channel) &= ~(0b11 << 2);
}

void pwm_cfg_duty(tpm_timer_t p_timer, tpm_channel_t p_channel, uint32_t p_cycle)
{
	TPM_CV(p_timer, p_channel) = ((TPM_MOD(p_timer) + 1) * p_cycle) / 100;
}

void pwm_cfg_duty_TPM0(uint32_t p_cycle)
{	
	TPM0_C0V = ((TPM0_MOD + 1) * p_cycle) / 100;
}

void pwm_cfg_polarity(tpm_timer_t p_timer, tpm_channel_t p_channel, 
						pwm_polarity_t p_polarity)
{
	TPM_CSC(p_timer, p_channel) &= ~(0b11 << 2);
	//TPM0_C0SC &= (0b10 << 2);
	TPM_CSC(p_timer, p_channel) |= (p_polarity << 2);
	TPM_CSC(p_timer, p_channel) |= (1 << 3);
	//TPM0_C0SC |= (p_polarity << 2);
}
