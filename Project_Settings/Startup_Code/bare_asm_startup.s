@***********************************************************************************************************************
@ FILE: bare_asm_startup.s
@
@ DESCRIPTION
@ Contains the startup routine, bare_startup, which calls various functions to initialize stuff and then calls main().
@
@ AUTHOR
@ Kevin R. Burger (burgerk@asu.edu)
@ Computer Science & Engineering
@ Arizona State University, Tempe AZ 85287-8809
@ Web: http://www.devlang.com
@***********************************************************************************************************************

.file       "bare_asm__startup.s"  @ Provides the name of the source file to the debugger
.syntax     unified                @ Use the ARM unified assemmbler syntax [Ref: as.pdf, Sect 9.4.2.1]

@=======================================================================================================================
@ Public Labels to be Imported
@=======================================================================================================================

.extern __init_sp                      @ Initial address of stack pointer register
.extern bare_hw_init                   @ bare_hw_init() defined in bare_startup.c
.extern bare_bss_zero_fill             @ bare_bss_zero_fill() defined in bare_startup.c
.extern bare_copy_rom_sections_to_ram  @ bare_copy_rom_sections_to_ram() defined in bare_startup.c
.extern main                           @ main() defined somewhere, usually in main.c

@=======================================================================================================================
@ Public Labels to be Exported
@=======================================================================================================================

.global bare_sp_init
.global bare_startup

@=======================================================================================================================
@ SECTION: .text
@=======================================================================================================================
.text

@-----------------------------------------------------------------------------------------------------------------------
@ FUNCTION: static void bare_sp_init()
@
@ DESCRIPTION
@ Writes a bottom-of-stack marker (0xDEADBEEF) at the last work of SRAM (0x20005FFC) and initializes the stack pointer
@ register to that address.
@
@ REGISTER USAGE
@ r0-r1, sp
@-----------------------------------------------------------------------------------------------------------------------
.func bare_sp_init, bare_sp_init
.type bare_sp_init, %function
.thumb_func
bare_sp_init:
    ldr     r0, =__init_sp              @ r0 = memory address where SP register is initialized to point to
    ldr     r1, dead_beef               @ r1 = 0xDEAD_BEEF
    str     r1, [r0]                    @ MEM[__init_sp] = 0xDEADBEEF
    mov     sp, r0                      @ sp = __init_sp
    bx      lr                          @ return
.align  2
dead_beef:  .word 0xDEADBEEF
.endfunc

@-----------------------------------------------------------------------------------------------------------------------
@ FUNCTION: static void bare_startup()
@
@ DESCRIPTION
@ This is the entry point of execution. Call various functions to initialize things and then call main(). Note that
@ main() should not return, but if it does we drop into an infinite loop.
@
@ REGISTER USAGE
@ None
@-----------------------------------------------------------------------------------------------------------------------
.func bare_startup, bare_startup
.type bare_startup, %function
.thumb_func
bare_startup:
    bl      bare_sp_init                   @ bare_sp_init()
    bl      bare_hw_init                   @ bare_hw_init()
    bl      bare_bss_zero_fill             @ bare_bss_zero_fill()
    bl      bare_copy_rom_sections_to_ram  @ bare_copy_rom_sections_to_ram()
    bl      main

@ Drop into an infinite loop if main() should return
1:  b       1b
.endfunc
