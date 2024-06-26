#include "gpio.h"
#include "uart.h"
#include "integer-math.h"
#include "clock.h"

void reset_periph(uint32_t loc) {
    PUT32(RESET + SET_OFFSET, 1 << loc);
    while(!((GET32(RESET_DONE) >> loc) & 0x1)) {;}
    PUT32(RESET + CLR_OFFSET, 1 << loc);
}

// Setup hardware uart
void uart_init(hw_uart_t *uart) {
    // 0. Setup UART Pins to correct function
    gpio_set_function(uart->tx, UART0_RXTX);

    if (uart->index != UART0 && uart->index != UART1) {
        return;
    }

    /*if (uart->index == UART0) {
      reset_periph(RESET_UART0);
    } else {
      reset_periph(RESET_UART1);
    }*/

    // 1. Setup UART Speed via clk_peri

    uint32_t baud_rate_divisor = udiv(CLK_PERI_DEFAULT_SPEED, 16 * uart->baud_rate);
    // Careful with overflow
    uint32_t baud_rate_frac = udiv(CLK_PERI_DEFAULT_SPEED * 100, 16 * uart->baud_rate) - (baud_rate_divisor * 100);
    uint32_t fbrd = udiv((baud_rate_frac * 64), 100);

    // 2. Set Integer and Fraction BaudRate pg. 452
    PUT32(uart->index + UART_IBRD_OFFSET, baud_rate_divisor >> 7);
    PUT32(uart->index + UART_FBRD_OFFSET, fbrd);
    // 3. Setup uart->index with 8n1 (pg 456)
    // 4. Enable fifos (pg 456)
    PUT32(uart->index + UART_LCR_OFFSET, 0x70);

    // 5. Enable UART, TRX, and RX pg 457
    PUT32(uart->index + UART_CTRL_OFFSET, 0x301);
}

// 1 = at least one byte on rx queue, 0 otherwise
static int uart_can_getc(void) {
    uint32_t empty = GET32(UART0 + UART_FR_OFFSET) >> 4;
    return ~empty & 0x1;
}

// returns one byte from the rx queue, if needed
// blocks until there is one.
int uart_getc(void) {
  while(!uart_can_getc()){;}
  return GET32(UART0 + UART_DATA_OFFSET);
}

// 1 = space to put at least one byte, 0 otherwise.
int uart_can_putc(hw_uart_t* uart) {
    return !((GET32(uart->index + UART_FR_OFFSET) >> 5) & 1);
}

// put one byte on the tx qqueue, if needed, blocks
// until TX has space.
void uart_putc(hw_uart_t* uart, unsigned char c) {
    while(!uart_can_putc(uart)){;}
    PUT32(uart->index + UART_DATA_OFFSET, c);
}


// simple wrapper routines useful later.
// a maybe-more intuitive name for clients.
int uart_has_data(void) {
    return uart_can_getc();
}

int uart_getc_async(void) { 
    if(!uart_has_data())
        return -1;
    return uart_getc();
}
