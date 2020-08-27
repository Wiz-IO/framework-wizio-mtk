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

#include <hal/hal_msdc.h>
#include <hal/hal_gpio.h>
#include <hal/hal_pctl.h>
#include <hal/hal_pmu.h>
#include <hal/hal_pll.h>
#include <hal/hal_ustimer.h>
#include <assert.h>
#include "debug.h"

sd_ctx_t sd_ctx;

int MSDC_Check_Card_Present(void)
{
    /* TODO */
    return true;
}

#define CHECK_CARD_PRESENT()            \
    if (0 == MSDC_Check_Card_Present()) \
        return -3;

void MSDC_SetClock(uint32_t clock)
{
    if (0 == clock)
        return;
    MSDC->SDC_CFG &= 0xFFFEFFFF;
    uint32_t cfg = ((MSDC_CLOCK + clock - 1) / clock);
    if (cfg > 2)
    {
        cfg = (cfg >> 2) + (((cfg & 3) != 0) ? 1 : 0);
        if (0 != cfg)
        {
            sd_ctx.op_clock = MSDC_CLOCK / (4 * cfg);
        }
        else
        {
            assert(0);
        }
    }
    else
    {
        cfg = 0;
        sd_ctx.op_clock = MSDC_CLOCK / 2;
    }
    printf("CLOCK %d %d %d\n", (int)clock, (int)cfg, (int)sd_ctx.op_clock);
    MSDC->CFG &= ~(3 << 3);
    MSDC->CFG |= 2 << 3; // MPLL/8
    MSDC->CFG &= ~(0xFF << 8);
    MSDC->CFG |= cfg << 8; // divider
    MSDC->SDC_CFG |= 0x10000u;
}

void MSDC_ClockInit(void)
{
    MSDC->CLKACB_CFG &= ~(MSDC_CLKACB_CFG_CLKPAD_RED); // REG_32(0xA0270080) &= 0xFFFFFF7F;
    MSDC->SDC_CFG &= 0x00FFFFFF;                       //
    MSDC->SDC_CFG |= 0x50000000;                       // read timeout
    PLL_CLK_CONDD |= 1;                                // RG_MSDC2_26M_SEL
    MSDC->CFG &= ~(MSDC_CFG_CLKSRC);                   // REG_32(0xA0270000) &= 0xFFFFFFE7;
    MSDC_SetClock(240);
}

void MSDC_IOInit(void)
{
    MSDC->IOCON = 9;                                      // 8 mA
    MSDC->IOCON |= MSDC_IOCON_SAMPON | MSDC_IOCON_CMDSEL; // 0x280000u;
    MSDC->IOCON &= ~(MSDC_IOCON_SAMPLEDLY);
    MSDC->IOCON |= 1 << 24;
    MSDC->IOCON1 = MSDC_IOCON1_ALL_PULLUP_47K;
    delay_m(1);
}

void MSDC_Reset(void)
{
    MSDC->CFG |= MSDC_CFG_RST;
    while (MSDC->CFG & MSDC_CFG_RST)
        ;
}

void MSDC_turnOnVMC(void)
{
    //LDO_CON0 |= VMC_SRCLKENA_SEL(1); // ???
    VMC_CON0 = RG_VMC_EN | VMC_ON_SEL | RG_VMC_VOSEL(VMC_VO33V); // VMC @ 3.3v
    delay_m(10);
}

void MSDC_Init(void)
{
    GPIO_Setup(GPIO27, GPMODE(GPIO27_MODE_MC2CM0));
    GPIO_Setup(GPIO28, GPMODE(GPIO28_MODE_MC2CK));
    GPIO_Setup(GPIO29, GPMODE(GPIO29_MODE_MC2DA0));
    PCTL_PowerDown(PD_MSDC2);
    MSDC->CFG = MSDC_CFG_MSDC | (1 << 24); //enable, fifo=1
    MSDC_IOInit();
    MSDC_turnOnVMC();
    delay_m(10);
    PCTL_PowerUp(PD_MSDC2);
    delay_m(1);
    MSDC_Reset();
}

void SD_DebugResponse(const char *txt)
{
    printf("%s ", txt);
    printf("%08X ", (int)MSDC->SDC_RES0);
    printf("%08X ", (int)MSDC->SDC_RES1);
    printf("%08X ", (int)MSDC->SDC_RES2);
    printf("%08X\n", (int)MSDC->SDC_RES3);
}

static int SD_Wait_SDC_STA(uint32_t mask)
{
    int res = 0x000FFFFF; // timeout
    do
    {
        CHECK_CARD_PRESENT();
    } while (--res && (MSDC->SDC_STA & mask));
    return res ? 0 : -2;
}

static void sd_get_bit_field_value(uint8_t *destination, uint8_t *source, uint16_t start_bit, uint16_t width)
{
    uint16_t i;
    uint16_t bytes, bits;
    memset(destination, 0, (width >> 8) + (0 == (width & 0x07) ? 0 : 1));
    for (i = 0; i < width; i++)
    {
        bytes = (start_bit + i) >> 3;
        bits = (start_bit + i) & 0x07;
        *(destination + (i >> 3)) |= (uint8_t)(((*(source + bytes) >> bits) & 1) << (i & 0x07));
    }
}

static void sd_analysis_csd(uint32_t *csd)
{
    uint32_t c_size_mult;
    uint8_t *csd_pointer = (uint8_t *)csd;

    sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.csd_structure, csd_pointer, 126, 2);
    sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.tacc, csd_pointer, 112, 8);
    sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.nsac, csd_pointer, 104, 8);
    sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.tran_speed, csd_pointer, 96, 8);
    sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.ccc, csd_pointer, 84, 12);
    sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.read_bl_len, csd_pointer, 80, 4);
    sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.read_bl_partial, csd_pointer, 79, 1);
    sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.write_blk_misalign, csd_pointer, 78, 1);
    sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.read_blk_misalign, csd_pointer, 77, 1);
    sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.dsr_imp, csd_pointer, 76, 1);
    sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.write_bl_len, csd_pointer, 22, 4);
    sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.write_bl_partial, csd_pointer, 21, 1);
    sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.wp_grp_enable, csd_pointer, 31, 1);
    sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.perm_write_protect, csd_pointer, 13, 1);
    sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.tmp_write_protect, csd_pointer, 12, 1);

    sd_ctx.csd.read_bl_len = 1 << sd_ctx.csd.read_bl_len;
    sd_ctx.csd.write_bl_len = 1 << sd_ctx.csd.write_bl_len;

    if (sd_ctx.csd.tmp_write_protect || sd_ctx.csd.perm_write_protect)
    {
        sd_ctx.is_write_protection = true;
    }

    /*there is some difference of CSD between SD and eMMC */
    if (sd_ctx.card_type == HAL_SD_TYPE_MMC_CARD || sd_ctx.card_type == HAL_SD_TYPE_MMC42_CARD)
    {
        sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.spec_vers, csd_pointer, 122, 4);
        sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.erase_sec_size_mmc, csd_pointer, 42, 5);
        sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.erase_grp_size_mmc, csd_pointer, 37, 5);
        sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.wp_grp_size_mmc, csd_pointer, 32, 5);
        sd_ctx.csd.erase_sec_size_mmc = (sd_ctx.csd.erase_sec_size_mmc + 1) * sd_ctx.csd.write_bl_len;
        sd_ctx.csd.erase_grp_size_mmc = (sd_ctx.csd.erase_grp_size_mmc + 1) * sd_ctx.csd.erase_sec_size_mmc;
        sd_ctx.csd.wp_grp_size_mmc = (sd_ctx.csd.wp_grp_size_mmc + 1) * sd_ctx.csd.erase_grp_size_mmc;
    }
    else
    {
        sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.sector_size, csd_pointer, 39, 7);
        sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.wp_grg_size, csd_pointer, 32, 7);
        sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.erase_blk_en, csd_pointer, 46, 1);
        sd_ctx.csd.sector_size = sd_ctx.csd.sector_size + 1;
        sd_ctx.csd.wp_grg_size = (sd_ctx.csd.wp_grg_size + 1) * sd_ctx.csd.sector_size;
    }

    if (sd_ctx.card_type == HAL_SD_TYPE_SD20_HCS_CARD && sd_ctx.csd.csd_structure >= CSD_VERSION_2_0)
    {
        sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.c_size, csd_pointer, 48, 22);
        /*in SD2.0, capacity = (C_SIZE + 1) * 512Kbyte*/
        sd_ctx.csd.capacity = (uint64_t)(sd_ctx.csd.c_size + 1) * 512 * 1024;
    }
    else
    {
        sd_get_bit_field_value((uint8_t *)&sd_ctx.csd.c_size, csd_pointer, 62, 12);
        sd_get_bit_field_value((uint8_t *)&c_size_mult, csd_pointer, 47, 3);
        /*in SD1.0, capacity = (C_SIZE + 1) * (1 << C_SIZE_MULT) * (READ_BL_LEN)*/
        sd_ctx.csd.capacity = (uint64_t)(sd_ctx.csd.c_size + 1) * (1 << (c_size_mult + 2)) * sd_ctx.csd.read_bl_len;
    }
}

int SD_WaitCmdReady(void)
{
    uint32_t cnt = 0x000FFFFF;
    do
    {
        CHECK_CARD_PRESENT();
        sd_ctx.sd_int = MSDC->INT; // read clear
    } while (--cnt && (0 == (sd_ctx.sd_int & 4)));
    sd_ctx.cmd_sta = MSDC->SDC_CMD_STA;
    //printf("INT: %X, CSTA: %X, DSTA: %X, %d\n", (int)sd_ctx.sd_int, (int)sd_ctx.cmd_sta, (int)MSDC->SDC_DAT_STA, (int)cnt);
    return sd_ctx.cmd_sta & 1 ? 0 : -1;
}

int SD_CheckStatus(void)
{
    int res = MSDC->SDC_RES0;
    if (res >> 16)
    {
        printf("ERROR SD_CheckStatus: %X\n", (int)res);
        if (res & 0x2000000)
            return 60;
        else
            return 11;
    }
    return res >> 16;
}

int SD_Send_Cmd(uint32_t cmd, uint32_t arg)
{
    if (SD_Wait_SDC_STA(SDC_STA_SDCBUSY))
        return -1;
    sd_ctx.sd_int = MSDC->INT; // read clear
    sd_ctx.cmd_sta = MSDC->SDC_CMD_STA;
    sd_ctx.dat_sta = MSDC->SDC_DAT_STA;
    MSDC->SDC_ARG = arg;
    MSDC->SDC_CMD = cmd;
    return SD_WaitCmdReady();
}

int SD_Reset(void)
{
    return SD_Send_Cmd(SDC_CMD_CMD0, SDC_NO_ARG);
}

int SD_GetCID(void)
{
    int res = SD_Send_Cmd(SDC_CMD_CMD2, SDC_NO_ARG);
    if (0 == res)
    {
        SD_DebugResponse("  CID");
    }
    return res;
}

int SD_GetRCA(void)
{
    int res;
    sd_ctx.rca = 0;
    if (sd_ctx.card_type == HAL_SD_TYPE_SD_CARD ||
        sd_ctx.card_type == HAL_SD_TYPE_SD20_HCS_CARD ||
        sd_ctx.card_type == HAL_SD_TYPE_SD20_LCS_CARD)
    {
        if ((res = SD_Send_Cmd(SDC_CMD_CMD3_SD, SDC_NO_ARG)))
            return res;
        sd_ctx.rca = MSDC->SDC_RES0 >> 16;
        printf("    rca: %X\n", (int)sd_ctx.rca);
    }
    else // NO MMC SUPPORT
        return -1;
    SD_DebugResponse("  RCA");
    return 0;
}

int SD_SelectCard(void)
{
    int res = SD_Send_Cmd(SDC_CMD_CMD7, sd_ctx.rca << 16);
    if (0 == res)
    {
        res = SD_Wait_SDC_STA(SDC_STA_SDCBUSY);
        if (0 == res)
            res = SD_CheckStatus();
    }
    return res;
}

int SD_Cmd55(int arg)
{
    int res;
    res = SD_Send_Cmd(183, arg << 16);
    if (0 == res)
    {
        res = SD_CheckStatus();
        if (0 == res)
        {
            if (MSDC->SDC_RES0 & 0x20)
                res = 0;
            else
                res = 13;
        }
    }
    return res;
}

int SD_Card_Type(void)
{
    msdc_register_t *msdc_register_base;
    int status;
    uint32_t response = 0;
    uint32_t ocr_argument = 0;
    uint32_t ocr_value = 0;
    bool is_first_command41 = true;

    status = SD_Send_Cmd(SDC_CMD_CMD8, SDC_CMD8_ARG);
    if (0 != status)
    {
        SD_Reset();
        sd_ctx.command8_response = SD_COMMAND8_RESPONSE_NO_RESPONSE;
    }
    else
    {
        response = MSDC->SDC_RES0;
        if (SDC_CMD8_ARG == response)
        {
            sd_ctx.command8_response = SD_COMMAND8_RESPONSE_VALID;
        }
        else
        {
            sd_ctx.command8_response = SD_COMMAND8_RESPONSE_INVALID;
        }
    }

    if (SD_COMMAND8_RESPONSE_NO_RESPONSE == sd_ctx.command8_response)
    {
        ocr_argument = SD_OCR_VOLTAGE_ARGUMENT;
    }
    else if (SD_COMMAND8_RESPONSE_VALID == sd_ctx.command8_response)
    {
        ocr_argument = SD_OCR_VOLTAGE_ARGUMENT | SD_OCR_CAPACITY_STATUS;
    }
    else if (SD_COMMAND8_RESPONSE_INVALID == sd_ctx.command8_response)
    {
        sd_ctx.error_status = -1; //ERROR_COMMAND8_INVALID;
        return HAL_SD_TYPE_UNKNOWN_CARD;
    }

    uint32_t count = 1100000;
    while (1)
    {
        /*wait MSDC not busy*/
        while (MSDC->SDC_STA & 1)
            ;

        status = SD_Cmd55(SD_RCA_DEFAULT);
        if (0 != status)
        {
            sd_ctx.error_status = status;
            return 255; //HAL_SD_TYPE_UNKNOWN_CARD;
        }

        /*wait MSDC not busy*/
        while (MSDC->SDC_STA & 1)
            ;

        if (is_first_command41)
        {
            status = SD_Send_Cmd(SDC_CMD_CMD41_SD, SDC_NO_ARG);
            is_first_command41 = false;
        }
        else
        {
            status = SD_Send_Cmd(SDC_CMD_CMD41_SD, ocr_argument);
        }

        if (0 != status)
        {
            sd_ctx.error_status = status;
            return 255; //HAL_SD_TYPE_UNKNOWN_CARD;
        }

        ocr_value = MSDC->SDC_RES0;
        if (!(ocr_value & SD_OCR_VOLTAGE_ARGUMENT))
        {
            sd_ctx.error_status = -2; //ERROR_OCR_NOT_SUPPORT;
            return 255;               //HAL_SD_TYPE_UNKNOWN_CARD;
        }

        if (false == MSDC_Check_Card_Present())
        {
            sd_ctx.error_status = -1; //ERROR_CARD_NOT_PRESENT;
            return 255;               //HAL_SD_TYPE_UNKNOWN_CARD;
        }

        if ((ocr_value & SD_OCR_BUSY_STATUS))
        {
            break;
        }
        else
        {
            if (--count == 0)
            {
                sd_ctx.error_status = -1; //ERROR_R3_OCR_BUSY;
                return 255;               //HAL_SD_TYPE_UNKNOWN_CARD;
            }
        }
    }

    sd_ctx.is_inactive = false;
    sd_ctx.sd_ocr = ocr_value;

    if (ocr_value & SD_OCR_CAPACITY_STATUS)
    {
        sd_ctx.card_type = HAL_SD_TYPE_SD20_HCS_CARD;
    }
    else if (SD_COMMAND8_RESPONSE_VALID == sd_ctx.command8_response)
    {
        sd_ctx.card_type = HAL_SD_TYPE_SD20_LCS_CARD;
    }
    else
    {
        sd_ctx.card_type = HAL_SD_TYPE_SD_CARD;
    }

    //sd_ctx.sd_state = READY_STA;
    return sd_ctx.card_type;
}

int SD_GetCSD(void)
{
    memset(&sd_ctx.csd, 0, sizeof(csd_information_t));
    int res = SD_Send_Cmd(SDC_CMD_CMD9, sd_ctx.rca << 16);
    uint32_t sd_csd[4];
    if (0 == res)
    {
        sd_csd[0] = MSDC->SDC_RES0;
        sd_csd[1] = MSDC->SDC_RES1;
        sd_csd[2] = MSDC->SDC_RES2;
        sd_csd[3] = MSDC->SDC_RES3;
        SD_DebugResponse("  CSD");
        sd_analysis_csd(sd_csd);
        printf("    capacity: %lu M\n", (uint32_t)sd_ctx.csd.capacity / (1024 * 1024));
        printf("    write block len: %lu\n", sd_ctx.csd.write_bl_len);
        printf("    read block len: %lu\n", sd_ctx.csd.read_bl_len);
        printf("    erase sector size: %lu\n", sd_ctx.csd.sector_size);
    }
    return res;
}

int SD_Init(void)
{
    int res;
    memset(&sd_ctx, 0, sizeof(sd_ctx));
    MSDC_Init();
    MSDC_ClockInit();                         // 240 kHz
    MSDC->SDC_CFG = 0x50018000 | SECTOR_SIZE; //

    MSDC->CFG |= MSDC_CFG_SCKON;    // enable clk
    delay_u(500);                   //
    MSDC->CFG &= ~(MSDC_CFG_SCKON); // stop clk

    if ((res = SD_Reset()))
    {
        printf("ERROR: SD_Reset()\n");
        return res;
    }

    if ((res = SD_Card_Type()))
    {
        //printf("ERROR: SD_Card_Type()\n");
        //return res;
    }

    if ((res = SD_GetCID()))
    {
        printf("ERROR: SD_GetCID()\n");
        return res;
    }

    if ((res = SD_GetRCA()))
    {
        printf("ERROR: SD_GetRCA()\n");
        return res;
    }

    if ((res = SD_GetCSD()))
    {
        printf("ERROR: SD_GetCSD()\n");
        return res;
    }

    if ((res = SD_SelectCard()))
    {
        printf("ERROR: SD_SelectCard()\n");
        return res;
    }
    MSDC_SetClock(13000);
    return res;
}

int SD_StopTrans(bool isTx)
{
    int res;
    uint32_t retry = 0;
    while (retry < 30)
    {
        if ((res = SD_Send_Cmd(SDC_CMD_CMD12, SDC_NO_ARG)))
            retry++;
        else
            break;
    }
    if (retry >= 30)
    {
        return res;
    }
    if (isTx)
        res = SD_Wait_SDC_STA(SDC_STA_SDCBUSY);
    return res;
}

int SD_ReadBlock(uint32_t sector, void *buffer, uint32_t num)
{
    int res;
    uint32_t to = 0x00FFFFFF;
    uint32_t mul = SECTOR_SIZE;
    uint32_t *rxbuffer = (uint32_t *)buffer;
    if (NULL == buffer || num == 0)
        return -1;
    if (sd_ctx.card_type == HAL_SD_TYPE_SD20_LCS_CARD || sd_ctx.card_type == HAL_SD_TYPE_SD20_HCS_CARD)
        mul = 1;
    if ((res = SD_Send_Cmd((num == 1 ? SDC_CMD_CMD17 : SDC_CMD_CMD18), sector * mul)))
    {
        printf("ERROR: READ CMD\n");
        goto exit;
    }
    if ((res = SD_CheckStatus()))
    {
        printf("ERROR: READ STATUS\n");
        goto exit;
    }
    uint64_t cnt = num * (SECTOR_SIZE >> 2);
    while (cnt)
    {
        if (--to == 0 || 0 == MSDC_Check_Card_Present())
        {
            res = -2;
            printf("ERROR: READ TIMEOUT: %X, %X, %d\n", (int)MSDC->STA, (int)MSDC->SDC_DAT_STA, (int)cnt);
            goto exit;
        }
        if (MSDC->STA & MSDC_STA_DRQ)
        {
            *rxbuffer++ = MSDC->DAT;
            --cnt;
        }
    }
exit:
    MSDC_Reset();
    if (num > 1)
        SD_StopTrans(false);
    sd_ctx.dat_sta = MSDC->SDC_DAT_STA;
    MSDC->STA |= MSDC_STA_FIFOCLR;
    return res;
}

int SD_WriteBlock(uint32_t sector, const void *buffer, uint32_t num)
{
    int res;
    uint32_t to = 0x00FFFFFF;
    uint32_t mul = SECTOR_SIZE;
    uint32_t *wrbuffer = (uint32_t *)buffer;
    if (NULL == buffer || num == 0)
        return -1;
    if (sd_ctx.card_type == HAL_SD_TYPE_SD20_LCS_CARD || sd_ctx.card_type == HAL_SD_TYPE_SD20_HCS_CARD)
        mul = 1;
    if ((res = SD_Send_Cmd((num == 1 ? SDC_CMD_CMD24 : SDC_CMD_CMD25), sector * mul)))
    {
        printf("ERROR: WRITE CMD\n");
        goto exit;
    }
    if ((res = SD_CheckStatus()))
    {
        printf("ERROR: WRITE STATUS\n");
        goto exit;
    }
    uint64_t cnt = num * (SECTOR_SIZE >> 2);
    while (cnt)
    {
        if (--to == 0 || 0 == MSDC_Check_Card_Present())
        {
            res = -2;
            printf("ERROR: READ TIMEOUT: %X, %X, %d\n", (int)MSDC->STA, (int)MSDC->SDC_DAT_STA, (int)cnt);
            goto exit;
        }
        sd_ctx.dat_sta = MSDC->SDC_DAT_STA;
        if (sd_ctx.dat_sta & SDC_DAT_STA_DATTO)
            goto exit;
        if (sd_ctx.dat_sta & (0xFF << 2)) // DATCRCCERR
            goto exit;
        if (0 == (MSDC->STA & MSDC_STA_BF))
            MSDC->DAT = *wrbuffer++;
        if (cnt == 0) // check data ready (512 bytes)
            SD_Wait_SDC_STA(SDC_STA_SDCBUSY);
    }
exit:
    MSDC_Reset();
    if (num > 1)
        SD_StopTrans(false);
    sd_ctx.dat_sta = MSDC->SDC_DAT_STA;
    MSDC->STA |= MSDC_STA_FIFOCLR;
    return res;
}
//