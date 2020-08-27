/*
* This file is part of the DZ09 project.
*
* Copyright (C) 2019 AJScorp
*
* This program is free software; you can redistribute it and/or modify 
* it under the terms of the GNU General Public License as published by 
* the Free Software Foundation; version 2 of the License.
*
* This program is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with this program; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. 
*/
#ifndef _MT6261_H_
#define _MT6261_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <hal_config.h>

#define MT6261

#define CONFIG_base                     0xA0010000   // Configuration Registers(Clock, Power Down, Version and Reset)
#define GPIO_Base                       0xA0020000   // General Purpose Inputs/Outputs
#define RGU_Base                        0xA0030000   // Reset generation unit
#define CIRQ_base                       0xA0060000   // Interrupt Controller
#define DMA_base                        0xA0070000   // DMA
#define DMA_AHB_base                    0xA0920000 // DMA
#define USART1_base                     0xA0080000   // USART 1
#define USART2_base                     0xA0090000   // USART 2
#define USART3_base                     0xA00A0000   // USART 3 add for MT6261
#define BTIF_base                       0xA00B0000   // BT interface
#define GPT_base                        0xA00C0000   // General Purpose Timers
#define KP_base                         0xA00D0000 // Keypad Scanner
#define PWM_base                        0xA00E0000 // Pulse-Width Modulation Outputs
#define I2C_base                        0xA0120000 // I2C
#define MSDC1_base                      0xA0130000 //
#define SFI_base                        0xA0140000 // Serial flash interface
#define MIXED_base                      0xA0170000   // Analog Chip Interface Controller (PLL, CLKSQ, FH, CLKSW and SIMLS)
#define PLL_base                        MIXED_base
#define TOPSM_base                      0xA0180000   // TOPSM0
#define MCU_TOPSM_base                  TOPSM_base
#define SPI_base                        0xA01E0000 // SPI
#define OSTIMER_base                    0xA01F0000   // OsTimer
#define MSDC2_base                      0xA0270000 // MS/SD Controller 2 
#define PWM2_base                       0xA0280000 // Pulse-Width Modulation Outputs
#define LCDIF_Base                      0xA0450000   // LCD controller
#define ARM_CONFG_base                  0xA0500000
#define BOOT_ENG_base                   0xA0510000   // Boot engine
#define CACHE_Base                      0xA0530000   // TCM Cache configuration
#define MPU_Base                        0xA0540000   // Memory protection unit
#define PMU_base                        0xA0700000   // PMU mixedsys
#define RTC_Base                        0xA0710000   // Real Time Clock
#define ABBSYS_base                     0xA0720000 // Analog baseband (ABB) controller
#define ANA_CFGSYS_base                 0xA0730000   // Analog die (MT6100) Configuration Registers (Clock, Reset, etc.)
#define ADIE_CIRQ_base                  0xA0760000   // Interrupt Controller (16-bit)
#define AUXADC_base                     0xA0790000   // Auxiliary ADC Unit
#define USB_base                        0xA0900000   // USB Controller

/* MODEMSYS APB */
#define IDMA_base                       0x82000000 // [32] APBADDR32 
#define DPRAM_CPU_base                  0x82200000 // [16]
#define AHB2DSPIO_base                  0x82800000    
#define MD2GCONFG_base                  0x82C00000 // [32] APBADDR32  
#define MD2G_MBIST_CONFG_base           0x82C10000    
#define APC_base                        0x82C30000    
#define CSD_ACC_base                    0x82C70000    
#define SHARE_base                      0x82CA0000 // [16] APBADDR  
#define IRDMA_base                      0x82CB0000    
#define PATCH_base                      0x82CC0000 // [??]
#define AFE_base                        0x82CD0000 // [16] APBADDR
#define BFE_base                        0x82CE0000 

/* MODEMSYS_PERI */
#define MDCONFIG_base                   0x83000000 // [32] APBADDR32 
#define MODEM2G_TOPSM_base              0x83010000 // [32] APBADDR32
#define TDMA_base                       0x83020000 // [??] uint16 
#define SHAREG2_base                    0x83030000 // [16] APBADDR   
#define DIVIDER_base                    0x83040000 
#define FCS_base                        0x83050000 
#define GCU_base                        0x83060000 
#define BSI_base                        0x83070000 
#define BPI_base                        0x83080000 


#define IDMA_CM              0x82000000
#define IDMA_PM              0x82100000
#define IDMA_DM              0x82200000
#define IDMA_SM              0x82300000
#define PATCH                0x82CC0000
#define AFE                  0x82CD0000
#define SHARE_D1             0x82CA0000
#define MD2G_CONFG           0x82C00000
#define ABBSYS_BASE          0xA0720000 
#define CONFIG_BASE          0xA2010000 
#define MODEM2G_TOPSM_BASE   0x83010000



#define VU16                            volatile uint16_t
#define VU32                            volatile uint32_t

#include <hal/hal_types.h>
#include <hal/hal_utils.h>

#define HAL_SEED                        (*(volatile uint32_t *)(0x70004044)) // for rand

#define ReadAddr8(addr)                 (*(volatile uint8_t *)(addr))
#define RegAddr8                        ReadAddr8
#define WriteAddr8(addr, data)          (*(volatile uint8_t *)(addr)) = (uint8_t)(data)

#define ReadAddr16(addr)                (*(volatile uint16_t *)(addr))
#define RegAddr16                       ReadAddr16
#define WriteAddr16(addr, data)         (*(volatile uint16_t *)(addr)) = (uint16_t)(data)

#define ReadAddr32(addr)                (*(volatile uint32_t *)(addr))
#define RegAddr32                       ReadAddr32
#define WriteAddr32(addr, data)         (*(volatile uint32_t *)(addr)) = (uint32_t)(data)

#define ReadReg8(reg)                   reg
#define WriteReg8(reg, data)            reg = (uint8_t)(data)

#define ReadReg16(reg)                  reg
#define WriteReg16(reg, data)           reg = (uint16_t)(data)

#define ReadReg32(reg)                  reg
#define WriteReg32(reg, data)           reg = (uint32_t)(data)

#define ARM_MEMORY(addr)                (*(volatile uint32_t*)(addr))
#define DSP_MEMORY(addr)                (*(volatile uint16_t*)(addr))

#define REG_32(addr)                    (*(volatile uint32_t*)(addr))
#define REG_16(addr)                    (*(volatile uint16_t*)(addr))


#define LOG_FUNC()                      printf("[F] %s()\n", __FUNCTION__)
#define LOG_END()                       printf("[F] %s( - )\n", __FUNCTION__)
#define MISSING()                       printf("<<< MISSING >>> %s()\n", __FUNCTION__)

#ifdef __cplusplus
}
#endif
#endif /* _MT6261_H_ */
