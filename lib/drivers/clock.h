/* --------------------------------------------------------------------------
 * -- (C) 2020 Paweł Kraszewski                                            --
 * --                                                                      --
 * -- Licensed as:                                                         --
 * --   Attribution-NonCommercial-ShareAlike 4.0 International             --
 * --   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode        --
 * ------------------------------------------------------------------------*/

#pragma once

#include "tools.h"

namespace MSP430::Driver::Clock {
    using MSP430::Tools::IOREG;

    enum class DCO : u16 {
        _1_00MHz = 0u << 1u,
        _2_67MHz = 1u << 1u,
        _3_50MHz = 2u << 1u,
        _4_00MHz = 3u << 1u,
        _5_33MHz = 4u << 1u,
        _7_00MHz = 5u << 1u,
        _8_00MHz = 6u << 1u,

        //_1_00MHz = 0u << 1u | 1u << 6u,
        //_5_33MHz = 1u << 1u | 1u << 6u,
        //_7_00MHz = 2u << 1u | 1u << 6u,
        //_8_00MHz = 3u << 1u | 1u << 6u,
        _16_00MHz = 4u << 1u | 1u << 6u,
        _21_00MHz = 5u << 1u | 1u << 6u,
        _24_00MHz = 6u << 1u | 1u << 6u,
    };

    enum class ACLK : u16 {
        LFXTCLK  = 0u,  //!< LFXTCLK when LFXT available, otherwise VLOCLK
        VLOCLK   = 1u,  //!< VLOCLK
        LFMODCLK = 2u,  //!< LFMODCLK
    };

    enum class MCLK : u16 {
        LFXTCLK  = 0u,  //!< LFXTCLK when LFXT available, otherwise VLOCLK
        VLOCLK   = 1u,  //!< VLOCLK
        LFMODCLK = 2u,  //!< LFMODCLK
        DCOCLK   = 3u,  //!< DCOCLK
        MODCLK   = 4u,  //!< MODCLK
        HFXTCLK  = 5u,  //!< HFXTCLK when HFXT available, otherwise DCOCLK
    };

    enum class DIV : u16 {
        _1  = 0,  //!< 1/1
        _2  = 1,  //!< 1/2
        _4  = 2,  //!< 1/4
        _8  = 3,  //!< 1/8
        _16 = 4,  //!< 1/16
        _32 = 5,  //!< 1/32
    };

    /**
     * The oscillator current can be adjusted to its drive needs. This in
     combination with the HFFREQ bits can be used for optimizing crystal power
     based on crystal characteristics.
     */
    enum class DRIVE : u16 {
        LOW  = 0,
        MED  = 1,
        HIGH = 2,
        MAX  = 3,
    };

    /**
     * The HFXT frequency selection. These bits must be set to the appropriate
     frequency for crystal or bypass modes of operation.
     */
    enum class HF : u16 {
        _0_4  = 0,  //!< 0 to 4 MHz
        _4_8  = 1,  //!< Greater than 4 MHz to 8 MHz
        _8_16 = 2,  //!< Greater than 8 MHz to 16 MHz
        // _16_24 = 3, //!< Greater than 16 MHz to 24 MHz
    };

    template <u16 addr>
    struct cs {
      private:
        constexpr static u16 CTL0_Key = 0xA500;

      public:
        IOREG<u16, addr + 0x00> CTL0;
        IOREG<u16, addr + 0x02> CTL1;
        IOREG<u16, addr + 0x04> CTL2;
        IOREG<u16, addr + 0x06> CTL3;
        IOREG<u16, addr + 0x08> CTL4;
        IOREG<u16, addr + 0x0A> CTL5;
        IOREG<u16, addr + 0x0C> CTL6;

        void unlock() { CTL0 = CTL0_Key; }
        void set_dco_freq(DCO f) { CTL1 = (u16)f; }
        void set_sources(ACLK aclk, MCLK smclk, MCLK mclk) {
            CTL2 = ((u16)aclk) << 8 | ((u16)smclk) << 4 | ((u16)mclk);
        }
        void set_dividers(DIV aclk, DIV smclk, DIV mclk) {
            CTL3 = ((u16)aclk) << 8 | ((u16)smclk) << 4 | ((u16)mclk);
        }
    };
}  // namespace MSP430::Driver::Clock
