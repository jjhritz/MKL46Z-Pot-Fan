//**********************************************************************************************************************
// FILE: frdm_led.c
//
// DESCRIPTION
// See comments in frdm_led.h.
//
// AUTHOR
// Kevin R. Burger (burgerk@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
// Web: http://www.devlang.com
//**********************************************************************************************************************
#include "frdm_led.h"
#include "global.h"

//======================================================================================================================
// Private Global Constants
//======================================================================================================================

int FRDM_LED_RED_PIN   = 29;  // The red LED is pin 29 of Port E
int FRDM_LED_GREEN_PIN = 5;   // The green LED is pin 5 of Port D

//======================================================================================================================
// Function Definitions
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: frdm_led_green_init()
//----------------------------------------------------------------------------------------------------------------------
void frdm_led_green_init()
{
    // The green LED on the FRDM-KL46Z board is connected to pin 98 which it PTD5. Configure the function of that pin
    // to be GPIO.
    PORTD_PCR5 = 0b001 << 8;

    // Configure pin PTD5 as an output pin.
    GPIOD_PDDR |= 1 << FRDM_LED_GREEN_PIN;

    // Turn it off.
    frdm_led_green_off();
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: frdm_led_green_off()
//----------------------------------------------------------------------------------------------------------------------
void frdm_led_green_off()
{
    GPIOD_PDOR |= 1 << FRDM_LED_GREEN_PIN;
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: frdm_led_green_on()
//----------------------------------------------------------------------------------------------------------------------
void frdm_led_green_on()
{
    GPIOD_PDOR &= ~(1 << FRDM_LED_GREEN_PIN);
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: frdm_led_green_state()
//----------------------------------------------------------------------------------------------------------------------
frdm_led_state_t frdm_led_green_state()
{
    // Read and return the state of the pin connected to the greenLED.
    return (frdm_led_state_t)((GPIOD_PDIR & (1 << FRDM_LED_GREEN_PIN)) >> FRDM_LED_GREEN_PIN);
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: frdm_led_green_toggle()
//----------------------------------------------------------------------------------------------------------------------
void frdm_led_green_toggle()
{
    frdm_led_state_t state = frdm_led_green_state();
    if (state == frdm_led_state_on) {
        frdm_led_green_off();
    } else {
        frdm_led_green_on();
    }
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: frdm_led_red_init()
//----------------------------------------------------------------------------------------------------------------------
void frdm_led_red_init()
{
    // The red LED on the FRDM-KL46Z board is connected to pin 26 which is PTE29. Configure the function of that pin
    // to be GPIO.
    PORTE_PCR29 = 0b001 << 8;

    // Configure pin PTE29 as an output pin.
    GPIOE_PDDR |= 1 << FRDM_LED_RED_PIN;

    // Bring the pin high. This makes the anode and cathode ends of the LED 3.3V so the LED will be off.
    GPIOE_PDOR |= 1 << FRDM_LED_RED_PIN;
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: frdm_led_red_off()
//----------------------------------------------------------------------------------------------------------------------
void frdm_led_red_off()
{
    GPIOE_PDOR |= 1 << FRDM_LED_RED_PIN;
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: frdm_led_red_on()
//----------------------------------------------------------------------------------------------------------------------
void frdm_led_red_on()
{
    GPIOE_PDOR &= ~(1 << FRDM_LED_RED_PIN);
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: frdm_led_red_state()
//----------------------------------------------------------------------------------------------------------------------
frdm_led_state_t frdm_led_red_state()
{
    // Read and return the state of the pin connected to the red LED.
    return (frdm_led_state_t)((GPIOE_PDIR & (1 << FRDM_LED_RED_PIN)) >> FRDM_LED_RED_PIN);
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: frdm_led_red_toggle()
//----------------------------------------------------------------------------------------------------------------------
void frdm_led_red_toggle()
{
    frdm_led_state_t state = frdm_led_red_state();
    if (state == frdm_led_state_on) {
        frdm_led_red_off();
    } else {
        frdm_led_red_on();
    }
}
