#pragma once
#include "tools.h"

namespace MSP430::PMM {
    template<u16 offs>
    struct pmm {
        IOREG<u16, offs + 0x00> CTL0;
        IOREG<u16, offs + 0x02> CTL1;
        IOREG<u16, offs + 0x0A> IFG;
        IOREG<u16, offs + 0x10> PM5CTL0;

        inline void unlock_pm5() { PM5CTL0 -= 0; }
    };
}// namespace MSP430::PMM
