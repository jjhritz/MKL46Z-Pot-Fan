//**********************************************************************************************************************
// FILE: pit.c
//
// DECRIPTION
// See comments in pit.h.
//
// AUTHORS
// Kevin R. Burger (burgerk@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe, AZ 85287-8809
// http://www.devlang.com
//**********************************************************************************************************************
#include "int.h"
#include "pit.h"

//======================================================================================================================
// Function Declarations
//======================================================================================================================

// I am not sure I need this function declaration but I was having issues with my pit_isr() not properly overriding the
// default one declared in bare_startup.c. Whether this declaration is needed or not, it does not hurt anything.
void pit_isr();

//======================================================================================================================
// Private Global Variable Definitions
//======================================================================================================================

// This enumerated type enumerates values to write to PIT_LDVAL to obtain interrupts at common frequencies.
static uint32_t g_pit_ldval[] = {
    47999999,  //     0.500 Hz, 2000.00 ms
    23999999,  //     1.000 Hz, 1000.00 ms
    19199999,  //     1.250 Hz,  800.00 ms
    17999999,  //     1.333 Hz,  750.00 ms
    14399999,  //     1.667 Hz,  600.00 ms
    11999999,  //     2.000 Hz,  500.00 ms
     9599999,  //     2.500 Hz,  400.00 ms
     5999999,  //     4.000 Hz,  250.00 ms
     4799999,  //     5.000 Hz,  200.00 ms
     2399999,  //    10.000 Hz,  100.00 ms
     1199999,  //    20.000 Hz,   50.00 ms
      599999,  //    40.000 Hz,   25.00 ms
      479999,  //    50.000 Hz,   20.00 ms
      239999,  //   100.000 Hz,   10.00 ms
      119999,  //   200.000 Hz,    5.00 ms
       59999,  //   400.000 Hz,    2.50 ms
       47999,  //   500.000 Hz,    2.00 ms
       31999,  //   750.000 Hz,    1.33 ms
       29999,  //   800.000 Hz,    1.25 ms
       23999,  //  1000.000 Hz,    1.00 ms = 1000us
       17999,  //  1333.333 Hz,    0.75 ms =  750 us
       11999,  //  2000.000 Hz     0.50 ms =  500 us
        5999,  //  4000.000 Hz,    0.25 ms =  250 us
        2399,  // 10000.000 Hz,    0.10 ms =  100 us
           0   // Undefined
};

// Pointers to the callback functions for PIT0 and PIT1. Both ptrs are initialized to null.
static callback_v_t g_callbacks[2] = {
    0,
    0
};

//======================================================================================================================
// Function Definitions
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: pit_cfg_freq()
//----------------------------------------------------------------------------------------------------------------------
void pit_cfg_freq(pit_timer_t p_timer, pit_freq_t p_freq, pit_state_t p_state)
{
    // Disable the timer before changing the the PIT timer period.
    pit_cfg_state(p_timer, pit_state_disabled);

    // Initialize PIT_LDVALn with the proper values based on p_freq. Note that p_freq is an index into g_pit_ldval[].
    PIT_LDVAL(p_timer) = g_pit_ldval[p_freq];

    // Enable or disable the timer based on p_state.
    pit_cfg_state(p_timer, p_state);
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: pit_cfg_state()
//----------------------------------------------------------------------------------------------------------------------
void pit_cfg_state(pit_timer_t p_timer, pit_state_t p_state)
{
    if (p_state == pit_state_disabled) {
        // Disable PIT timer p_timer by clearing PIT_TCTRLn[TEN].
        PIT_TCTRL(p_timer) &= ~(0x00000001);
    } else {
        // Ensable PIT timer p_timer by setting PIT_TCTRLn[TEN].
        PIT_TCTRL(p_timer) |= 0x00000001;
    }
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: pit_init()
//----------------------------------------------------------------------------------------------------------------------
void pit_init(pit_timer_t p_timer, pit_freq_t p_freq, pit_state_t p_state, callback_v_t p_callback)
{
    // Enable the PIT module by clearing PIT_MCR[MDIS]. Set PIT_MCR[FRZ] so the PIT timer will pause counting when the
    // debugger is halted.
    PIT_MCR = 0x0001;

    // Configure the frequency. Leave the timer disabled.
    pit_cfg_freq(p_timer, p_freq, pit_state_disabled);

    // Initialize PIT_TCTRLn: CHN = 0 (timer is not chained), TIE = 1 (timer interrupts are enabled), TEN = 1 (timer
    // is enabled).
    PIT_TCTRL0 = 0x00000002;

    // Clear interrupt request flag, just in case.
    PIT_TFLG0 = 0x00000001;

    // Configure PIT interrupts at priority 0. Enable interrupts.
    int_init(int_src_pit, int_priority_0, int_state_enabled);

    // Save a point to the user's callback function.
    g_callbacks[p_timer] = p_callback;
    
    // Enable or disable the timer based on p_state.
    pit_cfg_state(p_timer, p_state);
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: pit_ist()
//----------------------------------------------------------------------------------------------------------------------
void pit_isr()
{
    // Check to see if PIT0 interrupts.
    if (PIT_TFLG0 & 1) {
        // Clear the interrupt request flag.
        PIT_TFLG(0) |= 0x00000001;

        // If the user requested a callback, call it.
        if (g_callbacks[0]) g_callbacks[0]();
    }

    // It was PIT1 that interrupted.
    else {
        // Clear the interrupt request flag.
        PIT_TFLG(1) |= 0x00000001;

        // If the user requested a callback, call it.
        if (g_callbacks[1]) g_callbacks[1]();
    }
}
