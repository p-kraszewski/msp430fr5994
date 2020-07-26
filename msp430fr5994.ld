/* --------------------------------------------------------------------------
 * -- (C) 2020 Paweł Kraszewski                                            --
 * --                                                                      --
 * -- Licensed as:                                                         --
 * --   Attribution-NonCommercial-ShareAlike 4.0 International             --
 * --   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode        --
 * ------------------------------------------------------------------------*/

ENTRY(vec_Reset)


MEMORY
{
  RAM_TINY (RW) : ORIGIN = 0x0000A, LENGTH = 0x00016
  IO (RW)       : ORIGIN = 0x00020, LENGTH = 0x00FE0
  BOOT (RX)     : ORIGIN = 0x01000, LENGTH = 0x00800
  INFORM (RW)   : ORIGIN = 0x01800, LENGTH = 0x00200
  TLV (RW)      : ORIGIN = 0x01A00, LENGTH = 0x00100
  RAM (RW)      : ORIGIN = 0x01C00, LENGTH = 0x01000
  RAM_LEA (RW)  : ORIGIN = 0x02C00, LENGTH = 0x01000
  FRAM (RWX)    : ORIGIN = 0x04000, LENGTH = 0x0BF80
  VECTORS (WA)  : ORIGIN = 0x0FF80, LENGTH = 0x00080
  FRAM_HI (RWX) : ORIGIN = 0x10000, LENGTH = 0x34000
}

SECTIONS
{
  .vectors :
  {
    KEEP(*(.vectors))
  } >VECTORS =0

  .fram_high ORIGIN(FRAM_HI) : ALIGN(2)
  {
    *(.text.high);
    *(.persistent.high);
  } >FRAM_HI

  .fram_low ORIGIN(FRAM) : ALIGN(2)
  {
    KEEP(*(.Reset));
    KEEP(*(.text));
    *(.persistent.low);
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

  .bss.lea :
  {
    . = ALIGN(2);
    *(.bss.lea);
  } >RAM_LEA

  .bss.tiny :
  {
    . = ALIGN(2);
    *(.bss.tiny);
  } >RAM_TINY
}
