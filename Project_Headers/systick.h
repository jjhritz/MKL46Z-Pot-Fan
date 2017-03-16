//**********************************************************************************************************************
// FILE: systick.h
//
// DESCRIPTION
// Functions to initialize and use the SysTick timer.
//
// AUTHOR
// Kevin R. Burger (burgerk@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
// Web: http://www.devlang.com
//**********************************************************************************************************************
#ifndef SYSTICK_H
#define SYSTICK_H

//======================================================================================================================
// Public Types
//======================================================================================================================

// This typedef is used to specify the clock source for the SysTick counter.
typedef enum {
    systick_clk_src_external = 0,
    systick_clk_src_platform = 1
} systick_clk_src_t;

// This type is used to specify if the SysTick interrupt should be disabled or enabled.
typedef enum {
    systick_int_disabled = 0,
    systick_int_enabled  = 1
} systick_int_t;

// This enumerated type specifies the values to load into SYSTICK_RVR in order to obtain a specific SysTick ISR period.
// When the platform processor clock = 48 MHz (period = 20.8333 ns) is selected as the SysTick counter clock source,
// and SYST_RVR[RELOAD] = n, the counter will count down n, n-1, n-2, ..., 2, 1, 0. When it reaches 0, it will wrap
// around to SYST_RVR[RELOAD] and at that time, if configured, an interrupt request will be generated. For example, if
// SYST_RVR[RELOAD] = 5, it will count 5, 4, 3, 2, 1, 0, 5, and at this time, the interrupt request will be generated.
// This is 6 SysTick counts which would generate the interrupt request every 145.8333 ns. If our intent was to generate
// the interrupt request every 5 x 20.833 = 125 ns, then we actually have to write 4 to SYST_RVR[RELOAD]. Note that
// SYST_RVR[RELOAD] is limited to 24 bits, so the largest value that can be written to RELOAD is 0xFFFFFF which would
// give us a SysTick ISR period of approximately 349.5 ms.
typedef enum {
    systick_period_isr_125_ns =        5,
    systick_period_isr_250_ns =       11,
    systick_period_isr_375_ns =       17,
    systick_period_isr_500_ns =       23,
    systick_period_isr_625_ns =       29,
    systick_period_isr_750_ns =       35,
    systick_period_isr_875_ns =       41,
    systick_period_isr_1_us   =       47,
    systick_period_isr_5_us   =      239,
    systick_period_isr_10_us  =      479,
    systick_period_isr_50_us  =     2399,
    systick_period_isr_100_us =     4799,
    systick_period_isr_250_us =    11999,
    systick_period_isr_500_us =    23999,
    systick_period_isr_1_ms   =    47999,
    systick_period_isr_5_ms   =   239999,
    systick_period_isr_10_ms  =   479999,
    systick_period_isr_25_ms  =  1199999,
    systick_period_isr_50_ms  =  2399999,
    systick_period_isr_100_ms =  4799999,
    systick_period_isr_250_ms = 11999999
} systick_period_isr_t;

// This type us used to specify if the SysTick counter should be disabled or enabled.
typedef enum {
    systick_state_disabled = 0,
    systick_state_enabled  = 1
} systick_state_t;

//======================================================================================================================
// Public Function Declarations
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_cfg_callback()
//
// DESCRIPTION
// Configures the callback function that will be called at every SysTick interrupt. p_callback should be 0 if callbacks
// are not requested.
//----------------------------------------------------------------------------------------------------------------------
void systick_cfg_callback(callback_vu32_t p_callback);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_cfg_clk_src()
//
// DESCRIPTION
// Configures the clock source for the SysTick counter. The choices are: systick_clk_src_external for the external ref-
// erence clock; or systick_clk_src_platform for the platform clock.
//----------------------------------------------------------------------------------------------------------------------
void systick_cfg_clk_src(systick_clk_src_t p_clk_src);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_cfg_int_state()
//
// DESCRIPTION
// Enables or disables SysTick interrupts.
//----------------------------------------------------------------------------------------------------------------------
void systick_cfg_int_state(systick_int_t p_int);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_cfg_state()
//
// DESCRIPTION
// Disables or enables the SysTick counter to stop or begin counting.
//----------------------------------------------------------------------------------------------------------------------
void systick_cfg_state(systick_state_t p_state);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_init()
//
// DESCRIPTION
// SysTick uses the Platform clock to control the SysTick counter. We have configured the clatform clock to 48 MHz so
// the period is 20.83333... ns. The counter is a down counter, so when the counter is enabled, it will be loaded with
// SYST_RVR[RELOAD] and will decrement once every platform clock. When the counter reaches 0 and wraps around to
// SYST_RVR[RELOAD], if configured, it will  generate an exception, number 15 (IRQ -1). The default SysTick ISR is
// named systick_isr() in bare_startup.c which must be overridden if SysTick interrupts are enabled. We will get a whole
// number for the SysTick interrupt period when SYSTICK_RVR+1 is a multiple of 6. The systick_period_isr_t data type
// specifies RVR values for common time periods.
//----------------------------------------------------------------------------------------------------------------------
void systick_init(systick_period_isr_t, systick_int_t, systick_state_t, callback_vu32_t);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_read()
//
// DESCRIPTION
// Returns the current value of the counter, i.e, the value of the SYSTICK_CVR register.
//----------------------------------------------------------------------------------------------------------------------
uint32_t systick_read();

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_reset()
//
// DESCRIPTION
// When SysTick has been initialized by calling systick_init() and is either disabled (not counting) or enabled
// (counting and generate IRQs) this function will reset SysTick by: (1) stopping the counter; (2) loading SYST_RVR
// with the desired period; (3) clearing the counter to 0; (4) configuring the callback request; and (5) either disabl-
// ing or enabling the timer based on p_state.
//----------------------------------------------------------------------------------------------------------------------
void systick_reset(systick_period_isr_t p_period, systick_state_t p_state, callback_vu32_t p_callback);

#endif
