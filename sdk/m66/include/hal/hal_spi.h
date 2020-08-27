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

#ifndef _HAL_SPI_H_
#define _HAL_SPI_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include "hal.h"

#define SPI_FREQ        (32000) /* kHz */
#define SPI_FIFO_SIZE   (32)    /* 8 * ints */

////////////////////////////////////////////////////////////////////////////////////

#define SPI_CONF0_ADR                   (SPI_base + 0x0)
#define SPI_CONF0_REG                   (*((uint32_t volatile *)((SPI_base) + 0x0)))
    typedef struct
    {
        union {
            struct
            {
                unsigned TIME_HIGH  : 8;    
                unsigned TIME_LOW   : 8;        
                unsigned TIME_HOLD  : 8;                                    
                unsigned TIME_SETUP : 8; 
            };
            uint32_t reg;
        };
    } SPI_CONF0_T;
#define SPI_CONF0 (*((SPI_CONF0_T volatile *)(SPI_CONF0_ADR)))   

////////////////////////////////////////////////////////////////////////////////////
#define SPI_CONF1_ADR                   (SPI_base + 0x4)
#define SPI_CONF1_REG                   (*((uint32_t volatile *)((SPI_base) + 0x4)))
    typedef struct
    {
        union {
            struct
            {
                unsigned CS_IDLE_COUNT   : 8;
                unsigned PACKET_LOOP_CNT : 8;
                unsigned PACKET_LENGTH   : 10;
                unsigned DEVICE_SEL      : 1;
                unsigned reserved        : 2;
                unsigned GET_TICK_DLY    : 3;
            };
            uint32_t reg;
        };
    } SPI_CONF1_T;
#define SPI_CONF1                       (*((SPI_CONF1_T volatile *)(SPI_CONF1_ADR))) 

////////////////////////////////////////////////////////////////////////////////////

#define SPI_TX_ADDR_REG                 (*((uint32_t volatile *)((SPI_base) + 0x8)))
#define SPI_RX_ADDR_REG                 (*((uint32_t volatile *)((SPI_base) + 0xC)))
#define SPI_TX_FIFO_REG_ADDR            ((uint32_t volatile *)((SPI_base) + 0x10))
#define SPI_TX_FIFO_REG                 (*SPI_TX_FIFO_REG_ADDR)
#define SPI_RX_FIFO_REG_ADDR            ((uint32_t volatile *)((SPI_base) + 0x14))
#define SPI_RX_FIFO_REG                 (*SPI_RX_FIFO_REG_ADDR)

////////////////////////////////////////////////////////////////////////////////////

#define SPI_COMM_ADR                    (SPI_base + 0x18)
#define SPI_COMM_REG                    (*((uint32_t volatile *)((SPI_base) + 0x18)))
    typedef enum SPI_COMM_REG_BIT_POS
    {
        SPI_COMM_BIT_ACT            = 0,  //Command activate bit. Write 1 to this bit to trigger the SPI controller to start the transaction.
        SPI_COMM_BIT_RESUME         = 1,
        SPI_COMM_BIT_RESET          = 2,
        SPI_COMM_BIT_PAUSE_EN       = 4,
        SPI_COMM_BIT_CS_DEASSERT_EN = 5,
        SPI_COMM_BIT_SAMPLE_SEL     = 6,////Control bit of sample edge of miso,     0: Positive edge
        SPI_COMM_BIT_CS_POL         = 7,////Control bit of chip select polarity,    0: Active low
        SPI_COMM_BIT_CPHA           = 8,  //Defines the SPI clock format 0 or SPI clock format 1 during transmission
        SPI_COMM_BIT_CPOL           = 9,  //Control bit of the SCK polarity.
        SPI_COMM_BIT_RX_DMA_EN      = 10,
        SPI_COMM_BIT_TX_DMA_EN      = 11,
        SPI_COMM_BIT_TX_MSBF        = 12,
        SPI_COMM_BIT_RX_MSBF        = 13,
        SPI_COMM_BIT_RX_ENDIAN      = 14,
        SPI_COMM_BIT_TX_ENDIAN      = 15,
        SPI_COMM_BIT_FINISH_IE      = 16,
        SPI_COMM_BIT_PAUSE_IE       = 17
    }SPI_COMM_REG_BIT_POS;

    typedef enum SPI_COMM_MASK
    {
        SPI_COMM_MASK_ACT               = (1<<0),
        SPI_COMM_MASK_RESUME            = (1<<1),
        SPI_COMM_MASK_RESET             = (1<<2),
        SPI_COMM_MASK_PAUSE_EN          = (1<<4),
        SPI_COMM_MASK_CS_DEASSERT_EN    = (1<<5),
        SPI_COMM_MASK_SAMPLE_SEL        = (1<<6),        
        SPI_COMM_MASK_CS_POL            = (1<<7),            
        SPI_COMM_MASK_CPHA              = (1<<8),              
        SPI_COMM_MASK_CPOL              = (1<<9),              
        SPI_COMM_MASK_RX_DMA_EN         = (1<<10),
        SPI_COMM_MASK_TX_DMA_EN         = (1<<11),
        SPI_COMM_MASK_TX_MSBF           = (1<<12),
        SPI_COMM_MASK_RX_MSBF           = (1<<13),
        SPI_COMM_MASK_RX_ENDIAN         = (1<<14),
        SPI_COMM_MASK_TX_ENDIAN         = (1<<15),
        SPI_COMM_MASK_FINISH_IE         = (1<<16),
        SPI_COMM_MASK_PAUSE_IE          = (1<<17)
    }SPI_COMM_MASK; 

    typedef struct
    {
        union {
            struct
            {
                unsigned ACT               :1; 
                unsigned RESUME            :1;
                unsigned RESET             :1;
                unsigned reserve           :1; // !!!
                unsigned PAUSE_EN          :1;
                unsigned CS_DEASSERT_EN    :1;
                unsigned SAMPLE_SEL        :1;
                unsigned CS_POL            :1;
                unsigned CPHA              :1;
                unsigned CPOL              :1;
                unsigned DMA_EN_RX         :1;
                unsigned DMA_EN_TX         :1;
                unsigned TX_MSBF           :1;
                unsigned RX_MSBF           :1;
                unsigned RX_ENDIAN         :1;
                unsigned TX_ENDIAN         :1;
                unsigned FINISH_IE         :1;
                unsigned PAUSE_IE          :1;
            };
            uint32_t reg;
        };
    } SPI_COMM_T;
#define SPI_COMM (*((SPI_COMM_T volatile *)(SPI_COMM_ADR))) 

////////////////////////////////////////////////////////////////////////////////////

#define SPI_STATUS1_REG                 (*((uint32_t volatile *)((SPI_base) + 0x1C)))
    typedef enum SPI_STATUS1_BIT
    {
        SPI_STATUS1_BIT_FINISH = (1 << 0),
        SPI_STATUS1_BIT_PAUSE = (1 << 1)
    } SPI_STATUS1_BIT;

    ////////////////////////////////////////////////////////////////////////////////////

#define SPI_STATUS2_REG                 (*((uint32_t volatile *)((SPI_base) + 0x20)))

    enum SPI_STATUS2_BIT
    {
        SPI_STATUS2_BIT_BUSY = (1 << 0)
    };
    typedef enum SPI_STATUS2_BIT SPI_STATUS2_BIT;

////////////////////////////////////////////////////////////////////////////////////

#define SPI_GMC_SLOW_DOWN_REG           (*((uint32_t volatile *)((SPI_base) + 0x24)))
#define SPI_ULTRA_HIGH_PRIORITY_REG     (*((uint32_t volatile *)((SPI_base) + 0x28)))
#define SPI_PAD_MACRO_SELECT_REG        (*((uint32_t volatile *)((SPI_base) + 0x2C)))

////////////////////////////////////////////////////////////////////////////////////

    typedef enum SPI_STATUS_REG
    {
        SPI_STATUS_REG_1,
        SPI_STATUS_REG_2
    }SPI_STATUS_REG;

    typedef enum
    {
        SPI_GMC_SLOW_DOWN_ENABLE = 0,
        SPI_GMC_SPLIT_BURST_ENABLE = 4
    } SPI_GMC_SLOW_DOWN_REG_BIT_POS;

    typedef enum
    {
        SPI_ULTRA_HIGH_ENABLE = 0
    } SPI_ULTRA_HIGH_PRIORITY_REG_BIT_POS;

    typedef enum
    {
        SPI_LSB = 0,
        SPI_MSB
    } SPI_MLSB_E;

    typedef enum
    {
        SPI_MODE_FIFO = 0,
        SPI_MODE_DMA
    } SPI_MODE_E;

    typedef enum
    {
        SPI_CPOL_0 = 0,
        SPI_CPOL_1
    } SPI_CPOL_E;

    typedef enum
    {
        SPI_CPHA_0 = 0,
        SPI_CPHA_1
    } SPI_CPHA_E;

    typedef enum
    {
        SPI_NO_GET_TICK_MODE,
        SPI_GET_TICK_MODE_DELAY_1,
        SPI_GET_TICK_MODE_DELAY_2
    } SPI_GET_TICK_MODE_E;

    typedef enum
    {
        SPI_ENDIAN_LITTLE = 0,
        SPI_ENDIAN_BIG = 1
    } SPI_ENDIAN_E;

    typedef struct 
    {
        uint8_t setup_time;            // Range(0-255) = (T + 1) * CLK_PERIOD ?
        uint8_t hold_time;             // Range(0-255) = (T + 1) * CLK_PERIOD
        uint8_t clk_low;               // Range(0-255) = (T + 1) * CLK_PERIOD
        uint8_t clk_high;              // Range(0-255) = (T + 1) * CLK_PERIOD
        uint8_t idle_time;             // Range(0-255) = (T + 1) * CLK_PERIOD
        bool enable_pause_int;         //
        bool enable_finish_int;        //
        bool enable_pause_mode;        //
        bool enable_deassert_mode;     //
        SPI_MLSB_E tx_mlsb;            //
        SPI_MLSB_E rx_mlsb;            //
        SPI_MODE_E tx_mode;            // DMA / FIFO
        SPI_MODE_E rx_mode;            // DMA / FIFO
        SPI_CPOL_E clk_polarity;       //
        SPI_CPHA_E clk_format;         //
    #if 0
        SPI_GET_TICK_MODE_E get_tick;  //
        bool enable_ultra_high;        //
        uint16_t ultra_high_thresh;    //
        bool enable_gmc_slow_down;     //
        bool enable_gmc_split_burst;   //
        uint16_t gmc_slow_down_thresh; //
    #endif
    } spi_attr;

    typedef struct
    {
        uint32_t id;
        spi_attr attr;
    } spi_context;


spi_context *SPI_Create(void);
int SPI_Open(spi_context *ctx);
int SPI_Close(spi_context *ctx);
int SPI_Config(spi_context *ctx);
int SPI_SetSpeed(spi_context *ctx, uint16_t kHz, bool update);
int SPI_Transfer(spi_context *ctx, uint8_t *wr_buf, uint8_t *rd_buf, uint16_t size);

#ifdef __cplusplus
}
#endif
#endif /* _HAL_SPI_H_ */
