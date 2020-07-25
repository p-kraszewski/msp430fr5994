#include "lib/msp430fr5994.h"

volatile unsigned int i = 0;

int main() {
    using namespace MSP430::dev;

    wdt_a.stop();
    pmm.unlock_pm5();

    p1.DIR = 0b11;
    //p1.DIR.bit<1>().set();
    p1.OUT = 0b01;
    //    tb0.ccr<3>().bclr(1);
    //    p1.OUT.bits<0u, 2u>() = 3;

    while (true) {
        p1.OUT ^= 0b11;
        for (i = 0; i < 20000; i++)
            ;
    }
}

IRQ_HANDLER void irq_TB0_CCR0() {
}
