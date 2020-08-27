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

#include <hal/hal_pll.h>
#include <hal/hal_utils.h>
#include <debug.h>

#define SLEEP_CON       (*(volatile uint16_t*)(ARM_CONFG_base + 0x0040))

int enter_standby_mode(int mode)
{

    uint32_t savedmask = DisableInterrupts();

    PLL_CLK_CONDB |= 0x1000; // 0xA001_0104, bit 12 set to 1 for EMI1X clock = MCU
    while ((PLL_CLK_CONDB & 0x1000) == 0x0000)
        ;

    SLEEP_CON |= 1;
     __asm__ volatile ("nop");
     __asm__ volatile ("nop");
     __asm__ volatile ("nop");
     __asm__ volatile ("nop");
     __asm__ volatile ("nop");
     __asm__ volatile ("nop");

    PLL_CLK_CONDB &= 0xEFFF; // 0xA001_0104, bit 12 set to 0 for EMI1X clock = MCU
    while ((PLL_CLK_CONDB & 0x1000) == 0x1000)
        ;

    RestoreInterrupts(savedmask);
    return 0;
}