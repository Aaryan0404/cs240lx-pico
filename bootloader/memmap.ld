
MEMORY
{
    secondstage : origin = 0x10000000, LENGTH = 256
    flash : origin = 0x10000000 + 256, LENGTH = 2M - 256
    ram : ORIGIN = 0x20000000, LENGTH = 0x1000
}
SECTIONS
{
    .booloader : {
      KEEP(*(.text.secondstage))
    } > secondstage
    .text : { 
      __code_start__ = .;
      KEEP(*(.text.boot))
      *(.text*)
      __code_end__ = .;
      . = ALIGN(8);
    } > ram
}
