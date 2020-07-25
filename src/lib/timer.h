#pragma once

#include "tools.h"

namespace MSP430::Timer {
    template<u16 offs, u8 ccount>
    class timer {
    public:
        enum class CTLe : u16 {
            TBCLGRP_M = 0b11 << 13,   //!< TBxCLn grouping
            TBCLGRP_None = 0b00 << 13,//!< Each TBxCLn latch loads independently.
            TBCLGRP_2 = 0b01 << 13,   //!< TBxCL1+TBxCL2 (TBxCCR1 CLLD bits control the update);
                                      //!< TBxCL3+TBxCL4 (TBxCCR3 CLLD bits control the update);
                                      //!< TBxCL5+TBxCL6 (TBxCCR5 CLLD bits control the update);
                                      //!< TBxCL0 independent
            TBCLGRP_3 = 0b10 << 13,   //!< TBxCL1+TBxCL2+TBxCL3 (TBxCCR1 CLLD bits control the update);
                                      //!< TBxCL4+TBxCL5+TBxCL6 (TBxCCR4 CLLD bits control the update);
                                      //!< TBxCL0independent
            TBCLGRP_7 = 0b11 << 13,   //!< TBxCL0+TBxCL1+TBxCL2+TBxCL3+TBxCL4+TBxCL5+TBxCL6
                                      //!< (TBxCCR1 CLLD bits control the update)

            CNTL_M = 0b11 << 11, //!< Counter length
            CNTL_16 = 0b00 << 11,//!< 16-bit, TBxR(max) = 0FFFFh
            CNTL_12 = 0b01 << 11,//!< 12-bit, TBxR(max) = 0FFFh
            CNTL_10 = 0b10 << 11,//!< 10-bit, TBxR(max) = 03FFh
            CNTL_08 = 0b11 << 11,//!< 8-bit, TBxR(max) = 0FFh

            SRC_M = 0b11 << 8,//!< Timer_B clock source select
            TBCLK = 0b00 << 8,//!< TBxCLK
            ACLK = 0b01 << 8, //!< ACLK
            SMCLK = 0b10 << 8,//!< SMCLK
            INCLK = 0b11 << 8,//!< INCLK

            DIV_M = 0b11 << 6,//!< Input divider. These bits, along with the TBIDEX bits,
                              //!< select the divider for the
                              //!< input clock.
            DIV_1 = 0b00 << 6,//!< 1
            DIV_2 = 0b01 << 6,//!< 1/2
            DIV_4 = 0b10 << 6,//!< 1/4
            DIV_8 = 0b11 << 6,//!< 1/8

            MODE_M = 0b11 << 4, //!< Mode mask
            STOP = 0b00 << 4,   //!< Stop mode: Timer is halted
            UP = 0b01 << 4,     //!< Up mode: Timer counts up to TBxCL0
            CONT = 0b10 << 4,   //!< Continuous mode: Timer counts up to the value set by CNTL
            UP_DOWN = 0b11 << 4,//!< Up/down mode: Timer counts up to TBxCL0 and down to 0000h

            TBCLR = 1 << 2,//!< Setting this bit clears TBR, the clock divider logic (the divider
                           //!< setting remains unchanged), and the count direction. The TBCLR bit
                           //!< is automatically reset and is always read as zero.

            TBIE_M = 0b1 << 1,//!< Timer_B interrupt enable. This bit enables the TBIFG interrupt request.
            TBIE_D = 0b1 << 1,//!< Interrupt disabled
            TBIE_E = 0b1 << 1,//!< Interrupt enabled

            TBIFG_M = 0b1 << 0,//!< Timer_B interrupt flag
            TBIFG_N = 0b0 << 0,//!< No interrupt pending
            TBIFG_P = 0b1 << 0,//!< Interrupt pending
        };

        IOREG<u16, offs + 0x00> CTL;
        IOREG<u16, offs + 0x10> R;
        IOREG<u16, offs + 0x20> EX0;
        IOREG<u16, offs + 0x2E> IV;

        template<u8 nr>
        IOREG<u16, offs + 2 + 2 * nr> cctl() {
            static_assert(nr < ccount);
            IOREG<u16, offs + 2 + 2 * nr> r;
            return r;
        }

        template<u8 nr>
        IOREG<u16, offs + 12 + 2 * nr> ccr() {
            static_assert(nr < ccount);
            IOREG<u16, offs + 12 + 2 * nr> r;
            return r;
        }
    };

    template<u16 offs>
    class rtc_c {
    };
}// namespace MSP430::Timer