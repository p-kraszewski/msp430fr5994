/* --------------------------------------------------------------------------
 * -- (C) 2020 Paweł Kraszewski                                            --
 * --                                                                      --
 * -- Licensed as:                                                         --
 * --   Attribution-NonCommercial-ShareAlike 4.0 International             --
 * --   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode        --
 * ------------------------------------------------------------------------*/

#pragma once

#define PACKED __attribute((packed))
#define IRQ_HANDLER(id) extern "C" __attribute((interrupt)) void id()

#define CODE_HIGH __attribute((section(".text.high")))
#define DATA_LEA __attribute((section(".bss.lea")))
#define DATA_TINY __attribute((section(".bss.tiny")))
#define DATA_PERSISTENT __attribute((section(".persistent.low")))
#define DATA_PERSISTENT_HIGH __attribute((section(".persistent.high")))
#define NOINLINE __attribute((noinline))

namespace MSP430 {
    typedef unsigned char u8;
    typedef signed char i8;
    typedef unsigned short u16;
    typedef signed short i16;
    typedef unsigned long u32;
    typedef signed long i32;

    static_assert(sizeof(u8) == 1, "Bad u8/i8");
    static_assert(sizeof(u16) == 2, "Bad u16/i16");
    static_assert(sizeof(u32) == 4, "Bad u32/i32");

    namespace Tools {
        /**
         * Image of a single bit of a specific port
         * @tparam reg - type of port: u16 or u8
         * @tparam addr - base address of port
         * @tparam bitNo - bit number to access
         */
        template <typename reg, u16 addr, u8 bitNo>
        struct IOBIT {
          private:
            typedef IOBIT<reg, addr, bitNo> self;
            static constexpr u8 sizeInBits = 8 * sizeof(reg);
            static constexpr reg bitMask = 1u << bitNo;
            static_assert(bitNo < sizeInBits);

            inline volatile reg &ref() { return *((volatile reg *)addr); }

          public:
            /**
             * Set bit to 1
             */
            inline void set() { ref() |= bitMask; }

            /**
             * Set bit to 0
             */
            inline void clear() { ref() &= ~bitMask; }

            /**
             * Toggle value of bit
             */
            inline void toggle() { ref() ^= bitMask; }

            /**
             * Return value of bit as bool
             * @return
             */
            explicit inline operator bool() { return (ref() & bitMask) != 0; }

            /**
             * Return value of bit as int
             * @return value of 0 or 1
             */
            explicit inline operator reg() {
                return ((ref() & bitMask) == 0) ? 0 : 1;
            }

            /**
             * Assign value to bit
             * @param v non-zero means 1
             */
            inline self &operator=(reg v) {
                if (v != 0)
                    set();
                else
                    clear();
                return *this;
            }

            /**
             * Shortcut of bit set
             */
            inline void operator++() { set(); }

            /**
             * Shortcut of bit clear
             */
            inline void operator--() { clear(); }

            /**
             * Shortcut of bit toggle
             */
            inline void operator!() { toggle(); }
        };

        /**
         * Image of a bit range of a specific port.
         * Order of `firstBit` and `lastBit` does not matter
         *
         * @tparam reg - type of port: u16 or u8
         * @tparam addr - address of port
         * @tparam firstBit - begin of bit range
         * @tparam lastBit - end of bit range
         */
        template <typename reg, u16 addr, u8 firstBit, u8 lastBit>
        struct IOBITRANGE {
          private:
            typedef IOBITRANGE<reg, addr, firstBit, lastBit> self;
            static constexpr u8 lowBit =
                (firstBit > lastBit) ? lastBit : firstBit;
            static constexpr u8 highBit =
                (firstBit > lastBit) ? firstBit : lastBit;
            static constexpr u8 bitLength = highBit - lowBit;
            static constexpr u8 sizeInBits = 8 * sizeof(reg);
            static constexpr reg allOnes = ~(reg)0u;
            static constexpr reg bitMask = allOnes
                                           >> (sizeInBits - bitLength - 1);
            static_assert(lowBit + bitLength < sizeInBits,
                          "IOBITRANGE falls of register");
            static_assert(bitLength > 0, "IOBITRANGE null length");

            inline volatile reg &ref() { return *((volatile reg *)addr); }

          public:
            /**
             * Get value of bit range as int
             * @return
             */
            inline reg get() { return (ref() >> lowBit) & bitMask; }

            /**
             * Set new value to bit range.
             * Update is performed in two port read-modify-wites:
             *   - mask old value,
             *   - insert new value
             * @param in new value
             */
            inline void set(reg in) {
                ref() &= ~(bitMask << lowBit);
                ref() |= (in & bitMask) << lowBit;
            }

            /**
             * Set bit range to all-ones
             */
            inline void set() { ref() |= bitMask << lowBit; }

            /**
             * Set new value to bit range atomically.
             * Update is performed as single rad-modify-write with the help of
             * temporary variable:
             *   - read port to temp var,
             *   - mask and insert new value into temp var
             *   - write temp var back to port
             * @param in new value
             */
            inline void set_atomic(reg in = bitMask) {
                reg tmp = ref();
                tmp &= ((reg)(~bitMask)) << lowBit;
                tmp |= (in & bitMask) << lowBit;
                ref() = tmp;
            }

            /**
             * Clear range
             */
            inline void clr() { ref() &= ~(bitMask << lowBit); }

            /**
             * Cast tange to integer
             * @return value
             */
            explicit inline operator reg() { return get(); }

            /**
             * Assign to range
             * @param in new value
             */
            inline self &operator=(reg in) {
                set(in);
                return *this;
            }
        };

        /**
         * Image of single port of device
         * @tparam reg - type of port: u16 or u8
         * @tparam addr - base address of port
         */
        template <typename reg, u16 addr>
        struct IOREG {
          private:
            typedef IOREG<reg, addr> self;
            inline volatile reg &ref() { return *((volatile reg *)addr); }

          public:
            /**
             * Set new value to port
             * @param in new value
             */
            inline void set(reg in) { ref() = in; }

            /**
             * Assign new value to port
             * @param in new value
             * @return
             */
            inline self &operator=(reg in) {
                ref() = in;
                return *this;
            }

            /**
             * Get value from port
             * @return
             */
            inline reg get() { return ref(); }

            /**
             * Set single bit in port
             * Warning, there's no check if bit number is in range. If
             * bit number is constant, prefer to use bit<nr>() accessor that
             * is checked during compile-time,
             * @param m bit number.
             */
            inline void bset(u8 m) { ref() |= (1u << m); }

            /**
             * Clear single bit in port
             * Warning, there's no check if bit number is in range. If
             * bit number is constant, prefer to use bit<nr>() accessors that
             * is checked during compile-time,
             * @param m bit number.
             */
            inline void bclr(u8 m) { ref() &= ~(1u << m); }

            /**
             * Toggle single bit in port
             * Warning, there's no check if bit number is in range. If
             * bit number is constant, prefer to use bit<nr>() accessor that
             * is checked during compile-time,
             * @param m bit number.
             */
            inline void btoggle(u8 m) { ref() ^= (1u << m); }

            /**
             * Mask and set value of register.
             * Update is performed as single read-modify-write with the help of
             * temporary variable:
             *   - read port to temp var,
             *   - AND and OR new values into temp var
             *   - write temp var back to port
             * @param and_val - data to be ANDed with register
             * @param or_val - data to be ORed with register
             */
            inline void mask_set(reg and_val, reg or_val) {
                auto tmp = ref();
                tmp &= and_val;
                tmp |= or_val;
                ref() = tmp;
            }

            /**
             * Set bits of register
             * @param m value
             */
            inline void operator|=(reg m) { ref() |= m; }

            /**
             * Mask bits of register
             * @param m value
             */
            inline void operator&=(reg m) { ref() &= m; }

            /**
             * Toggle bits of register
             * @param m value
             */
            inline void operator^=(reg m) { ref() ^= m; }

            /**
             * Set specific bit in register (no range check)
             * @param bitNr bit number
             */
            inline void operator+=(u8 bitNr) { bset(bitNr); }

            /**
             * Clear specific bit in register (no range check)
             * @param bitNr bit number
             */
            inline void operator-=(u8 bitNr) { bclr(bitNr); }

            /**
             * Toggle specific bit in register (no range check)
             * @param bitNr bit number
             */
            inline void operator%=(u8 bitNr) { btoggle(bitNr); }

            /**
             * Return single bit accessor.
             * - Bit number 0 is leftmost.
             * - bitNr is compile-time checked for validity.
             * @tparam bitNr bit number
             * @return accessor
             */
            template <u8 bitNr>
            inline IOBIT<reg, addr, bitNr> bit() {
                IOBIT<reg, addr, bitNr> b;
                return b;
            }

            /**
             * Return bit range accessor.
             * - Bit number 0 is leftmost.
             * - Order of ends does not matter.
             * - Ends are compile-time checked for validity.
             * @tparam firstBit begin of bit range, inclusive
             * @tparam lastBit  end of bit range, inclusive
             * @return accessor
             */
            template <u8 firstBit, u8 lastBit>
            inline IOBITRANGE<reg, addr, firstBit, lastBit> bits() {
                IOBITRANGE<reg, addr, firstBit, lastBit> br;
                return br;
            }

            /**
             * Check if **all** bits selected by mask are set
             * @param mask - bits to check
             * @return
             */
            inline bool operator&&(reg mask) { return (ref() & mask) == mask; }

            /**
             * Check if **any** bit selected by mask is set
             * @param mask - bits to check
             * @return
             */
            inline bool operator||(reg mask) { return (ref() & mask) != 0; }
        };

    }  // namespace Tools
}  // namespace MSP430
