#pragma once

namespace MSP430::WDT_A {

    constexpr u16 WDTPW = 0x5a00;
    constexpr u16 WDTHOLD = 0x0080;

    template<u16 offs>
    struct wdt_a {
        IOREG<u16, offs> WDCTL;

        inline void stop() { WDCTL = WDTPW | WDTHOLD; }
        inline void start() { WDCTL = WDTPW; }
    } PACKED;
}// namespace MSP430::WDT_A
