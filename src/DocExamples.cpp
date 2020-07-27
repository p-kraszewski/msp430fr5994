/* --------------------------------------------------------------------------
 * -- (C) 2020 Paweł Kraszewski                                            --
 * --                                                                      --
 * -- Licensed as:                                                         --
 * --   Attribution-NonCommercial-ShareAlike 4.0 International             --
 * --   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode        --
 * ------------------------------------------------------------------------*/

#include <msp430fr5994.h>

using namespace MSP430::FR5994;

//-------------------------------------------
// Full register manipulation
// (8/16 bit, depending on particular device)
NOINLINE void full_reg() {

    p1.OUT = 0xAE;   // Assign
    p2.OUT |= 0x02;  // Set mask
    p3.OUT &= 0xFE;  // Clear mask
    p4.OUT ^= 0x81;  // Toggle mask
}

//------------------------
// Single bit manipulation
NOINLINE void bit_reg() {

    p1.OUT += 3;  // Set 3rd bit
    p2.OUT -= 4;  // Clear 4th
    p3.OUT %= 5;  // Toggle 5th bit

    // This form is preferred, as it checks
    // validity of bit number at compile time
    if (p4.IN.bit<5>()) {     // test bit p4.5 ...
        p5.OUT.bit<6>() = 1;  // ... set bit p5.6 to one if p4.5 was set
    }
}

//------------------------
// Bit range manipulation
NOINLINE MSP430::u8 bit_range() {

    // Set bits 4..7 (inclusive, 4 bits total) to a specific value
    p1.OUT.bits<4, 7>() = 5;

    // Order of bit numbers does not matter:
    // bits<4, 7> is the same as bits<7, 4>

    // Read bits 2..6 (inclusive, 5 bits total) as an integer
    // For safety reasons, explicit cast is required
    return (MSP430::u8)p2.IN.bits<2, 6>();
}

int main() {
    full_reg();
    bit_reg();
    bit_range();
}
