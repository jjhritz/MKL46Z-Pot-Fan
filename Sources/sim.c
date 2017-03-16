//************************************************************************************************************
// FILE: sim.c
//
// DESCRIPTION
// Code for enabling the System Clock Gating Control Register 5
// (SIM_SCGC5) by setting the bit corresponding to the desired port.
// See Freescale Kinetis KL46 Subfamily Reference Manual pp. 216 for 
// bit mapping.
//
// AUTHOR
// John J. Hritz (John.Hritz@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#include "global.h"
#include "sim.h"

void enable_SIM(uint32_t portBit)
{
	SIM_SCGC5 |= 1 << portBit;
}
