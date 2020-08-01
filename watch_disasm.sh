#!/bin/sh

# --------------------------------------------------------------------------
# -- (C) 2020 Paweł Kraszewski                                            --
# --                                                                      --
# -- Licensed as:                                                         --
# --   Attribution-NonCommercial-ShareAlike 4.0 International             --
# --   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode        --
# --------------------------------------------------------------------------

TGT="$1"

if [ ! -r "${TGT}" ]; then
	echo "${TGT} is not readable"
	exit 1
fi

OBJARGS=-dxs

msp430-elf-objdump ${OBJARGS} "${TGT}"

while inotifywait "${TGT}"; do
  msp430-elf-objdump ${OBJARGS} "${TGT}"
done
