//************************************************************************************************************
// FILE: tpm.h
//
// DESCRIPTION
// Header file for tpm.c; declares functions
//
// AUTHOR
// John J. Hritz (John.Hritz@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#ifndef TPM_H_
#define TPM_H_

#include "global.h"

#define TPM_SC(x) 		(*(volatile uint32_t *)(0x40038000u + (x) * 0x1000))
#define TPM_CNT(x) 		(*(volatile uint32_t *)(0x40038000u + (x) * 0x1000 + 0x04))
#define TPM_MOD(x) 		(*(volatile uint32_t *)(0x40038000u + (x) * 0x1000 + 0x08))
#define TPM_CSC(x, n) 	(*(volatile uint32_t *)(0x40038000u + (x) * 0x1000 + 0x0C + (n) * 0x08))
#define TPM_CV(x, n) 	(*(volatile uint32_t *)(0x40038000u + (x) * 0x1000 + 0x10 + (n) * 0x08))
#define TPM_STATUS(x) 	(*(volatile uint32_t *)(0x40038000u + (x) * 0x1000 + 0x50))
#define TPM_CONF(x) 	(*(volatile uint32_t *)(0x40038000u + (x) * 0x1000 + 0x84))


typedef enum
{
	tpm_ps_1 	=	0b000,
	tpm_ps_2	=	0b001,
	tpm_ps_4	=	0b010,
	tpm_ps_8	=	0b011,
	tpm_ps_16	=	0b100,
	tpm_ps_32	=	0b101,
	tpm_ps_64	=	0b110,
	tpm_ps_128	=	0b111
}tpm_prescale_t;

typedef enum
{
	tpm_state_disabled	=	0b00,
	tpm_state_enabled	=	0b01
}tpm_enable_state_t;

typedef enum {
    tpm_timer_0	=	0,
    tpm_timer_1 =	1,
    tpm_timer_2 =	2
} tpm_timer_t;

typedef enum {
    tpm_channel_0	=	0,
    tpm_channel_1	=	1,
    tpm_channel_2	=	2,
    tpm_channel_3	=	3,
    tpm_channel_4	=	4,
    tpm_channel_5	=	5
} tpm_channel_t;

extern tpm_timer_t g_busy_tpm_timer;
extern tpm_timer_t g_tpm0_ps;

void tpm_busy_delay_us(uint32_t p_timer, uint32_t p_delay_us); //tpm delay in microseconds

void tpm_busy_delay_ms(uint32_t p_delay_ms); //tpm delay in milliseconds

void tpm_init(tpm_timer_t p_timer, tpm_prescale_t p_ps, tpm_enable_state_t p_state);

void tpm_cfg_state(tpm_timer_t p_timer, tpm_enable_state_t p_state, tpm_prescale_t p_ps);

#endif
