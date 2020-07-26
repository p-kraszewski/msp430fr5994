#!/bin/sh

TGT=cmake-build-release/MSP430FR5994
OBJARGS=-dx

msp430-elf-objdump ${OBJARGS} "${TGT}"

while inotifywait "${TGT}"; do
	msp430-elf-objdump ${OBJARGS} "${TGT}"
done
