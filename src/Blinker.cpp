/* --------------------------------------------------------------------------
 * -- (C) 2020 Paweł Kraszewski                                            --
 * --                                                                      --
 * -- Licensed as:                                                         --
 * --   Attribution-NonCommercial-ShareAlike 4.0 International             --
 * --   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode        --
 * ------------------------------------------------------------------------*/

#include <msp430fr5994.h>

#pragma ide diagnostic ignored "EndlessLoop"

volatile unsigned int i DATA_TINY = 0;

void delay(MSP430::u16 n) {
    for (i = 0; i < n; i++)
        ;
}

int main() {
    using namespace MSP430::FR5994;

    wdt_a.stop();
    pmm.unlock_pm5();

    p1.DIR = 0b11;
    p1.OUT = 0b01;

    while (true) {
        p1.OUT ^= 0b11;
        delay(20000);
    }
}

// IRQ_HANDLER(irq_TB0_CCR0) {}
