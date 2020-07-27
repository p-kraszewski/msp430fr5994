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
        enum class WDT : u16 {
            /**
             * Watchdog timer password. Always read as 069h. Must be written as
             05Ah, or a PUC is generated.
             */
            PW = 0x5a00,

            /**
             * Watchdog timer hold. This bit stops the watchdog timer. Setting
             WDTHOLD = 1 when the WDT is not in use conserves power.
             */
            HOLD = 1 << 7,

            CLK_SM = 0b00 << 5,
            CLK_A = 0b01 << 5,
            CLK_VLO = 0b10 << 5,
            CLK_K = 0b11 << 5,

            // Watchdog timer mode select
            MODE_WD = 0 << 4,   //!< Watchdog mode
            MODE_TIM = 1 << 4,  //!<  Interval timer mode

            /**
             * Watchdog timer counter clear. Setting WDTCNTCL = 1 clears the
             count value to 0000h. WDTCNTCL is automatically reset.
             */
            CLR = 1 << 3,

            // Watchdog timer interval select. These bits select the watchdog
            // timer interval to set the WDTIFG flag or generate a PUC.
            TIS_2_6 = 7,   //!< 64 (1.95 ms at 32.768 kHz)
            TIS_2_9 = 6,   //!< 512 (15.625 ms at 32.768 kHz)
            TIS_2_13 = 5,  //!< 8'192 (250 ms at 32.768 kHz)
            TIS_2_15 = 4,  //!< 32'768 (1 s at 32.768 kHz)
            TIS_2_19 = 3,  //!< 524'288 (00:00:16 at 32.768 kHz)
            TIS_2_23 = 2,  //!< 8'388'608 (00:04:16 at 32.768 kHz)
            TIS_2_27 = 1,  //!< 134'217'728 (01:08:16 at 32.768 kHz)
            TIS_2_31 = 0,  //!< 2'147'483'648 (18:12:16 at 32.768 kHz)
        };

        IOREG<u16, addr> WDCTL;

        inline void reload() {}
        inline void stop() { WDCTL = WDT::PW | WDT::HOLD; }
        inline void start() { WDCTL = WDT::PW; }
    };
}  // namespace MSP430::Driver::WDT_A
