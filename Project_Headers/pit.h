//**********************************************************************************************************************
// FILE: pit.h
//
// DECRIPTION
// Functions for using the PIT timers.
//
// AUTHORS
// Kevin R. Burger (burgerk@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe, AZ 85287-8809
// Web: http://www.devlang.com
//**********************************************************************************************************************
#ifndef PIT_H
#define PIT_H

#include "global.h"
#include "int.h"

//======================================================================================================================
// Public Type Definitions
//======================================================================================================================

// This enumerated type enumerates common frequencies. Pass pit_freq_undef to pit_init() if you do not want to initi-
// alize the timer frequency at the time you initialize the timer. This would be the case if you intend to establish
// the timer frequency later by calling pit_cfg_freq(). Note, you do not want to enable a timer until you have config-
// ured a legal frequency. If you pass pit_freq_undef to pit_init() and never configure the frequency later, then 0
// will be written to PIT_LDVAL, which will cause the timer to generate an interrupt request on every processor clock
// cycles (that's bad).
typedef enum {
    pit_freq_0_5_hz   =  0, // 2000.00 ms
    pit_freq_1_hz     =  1, // 1000.00 ms
    pit_freq_1_25_hz  =  2, //  800.00 ms
    pit_freq_1_33_hz  =  3, //  750.00 ms
    pit_freq_1_67_hz  =  4, //  600.00 ms
    pit_freq_2_hz     =  5, //  500.00 ms
    pit_freq_2_5_hz   =  6, //  400.00 ms
    pit_freq_4_hz     =  7, //  250.00 ms
    pit_freq_5_hz     =  8, //  200.00 ms
    pit_freq_10_hz    =  9, //  100.00 ms
    pit_freq_20_hz    = 10, //   50.00 ms
    pit_freq_40_hz    = 11, //   25.00 ms
    pit_freq_50_hz    = 12, //   20.00 ms
    pit_freq_100_hz   = 13, //   10.00 ms
    pit_freq_200_hz   = 14, //    5.00 ms
    pit_freq_400_hz   = 15, //    2.50 ms
    pit_freq_500_hz   = 16, //    2.00 ms
    pit_freq_750_hz   = 17, //    1.33 ms
    pit_freq_800_hz   = 18, //    1.25 ms
    pit_freq_1_khz    = 19, //    1.00 ms
    pit_freq_1_33_khz = 20, //    0.75 ms = 750 us
    pit_freq_2_khz    = 21, //    0.50 ms = 500 us
    pit_freq_4_khz    = 22, //    0.25 ms = 250 us
    pit_freq_10_khz   = 23, //    0.10 ms = 100 us
    pit_freq_undef    = 99
} pit_freq_t;

// Pass to pit_init() and pit_cfg_state() to enable or disable a timer.
typedef enum {
    pit_state_disabled = 0,
    pit_state_enabled  = 1
} pit_state_t;

// The MKL46Z has two PIT timers, PIT0 and PIT1. Pass one of these values to the PIT functions to specify which timer
// you are configuring.
typedef enum {
    pit_timer_0 = 0,
    pit_timer_1 = 1
} pit_timer_t;

//======================================================================================================================
// Public Function Declarations
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: pit_cfg_freq()
//
// DESCRIPTION
// Configures the frequency of timer p_timer to p_freq. Note, the timer will be disabled to change the frequency. After
// changing the frequency, the timer is enabled or disabled on the value of p_state.
//----------------------------------------------------------------------------------------------------------------------
void pit_cfg_freq(pit_timer_t p_timer, pit_freq_t p_freq, pit_state_t p_state);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: pit_cfg_state()
//
// DESCRIPTION
// Configures timer p_timer to be either disabled or enabled based on the value of p_state.
//----------------------------------------------------------------------------------------------------------------------
void pit_cfg_state(pit_timer_t p_timer, pit_state_t p_state);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: pit_init()
//
// DESCRIPTION
// Initializes PIT timer p_timer to the interrupt at the specified frequency. Enables or disables the timer based on
// p_state. Saves a pointer to the user's callback function.
//----------------------------------------------------------------------------------------------------------------------
void pit_init(pit_timer_t p_timer, pit_freq_t p_freq, pit_state_t p_state, callback_v_t p_callback);

#endif
