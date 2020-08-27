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

#ifndef _DMA_HW_H
#define _DMA_HW_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <hal/hal.h>
#include <hal/hal_pctl.h>

#define DMA_MAX_CHANNEL             (8)
#define DMA_MAX_FULL_CHANNEL        (1)
#define DMA_FULL_CHANNEL_MASK       (0x00001)

#define DMA_CON_MASTER_RESERVED_0   (0)
#define DMA_CON_MASTER_RESERVED_1   (1)
#define DMA_CON_MASTER_MSDC         (2)
#define DMA_CON_MASTER_SIM          (3)
#define DMA_CON_MASTER_SIM2         (4)
#define DMA_CON_MASTER_USB1TX       (5)
#define DMA_CON_MASTER_USB1RX       (6)
#define DMA_CON_MASTER_USB2TX       (7)
#define DMA_CON_MASTER_MSDC2        (8)
#define DMA_CON_MASTER_UART1TX      (9)
#define DMA_CON_MASTER_UART1RX      (10)
#define DMA_CON_MASTER_UART2TX      (11)
#define DMA_CON_MASTER_UART2RX      (12)
#define DMA_CON_MASTER_UART3TX      (13)
#define DMA_CON_MASTER_UART3RX      (14)
#define DMA_CON_MASTER_BTIFTX       (15)
#define DMA_CON_MASTER_BTIFRX       (16)
#define DMA_CON_MASTER_RESERVED_17  (17)

#define DMA_VFIFO_CH_S               (9)  // start  of DMA Virtual fifo
#define DMA_VFIFO_CH_E               (16) // end of DMA Virtual fifo

typedef enum {
    DMA_RESERVED_0,  // 0
    DMA_RESERVED_1,  // 1
    DMA_MSDC,        // 2
    DMA_SIM,         // 3
    DMA_SIM2,        // 4
    DMA_USB1TX,      // 5
    DMA_USB1RX,      // 6
    DMA_USB2TX,      // 7
    DMA_MSDC2,       // 8
    DMA_UART1TX,     // 9
    DMA_UART1RX,     // 10
    DMA_UART2TX,     // 11
    DMA_UART2RX,     // 12
    DMA_UART3TX,     // 13
    DMA_UART3RX,     // 14
    DMA_BTIFRX,      // 15
    DMA_BTIFTX,      // 16
        DMA_SW,
        DMA_H264,
        DMA_MPEG4,
        DMA_L1_R2BC,
        DMA_TD_L1,
        DMA_IDLE
} DMA_MASTER;

typedef enum {
    VDMA_UART1TX_CH  = 9,
    VDMA_UART1RX_CH  = 10,
    VDMA_UART2TX_CH  = 11,
    VDMA_UART2RX_CH  = 12,
    VDMA_UART3TX_CH  = 13,
    VDMA_UART3RX_CH  = 14,
    VDMA_BTIFRX_CH   = 15,
    VDMA_BTIFTX_CH   = 16,
} DMA_VFIFO_UART;

#define DMA_GLBSTA                  (*(volatile uint32_t*)(DMA_base+0x0000))
#define DMA_GLBSTA2                 (*(volatile uint32_t*)(DMA_base+0x0004))
#define DMA_SRC(_n)                 (*(volatile uint32_t*)(DMA_base+(0x0100*(_n))))            /* _n = 1~3 */
#define DMA_DST(_n)                 (*(volatile uint32_t*)(DMA_base+0x0004+(0x0100*(_n))))     /* _n = 1~3 */
#define DMA_WPPT(_n)                (*(volatile uint32_t*)(DMA_base+0x0008+(0x0100*(_n))))     /* _n = 1~11 */
#define DMA_WPTO(_n)                (*(volatile uint32_t*)(DMA_base+0x000c+(0x0100*(_n))))     /* _n = 1~11 */
#define DMA_COUNT(_n)               (*(volatile uint32_t*)(DMA_base+0x0010+(0x0100*(_n))))     /* _n = 1~11 */
#define DMA_CON(_n)                 (*(volatile uint32_t*)(DMA_base+0x0014+(0x0100*(_n))))     /* _n = 1~11 */
#define DMA_START(_n)               (*(volatile uint32_t*)(DMA_base+0x0018+(0x0100*(_n))))     /* _n = 1~11 */
#define DMA_INTSTA(_n)              (*(volatile uint32_t*)(DMA_base+0x001c+(0x0100*(_n))))     /* _n = 1~11 */
#define DMA_ACKINT(_n)              (*(volatile uint32_t*)(DMA_base+0x0020+(0x0100*(_n))))     /* _n = 1~11 */
#define DMA_RLCT(_n)                (*(volatile uint32_t*)(DMA_base+0x0024+(0x0100*(_n))))     /* _n = 1~11 */
#define DMA_LIMITER(_n)             (*(volatile uint32_t*)(DMA_base+0x0028+(0x0100*(_n))))     /* _n = 1~11 */
#define DMA_PGMADDR(_n)             (*(volatile uint32_t*)(DMA_base+0x002c+(0x0100*(_n))))     /* _n = 4~11 */

#define DMA_VPORT_BASE              (DMA_AHB_base)
#define DMA_VPORT(_n)               (*(volatile uint32_t*)(DMA_VPORT_BASE+0x0100*(_n-DMA_VFIFO_CH_S))) // _n = 10~13 *
#define DMA_WRPTR(_n)               (*(volatile uint32_t*)(DMA_base+0x0030+(0x0100*(_n))))             // _n = 10~13 *
#define DMA_RDPTR(_n)               (*(volatile uint32_t*)(DMA_base+0x0034+(0x0100*(_n))))             // _n = 10~13 *
#define DMA_FFCNT(_n)               (*(volatile uint32_t*)(DMA_base+0x0038+(0x0100*(_n))))             // _n = 10~13 * the number of data stored in Virtual FIFO
#define DMA_FFSTA(_n)               (*(volatile uint32_t*)(DMA_base+0x003c+(0x0100*(_n))))             // _n = 10~13 *
#define DMA_ALTLEN(_n)              (*(volatile uint32_t*)(DMA_base+0x0040+(0x0100*(_n))))             // _n = 10~13 *
#define DMA_FFSIZE(_n)              (*(volatile uint32_t*)(DMA_base+0x0044+(0x0100*(_n))))             // _n = 10~13 *

#define DMA_GLBSTA_RUN(_n)          (0x00000001 << (2*((_n)-1)))        /* _n = 1~11 */
#define DMA_GLBSTA_IT(_n)           (0x00000002 << (2*((_n)-1)))        /* _n = 1~11 */

#define DMA_COUNT_MASK              0xffff

#define DMA_CON_SIZE_MASK           0x00000003
#define DMA_CON_SIZE_BYTE           0x00000000
#define DMA_CON_SIZE_SHORT          0x00000001
#define DMA_CON_SIZE_LONG           0x00000002
#define DMA_CON_SINC                0x00000004
#define DMA_CON_DINC                0x00000008
#define DMA_CON_DRQ                 0x00000010  /* 1:hw, 0:sw */
#define DMA_CON_ITEN                0x00008000  /* Interrupt enable */
#define DMA_CON_WPSD                0x00010000  /* 0:at source, 1: at destination */
#define DMA_CON_WPEN                0x00020000  /* 0:disable, 1: enable */
#define DMA_CON_DIR                 0x00040000  /* Only valid when dma = 4~11 */
#define DMA_CON_RXCONTRL            (DMA_CON_DIR | DMA_CON_DINC)
#define DMA_CON_TXCONTRL            (DMA_CON_SINC)
#define DMA_CON_SWCOPY              (DMA_CON_SINC | DMA_CON_DINC)
#define DMA_CON_B2W                 0x00000020  /*  word to byte or byte to word */
#define DMA_CON_BURST_SINGLE        0x00000000  /*  without burst mode */
#define DMA_CON_BURST_4BEAT         0x00000200  /*  4-beat incrementing burst */
#define DMA_CON_BURST_8BEAT         0x00000200  /*  4-beat incrementing burst */
#define DMA_CON_BURST_16BEAT        0x00000200  /*  4-beat incrementing burst */
#define DMA_CON_MASTER_MASK         0x01f00000  /* Only valid when dma = 4~11 */

#define DMA_START_BIT               0x00008000
#define DMA_STOP_BIT                0x00000000
#define DMA_ACKINT_BIT              0x00008000
#define DMA_INTSTA_BIT              0x00008000
#define DMA_RLCT_MASK               0x0000ffff
#define DMA_LIMITER_MASK            0x000000ff

#define DMA_FFSTA_FULL              0x0001
#define DMA_FFSTA_EMPTY             0x0002
#define DMA_FFSTA_ALERT             0x0004

#define DMA_POP(_n)                 (*(volatile kal_uint8*)DMA_VPORT(_n))
#define DMA_PUSH(_n,_data)          (*(volatile kal_uint8*)DMA_VPORT(_n)) = (kal_uint8)_data
#define DMA_GetVFIFO_Avail(ch)      DMA_FFCNT(ch)
#define DMA_GetVFIFO_Roomleft(ch)   DMA_FFSIZE(ch) - DMA_GetVFIFO_Avail(ch)
#define IS_VFIFO_FULL(ch)           DMA_FFSTA(ch)) & DMA_FFSTA_FULL
#define DMA_EnableINT(ch)           DMA_CON(ch)) |= DMA_CON_ITEN
#define DMA_DisableINT(ch)          DMA_CON(ch)) &= ~DMA_CON_ITEN

#define DMA_TASK_PUSH(ch,data)      while(IS_VFIFO_FULL(ch)); \
                                        DMA_PUSH(ch,data)

#define DMA_TX_TRIGGER_LEVEL(n)     ((n)/8)     // trigger level of tx vfifo
#define DMA_RX_TRIGGER_LEVEL(n)     ((n)*3/4)   // trigger level of rx vfifo
#define DMA_VIFOF_ALERT_LENGTH      (0x3f)      // flow control trigger level

#define DMA_POWER_ON()              PCTL_PowerUp(PD_DMA)
//#define DMA_SLEEP_ENABLE()          PDN_SET(PDN_DMA)
//#define DMA_SLEEP_DISABLE()         PDN_CLR(PDN_DMA)


#ifdef __cplusplus
}
#endif
#endif //_DMA_HW_H