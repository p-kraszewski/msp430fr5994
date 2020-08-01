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

    /**
     * I/O Configuration
     */
    enum class MODE {
        OUT,          //!< Output
        IN,           //!< Input
        IN_PULLUP,    //!< Input with pullup resistor
        IN_PULLDOWN,  //!< Input with pulldown resistor
    };

    /**
     *  I/O Function Selection
     */
    enum class FUNCTION {
        GPIO,  //!< General purpose I/O is selected
        F1,    //!< Primary module function is selected
        F2,    //!< Secondary module function is selected
        F3,    //!< Tertiary module function is selected

    };

    /**
     *  interrupt edge for the corresponding I/O pin.
     */
    enum class EDGE {
        RISING,
        FALLING,
    };

    /**
     * Generic simple I/O port interface
     * @tparam addr base address of device
     */
    template <u16 addr>
    struct port_simple {
        /**
         * Each bit in each PxIN register reflects the value of the input signal
         at the corresponding I/O pin when the pin is configured as I/O
         function. These registers are read only.
         */
        IOREG<u8, addr + 0x00> IN;

        /**
         * Each bit in each PxOUT register is the value to be output on the
         corresponding I/O pin when the pin is configured as I/O function,
         output direction.

            If the pin is configured as I/O function, input direction and the
         pullup or pulldown resistor are enabled; the corresponding bit in the
         PxOUT register selects pullup or pulldown.
         */
        IOREG<u8, addr + 0x02> OUT;

        /**
         * Each bit in each PxDIR register selects the direction of the
         corresponding I/O pin, regardless of the selected function for the pin.
         PxDIR bits for I/O pins that are selected for other functions must be
         set as required by the other function. 0=IN 1=OUT
         */
        IOREG<u8, addr + 0x04> DIR;

        /**
         * Each bit in each PxREN register enables or disables the pullup or
         pulldown resistor of the corresponding I/O pin. The corresponding bit
         in the PxOUT register selects if the pin contains a pullup or pulldown.
         */
        IOREG<u8, addr + 0x06> REN;

        /** Function Select Register0 */
        IOREG<u8, addr + 0x0A> SEL0;

        /** /** Function Select Register1 */
        IOREG<u8, addr + 0x0C> SEL1;

        /** Function Select Register01 */
        IOREG<u8, addr + 0x0E> SELC;

        void set_mode(MODE m, u8 pins);
        void set_function(FUNCTION f, u8 pins);
    };

    template <u16 addr>
    inline void port_simple<addr>::set_mode(MODE m, u8 pins) {
        switch (m) {
            case MODE::OUT: {
                DIR |= pins;
                break;
            }
            case MODE::IN: {
                DIR &= ~pins;
                REN &= ~pins;
                break;
            }
            case MODE::IN_PULLUP: {
                DIR &= ~pins;
                REN |= pins;
                OUT |= pins;
                break;
            }
            case MODE::IN_PULLDOWN: {
                DIR &= ~pins;
                REN |= pins;
                OUT &= ~pins;
                break;
            }
        }
    }

    template <u16 addr>
    inline void port_simple<addr>::set_function(FUNCTION f, u8 pins) {
        switch (f) {
            case FUNCTION::GPIO: {
                SEL0 &= ~pins;
                SEL1 &= ~pins;
                break;
            }
            case FUNCTION::F1: {
                SEL0 |= pins;
                SEL1 &= ~pins;
                break;
            }
            case FUNCTION::F2: {
                SEL0 &= ~pins;
                SEL1 |= pins;
                break;
            }
            case FUNCTION::F3: {
                SEL0 |= pins;
                SEL1 |= pins;
                break;
            }
        }
    }

    /**
     * Generic interrupt-generating I/O port interface
     * @tparam addr base address of device
     */
    template <u16 addr>
    struct port_int : public port_simple<addr> {
        IOREG<u8, addr + 0x16> IV;
        IOREG<u8, addr + 0x18> IES;
        IOREG<u8, addr + 0x1A> IE;
        IOREG<u8, addr + 0x1C> IFG;

        void int_enable(EDGE e, u8 pins);
        void int_disable(u8 pins);
    };

    template <u16 addr>
    inline void port_int<addr>::int_disable(u8 pins) {
        IE &= ~pins;
    }

    template <u16 addr>
    inline void port_int<addr>::int_enable(EDGE e, u8 pins) {
        switch (e) {
            case EDGE::RISING: IES &= ~pins; break;
            case EDGE::FALLING: IES |= pins; break;
        }
        IE |= pins;
    }
}  // namespace MSP430::Driver::GPIO
