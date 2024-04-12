#include "gpio.h"
#include "clock.h"

// TODO: Read XOSC section starting at pg.243 in rp2040 datasheet
void enable_clock() {
    uint32_t delay = 47;
    // 0. Setup XOSC delay
    PUT32(XOSC_STARTUP, delay);

    // 1. Enable XOSC
    PUT32(XOSC_CTRL, 0xfab << 12);

    // 2. Wait till XOSC is ready
    while(!(GET32(XOSC_STATUS) >> 31));

    // 3. Setup clk_ref so you can setup clock_sys
    PUT32(CLK_REF_CTRL, 0x2);

    // Lets your reference know you're running at 12MHZ
    PUT32(FC0_REF_KHZ, 0x12000);

    // Switch processor over
    PUT32(CLK_SYS_CTRL, 0x0);
}

//TODO: Use the timer section to make a delay_us function
void delay_us(uint32_t delay) {
    uint64_t start = time();
    while(time() - start < delay);
}

void delay_ms(uint32_t delay) {
  delay_us(delay * 1000);
}
