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

#ifndef _OST_H_
#define _OST_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <hal/hal.h>

#define OST_KEY                0x11530000

#define OST_MAX_WAIT_TIME      600

#define OST_CON                (*(volatile uint32_t*)(OSTIMER_base+0x00))  /* OS Timer Control Register */
#define OST_CMD                (*(volatile uint32_t*)(OSTIMER_base+0x04))  /* OS Timer Command Register 0x1153-0000 */
#define OST_STA                (*(volatile uint32_t*)(OSTIMER_base+0x08))  /* OS Timer Command Status Register */
#define OST_FRM                (*(volatile uint32_t*)(OSTIMER_base+0x0C))  /* OS Timer Frame Duration [12:0] */
#define OST_FRM_F32K           (*(volatile uint32_t*)(OSTIMER_base+0x10))  /* OS Timer Frame Duration by 32K clock [8:0] */
#define OST_UFN                (*(volatile uint32_t*)(OSTIMER_base+0x14))  /* OS Timer Un-alignment Frame Number [31:0] */
#define OST_AFN                (*(volatile uint32_t*)(OSTIMER_base+0x18))  /* OS Timer Alignment Frame Number [31:0] */
#define OST_AFN_DLY            (*(volatile uint32_t*)(OSTIMER_base+0x1C))  /* OS Timer Alignment Frame Delay Number [31:0]*/
#define OST_UFN_R              (*(volatile uint32_t*)(OSTIMER_base+0x20))  /* Current OS Timer Un-alignement Frame Number [31:0] */
#define OST_AFN_R              (*(volatile uint32_t*)(OSTIMER_base+0x24))  /* Current OS Timer Alignement Frame Number [31:0] */
#define OST_INT_MASK           (*(volatile uint32_t*)(OSTIMER_base+0x30))  /* OS Timer Interrupt Mask (All default masked: disabled) */
#define OST_ISR                (*(volatile uint32_t*)(OSTIMER_base+0x40))  /* OS Timer Interrupt Status */
#define OST_EVENT_MASK         (*(volatile uint32_t*)(OSTIMER_base+0x50))  /* OS Timer Event Mask (All default enabled) */
#define OST_WAKEUP_STA         (*(volatile uint32_t*)(OSTIMER_base+0x54))  /* OS Timer Event Wakeup Status [auto-clear with source]: record the first wakeup sources       */
#define OST_DBG_WAKEUP         (*(volatile uint32_t*)(OSTIMER_base+0x60))  /* OS Timer Debug Wakeup */

#define TOPSM_OST_PWR_CON      (*(volatile uint32_t *)(TOPSM_base+0x0800))

void OST_Init(void);
int OST_SetFrameDuration(uint16_t frm_dur);
int OST_Enable(uint32_t frm);
void OST_Begin(void);

uint32_t OST_GetUFN(void);
uint32_t OST_GetAFN(void);

#ifdef __cplusplus
}
#endif
#endif /* _OST_H_ */
