////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Georgi Angelov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////

#include "variant.h"
#include <hal_interface.h>
#include <hal/hal_pmu.h>

PinDescription pinsMap[21] = {
    /* 00 */ {GPIO3,    -1, NULL}, // LED, PWM4
    /* 01 */ {GPIO51,   -1, NULL},
    /* 02 */ {GPIO11,   GPIO11_MODE_EINT10, NULL}, // UART1_TX
    /* 03 */ {GPIO10,   GPIO10_MODE_EINT9, NULL},  // UART1_RX
    /* 04 */ {GPIO29,   -1, NULL}, // I2C_SDA
    /* 05 */ {GPIO43,   -1, NULL}, // I2C_SCL
    /* 06 */ {GPIO44,   -1, NULL},
    /* 07 */ {GPIO14,   GPIO14_MODE_EINT12, NULL},
    /* 08 */ {GPIO4,    GPIO04_MODE_EINT3, NULL},
    /* 09 */ {GPIO22,   -1, NULL},
    /* 10 */ {GPIO0,    GPIO00_MODE_EINT0, NULL}, // UART3_RX
    /* 11 */ {GPIO1,    GPIO01_MODE_EINT1, NULL}, // UART3_TX
    /* 12 */ {GPIO26,   -1, NULL}, // SPI_CS,       LSCK1 
    /* 13 */ {GPIO29,   -1, NULL}, // SPI_MISO,     LSCE_B1
    /* 14 */ {GPIO28,   -1, NULL}, // SPI_CLK,      LSDA1 
    /* 15 */ {GPIO27,   -1, NULL}, // SPI_MOSI,     LSA0DA1
    /* 16 */ {GPIO12,   -1, NULL}, // UART2_RX
    /* 17 */ {GPIO17,   -1, NULL}, // UART2_TX
    /*SIM1*/
    /* 18 */ {GPIO37,   -1, NULL},
    /* 19 */ {GPIO38,   -1, NULL},
    /* 19 */ {GPIO39,   -1, NULL},
};

void sim1_to_gpio(bool volt_3)
{
    VSIM1_CON2 |= 2;           // 1: SIM1 is controlled by VSIM1_CON0
    VSIM1_CON0 = 0x0400;       // reset value
    VSIM1_CON0 |= volt_3 << 4; // 0:1.8v, 1:3.0v
    VSIM1_CON0 |= 1;           // 1: VSIM1 Enable
}