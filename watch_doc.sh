#!/bin/sh

# --------------------------------------------------------------------------
# -- (C) 2020 Paweł Kraszewski                                            --
# --                                                                      --
# -- Licensed as:                                                         --
# --   Attribution-NonCommercial-ShareAlike 4.0 International             --
# --   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode        --
# --------------------------------------------------------------------------

TGT="readme.adoc"

PDF="${TGT%.adoc}.pdf"
echo "${TGT} -> ${PDF}"

if [ ! -r "${TGT}" ]; then
	echo "${TGT} is not readable"
	exit 1
fi

asciidoctor-pdf "${TGT}"
[ -r "${PDF}" ] && okular "${PDF}"

while inotifywait "${TGT}"; do
  asciidoctor-pdf "${TGT}"
done
