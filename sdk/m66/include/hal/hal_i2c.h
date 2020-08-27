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

#ifndef _HAL_I2C_H_
#define _HAL_I2C_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <hal/hal.h>

#define I2C_CLOCK               (13000)

#define I2C_DATA_PORT_ADR       (I2C_base + 0x0000)
#define I2C_DATA_PORT_REG       (*(volatile uint16_t *)(I2C_DATA_PORT_ADR))

#define I2C_SLAVE_ADR          (I2C_base + 0x0004)
#define I2C_SLAVE_REG          (*(volatile uint16_t *)(I2C_SLAVE_ADR))
#define I2C_SLAVE_RW                        (1 << 0)
typedef struct {
    union {
        struct
        {
                unsigned RW     : 1;    
                unsigned SLAVE  : 7;        
        };
        uint16_t reg;
    };
} I2C_ADDRESS_T;
#define I2C_ADDRESS (*((I2C_ADDRESS_T volatile *)(I2C_SLAVE_ADR)))

#define I2C_INT_MASK_ADR            (I2C_base + 0x0008)
#define I2C_INT_MASK_REG            (*(volatile uint16_t *)(I2C_INT_MASK_ADR))
#define I2C_INT_MASK_TRANSAC_COMP           (1 << 0)
#define I2C_INT_MASK_ACKERR                 (1 << 1)
#define I2C_INT_MASK_HS_NACK_ERR            (1 << 2)
typedef struct {
    union {
        struct
        {
                unsigned TRANSAC_COMP   : 1;    
                unsigned ACKERR         : 1;      
                unsigned HS_NACK_ERR    : 1;                   
        };
        uint16_t reg;
    };
} I2C_INT_MASK_T;
#define I2C_INT_MASK (*((I2C_INT_MASK_T volatile *)(I2C_INT_MASK_ADR)))

#define I2C_INT_STAT_ADR                (I2C_base + 0x000C)
#define I2C_INT_STAT_REG                (*(volatile uint16_t *)I2C_INT_STAT_ADR)
#define I2C_INT_STAT_TRANSAC_COMP           (1 << 0)
#define I2C_INT_STAT_ACKERR                 (1 << 1)
#define I2C_INT_STAT_HS_NACKERR             (1 << 2)
#define I2C_INT_STAT_TIMEOUT                (1 << 4)

#define I2C_CONTROL_ADR             (I2C_base + 0x0010)
#define I2C_CONTROL_REG             (*(volatile uint16_t *)I2C_CONTROL_ADR)
#define  I2C_CTL_MODE_BIT                   (1 << 0) // 01 NOT USED
#define I2C_CONTROL_RS_STOP                 (1 << 1) // 02
#define I2C_CONTROL_DMA_EN                  (1 << 2) // 04
#define I2C_CONTROL_CLKEXT_EN               (1 << 3) // 08
#define I2C_CONTROL_DIR_CHANGE              (1 << 4) // 10
#define I2C_CONTROL_ACKERR_DET_EN           (1 << 5) // 20
#define I2C_CONTROL_TRANCFER_LEN_CHANGE     (1 << 6) // 40

typedef struct {
    union {
        struct
        {
                unsigned CTL_MODE_BIT           : 1; // 0 01    CTL_MODE_BIT NOT USED  
                unsigned RS_STOP                : 1; // 1 02    1: REPEATED-START      
                unsigned DMA_EN                 : 1; // 2 04
                unsigned CLK_EXT_EN             : 1; // 3 08       
                unsigned DIR_CHANGE             : 1; // 4 10 
                unsigned ACKERR_DET_EN          : 1; // 5 20
                unsigned TRANSFER_LEN_CHANGE    : 1; // 6 40
        };
        uint16_t reg;
    };
} I2C_CONTROL_T;
#define I2C_CONTROL (*((I2C_CONTROL_T volatile *)(I2C_CONTROL_ADR)))

#define I2C_TRANSFER_LEN_ADR        (I2C_base + 0x0014)
#define I2C_TRANSFER_LEN_REG        (*(volatile uint16_t *)I2C_TRANSFER_LEN_ADR) 

#define I2C_TRANSAC_LEN_ADR         (I2C_base + 0x0018)
#define I2C_TRANSAC_LEN_REG         (*(volatile uint16_t *)I2C_TRANSAC_LEN_ADR) 

#define I2C_DELAY_LEN_ADR           (I2C_base + 0x001C)
#define I2C_DELAY_LEN_REG           (*(volatile uint16_t *)I2C_DELAY_LEN_ADR) 

#define I2C_TIMING_ADR              (I2C_base + 0x0020)
#define I2C_TIMING_REG              (*(volatile uint16_t *)I2C_TIMING_ADR) 
typedef struct {
    union {
        struct
        {
                unsigned STEP_CNT_DIV    : 6;    
                unsigned reserve         : 2;      
                unsigned SAMPLE_CNT_DIV  : 3; // shift 8 [3]
                //other               
        };
        uint16_t reg;
    };
} I2C_TIMING_T;
#define I2C_TIMING (*((I2C_TIMING_T volatile *)(I2C_TIMING_ADR)))

#define I2C_START_REG           (*(volatile uint16_t *)(I2C_base + 0x0024))  
#define I2C_FIFO_STAT_REG       (*(volatile uint16_t *)(I2C_base + 0x0030)) // 
#define I2C_FIFO_THRESH_REG     (*(volatile uint16_t *)(I2C_base + 0x0034)) // 
#define I2C_FIFO_ADDR_CLR_REG   (*(volatile uint16_t *)(I2C_base + 0x0038)) // 
#define I2C_IO_CONFIG_REG       (*(volatile uint16_t *)(I2C_base + 0x0040)) // 3: open drain, 4: bus detect
#define I2C_MULTI_MASTER_REG    (*(volatile uint16_t *)(I2C_base + 0x0044)) // ARBITRATION_BIT, CLOCK_SYNC, BUS_BUSY_DET

#define I2C_HS_ADR              (I2C_base + 0x0048) 
#define I2C_HS_REG              (*(volatile uint16_t *)I2C_HS_ADR) 
typedef struct {
    union {
        struct
        {
            unsigned EN             : 1; //    
            unsigned NACKERR_DET_EN : 1; //     
            unsigned reserve_32     : 2; //  
            unsigned MASTER_CODE    : 3; //
            unsigned reserve_7      : 1; //
            unsigned STEP_CNT_DIV   : 3; // 
            unsigned reserve_11     : 1; //
            unsigned SAMPLE_CNT_DIV : 3; //     
        };
        uint16_t reg;
    };
} I2C_HS_T;
#define I2C_HS (*((I2C_HS_T volatile *)(I2C_HS_ADR)))

#define I2C_SOFTRESET_REG           (*(volatile uint16_t *)(I2C_base + 0x0050)) 
#define I2C_TRANSFER_LEN_AUX_REG    (*(volatile uint16_t *)(I2C_base + 0x006C)) 
#define I2C_TIMEOUT_REG             (*(volatile uint16_t *)(I2C_base + 0x0074)) 
#define REG_I2C_VERSION_REG         (*(volatile uint16_t *)(I2C_base + 0x0078))

    typedef enum
    {
        I2C_TRANSACTION_WRITE = 0x0,
        I2C_TRANSACTION_READ = 0x1,
        I2C_TRANSACTION_CONT_WRITE = 0x2,
        I2C_TRANSACTION_CONT_READ = 0x3,
        I2C_TRANSACTION_WRITE_AND_READ = 0x4,
    } I2C_TRANSACTION_TYPE;

    typedef enum
    {
        I2C_TRANSACTION_LO_MODE = 0x0,
        I2C_TRANSACTION_HI_MODE = 0x1,
    } I2C_TRANSACTION_MODE;

    typedef struct
    {
        uint8_t SlaveAddress;      //the 7 bits address of the slave device
        I2C_TRANSACTION_MODE Mode; //I2C_TRANSACTION_FAST_MODE or I2C_TRANSACTION_HIGH_SPEED_MODE
        uint16_t LoSpeed;          //The speed of I2C fast mode(Kb)
        uint16_t HiSpeed;          //The speed of I2C high speed mode(Kb)
        uint8_t Delay;             //number of half pulse between transfers in a trasaction
        uint8_t IO;
    } i2c_config_t;

    typedef struct
    {
        uint32_t id;
        i2c_config_t config;
        uint8_t fs_sample_cnt_div; // these two parameters are used to specify I2C clock rate
        uint8_t fs_step_cnt_div;   // half pulse width=step_cnt_div*sample_cnt_div*(1/13Mhz)
        uint8_t hs_sample_cnt_div; // these two parameters are used to specify I2C clock rate
        uint8_t hs_step_cnt_div;   // half pulse width=step_cnt_div*sample_cnt_div*(1/13Mhz)
        int transaction_result;    // The result of the end of transaction (I2C_TRANSACTION_COMPLETE|I2C_TRANSACTION_FAIL)
    } i2c_context_t;


    i2c_context_t *I2C_Create(void);
    int I2C_Open(i2c_context_t *ctx);
    int I2C_Close(i2c_context_t *ctx);

    void I2C_RecalculateSpeed(i2c_context_t *ctx);
    int I2C_UpdateSpeed(i2c_context_t *ctx, bool recalculate);
    int I2C_BeginTransaction(i2c_context_t *ctx, I2C_TRANSACTION_TYPE command,
                             uint8_t *write_buffer, uint32_t write_len,
                             uint8_t *read_buffer, uint32_t read_len);

#ifdef __cplusplus
}
#endif
#endif /* _HAL_I2C_H_ */
