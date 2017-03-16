//**********************************************************************************************************************
// FILE: adc.c
//
// DESCRIPTION
// See comments in adc.h.
//
// AUTHOR
// Kevin R. Burger (burgerk@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
// Web: http://www.devlang.com
//**********************************************************************************************************************
#include "adc.h"

//======================================================================================================================
// Private Function Declarations
//======================================================================================================================

static int adc_calibrate();
static void adc_internal_init();

//======================================================================================================================
// Function Definitions
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: adc_calibrate()
//
// DESCRIPTION
// Calibrates the ADC. This only needs to be performed once during power up.
//---------------------------------------------------------------------------------------------------------------------
static int adc_calibrate()
{
    // Clear ADC0_SC2[ADTRG] to enable software-initiated converions for the calibration procedure.
    ADC0_SC2 &= ~(1 << 6);

    // Clear ADC0_SC3[ADCO] for a single conversion. Clear ADC0_SC3[AVGE] to disable hardware averaging.
    ADC0_SC3 &= ~(0b11 << 2);

    // Set ADC0_SC3[AVGE] to enable hardware averaging. Write 0b11 to ADC0_SC3[AVGS] to select 32 samples to average.
    ADC0_SC3 |= 0b111;

    // Start the calibration procedure by setting ADC0_SC3[CAL].
    ADC0_SC3 |= 1 << 7;

    // Poll ADC0_SC1A[COCO] waiting for it to go to 1 indicating that the calibration has completed.
    while (~ADC0_SC1A & (1 << 7)) {}

    // ADC0_SC3[CALF] will be 1 if calibration failed. In this case, we return -1 to indicate failure.
    if (ADC0_SC3 & (1 << 6)) {
        return -1;
    }

    // Sum the ADC0_CLPx registers to determine the plus-side gain (a 16-bit unsigned int). Next divide gain by 2 and
    // set the most significant bit. Finally, write the gain to ADC0_PG.
    uint16_t gain = ADC0_CLP0 + ADC0_CLP1 + ADC0_CLP2 + ADC0_CLP3 + ADC0_CLP4 + ADC0_CLPS;
    ADC0_PG = (gain >> 1) | 0x8000;

    // Sum the ADC0_CLMx registers to determine the minus-side gain (a 16-bit unsigned int).
    gain = ADC0_CLM0 + ADC0_CLM1 + ADC0_CLM2 + ADC0_CLM3 + ADC0_CLM4 + ADC0_CLMS;
    ADC0_MG = (gain >> 1) | 0x8000;

    // Clear ADC0__SC3[CAL] to indicat that the calibration procedure is completed.
    ADC0_SC3 &= ~(1 << 7);

    // Return 0 to indicate a successful calibration.
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: adc_convert()
//
// DESCRIPTION
// Performs a single 12-bit converstion on channel 11. Returns the sampled 12-bit value.
//---------------------------------------------------------------------------------------------------------------------
uint16_t adc_convert()
{
    // After initialization and calibration, ADC0 is ready for operation. However, it is and will remain disabled until
    // we initiate a single 12-bit conversion by setting ADC0[ADCH] to not all 1's to select the desired channel, which
    // is 11 in our case. We start by clearing the ADCH field and then writing the desired channel into ADCH.
    uint32_t adc0_sc1a = ADC0_SC1A;
    adc0_sc1a &= ~(0b11111);
    adc0_sc1a |= 0b01011;
    ADC0_SC1A = adc0_sc1a;

    // Poll ADC0_SC1A[COCO] waiting for it to go to 1 indicating that the conversion has completed.
    while (~ADC0_SC1A & 0x00000080) {}

    // The sampled digital value is in ADC0_RA. Read it.
    return (uint16_t)ADC0_RA;
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: adc_init()
//---------------------------------------------------------------------------------------------------------------------
void adc_init()
{
    // Initialize ADC0.
    adc_internal_init();

    // Perform the ADC calibration procedure with ADC0 configured as above.
    int c = adc_calibrate();

    // adc_calibration() returns -1 if calibration fails. In this case, execute a breakpoint instruction to halt exec-
    // ution.
    if (c == -1) {
        __asm("bkpt");
    }

    // Now, reinitialize ADC0 the way we did before the calibration procedure. ADC0 is now ready to perform conversions.
    adc_internal_init();
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: adc_internal_init()
//
// DESCRIPTION
// Initializes the various ADC registers for the configuration we want.
//---------------------------------------------------------------------------------------------------------------------
static void adc_internal_init()
{
    // Initialize ADC0 to the desired configuration for 12-bit singled ended one-time conversions, prior to calibration.
    // Configure ADC0_SC1A: COCO = 0, AIEN = 0 (disable interrupts), DIFF = 0 (single-ended conversions), ADCH =
    // 0b11111 to disable ADC0.
    ADC0_SC1A = 0x0000001F;

    // Initialize ADC0_CFG1: ADLPC = 0 (normal power), ADIV = 0b01 (divide ADC clock ref by 2), ADLSMP = 0 (short sample
    // time), MODE = 0b01 (12-bit conversions), ADICLK = 0b01 (ADC clock ref is bus clock/2).
    ADC0_CFG1 = 0x00000025;

    // Initialize ADC0_CFG2: MUXSEL = 0 (ADC0 subchannel A), ADACKEN = 0 (asynchronous clock output disabled), ADHSC
    // = 0 (normal conversion sequence), ADLSTS = 0b00 (default longest sample time).
    ADC0_CFG2 = 0x00000000;

    // Initialize ADC0_SC2: ADACT = 0, ADTRG = 0 (software-initiated conversions), ACFE = 0 (compare function disabled),
    // ACFGT = 0b00 (n/a since compare disabled), ACREN = 0 (n/s since compare disabled), DMAEN = 0 (DMA disabled),
    // REFSEL = 0b00 (voltage ref is V_REFH and V_REFL).
    ADC0_SC2 = 0x00000000;

    // Initialize ADC0_SC3: CAL = 0 (disable calibration procedure), CALF = 0, reserved = 0b00, ADCO = 0 (single conver-
    // sions), AVGE = 0 (hardware averaging disabled), AVGS = 0b00 (n/a since AVGE = 0).
    ADC0_SC3 = 0x00000000;
}
