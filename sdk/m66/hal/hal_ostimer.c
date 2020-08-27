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

#include <hal/hal_ostimer.h>
#include <hal/hal_pctl.h>
#include <hal/hal_nvic.h>

void OST_Init(void)
{
    TOPSM_OST_PWR_CON |= 0x200; // MEMORY[0xA0180800] = 0x200; default = 0x00002244
    CNFG_PDN_CON1_CLR = 0x0100; // MEMORY[0xA0010324] = 0x100; PD_ARM on
    OST_EVENT_MASK = 0;         // MEMORY[0xA01F0050] = 0x3EC81; ??? [18:0]
    OST_INT_MASK = 1;           // MEMORY[0xA01F0030] = 1; for time-out interrupt
    OST_ISR = 0x001F;           // MEMORY[0xA01F0040] = 0x1F; // clear all interrupt status [4:0]
}

int OST_SetFrameDuration(uint16_t frm_dur)
{
    if ((frm_dur < 1000) || (frm_dur > 8000))
        return -1;
    uint16_t ost_frm_num = 0;
    uint16_t tmp_frm_f32k = (frm_dur * 8) / 245; // 30.5176
    printf("tmp_frm_f32k = %d\n", (int)tmp_frm_f32k);
    uint16_t test = ((uint16_t)0xA4 + tmp_frm_f32k - 1) / (uint16_t)tmp_frm_f32k; // CLK_SETTLE(0xA0) + 4
    printf("test = %d\n", (int)test);
    if (test + 1 <= 2)
        ost_frm_num = 2;
    else
        ost_frm_num = test + 1;
    int result = (ost_frm_num << 12) + ost_frm_num * tmp_frm_f32k - 5;
    printf("OST_FRM_F32K = %d\n", result);
    OST_FRM_F32K = result;
    OST_FRM = frm_dur;
    return result;
}

static int OST_wait_status_ready(void)
{
    int cnt = 0;
    do
    {
        if (++cnt >= OST_MAX_WAIT_TIME)
        {
            //printf("[ERROR] OST_MAX_WAIT_TIME\n");
            return -1;
        }
    } while (!(OST_STA & 2)); // CMD_CPL
    //printf("[CMD_CPL] %d\n", cnt);
    return 0;
}

int OST_Enable(uint32_t frm)
{
    uint32_t con = OST_CON;
    con |= 0x2;    /* Always enable UFN down-count feature in ARM OS Timer Driver */
    con &= ~(0x4); /* Disable OST DBG mode */
    con |= 0x1;    /* Enable OS Timer */
    uint32_t flag = DisableInterrupts();
    OST_AFN = -1;  // max
    OST_UFN = frm; // irq interval
    OST_CON = con;
    OST_CMD = OST_KEY | 0xE004; // all regs - write
    OST_wait_status_ready();
    RestoreInterrupts(flag);
    return 0;
}

void OST_Begin(void) // 1ms
{
    OST_Init();
    OST_SetFrameDuration(1000);
    OST_Enable(1000);
}

uint32_t OST_GetUFN(void)
{
    OST_CMD = OST_KEY | 2;
    OST_wait_status_ready();
    return OST_UFN_R;
}

uint32_t OST_GetAFN(void)
{
    OST_CMD = OST_KEY | 2;
    OST_wait_status_ready();
    return OST_AFN_R;
}