/* --------------------------------------------------------------------------
 * -- (C) 2020 Paweł Kraszewski                                            --
 * --                                                                      --
 * -- Licensed as:                                                         --
 * --   Attribution-NonCommercial-ShareAlike 4.0 International             --
 * --   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode        --
 * ------------------------------------------------------------------------*/

#include <msp430fr5994.h>

#pragma ide diagnostic ignored "EndlessLoop"

using namespace MSP430::FR5994;
using MSP430::u16, MSP430::u8;

NOINLINE void delay(u16 n) {
    static volatile unsigned int i DATA_TINY = 0;
    for (i = 0; i < n; i++)
        ;
}

int main() {

    wdt_a.stop();

    cs.unlock();
    cs.set_sources(MSP430::Driver::Clock::ACLK::LFXTCLK,
                   MSP430::Driver::Clock::MCLK::LFXTCLK,
                   MSP430::Driver::Clock::MCLK::LFXTCLK);
    cs.set_dividers(MSP430::Driver::Clock::DIV::_1,
                    MSP430::Driver::Clock::DIV::_1,
                    MSP430::Driver::Clock::DIV::_1);

    pmm.unlock_pm5();

    p1.set_mode(GPIO::MODE::OUT, 0b11);
    p1.OUT.bit<1>().set();

    ta0.cctl<0>() = 1u << 4u;
    ta0.CTL       = ta0.DIV_1 | ta0.CLK_A | ta0.UP | ta0.TBIE_E;
    ta0.ccr<0>()  = 16384;

    set_low_power(MSP430::POWER::MODE4);
    // Should never reach here, unless some interrupt disables low power mode.

    // If it reaches somehow, then switch off green LED and loop
    p1.OUT.bit<1>().clear();
    while (true) {
    }
}

IRQ_HANDLER(TA0_CCR0) {
    // Acknowledge interrupt
    volatile auto iv = ta0.IV.get();

    // Flip red LED
    p1.OUT.bit<0>().toggle();
}
