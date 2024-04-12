TOOLCHAIN := arm-none-eabi
GCC := $(TOOLCHAIN)-gcc
LD := $(TOOLCHAIN)-ld
OBJDUMP := $(TOOLCHAIN)-objdump
OBJCOPY := $(TOOLCHAIN)-objcopy

CPROGS := $(wildcard *.c)
ASMPROGS := $(wildcard *.S)
OBJS := $(CPROGS:.c=.o) $(ASMPROGS:.S=.o)

CFLAGS := -mcpu=cortex-m0plus -mtune=cortex-m0plus -g

all: pi.list pi.uf2

$(CPROGS:.c=.o): %.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@
$(ASMPROGS:.S=.o): %.o: %.S
	$(GCC) $(CFLAGS) -c $< -o $@

pi.elf: memmap.ld $(OBJS)
	$(LD) -o $@ -T memmap.ld $(OBJS)

pi.list: pi.elf
	$(OBJDUMP) -d $< > $@

pi.bin: pi.elf
	$(OBJCOPY) -O binary $< $@

pi-patched.bin: pi.bin
	cat pi.bin | python3 patch-crc.py > pi-patched.bin

pi.uf2: pi-patched.bin
	tools/uf2conv/uf2conv.py $< --family rp2040 -b 0x10000000 --convert --output $@

clean:
	rm -f *.o *.elf *.list *.bin *.uf2

install: pi.uf2 pi.list
	sudo mount $(wildcard /dev/sda*1) /mnt/sd
	sudo cp pi.uf2 /mnt/sd
	sync

.PHONY: all clean install

#!/bin/sh
# arm-none-eabi-gcc -mcpu=cortex-m0plus -mtune=cortex-m0plus -g -c uart.c
# arm-none-eabi-gcc -mcpu=cortex-m0plus -mtune=cortex-m0plus -g -c gpio.c
# arm-none-eabi-gcc -mcpu=cortex-m0plus -mtune=cortex-m0plus -g -c notmain.c
# arm-none-eabi-gcc -mcpu=cortex-m0plus -mtune=cortex-m0plus -g -c start.S
# arm-none-eabi-ld -o pi.elf -T memmap.ld start.o notmain.o gpio.o uart.o
# arm-none-eabi-objdump -d pi.elf > pi.list
# arm-none-eabi-objcopy -O binary pi.elf pi.bin
# cat pi.bin | python3 patch-crc.py > pi-patched.bin
# tools/uf2conv/uf2conv.py pi-patched.bin --family rp2040 -b 0x10000000 --convert --output pi.uf2
