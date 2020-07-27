/* --------------------------------------------------------------------------
 * -- (C) 2020 Paweł Kraszewski                                            --
 * --                                                                      --
 * -- Licensed as:                                                         --
 * --   Attribution-NonCommercial-ShareAlike 4.0 International             --
 * --   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode        --
 * ------------------------------------------------------------------------*/

#pragma once

#include "tools.h"
namespace MSP430::Driver::GPIO {
    using MSP430::Tools::IOREG;

    template <u16 addr>
    struct port_simple {
        IOREG<u8, addr + 0x00> IN;
        IOREG<u8, addr + 0x02> OUT;
        IOREG<u8, addr + 0x04> DIR;
        IOREG<u8, addr + 0x06> REN;
        IOREG<u8, addr + 0x0A> SEL0;
        IOREG<u8, addr + 0x0C> SEL1;
        IOREG<u8, addr + 0x0E> SELC;
    };

    template <u16 addr>
    struct port_int : public port_simple<addr> {
        IOREG<u8, addr + 0x16> IV;
        IOREG<u8, addr + 0x18> IES;
        IOREG<u8, addr + 0x1A> IE;
        IOREG<u8, addr + 0x1C> IFG;
    };
}  // namespace MSP430::Driver::GPIO
