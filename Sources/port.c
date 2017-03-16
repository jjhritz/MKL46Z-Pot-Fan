//************************************************************************************************************
// FILE: port.c
//
// DESCRIPTION
// Writes 001 into the MUX field for pin control register (PORTx_PCRn)
// to enable the ports for GPIO
// Currently only writes to Port B, pins 0-3.  Still trying to figure out
// how to generalize it.
//
// AUTHOR
// John J. Hritz (John.Hritz@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************

#include "port.h"

void port_gpio_config(uint32_t port, uint32_t pin)
{
	PORT_PCR(port, pin) &= 0b000 << 8;
	PORT_PCR(port, pin) |= 0b001 << 8;
}


