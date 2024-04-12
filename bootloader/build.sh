#/bin/sh

arm-none-eabi-gcc -mcpu=cortex-m0plus -mtune=cortex-m0plus -g -c uart.c
arm-none-eabi-gcc -mcpu=cortex-m0plus -mtune=cortex-m0plus -g -c gpio.c
arm-none-eabi-gcc -mcpu=cortex-m0plus -mtune=cortex-m0plus -g -c notmain.c
arm-none-eabi-gcc -mcpu=cortex-m0plus -mtune=cortex-m0plus -g -c start.S
arm-none-eabi-ld -o pi.elf -T memmap.ld start.o notmain.o gpio.o uart.o
arm-none-eabi-objdump -d pi.elf > pi.list
arm-none-eabi-objcopy -O binary pi.elf pi.bin
cat pi.bin | python3 patch-crc.py > pi-patched.bin
tools/uf2conv/uf2conv.py pi-patched.bin --family rp2040 -b 0x10000000 --convert --output pi.uf2
