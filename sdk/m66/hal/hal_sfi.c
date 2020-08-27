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

#include <hal/hal_sfi.h>
#include <hal/hal_pctl.h>
#include <hal/hal_usart.h>
#include <hal/hal_utils.h>
#include <debug.h>

#if 0
#define SFI_UART(X) USART1->TXB = X
static char SFI_DBG_BUFFER[256];
#define DBG_SFI(FORMAT, ...)                            \
    {                                                   \
        sprintf(SFI_DBG_BUFFER, FORMAT, ##__VA_ARGS__); \
        PANIC(SFI_DBG_BUFFER, false);                   \
    }
#else
#define SFI_UART(X)
#define DBG_SFI(FORMAT, ...)
#endif

#ifndef PRINTF
#define PRINTF printf
#endif

/*

    INTERFACE
    
*/

#define RAM_FUNC __attribute__((__section__(".ramfunc")))
#define SFI_IsQPIMode_CS0() (RW_SFI_DIRECT_CTL & SFI_QPI_EN)
#define SFI_IsQPIMode_CS1() (RW_SFI_DIRECT_CTL2 & SFI_QPI_EN)

#define SFI_MaskAhbChannel(x)                                              \
    do                                                                     \
    {                                                                      \
        if (x != 0)                                                        \
            RW_SFI_MISC_CTL3 = RW_SFI_MISC_CTL3 | (SFI_CH2_TRANS_MASK);    \
        else                                                               \
            RW_SFI_MISC_CTL3 = RW_SFI_MISC_CTL3 & (~(SFI_CH2_TRANS_MASK)); \
    } while (0)

static bool NOR_BUSY = false;

void RAM_FUNC SFI_MacLeave(const uint16_t CS)
{
    RW_SFI_MAC_CTL &= ~(SFI_TRIG | SFI_MAC_SIO_SEL | SFI_MAC_SEL);
    while (RW_SFI_MAC_CTL & SFI_WIP_READY)
        ;
    RW_SFI_MAC_CTL &= ~(SFI_MAC_EN);
    while (RW_SFI_MAC_CTL & SFI_MAC_EN)
        ;

    if (NOR_BUSY)
        SFI_MaskAhbChannel(0);
    NOR_BUSY = false;
    return;
}

void RAM_FUNC SFI_MacTrigger(const uint16_t CS)
{
    uint32_t val = RW_SFI_MAC_CTL;
    val |= (SFI_TRIG | SFI_MAC_EN);
    if (CS == 1)
        val |= SFI_MAC_SEL;
    RW_SFI_MAC_CTL = val;
    while (!(RW_SFI_MAC_CTL & SFI_WIP_READY))
        ;
    while (RW_SFI_MAC_CTL & SFI_WIP)
        ;
    return;
}

void RAM_FUNC SFI_MacEnable(const uint16_t CS)
{
    NOR_BUSY = true;
    uint32_t val;
    SFI_MaskAhbChannel(1);
    val = RW_SFI_MAC_CTL;
    if (CS == 0)
    {
        if (SFI_IsQPIMode_CS0())
            val |= SFI_MAC_SIO_SEL;
    }
    else if (CS == 1)
    {
        if (SFI_IsQPIMode_CS1())
            val |= SFI_MAC_SIO_SEL;
    }
    else
    {
        PANIC((char *)"NOR_MTD_DATA_CORRUPTED", true);
    }
    val |= SFI_MAC_EN;
    while (!(RW_SFI_MISC_CTL3 & SFI_CH2_TRANS_IDLE))
        ;
    while (!(RW_SFI_MISC_CTL & SFI_REQ_IDLE))
        ;
    RW_SFI_MAC_CTL = val;
    return;
}

void RAM_FUNC SFI_MacWaitReady(const uint16_t CS)
{
    SFI_MacTrigger(CS);
    SFI_MacLeave(CS);
    return;
}

uint32_t RAM_FUNC SFI_ReverseByteOrder(uint32_t num)
{
    unsigned int ret = 0;
    ret |= ((num >> 24) & 0x000000FF);
    ret |= ((num >> 8) & 0x0000FF00);
    ret |= ((num << 8) & 0x00FF0000);
    ret |= ((num << 24) & 0xFF000000);
    return ret;
}

void RAM_FUNC SFI_Command_Ext(const uint16_t CS, const uint8_t *cmd, uint8_t *data, const uint16_t outl, const uint16_t inl)
{
    uint32_t tmp;
    uint32_t i, j;
    uint8_t *p_data, *p_tmp;
    p_tmp = (uint8_t *)(&tmp);
    for (i = 0, p_data = (uint8_t *)RW_SFI_GPRAM_DATA_ADR; i < outl; p_data += 4)
    {
        for (j = 0, tmp = 0; i < outl && j < 4; i++, j++)
            p_tmp[j] = cmd[i];
        *p_data = tmp;
    }
    RW_SFI_MAC_OUTL = outl;
    RW_SFI_MAC_INL = inl;
    SFI_MacEnable(CS);
    SFI_MacWaitReady(CS);
    for (i = 0, p_data = (uint8_t *)RW_SFI_GPRAM_DATA_ADR + outl; i < inl; ++i, ++data, ++p_data)
        *data = *p_data;

    return;
}

void RAM_FUNC SFI_Command(const uint16_t CS, const uint32_t cmd)
{
    uint32_t cmdi, cmdo;
    uint8_t len;
    for (cmdi = cmd, len = 0, cmdo = 0; cmdi != 0; cmdi = cmdi >> 8, len++)
        cmdo = (cmdo << 8) | (cmdi & 0xFF);
    if (len)
    {
        SFI_MacEnable(CS);
        RW_SFI_GPRAM_DATA = cmdo;
        RW_SFI_MAC_OUTL = len;
        RW_SFI_MAC_INL = 0;
        SFI_MacWaitReady(CS);
    }
    return;
}

void RAM_FUNC SFI_CommandAddress(const uint16_t CS, const uint8_t cmd, const uint32_t address, const uint32_t address_bytes)
{
    uint32_t cmd1, cmd2;
    cmd2 = SFI_ReverseByteOrder(address);
    if (address_bytes == 3)
    {
        cmd1 = (cmd2 & 0xFFFFFF00) | cmd;
    }
    else
    {
        cmd1 = (cmd2 << 8) | cmd;
        cmd2 = cmd2 >> 24;
    }
    RW_SFI_GPRAM_DATA = cmd1;
    RW_SFI_GPRAM_DATA_4 = cmd2;
    RW_SFI_MAC_OUTL = address_bytes + 1;
    RW_SFI_MAC_INL = 0;
    SFI_MacEnable(CS);
    SFI_MacWaitReady(CS);
    return;
}

uint32_t RAM_FUNC SFI_GPRAM_Write_C1A3(const uint32_t cmd, const uint32_t address)
{
    return ((SFI_ReverseByteOrder(address) & 0xFFFFFF00) | (cmd & 0xFF));
}

uint32_t RAM_FUNC SFI_GPRAM_Write_C1A4(const uint32_t cmd, const uint32_t address, uint8_t **p_data, uint32_t *length)
{
    sf_uint tmp;
    uint32_t cmd1, cmd2, i;
    cmd2 = SFI_ReverseByteOrder(address);
    cmd1 = cmd2 << 8 | (cmd & 0xFF);
    cmd2 = cmd2 >> 24;
    tmp.u8[0] = cmd2 & 0x000000FF;
    for (i = 1; i < 4; i++)
    {
        tmp.u8[i] = **p_data;
        (*p_data)++;
    }
    cmd2 = tmp.u32;
    *length = (*length > 3) ? (*length - 3) : 0;
    RW_SFI_GPRAM_DATA_4 = cmd2;
    return cmd1;
}

void RAM_FUNC SFI_GPRAM_Write(uint32_t gpram_offset, void *buff, int32_t length)
{
    uint32_t i;
    uint8_t *p_data = (uint8_t *)buff;
    sf_uint tmp;
    gpram_offset += RW_SFI_GPRAM_DATA_ADR;
    // source address is 4-byte aligned, and gpram_offset is 4-bytes aligned, too
    if (0 == ((uint32_t)buff & 3))
    {
        for (; length > 0; gpram_offset += 4, p_data += 4, length -= 4)
            WriteAddr32(gpram_offset, *(uint32_t *)p_data);
    }
    // source address is 2-byte aligned
    else if (0 == ((uint32_t)p_data & 1))
    {
        for (; length > 0; gpram_offset += 4, p_data += 4, length -= 4)
        {
            tmp.u16[0] = *(uint16_t *)p_data;
            tmp.u16[1] = *((uint16_t *)p_data + 1);
            WriteAddr32(gpram_offset, tmp.u32);
        }
    }
    // source data is NOT 2-byte aligned
    else
    {
        for (; length > 0; gpram_offset += 4, length -= 4)
        {
            for (i = 0; i < 4 && i < length; i++)
                tmp.u8[i] = *p_data++;
            WriteAddr32(gpram_offset, tmp.u32);
        }
    }
    return;
}

void RAM_FUNC SFI_ReadDeviceID(const uint16_t CS, uint8_t *id, uint8_t cmd)
{
    SFI_Command_Ext(CS, &cmd, id, 1, SF_FLASH_ID_LENGTH); // Read 3 bytes ID (Vendor + Density + Device)  EF7016 W25Q32JV
}

void RAM_FUNC SFI_WaitBusy(const uint16_t CS) // if busy is bit 0
{
    int result;
    do
    {
        SFI_MacEnable(CS);
        RW_SFI_GPRAM_DATA = SF_CMD_READ_SR;
        RW_SFI_MAC_OUTL = 1;
        RW_SFI_MAC_INL = 1;
        SFI_MacWaitReady(CS);
        result = (unsigned char)(RW_SFI_GPRAM_DATA >> 8);
    } while (result & 1);
}

int RAM_FUNC SFI_BlankCheck(uint32_t Address, uint32_t Size)
{
    uint8_t *p_data = (uint8_t *)Address;
    while (Size--)
        if (*p_data++ != 0xFF)
            return -1;
    return 0;
}

/*

    DRIVER

*/

void RAM_FUNC SFD_Command(SF_CONTEXT *D, const uint32_t cmd, uint8_t outlen)
{
    uint32_t savedMask;
    savedMask = DisableInterrupts();
    SFI_MacEnable(D->CS);
    RW_SFI_GPRAM_DATA = cmd;
    RW_SFI_MAC_OUTL = outlen;
    RW_SFI_MAC_INL = 0;
    SFI_MacWaitReady(D->CS);
    RestoreInterrupts(savedMask);
    return;
}

void RAM_FUNC SFD_WriteEnable(SF_CONTEXT *D)
{
    SFD_Command(D, D->CMD.WriteEnable, 1);
}

uint32_t RAM_FUNC SFD_ReadStatus(SF_CONTEXT *D, uint8_t cmd)
{
    uint32_t result, savedMask = DisableInterrupts();
    SFI_MacEnable(D->CS);
    RW_SFI_GPRAM_DATA = cmd;
    RW_SFI_MAC_OUTL = 1;
    RW_SFI_MAC_INL = 1;
    SFI_MacWaitReady(D->CS);
    result = (unsigned char)(RW_SFI_GPRAM_DATA >> 8);
    RestoreInterrupts(savedMask);
    return result;
}

void RAM_FUNC SFD_WaitStatusReady(SF_CONTEXT *D)
{
    uint32_t savedMask = DisableInterrupts();
    while (SFD_ReadStatus(D, D->CMD.ReadReadyReg) & D->STATUS[SF_SR_BUSY])
        ;
    RestoreInterrupts(savedMask);
}

int RAM_FUNC SFD_Erase4k(SF_CONTEXT *D, uint32_t Address)
{
    if (Address % 4096)
    {
        PRINTF("[ERROR] SFD_Erase4K( align )\n");
        return -1;
    }

    if (0 == SFI_BlankCheck(Address, 4096))
    {
        PRINTF("[SF-E] SKIP\n");
        return 0; // skip if is blank
    }

    //PRINTF("[SF-E] 0x%08lX\n", Address);

    SFD_WaitStatusReady(D);

    uint32_t savedMask = DisableInterrupts();

    if (0 == (D->FIX & sf_fix_gigadevice_b))
        SFD_WriteEnable(D);

    if (D->FIX & sf_fix_gigadevice_b)
        SFI_Command(D->CS, D->CMD.Erase4K);

    SFI_CommandAddress(D->CS, D->CMD.Erase4K, Address, D->AddressSize);

    RestoreInterrupts(savedMask);

    SFD_WaitStatusReady(D);

    int res = SFI_BlankCheck(Address, 4096);
    if (res)
    {
        PRINTF("[ERROR] Erase4k( blank-check )\n");
    }
    return res; // 0 ok
}

int RAM_FUNC SFD_ProgramData(SF_CONTEXT *D, uint32_t __Address, void *__Data, uint32_t __Size)
{
    //PRINTF("[PROGRAM] 0x%08lX\n", __Address);
    //DUMP("  SRC", (char *)__Data, 16);

    uint8_t *data = __Data; // backup arg for compare
    uint32_t ram_len, write_min, cmd1, address = __Address & 0x00FFFFFF;
    int size = __Size;

    while (size > 0)
    {
        ram_len = SF_PROGRAM_BYTE_SIZE - (address & (SF_PROGRAM_BYTE_SIZE - 1));
        ram_len = write_min = ((size > ram_len) ? ram_len : size);
        /*** BEGIN WRITE *************************************************************/
        //FIX FLASH?
        SFI_Command(D->CS, D->CMD.WriteEnable);
        if (D->AddressSize == 4)
        {
            cmd1 = SFI_GPRAM_Write_C1A4(D->CMD.PageProg, address, &data, &ram_len);
            SFI_GPRAM_Write(8, data, ram_len);
        }
        else
        {
            cmd1 = SFI_GPRAM_Write_C1A3(D->CMD.PageProg, address);
            SFI_GPRAM_Write(4, data, ram_len);
        }
        RW_SFI_GPRAM_DATA = cmd1;
        RW_SFI_MAC_OUTL = write_min + D->AddressSize + 1;
        RW_SFI_MAC_INL = 0;
        SFI_MacEnable(D->CS);
        SFI_MacWaitReady(D->CS);
        SFD_WaitStatusReady(D);
        /*** END WRITE *************************************************************/
        data += ram_len;
        address += write_min;
        size -= write_min;
    }

    int res = memcmp((void *)__Address, __Data, __Size); // COMPARE
    if (res)
    {
        PRINTF("[ERROR] SFD_ProgramData compare: 0x%08lX\n", __Address);
    }
    return res; // 0 ok
}

static void SFD_InitCommon(SF_CONTEXT *D)
{
    memset(D, 0, sizeof(SF_CONTEXT));
    D->CMD.ReadReadyReg = SF_CMD_READ_SR;
    D->CMD.WriteEnable = SF_CMD_WREN;
    D->CMD.PageProg = SF_CMD_PAGE_PROG;
    D->CMD.Erase4K = SF_CMD_ERASE_SECTOR;
    D->AddressSize = 3; // if > 16M = 4
}

static int SFD_INIT_WINBOND(SF_CONTEXT *D)
{
    D->STATUS[SF_SR_BUSY] = 1;
    D->FIX = sf_fix_winbond_dw;
    PRINTF("[SF] WINBOND SELECTED\n");
#if 0
    uint8_t uid[16];
    uint8_t cmd = SF_CMD_WINBOND_READ_UNIQUE_ID;
    SFI_Command_Ext(0, &cmd, uid, 1, 4 + 8);
    DUMP("WINBOND UID", uid, 16);
#endif
    return 0;
}

/*

    USER API

*/

static SF_CONTEXT SerialFlash;
static uint8_t backup[4096];

int SerialFlash_Read(uint32_t Address, const void *Data, int Size)
{
    if (NULL == Data || Size < 1)
        return -1;
    memcpy(Data, (void *)Address, Size);
    return 0;
}

int SerialFlash_Write(uint32_t __Address, const void *__Data, int __Size)
{
    if (NULL == __Data || __Size < 1)
        return -10;
    SF_CONTEXT *D = &SerialFlash;
    if (0 == SFI_BlankCheck(__Address, __Size))
    {
        if (SFD_ProgramData(D, __Address, __Data, __Size))
            return -11;
        return 0;
    }

    char *data = (char *)__Data;
    int size = __Size;
    uint32_t offset, cnt, address = __Address & 0xFFFFF000;
    while (size > 0)
    {
        memcpy(backup, (void *)address, 0x1000);
        offset = __Address & 0xFFF;
        cnt = 0x1000 - offset;
        memcpy(backup + offset, __Data, cnt);
        __Data += cnt;
        __Address += cnt;

        if (SFD_Erase4k(D, address))
            return -12;
        if (SFD_ProgramData(D, address, backup, 4096))
            return -13;
        address += 4096;
        size -= 4096;
    };
    return 0;
}

bool SerialFlash_Init(void)
{
    PCTL_PowerUp(PD_SFC);
    SFD_InitCommon(&SerialFlash);

    SFI_ReadDeviceID(0, SerialFlash.FlashID, SF_CMD_READ_ID);
    PRINTF("[SF] FLASH ID = %02X %02X %02X\n", (int)SerialFlash.FlashID[0], (int)SerialFlash.FlashID[1], (int)SerialFlash.FlashID[2]);
    //PRINTF("[SF] ID = %08X\n", *(int *)SerialFlash.FlashID); // 001670EF

    //FLASH ID = EF 40 16
    switch (SerialFlash.FlashID[0])
    {
    case JECDEC_TYPE_WINBOND: // 0xEF
        SFD_INIT_WINBOND(&SerialFlash);
        return true;

    default:
        PRINTF("\n[ERROR] FLASH NOT SUPPORT\n");
    }
    return false;
}

/*













*/
void SF_EraseTest(uint32_t Address)
{
    //Address = 0x102C7000;
    SFI_Command(0, SF_CMD_WREN);
    PRINTF("SF_CMD_WREN\n");
    uint32_t savedMask = DisableInterrupts();
    SFI_CommandAddress(0, SF_CMD_ERASE_SECTOR, Address, 4);
    SFI_WaitBusy(0);
    RestoreInterrupts(savedMask);
    DUMP("ERASE", (char *)Address - 16, 32);
    DUMP("ERASE", (char *)Address + 4096 - 16, 32);
}
