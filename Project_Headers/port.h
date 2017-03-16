//************************************************************************************************************
// FILE: port.h
//
// DESCRIPTION
//
// AUTHOR
// John J. Hritz (John.Hritz@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#ifndef PORT_H_
#define PORT_H_

#include "global.h"


#define PORT_PCR(port, pin) (*(volatile uint32_t *)(0x40049000u + 0x1000 * port + 4 * pin))
#define PORT_ISFR(port) (*(volatile uint32_t* )(0x40049000u + 0x1000 * port + 0xA0))

#define PORT_A 0
#define PORT_B 1
#define PORT_C 2
#define PORT_D 3
#define PORT_E 4

extern callback_v_t g_portc_callback[32];

void port_gpio_config(uint32_t, uint32_t);
void portscd_isr();
void callback_init();


/*
void porta_gpio_config();
void portb_gpio_config();
void portd_gpio_config();
*/

#endif
