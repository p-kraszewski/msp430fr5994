.macro IRQ handler
    .weak  irq_\handler
    .set   irq_\handler, vec_Unhandled
    .word  irq_\handler
.endm

.macro DEV base name
    .org  \base - 0x100
    .global io_\name
io_\name:
.endm


.section .Reset, "ax"
.global vec_Reset
.type vec_Reset,%function
vec_Reset:
    mov #_stack,r1
    br #main

.global vec_Unhandled
.type vec_Unhandled,%function
vec_Unhandled:
    reti

.section .stack, ""
.global _stack
_stack:

.section .vectors, "a"
.org 0x34
    IRQ   LEA
    IRQ   P8
    IRQ   P7
    IRQ   eUSCI_B3
    IRQ   eUSCI_B2
    IRQ   eUSCI_B1
    IRQ   eUSCI_A3
    IRQ   eUSCI_A2
    IRQ   P6
    IRQ   P5
    IRQ   TA4_CCR1
    IRQ   TA4_CCR0
    IRQ   AES
    IRQ   RTC_C
    IRQ   P4
    IRQ   P3
    IRQ   TA3_CCR1
    IRQ   TA3_CCR0
    IRQ   P2
    IRQ   TA2_CCR1
    IRQ   TA2_CCR0
    IRQ   P1
    IRQ   TA1_CCR1
    IRQ   TA1_CCR0
    IRQ   DMA
    IRQ   eUSCI_A1
    IRQ   TA0_CCR1
    IRQ   TA0_CCR0
    IRQ   ADC12_B
    IRQ   eUSCI_B0
    IRQ   eUSCI_A0
    IRQ   WDT
    IRQ   TB0_CCR1
    IRQ   TB0_CCR0
    IRQ   Comparator_E
    IRQ   User_NMI
    IRQ   System_NMI
    .word   vec_Reset
