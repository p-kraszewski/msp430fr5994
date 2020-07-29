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

    enum CTL : u16 {
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

        /**
         * Watchdog timer counter clear. Setting WDTCNTCL = 1 clears the
         count value to 0000h. WDTCNTCL is automatically reset.
         */
        CLR = 1 << 3,

    };

    enum class CLK : u16 {
        SM = 0b00 << 5,
        A = 0b01 << 5,
        VLO = 0b10 << 5,
        K = 0b11 << 5,
    };

    enum class MODE : u16 {
        // Watchdog timer mode select
        WD = 0 << 4,   //!< Watchdog mode
        TIM = 1 << 4,  //!< Interval timer mode
    };

    enum class INTERVAL : u16 {
        // Watchdog timer interval select. These bits select the watchdog
        // timer interval to set the WDTIFG flag or generate a PUC.
        I2_6 = 7,   //!< 64
        I2_9 = 6,   //!< 512
        I2_13 = 5,  //!< 8'192
        I2_15 = 4,  //!< 32'768
        I2_19 = 3,  //!< 524'288
        I2_23 = 2,  //!< 8'388'608
        I2_27 = 1,  //!< 134'217'728
        I2_31 = 0,  //!< 2'147'483'648

        T_2ms = 7,    //!< 1.95 ms at 32.768 kHz
        T_16ms = 6,   //!< 15.625 ms at 32.768 kHz
        T_250ms = 5,  //!< 250 ms at 32.768 kHz
        T_1s = 4,     //!< 1 s at 32.768 kHz
        T_16s = 3,    //!< 00:00:16 at 32.768 kHz
        T_5m = 2,     //!< 00:04:16 at 32.768 kHz
        T_1h = 1,     //!< 01:08:16 at 32.768 kHz
        T_18h = 0,    //!< 18:12:16 at 32.768 kHz
    };                // namespace MSP430::Driver::WDT_A

    template <u16 addr>
    struct wdt_a {

        IOREG<u16, addr> WDCTL;

        inline void write(u16 val) { WDCTL = (CTL::PW | (val & 0xFF)); }

        inline void reload() { write(WDCTL.get() | CTL::CLR); }
        inline void stop() { write(WDCTL.get() | CTL::HOLD); }
        inline void start() { write(WDCTL.get() & ~CTL::HOLD); }

        template <CLK ClkSource, INTERVAL Interval, MODE Mode>
        inline void setup() {
            write((u16)ClkSource | (u16)Interval | (u16)Mode | CTL::CLR);
        }
    };
}  // namespace MSP430::Driver::WDT_A
