/* --------------------------------------------------------------------------
 * -- (C) 2020 Paweł Kraszewski                                            --
 * --                                                                      --
 * -- Licensed as:                                                         --
 * --   Attribution-NonCommercial-ShareAlike 4.0 International             --
 * --   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode        --
 * ------------------------------------------------------------------------*/

#pragma once

#include "tools.h"

namespace MSP430::Driver::PMM {
    using MSP430::Tools::IOREG;

    /**
     * Power Management Module driver
     * @tparam addr base address of device
     */
    template <u16 addr>
    struct pmm {
        IOREG<u16, addr + 0x00> CTL0;
        IOREG<u16, addr + 0x02> CTL1;
        IOREG<u16, addr + 0x0A> IFG;
        IOREG<u16, addr + 0x10> PM5CTL0;

        /**
         * Necesary to be called after each POR (early in the boot) to enable
         * I/O ports
         */
        void unlock_pm5() { PM5CTL0.template bit<0>().clear(); }
    };

}  // namespace MSP430::Driver::PMM
