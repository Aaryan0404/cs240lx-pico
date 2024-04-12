#include "gpio.h"
#include "uart.h"
#include <stdint.h>

void cstart() {
  void notmain();
  notmain();
}

void notmain() {
    gpio_reset();
    
    gpio_set_output(15); 

    hw_uart_t uart = {
      .tx = 0,
      .rx = 1,
      .baud_rate = 115200,
      .index = UART0
    };
    uart_init(&uart);

    while (1) {
        uart_putc(&uart, 'a');
        gpio_set_on(15);
        DELAY(0x100000);
        gpio_set_off(15);
        DELAY(0x100000);
    }
}
