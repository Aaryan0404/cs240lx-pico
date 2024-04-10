#include "gpio.h"
#include <stdint.h>

void cstart() {
  void notmain();
  notmain();
}

void notmain() {
    gpio_reset();
    
    gpio_set_output(15); 

    while (1) {
        gpio_set_on(15);
        DELAY(0x100000);
        gpio_set_off(15);
        DELAY(0x100000);
    }
}
