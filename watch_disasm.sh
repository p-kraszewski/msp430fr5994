#!/bin/sh

# --------------------------------------------------------------------------
# -- (C) 2020 Paweł Kraszewski                                            --
# --                                                                      --
# -- Licensed as:                                                         --
# --   Attribution-NonCommercial-ShareAlike 4.0 International             --
# --   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode        --
# --------------------------------------------------------------------------

TGT=cmake-build-release/MSP430FR5994
OBJARGS=-dx

msp430-elf-objdump ${OBJARGS} "${TGT}"

while inotifywait "${TGT}"; do
  msp430-elf-objdump ${OBJARGS} "${TGT}"
done
