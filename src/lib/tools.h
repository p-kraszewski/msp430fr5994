/* --------------------------------------------------------------------------
 * -- (C) 2020 Paweł Kraszewski                                            --
 * --                                                                      --
 * -- Licensed as:                                                         --
 * --   Attribution-NonCommercial-ShareAlike 4.0 International             --
 * --   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode        --
 * ------------------------------------------------------------------------*/

#pragma once

#define PACKED __attribute((packed))
#define IRQ_HANDLER extern "C" __attribute((interrupt))

#define CODE_HIGH __attribute((section(".text.high")))
#define DATA_LEA __attribute((section(".bss.lea")))
#define DATA_TINY __attribute((section(".bss.tiny")))
#define DATA_PERSISTENT __attribute((section(".persistent.low")))
#define DATA_PERSISTENT_HIGH __attribute((section(".persistent.high")))
#define NONLINE __attribute((noinline))

namespace MSP430 {
    typedef unsigned char u8;
    typedef signed char i8;
    typedef unsigned short u16;
    typedef signed short i16;
    typedef unsigned long u32;
    typedef signed long i32;

    static_assert(sizeof(u8) == 1, "Bad u32/i32");
    static_assert(sizeof(u16) == 2, "Bad u32/i32");
    static_assert(sizeof(u32) == 4, "Bad u32/i32");

    namespace Tools {
        template <typename reg, u16 addr, u8 bitNo>
        class IOBIT {
            static constexpr u8 sizeInBits = 8 * sizeof(reg);
            static constexpr reg bitMask = 1 << bitNo;
            static_assert(bitNo < sizeInBits);

          private:
            inline volatile reg &ref() { return *((volatile reg *)addr); }

          public:
            inline void set() { ref() |= bitMask; }
            inline void flip() { ref() ^= bitMask; }
            inline void clear() { ref() &= ~bitMask; }

            inline operator bool() { return (ref() & bitMask) != 0; }
            inline void operator=(bool v) {
                if (v)
                    set();
                else
                    clear();
            }

            inline void operator=(reg v) {
                if (v != 0)
                    set();
                else
                    clear();
            }

            inline void operator++() { set(); }
            inline void operator--() { clear(); }
            inline void operator!() { flip(); }
        };

        template <typename reg, u16 addr, u8 firstBit, u8 lastBit>
        class IOBITRANGE {
            static constexpr u8 lowBit =
                (firstBit > lastBit) ? lastBit : firstBit;
            static constexpr u8 highBit =
                (firstBit > lastBit) ? firstBit : lastBit;
            static constexpr u8 bitLength = highBit - lowBit;
            static constexpr u8 sizeInBits = 8 * sizeof(reg);
            static constexpr reg allOnes = ~(reg)0u;
            static constexpr reg bitMask = allOnes >> (sizeInBits - bitLength);
            static_assert(lowBit + bitLength < sizeInBits,
                          "IOBITRANGE falls of register");
            static_assert(bitLength > 0, "IOBITRANGE null length");

          private:
            inline volatile reg &ref() { return *((volatile reg *)addr); }

          public:
            inline reg get() { return (ref() >> lowBit) & bitMask; }
            inline void set(reg in) {
                ref() &= ~(bitMask << lowBit);
                ref() |= (in & bitMask) << lowBit;
            }
            inline void set_atomic(reg in) {
                reg tmp = ref();
                tmp &= ((reg)(~bitMask)) << lowBit;
                tmp |= (in & bitMask) << lowBit;
                ref() = tmp;
            }
            inline void clr() { return set(0); }
            inline operator reg() { return get(); }
            inline void operator=(reg in) { set(in); }
        };

        template <typename reg, u16 addr>
        class IOREG {
          private:
            inline volatile reg &ref() { return *((volatile reg *)addr); }

          public:
            inline void W(reg nv) { ref() = nv; }
            inline reg R() { return ref(); }

            inline void bset(u8 m) { ref() |= (1 << m); }

            inline void bclr(u8 m) { ref() &= ~(1 << m); }

            inline void btoggle(u8 m) { ref() ^= (1 << m); }

            inline IOREG<reg, addr> &mask_set(reg mask, reg val) {
                auto tmp = ref();
                tmp &= mask;
                tmp |= val;
                ref() = tmp;
                return *this;
            }

            inline IOREG<reg, addr> &operator|=(reg m) {
                ref() |= m;
                return *this;
            }
            inline IOREG<reg, addr> &operator&=(reg m) {
                ref() &= m;
                return *this;
            }
            inline IOREG<reg, addr> &operator^=(reg m) {
                ref() ^= m;
                return *this;
            }
            inline IOREG<reg, addr> &operator=(reg m) {
                ref() = m;
                return *this;
            }
            inline IOREG<reg, addr> &operator+=(u8 m) {
                bset(m);
                return *this;
            }
            inline IOREG<reg, addr> &operator-=(u8 m) {
                bclr(m);
                return *this;
            }
            inline IOREG<reg, addr> &operator%=(u8 m) {
                btoggle(m);
                return *this;
            }

            template <u8 bitNo>
            inline IOBIT<reg, addr, bitNo> bit() {
                IOBIT<reg, addr, bitNo> b;
                return b;
            }

            template <u8 firstBit, u8 lastBit>
            inline IOBITRANGE<reg, addr, firstBit, lastBit> bits() {
                IOBITRANGE<reg, addr, firstBit, lastBit> br;
                return br;
            }

            inline bool operator&&(reg m) { return (ref() & m) == m; }
            inline bool operator||(reg m) { return (ref() & m) != 0; }
        };

    }  // namespace Tools
}  // namespace MSP430
