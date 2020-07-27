/* --------------------------------------------------------------------------
 * -- (C) 2020 Paweł Kraszewski                                            --
 * --                                                                      --
 * -- Licensed as:                                                         --
 * --   Attribution-NonCommercial-ShareAlike 4.0 International             --
 * --   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode        --
 * ------------------------------------------------------------------------*/

#pragma once

namespace MSP430::Driver::WDT_A {

    using MSP430::Tools::IOREG;

    template <u16 addr>
    struct wdt_a {
        static constexpr u16 WDTPW = 0x5a00;
        static constexpr u16 WDTHOLD = 0x0080;

        IOREG<u16, addr> WDCTL;

        inline void stop() { WDCTL = WDTPW | WDTHOLD; }
        inline void start() { WDCTL = WDTPW; }
    };
}  // namespace MSP430::Driver::WDT_A
