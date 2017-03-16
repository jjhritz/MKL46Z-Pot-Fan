//************************************************************************************************************
// FILE: tpm.c
//
// DESCRIPTION
//
// AUTHOR
// John J. Hritz (John.Hritz@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#include "global.h"
#include "tpm.h"


void tpm_init(tpm_timer_t p_timer, tpm_prescale_t p_ps, tpm_enable_state_t p_state)
{
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(0b11);
	
	TPM_SC(p_timer) = 0;
	
	TPM_CONF(p_timer) = 0;
	
	tpm_cfg_state(p_timer, p_state, p_ps);
}

void tpm_cfg_state(tpm_timer_t p_timer, tpm_enable_state_t p_state, tpm_prescale_t p_ps)
{
	TPM_SC(p_timer) &= ~(0b11 << 3);
	TPM_SC(p_timer) |= (p_state << 3);
	
	//Bug band-aid; For some reason TPMx_SC[PS] gets reset to 0 whenever TPMx_SC gets written to
	TPM_SC(p_timer) |= (p_ps << 0);
}

void tpm_busy_delay_us(uint32_t p_timer, uint32_t p_delay_us)
{
	// Initialize SIM_SOPT2 to select MCGIRCLK as the TPM0 clock source.
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(0b11);
		
	// Initialize TPM0_SC to: clear TOF (in case it was set) 
	TPM_SC(p_timer) |= 1 << 7;
		
	//Intialize TPM0_SC to: operate in up-counting mode
	TPM_SC(p_timer) &= ~(1 << 5);
			
	// Initialize TPM0_SC to: disable the TPM counter
	TPM_SC(p_timer) &= ~(0b11 << 3);
			
	//Clear TPM0_SC[PS]
	TPM_SC(p_timer) &= ~(0b111 << 0);
	
	//Initialize TPM0_SC[PS] to divide MCGIRCLK by 4.
	TPM_SC(p_timer) |= (1 << 1);
		
	// Write 0 to TPM0_CNT[COUNT] to reset the TPM counter to 0.
	TPM_CNT(p_timer) = 0;
		
	// Write (p_delay_us - 1) to TPM0_MOD[MOD] to cause TOF to be set after (p_delay_us - 1) us have elapsed.
	TPM_MOD(p_timer) = (p_delay_us - 1);
		
	// Write 0 to TPM_CONF to: clear CSOO so the counter will continue counting on overflow; 
	TPM_CONF(p_timer) = 0;
			
	// Write 0 to TPM_CONF to clear CSOT which will cause the counter to begin counting immediately after it is enabled.
	TPM_CONF(p_timer) &= 0 << 16;
	
	//Write 01 to TPM0_SC[CMOD] so the TPM counter increments on ever TPM counter clock
	TPM_SC(p_timer) |= (1 << 3);

	// At this time, the TPM counter will begin counting. Wait in a loop polling TOM0_SC[TOF] until it is set.
	while (~TPM_SC(p_timer) & TPM_SC_TOF_MASK) {}
	
	// Write to TPM0_SC[TOF] to clear the timer overflow flag.
	TPM_SC(p_timer) |= 1 << 7;
}

void tpm_busy_delay_ms(uint32_t p_delay_ms)
{
	//Enable the clock gate to the TPM module
	SIM_SCGC6 |= 1 << 24;
	
	// Initialize SIM_SOPT2 to select MCGIRCLK as the TPM1 clock source.
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(0b11);
	
	// Initialize TPM1_SC to: clear TOF (in case it was set) 
	TPM1_SC |= 1 << 7;
	
	//Intialize TPM1_SC to: operate in up-counting mode
	TPM1_SC &= ~(1 << 5);
		
	// Initialize TPM_SC to: disable the TPM counter
	TPM1_SC &= ~(0b11 << 3);
		
	//Clear TPM1_SC[PS]
	TPM1_SC &= ~(0b111 << 0);
	
	//Initialize TPM1_SC[PS] to divide MCGIRCLK by 4.
	TPM1_SC |= (1 << 1);
	
	// Write 0 to TPM1_CNT[COUNT] to reset the TPM counter to 0.
	TPM1_CNT = 0;
	
	// Write 999 to TPM1_MOD[MOD] to cause TOF to be set after 1000 us have elapsed.
	TPM1_MOD = 999;
	
	// Write 0 to TPM_CONF to: clear CSOO so the counter will continue counting on overflow; 
	TPM1_CONF = 0;
		
	// Write 0 to TPM_CONF to clear CSOT which will cause the counter to begin counting immediately after it is enabled.
	TPM1_CONF &= 0 << 16;
	
	//Write 01 to TPM1_SC[CMOD] so the TPM counter increments on ever TPM counter clock
	TPM1_SC |= (1 << 3);
	
	for(uint32_t i = 0; i < p_delay_ms; i++)
	{
		// At this time, the TPM counter will begin counting. Wait in a loop polling TOM0_SC[TOF] until it is set.
		while (~TPM1_SC & TPM_SC_TOF_MASK) {}
	
		// Write to TPM0_SC[TOF] to clear the timer overflow flag.
		TPM1_SC |= 1 << 7;
	}

}
