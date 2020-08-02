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
        struct Mutator {
            MSP430::Driver::Clock::cs<addr> &clk;
            u16                              R0, R1, R2, R3, R4, R5, R6;
            bool                             R1c, R2c, R3c, R4c, R5c, R6c;

            Mutator(MSP430::Driver::Clock::cs<addr> clk, bool update)
                : clk(clk)
                , R0(0xA500)
                , R1(update ? clk.CTL1.get() : 0)
                , R2(update ? clk.CTL2.get() : 0)
                , R3(update ? clk.CTL3.get() : 0)
                , R4(update ? clk.CTL4.get() : 0)
                , R5(update ? clk.CTL5.get() : 0)
                , R6(update ? clk.CTL6.get() : 0)
                , R1c(false)
                , R2c(false)
                , R3c(false)
                , R4c(false)
                , R5c(false)
                , R6c(false) {}

            ~Mutator() {
                clk.CTL0 = 0xA500;
                if (R1c)
                    clk.CTL1 = R1;
                if (R2c)
                    clk.CTL2 = R2;
                if (R3c)
                    clk.CTL3 = R3;
                if (R4c)
                    clk.CTL4 = R4;
                if (R5c)
                    clk.CTL5 = R5;
                if (R6c)
                    clk.CTL6 = R6;
            }

            Mutator &Set_DCO(DCO freq) {
                R1c = true;
                R1  = (u16)freq;
                return *this;
            }

            Mutator &Set_ACLK(ACLK src, DIV div) {
                R2c = true;
                R3c = true;
                mask_write<10, 8>(R2, (u16)src);
                mask_write<10, 8>(R3, (u16)div);
                return *this;
            }

            Mutator &Set_SMCLK(MCLK src, DIV div) {
                R2c = true;
                R3c = true;
                mask_write<6, 4>(R2, (u16)src);
                mask_write<6, 4>(R3, (u16)div);
                return *this;
            }

            Mutator &Set_MCLK(MCLK src, DIV div) {
                R2c = true;
                R3c = true;
                mask_write<2, 0>(R2, (u16)src);
                mask_write<2, 0>(R3, (u16)div);
                return *this;
            }

            Mutator &Set_HFXT(HF freq, DRIVE drive = DRIVE::LOW,
                              bool bypass = false) {
                R4c = true;
                mask_write<15, 14>(R4, (u16)drive);
                mask_write<12, 12>(R4, (u16)bypass);
                mask_write<11, 10>(R4, (u16)freq);
                mask_write<8, 8>(R4, (u16)0);
                return *this;
            }
            Mutator &Enable_HFXT(bool on) {
                R4c = true;
                mask_write<8, 8>(R4, (u16)!on);
                return *this;
            }

            Mutator &Set_LFXT(DRIVE drive = DRIVE::LOW, bool bypass = false) {
                R4c = true;
                mask_write<7, 6>(R4, (u16)drive);
                mask_write<4, 4>(R4, (u16)bypass);
                mask_write<0, 0>(R4, (u16)0);
                return *this;
            }

            Mutator &Enable_LFXT(bool on) {
                R4c = true;
                mask_write<0, 0>(R4, (u16)!on);
                return *this;
            }

            Mutator &Enable_VLO(bool on) {
                R4c = true;
                mask_write<3, 3>(R4, (u16)!on);
                return *this;
            }

            Mutator &Enable_SMCLK(bool on) {
                R4c = true;
                mask_write<1, 1>(R4, (u16)!on);
                return *this;
            }
        };

      public:
        IOREG<u16, addr + 0x00> CTL0;
        IOREG<u16, addr + 0x02> CTL1;
        IOREG<u16, addr + 0x04> CTL2;
        IOREG<u16, addr + 0x06> CTL3;
        IOREG<u16, addr + 0x08> CTL4;
        IOREG<u16, addr + 0x0A> CTL5;
        IOREG<u16, addr + 0x0C> CTL6;

        Mutator New() {
            Mutator t(*this, false);
            return t;
        }

        Mutator Update() {
            Mutator t(*this, true);
            return t;
        }
    };
}  // namespace MSP430::Driver::Clock
