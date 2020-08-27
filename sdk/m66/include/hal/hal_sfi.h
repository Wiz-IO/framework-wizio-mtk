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
#ifndef _SFI_H_
#define _SFI_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include "hal.h"

#define RW_SFI_MAC_CTL              (*(volatile uint32_t*)(SFI_base + 0x0000))
#define SFI_WIP                     (1 << 0)    //(0x00000001)   // b0
#define SFI_WIP_READY               (1 << 1)    //(0x00000002)   // b1
#define SFI_TRIG                    (1 << 2)    //(0x00000004)   // b2
#define SFI_MAC_EN                  (1 << 3)    //(0x00000008)   // b3
#define SFI_MAC_SIO_SEL             (1 << 4)    //(0x00000010)   // b4
#define SFI_RELEASE_MAC             (1 << 16)   //(0x00010000)   // b16
#define SFI_MAC_SEL                 (1 << 28)   //(0x10000000)   // b28
#define RW_SFI_DIRECT_CTL           (*(volatile uint32_t*)(SFI_base + 0x0004))
// RW_SF_DIRECT_CTL
#define SFI_QPI_EN                  (1 << 0)    //(0x00000001)  // b0
#define SFI_CMD1_EXTADDR_EN         (1 << 1)    //(0x00000002)  // b1
#define SFI_CMD2_EN                 (1 << 2)    //(0x00000004)  // b2
#define SFI_CMD2_EXTADDR_EN         (1 << 3)    //(0x00000008)  // b3
#define SFI_DR_MODE_MASK            (0x00000070)  // b4~b6
#define SFI_DR_CMD2_DUMMY_CYC_MASK  (0x00000F00)  // b8~b11
#define SFI_DR_CMD1_DUMMY_CYC_MASK  (0x0000F000)  // b12~b15
#define SFI_DR_CMD2_DUMMY_CYC_OFFSET         (8)
#define SFI_DR_CMD1_DUMMY_CYC_OFFSET        (12)
#define SFI_DR_CMD2_MASK            (0x00FF0000)  // b16~b23
#define SFI_DR_CMD1_MASK            (0xFF000000)  // b24~b31
#define SFI_DR_CMD2_OFFSET                  (16)
#define SFI_DR_CMD1_OFFSET                  (24)
// RW_SF_DIRECT_CTL: Serial flash direct read mode
#define SFI_DR_SPI_NORMAL_READ_MODE (0x00000000)
#define SFI_DR_SPI_FAST_READ_MODE   (0x00000010)
#define SFI_DR_QPI_FAST_READ_MODE   (0x00000070)

#define RW_SFI_MISC_CTL             (*(volatile uint32_t*)(SFI_base + 0x0008))
// RW_SF_MISC_CTL
#define SFI_DEL_LATCH_MASK          (0x00000003)  // b0~01
#define SFI_DEL_LATCH_OFFSET        (0)
#define SFI_REQ_IDLE                (0x00000100)  // b8
#define SFI_BOOT_REMAP              (0x00000200)  // b9
#define SFI_SYNC_EN                 (0x00020000)  // b17
#define SFI_NO_RELOAD               (0x00800000)  // b23
#define SFI_CS_EXT_MASK             (0xF0000000)  // b28~b31
#define RW_SFI_MISC_CTL2            (*(volatile uint32_t*)(SFI_base + 0x000C))
// RW_SF_MISC_CTL2
#define SFI_WRAP_ACC_EN             (0x00000001)  // b0
#define SFI_DDR                     (0x00000002)  // b1
#define RW_SFI_MAC_OUTL             (*(volatile uint32_t*)(SFI_base + 0x0010))
#define RW_SFI_MAC_INL              (*(volatile uint32_t*)(SFI_base + 0x0014))
#define RW_SFI_RESET_CTL            (*(volatile uint32_t*)(SFI_base + 0x0018))
#define RW_SFI_STA2_CTL             (*(volatile uint32_t*)(SFI_base + 0x001C))
// RW_SF_STA2_CTL
#define SFI_KEEP_READ_SETTING       (0x80000000)  // b31
#define RW_SFI_DLY_CTL1             (*(volatile uint32_t*)(SFI_base + 0x0020))
#define RW_SFI_DLY_CTL2             (*(volatile uint32_t*)(SFI_base + 0x0024))
#define RW_SFI_DLY_CTL3             (*(volatile uint32_t*)(SFI_base + 0x0028))
#define RW_SFI_DLY_CTL4             (*(volatile uint32_t*)(SFI_base + 0x0030))
#define RW_SFI_DLY_CTL5             (*(volatile uint32_t*)(SFI_base + 0x0034))
#define RW_SFI_DLY_CTL6             (*(volatile uint32_t*)(SFI_base + 0x0038))
#define RW_SFI_DIRECT_CTL2          (*(volatile uint32_t*)(SFI_base + 0x0040))
#define RW_SFI_MISC_CTL3            (*(volatile uint32_t*)(SFI_base + 0x0044))
// RW_SF_MISC_CTL3
#define SFI_DEL_LATCH_MASK2         (0x00030000)  // b16~017
#define SFI_DEL_LATCH_OFFSET2       (16)
#define SFI_SF2_EN                  (0x00000001)    // b0
#define SFI_SF1_SIZE_MASK           (0x00000070)    // b4~6
#define SFI_SF1_SIZE_OFFSET         (4)             // b4~6
#define SFI_CH1_TRANS_MASK          (0x00000100)    // b8
#define SFI_CH2_TRANS_MASK          (0x00000200)    // b9
#define SFI_CH3_TRANS_MASK          (0x00000400)    // b10
#define SFI_CH1_TRANS_IDLE          (0x00001000)    // b12
#define SFI_CH2_TRANS_IDLE          (0x00002000)    // b13
#define SFI_CH3_TRANS_IDLE          (0x00004000)    // b14


#define RW_SFI_GPRAM_DATA_ADR       (volatile uint32_t)(SFI_base + 0x0800)
#define RW_SFI_GPRAM_DATA           (*(volatile uint32_t*)(SFI_base + 0x0800))
#define RW_SFI_GPRAM_DATA_4         (*(volatile uint32_t*)(SFI_base + 0x0804))

// Performance Monitor
#define SF_PERF_MON1                (*(volatile uint32_t*)(SFI_base + 0x0080))
#define SF_PERF_MON2                (*(volatile uint32_t*)(SFI_base + 0x0084))
#define SF_PERF_MON3                (*(volatile uint32_t*)(SFI_base + 0x0088))
#define SF_PERF_MON4                (*(volatile uint32_t*)(SFI_base + 0x008C))
#define SF_PERF_MON5                (*(volatile uint32_t*)(SFI_base + 0x0090))
#define SF_PERF_MON6                (*(volatile uint32_t*)(SFI_base + 0x0094))
#define SF_PERF_MON7                (*(volatile uint32_t*)(SFI_base + 0x0098))
#define SF_PERF_MON8                (*(volatile uint32_t*)(SFI_base + 0x009C))
#define SF_PERF_MON9                (*(volatile uint32_t*)(SFI_base + 0x00A0))
#define SF_PERF_MON10               (*(volatile uint32_t*)(SFI_base + 0x00A4))
#define SF_PERF_MON11               (*(volatile uint32_t*)(SFI_base + 0x00A8))
#define SF_PERF_MON12               (*(volatile uint32_t*)(SFI_base + 0x00AC))
#define SF_PERF_MON13               (*(volatile uint32_t*)(SFI_base + 0x00B0))

#define SF_PROGRAM_BYTE_SIZE        (128)
#define SF_FLASH_ID_LENGTH          (3) 

typedef union {
    uint32_t   u32;
    uint16_t   u16[2];
    uint8_t    u8[4];
} sf_uint;

typedef enum {
    sf_fix_winbond_cv   =0x00000001,
    sf_fix_winbond_dw   =0x00000002,
    sf_fix_mxic_35e     =0x00000004,
    sf_fix_fidelix_q32a =0x00000008,
    sf_fix_gigadevice_b =0x00000010
} Enum_SF_FIX;

typedef enum {
    SF_SR_BUSY      = 0,
    SF_SR_WEL       = 1, // WRITE ENABLE LATCH
    SF_SR_WSE       = 2,
    SF_SR_WSP       = 3,
    SF_SR_BP        = 4,
    SF_SR_SRWD      = 5,
    SF_SR_OTPLOCK   = 6,
    SF_SR_FAIL      = 7,
    SF_SR_COUNT // Number of SR map entries
} SF_Status_Enum;

typedef enum {
    SF_SST_SR_WEL          = 0x02, // Write Enable Latch
    SF_SST_SR_WSE          = 0x04, // Write Suspend Erase Status
    SF_SST_SR_WSP          = 0x08, // Write Suspend Program Status
    SF_SST_SR_WPLD         = 0x10, // Write Protection Lockdown Status (Resets to 0 after a power cycle)
    SF_SST_SR_SEC          = 0x20, // Security ID status (Once set by Lock Security ID (0x85h) it can not be reset)
    SF_SST_SR_BUSY         = 0x80, // Write operation status (Write In Progress Bit)
    SF_SST_SR_ALL          = 0xFF,

    SF_MXIC_SR_BUSY        = 0x01, // Write operation status (Write In Progress Bit)
    SF_MXIC_SR_WEL         = 0x02, // Write Enable Latch
    SF_MXIC_SR_WSE         = 0x08, // Write Suspend Erase Status (in security register)
    SF_MXIC_SR_WSP         = 0x04, // Write Suspend Program Status (in security register)
    SF_MXIC_SR_BP          = 0x3C, // Block protection (MXIC only)
    SF_MXIC_SR_SRWD        = 0x80, // Status register write disable (MXIC only)
    SF_MXIC_SR_ALL         = 0xFF,

    SF_MXIC_SSR_OTP        = 0x01,
    SF_MXIC_SSR_LDSO       = 0x02,
    SF_MXIC_SSR_WSP        = 0x04,
    SF_MXIC_SSR_WSE        = 0x08,
    SF_MXIC_SSR_PFAIL      = 0x20,
    SF_MXIC_SSR_EFAIL      = 0x40,
    SF_MXIC_SSR_WPSEL      = 0x80,

    SF_EON_SSR_WSE         = 0x04,
    SF_EON_SSR_WSP         = 0x08,

    SF_WINBOND_SSR_SUS     = 0x80,
    SF_WINBOND_SSR_OTP     = 0x3C, //Security register lock bits (LB0, LB1, LB2, LB3)

    SF_NUMONYX_SSR_WSP     = 0x04,
    SF_NUMONYX_SSR_WSE     = 0x40
} SF_COMMON_COMMANDS;

typedef struct
{
    uint8_t Read;
    uint8_t ReadReadyReg;
    uint8_t WriteEnable;
    uint8_t Erase4K;
    uint8_t PageProg;
} SF_COMMANDS;

typedef struct
{
    uint8_t FlashID[SF_FLASH_ID_LENGTH + 1];
    uint8_t AddressSize;
    uint16_t CS;
    uint32_t FIX;
    uint8_t STATUS[SF_SR_COUNT];
    SF_COMMANDS CMD;
} SF_CONTEXT;

//JECDEC vendor id
//http://www.idhw.com/textual/chip/jedec_spd_man.html
typedef enum 
{
    JECDEC_TYPE_NONE            = 0,
    JECDEC_TYPE_NUMONYX         = 0x20,
    JECDEC_TYPE_NUMONYX_W       = 0x2C,
    JECDEC_TYPE_EON             = 0x1C,
    JECDEC_TYPE_SST             = 0xBF,
    JECDEC_TYPE_MXIC            = 0xC2,
    JECDEC_TYPE_GIGADEVICE      = 0xC8,
    JECDEC_TYPE_GIGADEVICE_MD   = 0x51,
    JECDEC_TYPE_WINBOND         = 0xEF,
    JECDEC_TYPE_FIDELIX         = 0xF8,
    JECDEC_TYPE_ESMT            = 0x8C,
    JECDEC_TYPE_COUNT
} JECDEC;

// General Serial Flash Commands
#define SF_CMD_WRITE_SR                     (0x01)
#define SF_CMD_PAGE_PROG                    (0x02)
#define SF_CMD_READ                         (0x03)
#define SF_CMD_NORMAL_READ                  (0x00030000)  
#define SF_CMD_READ_SR                      (0x05)
#define SF_CMD_WREN                         (0x06)
#define SF_CMD_FAST_READ                    (0x000B0000)  
#define SF_CMD_RESUME                       (0x30)
#define SF_CMD_WBPR                         (0x42)
#define SF_CMD_RSTEN                        (0x66)
#define SF_CMD_RBPR                         (0x72)
#define SF_CMD_RST                          (0x99)
#define SF_CMD_SUSPEND                      (0xB0)
#define SF_CMD_ERASE_CHIP                   (0xC7)
#define SF_CMD_ERASE_BLOCK                  (0xD8)
#define SF_CMD_ERASE_SMALL_BLOCK            (0x52)
#define SF_CMD_ERASE_SECTOR                 (0x20)
#define SF_CMD_READ_ID                      (0x9F)
#define SF_CMD_ENTER_DPD                    (0xB9)
#define SF_CMD_LEAVE_DPD                    (0xAB)
#define SF_CMD_READ_ID_QPI                  (0xAF)

// Serial Flash commands (SST specific)
#define SF_CMD_SST_QPIEN                    (0x38)      
#define SF_CMD_SST_QPIRST                   (0xFF)
#define SF_CMD_SST_SET_BURST_LENGTH         (0xC0)
#define SF_CMD_SST_BURST_LENGTH_32_BYTE     (0x02)
#define SF_CMD_SST_FAST_READ                (0x0B)
#define SF_CMD_SST_FAST_READ_WRAP           (0x0C)
#define SF_CMD_SST_READ_SECURITY_ID         (0x88)    
#define SF_CMD_SST_PROG_SECURITY_ID         (0xA5)    
#define SF_CMD_SST_LOCK_SECURITY_ID         (0x85)    

// Serial Flash commands (MXIC specific)
#define SF_CMD_MXIC_QPIEN                   (0x35)
#define SF_CMD_MXIC_RSTQPI                  (0xF5)
#define SF_CMD_MXIC_READ_SEC_SR             (0x2B)  
#define SF_CMD_MXIC_WRITE_SEC_SR            (0x2F)  
#define SF_CMD_MXIC_SET_BURST_LENGTH        (0xC0)
#define SF_CMD_MXIC_BURST_LENGTH_32_BYTE    (0x02)
#define SF_CMD_MXIC_BURST_LENGTH_DISABLE    (0x10)
#define SF_CMD_MXIC_FAST_READ_WRAP          (0x0B)  
#define SF_CMD_MXIC_FAST_READ_ENH           (0xEB)  
#define SF_CMD_MXIC_ENTER_SECURED_OTP       (0xB1)  
#define SF_CMD_MXIC_EXIT_SECURED_OTP        (0xC1)  

// Serial Flash commands (EON specific)
#define SF_CMD_EON_READ_SUSPEND_SR          (0x09)  

// Serial Flash commands (WINBOND specific)
#define SF_CMD_WINBOND_READ_SUSPEND_SR      (0x35)  
#define SF_CMD_WINBOND_SUSPEND              (0x75)  
#define SF_CMD_WINBOND_RESUME               (0x7A)  
#define SF_CMD_WINBOND_READ_SECURITY_REG    (0x48)  
#define SF_CMD_WINBOND_PROG_SECURITY_REG    (0x42)  
#define SF_CMD_WINBOND_READ_UNIQUE_ID       (0x4B)  

// Serial Flash commands (NUMONYX specific)
#define SF_CMD_NUMONYX_READ_FLAG_SR         (0x70)  
#define SF_CMD_NUMONYX_WRITE_FLAG_SR        (0x50)  
#define SF_CMD_NUMONYX_READ_LOCK_SR         (0xE8)  
#define SF_CMD_NUMONYX_WRITE_LOCK_SR        (0xE5)  
#define SF_CMD_NUMONYX_READ_CONFIG_SR       (0x85)  
#define SF_CMD_NUMONYX_WRITE_CONFIG_SR      (0x81)  

// Serial Flash commands (GIGADEVICE specific)
#define SF_CMD_GIGADEVICE_HPM_ENTER         (0xA3000000)  
#define SF_CMD_GIGADEVICE_HPM_ENTER_CMD     (0xA3)        
#define SF_CMD_GIGADEVICE_HPM_EXIT          (0xAB)        


// API
bool SerialFlash_Init(void);
int SerialFlash_Write(uint32_t Address, const void *Data, int Size);
int SerialFlash_Read(uint32_t Address, const void *Data, int Size); 

#ifdef __cplusplus
}
#endif
#endif /* _SFI_H_ */