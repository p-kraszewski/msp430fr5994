# --------------------------------------------------------------------------
# -- (C) 2020 Paweł Kraszewski                                            --
# --                                                                      --
# -- Licensed as:                                                         --
# --   Attribution-NonCommercial-ShareAlike 4.0 International             --
# --   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode        --
# --------------------------------------------------------------------------

CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR msp430)
SET(TRIPLE msp430-none-elf)
SET(MSP_PREFIX msp430-elf-)
SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/msp430fr5994.ld)
SET(COMP_ARCH "-mcpu=msp430x -mmcu=msp430fr5994")

SET(LINKER_FLAGS "-nostdlib -static -mlarge -Wl,--whole-archive")
SET(C_FLAGS "-O3 -mlarge -mhwmult=auto")
SET(ASM_FLAGS "-ml")

SET(CMAKE_CXX_STANDARD 17)

SET(CMAKE_ASM_COMPILER ${MSP_PREFIX}as)
SET(CMAKE_C_COMPILER ${MSP_PREFIX}gcc)
SET(CMAKE_CXX_COMPILER ${MSP_PREFIX}g++)
SET(CMAKE_CXX_COMPILER_AR ${MSP_PREFIX}ar)
SET(CMAKE_CXX_COMPILER_RANLIB ${MSP_PREFIX}ranlib)

SET(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

SET(CMAKE_CXX_FLAGS "${COMP_ARCH} ${C_FLAGS}")
SET(CMAKE_C_FLAGS "${COMP_ARCH} ${C_FLAGS}")
SET(CMAKE_ASM_FLAGS "${COMP_ARCH} ${ASM_FLAGS}")
STRING(REPLACE "-O3" "" CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE}")
STRING(REPLACE "-DNDEBUG" "" CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE}")

SET(LD_FLAGS "${LINKER_FLAGS}")
SET(LDFLAGS "${LINKER_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS} -T ${LINKER_SCRIPT}")

ENABLE_LANGUAGE(ASM CXX)

PROJECT(MSP430FR5994)
ADD_LIBRARY(${PROJECT_NAME} STATIC lib/rt.S)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} INTERFACE lib/)

PROJECT(Blinker)
ADD_EXECUTABLE(${PROJECT_NAME} src/${PROJECT_NAME}.cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} MSP430FR5994)

PROJECT(DocExamples)
ADD_EXECUTABLE(${PROJECT_NAME} src/${PROJECT_NAME}.cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} MSP430FR5994)
