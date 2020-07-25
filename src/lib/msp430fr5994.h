#pragma once

#include "gpio.h"
#include "pmm.h"
#include "timer.h"
#include "tools.h"
#include "wdt_a.h"

namespace MSP430::dev {
    WDT_A::wdt_a<0x15C> wdt_a;
    PMM::pmm<0x120> pmm;

    GPIO::port_int<0x200> p1;
    GPIO::port_int<0x201> p2;
    GPIO::port_int<0x220> p3;
    GPIO::port_int<0x221> p4;
    GPIO::port_int<0x240> p5;
    GPIO::port_int<0x241> p6;
    GPIO::port_int<0x260> p7;
    GPIO::port_int<0x261> p8;
    GPIO::port_simple<0x320> pj;

    Timer::timer<0x340, 3> ta0;
    Timer::timer<0x380, 3> ta1;
    Timer::timer<0x380, 2> ta2;
    Timer::timer<0x440, 2> ta3;
    Timer::timer<0x3C0, 7> tb0;
}// namespace MSP430::dev
