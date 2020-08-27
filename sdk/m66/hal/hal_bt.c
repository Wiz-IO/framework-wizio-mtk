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

#include <hal/hal_bt.h>
#include <hal/hal_pctl.h>
#include <hal/hal_nvic.h>
#include <hal/hal_ustimer.h>

void BT_MT6261_ARM7_Initialize(void)
{
    //ARM7_Initialize()
    RegAddr32(0xA0010310) |= (1 << 15);
    delay_u(5);
    RegAddr32(0xA0010310) |= ((1 << 13) | (1 << 9));
    RegAddr32(0xA0180804) &= ~(TOPSM_PWR_ON | TOPSM_PWR_REQ_EN | TOPSM_PWR_RESET);
    delay_u(150);

    //ARM7_PowerOn_Digital()
    RegAddr32(0xA0010320) |= 0xA200;
    RegAddr32(0xA0180804) |= 0x44;
    delay_u(50);
}

void BT_Power(int on_off)
{
    static int bt_once = 0;
    if (0 == bt_once)
    {
        bt_once = 1;
        BT_MT6261_ARM7_Initialize();
    }

    if (on_off)
        PCTL_PowerUp(PD_BTIF); // 0xA0010328 =2
    else
        PCTL_PowerDown(PD_BTIF); // 0xA0010318 = 2
    USC_Pause_us(50);
}

void BT_Write(char byte)
{
    while (!(BTIF_LSR & MASK_LSR_TEMP)) // 1 is empty
        ;
    BTIF_THR = byte;
}

void BT_Init(int on_off, void (*bt_irq_handler)(void))
{
    BT_Power(on_off);
    //  GPIO_Setup(31, GPMODE(4));
    //  GPIO_Setup(32, GPMODE(4));
    BTIF_SLEEP_EN = 1;
    BTIF_IER = 1; // Rx
    NVIC_RegisterIRQ(IRQ_BTIF_CODE, bt_irq_handler, IRQ_SENS_EDGE, true);
    printf("[Bluetooth] Begin\n");
}

/*
[HCI] opcode: 0x0C03  01 03 0C 00 .04.0E.04.01.03.0C.00                         result: 0x0
[HCI] opcode: 0x1001  01 01 10 00.04.0E.0C.01.01.10.00.06.00.00.06.46.00.00.00  result: 0x0
[HCI] opcode: 0x0C01  01 01 0C 08 FF FF FF FF FF FF FF 3F.04.0E.04.01.01.0C.00  result: 0x0
[HCI] opcode: 0x2002  01 02 20 00.04.0E.07.01.02.20.0C.00.00.00                 result: 0xC
*/


//https://github.com/bluekitchen/btstack