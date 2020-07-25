#pragma once

#define PACKED __attribute((packed))
#define IRQ_HANDLER extern "C" __attribute((interrupt))

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


    template<typename port, u16 addr, u8 bitNo>
    class IOBIT {
        static constexpr u8 sizeInBits = 8 * sizeof(port);
        static constexpr port bitMask = 1 << bitNo;
        static_assert(bitNo < sizeInBits);

    private:
        inline volatile port &ref() { return *((volatile port *) addr); }

    public:
        inline void set() { ref() |= bitMask; }
        inline void flip() { ref() ^= bitMask; }
        inline void clear() { ref() &= ~bitMask; }

        inline operator bool() { return (ref() & bitMask) != 0; }
        inline void operator=(bool v) {
            if (v) set();
            else
                clear();
        }

        inline void operator++() { set(); }
        inline void operator--() { clear(); }
        inline void operator!() { flip(); }
    };

    template<typename type, u16 addr, u8 firstBit, u8 lastBit>
    class IOBITRANGE {
        static constexpr u8 lowBit = (firstBit > lastBit) ? lastBit : firstBit;
        static constexpr u8 highBit = (firstBit > lastBit) ? firstBit : lastBit;
        static constexpr u8 bitLength = highBit - lowBit + 1;
        static constexpr u8 sizeInBits = 8 * sizeof(type);
        static constexpr type bitMask = (~(type) 0u >> (sizeInBits - bitLength));
        static_assert(lowBit + bitLength < sizeInBits, "IOBITRANGE falls of register");
        static_assert(bitLength > 0, "IOBITRANGE null length");

    private:
        inline volatile type &ref() { return *((volatile type *) addr); }

    public:
        inline type get() { return (ref() >> lowBit) & bitMask; }
        inline void set(type in) {
            constexpr type andMask = ((type)(~bitMask)) << lowBit;
            type orMask = (in & bitMask) << lowBit;
            type t = ref();
            ref() = (t & andMask) | orMask;
        }
        inline void clr() { return set(0); }
        inline operator type() { return get(); }
        inline void operator=(type in) { set(in); }
    };

    template<typename port, u16 addr>
    class IOREG {
    private:
        inline volatile port &ref() { return *((volatile port *) addr); }

    public:
        inline void W(port nv) { ref() = nv; }
        inline port R() { return ref(); }

        inline void bset(u8 m) {
            ref() |= (1 << m);
        }

        inline void bclr(u8 m) {
            ref() &= ~(1 << m);
        }

        inline void btoggle(u8 m) {
            ref() ^= (1 << m);
        }

        inline IOREG<port, addr> &mask_set(port mask, port val) {
            auto tmp = ref();
            tmp &= mask;
            tmp |= val;
            ref() = tmp;
            return *this;
        }

        inline IOREG<port, addr> &operator|=(port m) {
            ref() |= m;
            return *this;
        }
        inline IOREG<port, addr> &operator&=(port m) {
            ref() &= m;
            return *this;
        }
        inline IOREG<port, addr> &operator^=(port m) {
            ref() ^= m;
            return *this;
        }
        inline IOREG<port, addr> &operator=(port m) {
            ref() = m;
            return *this;
        }
        inline IOREG<port, addr> &operator+=(u8 m) {
            bset(m);
            return *this;
        }
        inline IOREG<port, addr> &operator-=(u8 m) {
            bclr(m);
            return *this;
        }
        inline IOREG<port, addr> &operator%=(u8 m) {
            btoggle(m);
            return *this;
        }

        template<u8 bitNo>
        inline IOBIT<port, addr, bitNo> bit() {
            IOBIT<port, addr, bitNo> b;
            return b;
        }

        template<u8 firstBit, u8 lastBit>
        inline IOBITRANGE<port, addr, firstBit, lastBit> bits() {
            IOBITRANGE<port, addr, firstBit, lastBit> br;
            return br;
        }

        inline bool operator&&(port m) { return (ref() & m) == m; }
        inline bool operator||(port m) { return (ref() & m) != 0; }
    };


}// namespace MSP430
