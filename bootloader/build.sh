#/bin/sh

arm-none-eabi-gcc -mcpu=cortex-m0plus -mtune=cortex-m0plus -g -c start.S
arm-none-eabi-ld -o pi.elf -T memmap.ld start.o
arm-none-eabi-objdump -d pi.elf > pi.list
arm-none-eabi-objcopy -O binary pi.elf pi.bin
