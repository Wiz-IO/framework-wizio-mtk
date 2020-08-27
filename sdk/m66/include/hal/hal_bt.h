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

#ifndef _BT_H_
#define _BT_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <hal/hal.h>

//#define BTIF_base             (0xA00B0000)
#define BTIF_RBR                (*(volatile uint16_t *)(BTIF_base + 0x0000)) // RX
#define BTIF_THR                (*(volatile uint16_t *)(BTIF_base + 0x0000)) // TX
#define BTIF_IER                (*(volatile uint16_t *)(BTIF_base + 0x0004))
#define BTIF_IIR                (*(volatile uint16_t *)(BTIF_base + 0x0008))
#define BTIF_FIFOCTRL           (*(volatile uint16_t *)(BTIF_base + 0x0008))
#define BTIF_FAKELCR            (*(volatile uint16_t *)(BTIF_base + 0x000C))
#define BTIF_LSR                (*(volatile uint16_t *)(BTIF_base + 0x0014))
#define BTIF_SLEEP_EN           (*(volatile uint16_t *)(BTIF_base + 0x0048))
#define BTIF_DMA_EN             (*(volatile uint16_t *)(BTIF_base + 0x004C))
#define BTIF_RTOCNT             (*(volatile uint16_t *)(BTIF_base + 0x0054))
#define BTIF_TRI_LVL            (*(volatile uint16_t *)(BTIF_base + 0x0060))
#define BTIF_WAK                (*(volatile uint16_t *)(BTIF_base + 0x0064))
#define BTIF_WAT_TIME           (*(volatile uint16_t *)(BTIF_base + 0x0068))
#define BTIF_HANDSHAKE          (*(volatile uint16_t *)(BTIF_base + 0x006C))

/*0x14, 0x60 at reset*/
#define MASK_LSR_DR             (1<<0) /*1: Rx full*/
#define MASK_LSR_THRE           (1<<5) 
#define MASK_LSR_TEMP           (1<<6) /*1: Tx holding register is empty*/


#define TOPSM_RM_PWR_CON1       (*(volatile uint32_t *)(TOPSM_base + 0x0804))
#define TOPSM_PWR_ON            (0x00000004)     
#define TOPSM_PWR_REQ_EN        (0x00000040) 
#define TOPSM_PWR_RESET         (0x00002000) 

void BT_Power(int on_off);
char BT_Read(void);
void BT_Write(char byte);
void BT_Init(int on_off, void (*bt_irq_handler)(void));


#ifdef __cplusplus
}
#endif
#endif