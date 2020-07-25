ENTRY(vec_Reset)


MEMORY
{
  VECTORS (WA)  : ORIGIN = 0x0FF80, LENGTH = 0x00080
  FRAM (RX)    : ORIGIN = 0x04000, LENGTH = 0x40000
  RAM (RW)     : ORIGIN = 0x01C00, LENGTH = 0x01000
  RAM_LEA (RW) : ORIGIN = 0x02C00, LENGTH = 0x01000
  TLV (R)     : ORIGIN = 0x01A00, LENGTH = 0x00100
  BSL (R)     : ORIGIN = 0x01000, LENGTH = 0x00800
}

SECTIONS
{
  .vectors :
  {
    KEEP(*(.vectors))
  } >VECTORS =0

  .text ORIGIN(FRAM) : ALIGN(2)
  {
   KEEP(*(.Reset));
   KEEP(*(.text));
  } >FRAM

  .bss :
    {
      . = ALIGN(2);
      PROVIDE (__bssstart = .);
      *(.bss)
      PROVIDE (__bssend = .);
    } >RAM
    PROVIDE (__bsssize = SIZEOF(.bss));

  .stack (ORIGIN (RAM) + LENGTH (RAM)) :
    {
      PROVIDE (__stack = .);
      PROVIDE (__stack_size = 0x100);
    } >RAM
}
