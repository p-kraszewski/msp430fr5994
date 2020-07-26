#include "lib/msp430fr5994.h"

DATA_TINY volatile unsigned int i = 0;

CODE_HIGH NONLINE void delay(MSP430::u16 n) {
    for (i = 0; i < n; i++)
        ;
}

int main() {
    using namespace MSP430::dev;

    wdt_a.stop();
    pmm.unlock_pm5();

    p1.DIR = 0b11;
    //p1.DIR.bit<1>().set();
    p1.OUT = 0b01;
    //    tb0.ccr<3>().bclr(1);
    //    p1.OUT.bits<4, 7>() = 3;

    while (true) {
        p1.OUT ^= 0b11;
        delay(20000);
    }
}

IRQ_HANDLER void irq_TB0_CCR0() {
}
