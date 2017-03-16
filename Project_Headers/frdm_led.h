//**********************************************************************************************************************
// FILE: frdm_led.h
//
// DESCRIPTION
// The frdm_led module contains functions for initializing and using the red and green LED's on the FRDM-KL46Z board.
//
// AUTHOR
// Kevin R. Burger (burgerk@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
// Web: http://www.devlang.com
//**********************************************************************************************************************
#ifndef FRDM_LED_H
#define FRDM_LED_H

//======================================================================================================================
// Public Type Definitions
//======================================================================================================================

typedef enum {
    frdm_led_state_off = 1,  // To turn the LED off we must bring the state of the GPIO pin to high (logical 1)
    frdm_led_state_on  = 0   // To turn the LED on we must bring the state of the GPIO pin to low (logical 0)
} frdm_led_state_t;

//======================================================================================================================
// Public Function Declarations
//======================================================================================================================

void frdm_led_green_init();
void frdm_led_green_on();
void frdm_led_green_off();
frdm_led_state_t frdm_green_red_state();
void frdm_led_green_toggle();
void frdm_led_red_init();
void frdm_led_red_on();
void frdm_led_red_off();
frdm_led_state_t frdm_led_red_state();
void frdm_led_red_toggle();

#endif
