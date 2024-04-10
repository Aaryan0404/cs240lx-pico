
MEMORY
{
    flash : origin = 0x10000000, LENGTH = 256
    ram : ORIGIN = 0x20000000, LENGTH = 0x1000
}
SECTIONS
{
    .boot : {
      *(*.boot*)
      *(*.text*)
    } > flash
    .text : { 
    } > ram
}
