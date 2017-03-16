//**********************************************************************************************************************
// FILE: global.h
//
// DESCRIPTION
// Use typedef for different size integers. Includes MKL46Z4.h. Almost every source code file we write will need to
// include global.h.
//
// AUTHOR
// Kevin R. Burger (burgerk@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
// Web: http://www.devlang.com
//**********************************************************************************************************************
#ifndef GLOBAL_H
#define GLOBAL_H

typedef signed char         int8_t;
typedef short int           int16_t;
typedef long int            int32_t;

typedef unsigned char       uint8_t;
typedef unsigned short int  uint16_t;
typedef unsigned long int   uint32_t;

// Simulates the bool data type that C does not have.
typedef enum {
    false = 0,
    true = 1,
} bool_t;

// The callback_v_t data type is a pointer to a void function with no arguments.
typedef void(*callback_v_t)();

// The callback_vi_t data type is a pointer to a void function with one int argument.
typedef void(*callback_vi_t)(int);

// The callback_vii_t data type is a pointer to a void function with two int arguments.
typedef void(*callback_vii_t)(int, int);

// The callback_i_t data type is a pointer to a function with no arguments that returns an int.
typedef int(*callback_i_t)();

// The callback_ii_t data type is a pointer to a function with one int argument that returns an int.
typedef int(*callback_ii_t)(int);

// The callback_iii_t data type is a pointer to a function with two int arguments that returns an int.
typedef int(*callback_iii_t)(int, int);

// The callback_vu32_t data type is a pointer to a void function with one uint32_t argument.
typedef void(*callback_vu32_t)();

extern uint32_t const GLOBAL_FREQ_EXT_CRYSTAL_HZ;
extern uint32_t const GLOBAL_FREQ_EXT_CRYSTAL_KHZ;
extern uint32_t const GLOBAL_FREQ_EXT_CRYSTAL_MHZ;

extern uint32_t global_bus_clock_freq_mhz;
extern uint32_t global_flash_clock_freq_mhz;
extern uint32_t global_core_clock_freq_mhz;
extern uint32_t global_mcgirclk_freq_mhz;
extern uint32_t global_platform_clock_freq_mhz;
extern uint32_t global_oscerclk_freq_mhz;
extern uint32_t global_system_clock_freq_mhz;

#include "mkl46z.h"

#endif
