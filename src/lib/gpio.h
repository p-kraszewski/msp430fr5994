#pragma once

#include "tools.h"
namespace MSP430::GPIO {
    template<u16 port>
    struct port_simple {
        IOREG<u8, port + 0x00> IN;
        IOREG<u8, port + 0x02> OUT;
        IOREG<u8, port + 0x04> DIR;
        IOREG<u8, port + 0x06> REN;
        IOREG<u8, port + 0x0A> SEL0;
        IOREG<u8, port + 0x0C> SEL1;
        IOREG<u8, port + 0x0E> SELC;
    };

    template<u16 port>
    struct port_int : public port_simple<port> {
        IOREG<u8, port + 0x16> IV;
        IOREG<u8, port + 0x18> IES;
        IOREG<u8, port + 0x1A> IE;
        IOREG<u8, port + 0x1C> IFG;
    };
}// namespace MSP430::GPIO
