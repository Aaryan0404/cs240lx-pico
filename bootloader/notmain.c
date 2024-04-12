#include "gpio.h"
#include "clock.h"
#include "uart.h"
#include "sw-uart.h"
#include <stdint.h>

void cstart() {
  void notmain();
  notmain();
}

void notmain() {
    gpio_reset();
    enable_clock();

    gpio_set_output(0); 
    gpio_set_input(1); 
    sw_uart_t uart = sw_uart_init(0,1,57600);
    
    gpio_set_output(15); 

    while (1) {
        sw_uart_putc(&uart, 'a');
        gpio_set_on(15);
        //DELAY(0x100000);
        delay_ms(1000);
        gpio_set_off(15);
        delay_ms(1000);
    }
}
