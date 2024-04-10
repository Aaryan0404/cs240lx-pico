OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

STACK_SIZE = 10K;

MEMORY
{
    flash (rw) : ORIGIN = 0x10000000, LENGTH = 2M
    ram (rwx) : ORIGIN = 0x20000000, LENGTH = 0x40740
}
SECTIONS
{
    .text : { 
      __boot_start__ = .;
      KEEP(*(.text.boot))
      . = ALIGN(256);
      __boot_end__ = .;
      __code_start__ = .;
      KEEP(*(.text.start))
      *(.text*)
      __code_end__ = .;
      . = ALIGN(8);
    } > ram AT > flash
    FLASH_START = LOADADDR(.text);

    /* initialized data needs to be moved to ram instead of flash */
    .data 0x20002710: {
        . = ALIGN(8);
        __data_start__ = .;
        *(.data*)
        . = ALIGN(8);
        __data_end__ = .;
    } > ram AT > flash

    /* global constants */
    .rodata : { *(.rodata*) } > flash
    FLASH_END = ADDR(.rodata);

    /* data initialized to zero that will be written to in mymain */
    .bss (NOLOAD) : {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end__ = .;
    } > ram 

    .stack (NOLOAD): {
      . = ALIGN(8);
      __stack_start__ = .;
      . = . + STACK_SIZE;
      . = ALIGN(8);
      __stack_end__ = .;
    } > ram
}
