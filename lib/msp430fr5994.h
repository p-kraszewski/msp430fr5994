/* --------------------------------------------------------------------------
 * -- (C) 2020 Paweł Kraszewski                                            --
 * --                                                                      --
 * -- Licensed as:                                                         --
 * --   Attribution-NonCommercial-ShareAlike 4.0 International             --
 * --   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode        --
 * ------------------------------------------------------------------------*/

#pragma once

#include "drivers/tools.h"
#include "drivers/clock.h"
#include "drivers/gpio.h"
#include "drivers/pmm.h"
#include "drivers/timer.h"
#include "drivers/wdt_a.h"

namespace MSP430::FR5994 {
    using namespace Driver;

    Driver::WDT_A::wdt_a<0x15C> wdt_a;
    Driver::PMM::pmm<0x120>     pmm;
    Driver::Clock::cs<0x160>    cs;

    Driver::GPIO::port_int<0x200>    p1;
    Driver::GPIO::port_int<0x201>    p2;
    Driver::GPIO::port_int<0x220>    p3;
    Driver::GPIO::port_int<0x221>    p4;
    Driver::GPIO::port_int<0x240>    p5;
    Driver::GPIO::port_int<0x241>    p6;
    Driver::GPIO::port_int<0x260>    p7;
    Driver::GPIO::port_int<0x261>    p8;
    Driver::GPIO::port_simple<0x320> pj;

    Driver::Timer::TA<0x340, 3> ta0;
    Driver::Timer::TA<0x380, 3> ta1;
    Driver::Timer::TA<0x380, 2> ta2;
    Driver::Timer::TA<0x440, 2> ta3;
    Driver::Timer::TA<0x7C0, 2> ta4;
    Driver::Timer::TB<0x3C0, 7> tb0;
}  // namespace MSP430::FR5994
