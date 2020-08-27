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

#if 0

#include <stdint.h>
#include <hal/hal.h>
#include <hal/hal_ustimer.h>
#include <hal/hal_pmu.h>
#include <hal/hal_audio.h>
#include <debug.h>

void dump16(uint32_t addr, int size_short);
void dump32(uint32_t addr, int size_int);

extern int patch_zip_size;
extern int patch_size_bytes;
extern char raw_patch_content[372];
extern char patch_content_GSM_compress[];
extern DLL_Func dll_Header_Kernel;
extern DLL_Func dll_Header_Audio;

extern uint8_t p_ena[];
extern uint8_t p_page[];
extern uint8_t p_add[];
extern uint8_t p_val[];
extern int FH_table[];

#define CLK_SETTLE 0xA0

static void MD_TOPSM_Init(void) 
{
    RM_CLK_SETTLE = CLK_SETTLE + 0x2020000;  //
    REG_32(0xA0170094) = CLK_SETTLE + 0x200; // MIXED_base
    RM_TMR_TRG0 = 0;                         //
    RM_TMR_PWR0 = 1;                         //
    RM_PERI_CON = 0x1000082;                 //
    REG_32(0xA0180034) = 6;                  //
    REG_32(0xA0180200) |= 0x1000u;           //
    REG_32(0xA0180824) = 0x2F00010;          //
    REG_32(0xA0180828) = 0xFFF0270;          //
    REG_32(0xA0180830) = 0xFEFEFEFE;         //
    REG_32(0xA0180834) = 0xFFFFFFFE;         //
    REG_32(0xA0180840) = 0xFFFEEEEE;         //
    REG_32(0xA0180850) = 1;                  //
    REG_32(0xA0180860) = 0xFFFFFFFE;         //
    REG_32(0xA01808B0) = 0xFFFFFFFD;         //
    REG_32(0xA01808C0) = 0xFFFFFFFF;         //
    REG_32(0xA01808D0) = 0xFFFFFFFF;         //
    REG_32(0xA0180900) = 6;                  //
    REG_32(0xA0180A38) = 15;                 //

    REG_32(0xA0730030) |= 0x500u; // ANA_CFGSYS_base
    REG_32(0xA0730034) &= 0xFFFFFF00;
    REG_32(0xA0730034) += 0x26;
}

static void MODEM_TOPSM_Init(void)
{
    REG_32(0x83010000) = CLK_SETTLE + 0x2020000; // DSP_TOPSM_RM_CLK_SETTLE
    REG_32(0x83010010) = 0;                      //
    REG_32(0x83010018) = 1;                      // DSP_TOPSM_RM_TMR_PWR0               Resource Manager Timer Power Enable Control 0
    REG_32(0x83010200) |= 0x1000u;               // DSP_TOPSM_CCF_CLK_CON
    REG_32(0x83010824) = 0x2F00010;              // DSP_TOPSM_RM_PWR_PER0
    REG_32(0x83010828) = 0xFFF0270;              //
    REG_32(0x83010830) = 0xFEFEFEFE;             // DSP_TOPSM_PROTECT_ACK_MASK
    REG_32(0x83010840) = 0xFFFFFEEE;             // DSP_TOPSM_RM_SM_PLL_MASK0
    REG_32(0x83010850) = 3;                      // DSP_TOPSM_RM_SM_PWR                 
    REG_32(0x83010860) = -1;                     // DSP_TOPSM_RM_SM_TRG                 Sleep Manager Cross Trigger Control
    REG_32(0x83010890) = -2;                     // DSP_TOPSM_RM_SM_MASK                Sleep Manager Enable Control
    REG_32(0x830108B0) = -2;                     // DSP_TOPSM_SM_REQ_MASK               Sleep Manager Request Source Mask
    REG_32(0x830108C0) = -1;                     // DSP_TOPSM_SM_SLV_REQ_IRQ
    REG_32(0x830108D0) = -1;                     // DSP_TOPSM_SM_MAS_RDY_IRQ
    REG_32(0x83010900) = 6;                      // DSP_TOPSM_SM_TRIG_SETTLE0
    REG_32(0x83010A38) = 0;                      // MODEM2G_TOPSM_PROTECT_ACK_MASK
    while (1 != (REG_32(0x83010040) & 0xF))      // DSP_TOPSM_RM_TMR_SSTA               Resource Manager Timer Sleep Control State
        ;
}

static void idma_load_cm(int page, int addr, unsigned int size_bytes, char *data /*4 aligned 4*/)
{
    ////dump32((int)data, size);
    size_bytes /= 4; // size of ints
    data += 4;       // skip first zeroes
    int *p = (int *)((page << 16) + (4 * addr) + IDMA_base);
    printf("[IDMA-CM] %X [ %d ]\n", (int)p, size_bytes);
    for (int i = 1; i < size_bytes - 1; ++i)
    {
        //printf("%p = %08X ", p, *(int*)data);
        *p++ = *(int *)data; // 0X8202F000 = 0053A530 0X8202F004 = 0052AC39 ...
        data += 4;
    }
    *p = 0; // add last zero
    ////printf("\n");
}

static void idma_load_cm_d2(int page, int addr, int size_bytes, char *data /*3 aligned to 4*/)
{
    ////dump32((int)data, size);
    int d, i = 0;
    int *p = (int *)((page << 16) + (4 * addr) + IDMA_base);
    printf("[IDMA-CM-D2] %X [ %d ]\n", (int)p, size_bytes);
    while (size_bytes > 0)
    {
        d = *(char *)(data + 2) + ((*(char *)(data + 1) + (*data << 8)) << 8); // M[2] [1] [0] S00
        ////printf("%p = %08X ", p, d);
        *p++ = d; // 0x82041000 = 0090D707 0x82041004 = 0054EE19 ...
        size_bytes -= 3;
        data += 3;
    }
    ////printf("\n");
}

static int write_patch(int size)
{
    int res = 0;
    int *p;
    short *P_ENA = (short *)p_ena;
    short *P_PAGE = (short *)p_page;
    short *P_VAL = (short *)p_val;
    short *P_ADD = (short *)p_add;
    for (int i = 0; i < size; i++)
    {
        res += P_ENA[i];
        p = (int *)(16 * i + PATCH_base); // [32] bits base
        p[0] = *P_PAGE++;
        p[1] = *P_ADD++;
        p[2] = *P_VAL++ & 0xFFFF; // LOW 16
        p[3] = *P_VAL++ & 0xFFFF; // HIGH 08
        ////printf("[PATCH-%d] %p Page=%04X, Addr=%04X, Val=%04X.%04X \n", size, p, p[0], p[1], p[3], p[2]);
    }
    return res; // is enabled
}

static void idma_load_genral(void)
{
    SHARE_DSPCON = 15;                                                   // ? dsp boot
    char *patch_content = (char *)calloc(patch_size_bytes, 1);           // 4096
    memcpy(patch_content, raw_patch_content, sizeof(raw_patch_content)); // [372]
    SHARE_DSPCON = 13;                                                   // ? dsp boot
    REG_32(PATCH_base + 0x100) = 0;                                      // disable pactch
    if (write_patch(16))                                                 // all patches
        idma_load_cm(2, 0x3C00, patch_size_bytes, patch_content);
    free(patch_content);
}

static void write_patch_config(int flag)
{
    if (flag)
    {
        write_patch(2);                 // first 2 patches
        REG_32(PATCH_base + 0x100) = 3; // enable patch
    }
    else
    {
        short mask;
        short *p = (short *)0x82207704; // ??? DP_NB_RESULT_RX0_BASE
        short *P_ENA = (short *)p_ena;
        P_ENA += 2;
        short *P_PAGE = (short *)p_page;
        P_PAGE += 2;
        short *P_ADDR = (short *)p_add;
        P_ADDR += 2;
        int *P_VAL = (int *)p_val;
        P_VAL += 2;
        for (int i = 0; i < 6; i++) // next 6
        {
            if (*P_ENA++)
                mask = 0xA500;
            else
                mask = 0x5A00;
            int val = *P_VAL++;
            p[0] = mask | val >> 16;
            p[1] = val & 0xFFFF;
            p[2] = *P_ADDR++;
            p[3] = *P_PAGE++;
            ////printf("[patch-0] %p Page=%04X, Addr=%04X, Val=%04X.%04X \n", p, (int)p[3], (int)p[2], (int)p[0] & 0xFFFF, (int)p[1] & 0xFFFF);
        }
    }
}

static void dp_dummy_write_read(void)
{
    int TEST = 0x5555;
    REG_16(0x82207E92) = TEST;   // DP_3F49_????
    REG_16(0x82207E94) = 0x6666; // DP_3F4A_????
    int cnt = 0;
    do
    {
        if (TEST == 0xAAAA)
            break;
        TEST = REG_16(0x82207E92) = 0xAAAA; // DP_3F49_????
        delay_u(1);
    } while (cnt++ <= 1000000);
    printf("[DSP] Dummy RW test ok\n");
}

void dsp_wait_ready(void)
{
    while (DSP_STATUS != 0x5555)
        ;
    DSP_STATUS = 0;

    write_patch_config(0); // to share base

    MCU_STATUS = 0x6666;
    while (DSP_STATUS != 0x5555)
        ;
    printf("[DSP] Ready\n");
}

void dsp_wakeup(void)
{
    write_patch_config(1); // to patch base
    SHARE_DSPCON = DSP_RESET_BEGIN;
    REG_32(0x82C00020) = 64;
    delay_m(100); // WAIT_TIME_QB(100);
    SHARE_DSPCON = DSP_RESET_END;
    dp_dummy_write_read();
    DSP_STATUS = 0;
    MCU_STATUS_MTCMOS = 0;
    REG_16(0x82CA0074) = 2; // [16] SHARE_base SHARE_D2M_WAKEUP ???
    REG_32(0x82000000) = 0; // fd216_dsp_init()
    printf("[DSP] Wakeup\n");
}

int dsp_dynamic_download(int id)
{
    int i = 0;
    while (REG_16(0x82207E78) & 0x8000) // DYNAMIC_DOWNLOAD_STATUS
    {
        if (i >= 10)
        {
            REG_16(0x82207E78) &= 0x7FFFu; // DYNAMIC_DOWNLOAD_STATUS
            break;
        }
        delay_m(1);
        i++;
    }

    REG_16(0x82207410) = -1;                 // DP_SLOW_IDLE_DIVIDER L1Audio_Disable_DSPSlowIdle()
    REG_16(0x82207E76) = (1 << id) | 0x8000; // ??? 1 << id ? DYNAMIC_DOWNLOAD_MCU_STATUS
    SHARE_M2DI2 = 0;                         // REG_16(0x83030008) = 0; // Interrupt2 Register

    printf("[DSP] DLL Handshake(1) ");
    i = 0x20000;
    while (REG_16(0x82207CF4) != 0x8888) // DP2_DDL_MCU_DSP_HAND_SHAKE
    {
        if (0 == i--)
        {
            printf("[ABORT] DP2_DDL_MCU_DSP_HAND_SHAKE(1) %X\n", (int)REG_16(0x82207CF4));
            abort();
        }
    }
    printf("Ready: %X\n", (int)REG_16(0x82207CF4));
    REG_16(0x82207CF4) = 0; // DP2_DDL_MCU_DSP_HAND_SHAKE

    /* !!! UPLOAD FUNCTIONS !!! */
    idma_load_cm_d2(4, dll_Header_Kernel.Dsp_Start_Addr, dll_Header_Kernel.Array_Len, dll_Header_Kernel.Array_Addr);
    idma_load_cm_d2(4, dll_Header_Audio.Dsp_Start_Addr, dll_Header_Audio.Array_Len, dll_Header_Audio.Array_Addr);
    //DDID_SPH_TCH_1 2 3

    REG_16(0x82207410) = 11; // DP_SLOW_IDLE_DIVIDER L1D_DSP_EnableDspSlowIdle()
    REG_16(0x82207E76) = 0;  // DYNAMIC_DOWNLOAD_MCU_STATUS
    SHARE_M2DI2 = 1;         // REG_16(0x83030008); // Interrupt2 Register

    printf("[DSP] DLL Handshake(2) ");
    i = 0x20000;
    while (REG_16(0x82207CF4) != 0x8888) // DP2_DDL_MCU_DSP_HAND_SHAKE
    {
        if (0 == i--)
        {
            printf("[ABORT] DP2_DDL_MCU_DSP_HAND_SHAKE(2) %X\n", (int)REG_16(0x82207CF4));
            abort();
        }
    }
    printf("Ready: %X\n", (int)REG_16(0x82207CF4));
    REG_16(0x82207CF4) = 0; // DP2_DDL_MCU_DSP_HAND_SHAKE
    return 0;
}

void dsp_hw_init(void)
{
    ////void L1D_HWCG_Init(void)
    REG_16(0x82C00058) &= 0xFFDFu; // HWCG_BYPASS &= 0xFFDFu;
    REG_32(0x83000400) |= 4;       // MDCONFIG_base
    REG_16(0x82C00214) |= 0x200u;  // HW_MD2GSYS_BUS_CG_ENA(); // enable MD2GSYS bus clock gating

    ////void L1D_MD2G_PWD_Init(void)
    REG_32(0x83010800) = (REG_32(0x83010800) & 0xFFFFE0FF) | 0x100; // Power Control Register
    REG_32(0x83010800) &= 0xFFFFDFFF;

    ////void L1D_Init_MPLLFH(void) MIXED_base
    REG_32(0xA0170514) = 0x4000;
    REG_32(0xA0170500) &= 0xFCFFu;
    REG_32(0xA0170508) = 0x500;
    for (int i = 0; i < 64; i++)
    {
        REG_32(0xA017050C) = FH_table[i] | 0x800;
        delay_m(1);
    }
    REG_32(0xA0170510) &= 2 | 0xB19D;
    REG_32(0xA0170510) &= 0xFFF7;
    for (int i = 0; i < 6; i++)
        *(short *)(4 * i + 0x83020320) = 0x3FFF;
    printf("[DSP] HW INIT\n");
}

void AM_Init(void)
{
    REG_16(0x82207DF4) = 0x5F;     // DP_Audio_ASP_DEL_W_ADDR
    REG_16(0x82207DF8) = 0x5F;     // DP_Audio_ASP_DEL_MARGIN_W_ADDR
    REG_16(0x82207CAA) = 14;       // Pseudo_SAL_Dsp_Sph_Init()
    REG_16(0x82207CAE) = 0x405;    // ...
    REG_16(0x82207CAA) = 0xE;      // DP_ENHANCED_AUDIO_CTRL
    REG_16(0x82207CAC) = 0;        // ?
    REG_16(0x82207C6A) = 0;        // DP2_EC_SUPP
    REG_16(0x82207C6C) = 0;        // DP_AEC_CTRL
    REG_16(0x82207C72) = 0;        // DP_AEC_CONTROL_WORD
    REG_16(0x82207AE4) &= 0xFFDFu; // SPH_MUTE_CTRL
    REG_16(0x82207AEC) = 0x8000u;  // AUDIO_PAR Pseudo_SAL_Set_Sph_Nml_Mod(1)
    REG_16(0x82207DF0) = 0;        // DP_AUDIO_ASP_TYPE_ADDR
    REG_16(0x82207DF2) = 0;        // DP_AUDIO_ASP_FS_ADDR
    REG_16(0x82207DF4) = 0x5F;     // DP_Audio_ASP_DEL_W_ADDR
    REG_16(0x82207DF8) = 0x5F;     // DP_Audio_ASP_DEL_MARGIN_W_ADDR
    REG_16(0x82207DF6) = 0;        // DP_AUDIO_DEC_FRAME_LENGTH
    REG_16(0x82207B14) = 0x8000u;  // DP_ASP_FORCE_KT_8K
    REG_16(0x82CA000C) |= 0x10u;   // DSP_IO4_On()
    REG_16(0x82CA000C) |= 0x20u;   // DSP_IO5_On()
    REG_16(0x82CA000C) |= 0x40u;   // DSP_IO6_On()
}

void AM_DSP_AfeOn(void)
{
    AFE_VMCU_CON1 &= ~0x0200;
    DSP_SCR_CTRL = 0x8; // AM_DSP_AfeOn 8
}

#if 1
const uint16_t DSP_PCM_IDLE_DELAY_TABLE[7][3] =
    {
        {0x20, 0x20, 0x4B + (16 << 10) + (2 << 8)},    // for AMR codec
        {0x20, 0x20, 0x96 + (16 << 10) + (2 << 8)},    // for speech codec except AMR
        {0x0A, 0x0A, 0x4F + (16 << 10) + (2 << 8)},    // for PCM8K_Playback.....
        {0x0A, 0x0A, 0x28 + (16 << 10) + (2 << 8)},    // for PCM16K
        {0x0A, 0x0A, 0x9F + (16 << 10) + (2 << 8)},    // for Keytone/Tone/ (4)
        {0x0A, 0x0A, 0x96 + (16 << 10) + (2 << 8)},    // for PCM8K_Playback/VoIP
        {27 * 8, 14 * 8, 0x96 + (16 << 10) + (2 << 8)} // for 3G Speech
};

static void AM_Write_Idle_Delay(uint8_t type)
{
    DP_DEL_PCM_R = DSP_PCM_IDLE_DELAY_TABLE[type][0];
    DP_DEL_PCM_W = DSP_PCM_IDLE_DELAY_TABLE[type][1];
    DP_MARGIN_PCM_W = DSP_PCM_IDLE_DELAY_TABLE[type][2];
    DP_SC_EVLUATE_DELAY = 0;
}
#endif

void AM_DSP_ToneOn(bool on)
{
    if (on)
    {
        AFE_VMCU_CON1 &= ~0x0200; // VMODE4K
        DSP_SCR_CTRL = 4; // AM_DSP_KeyToneOn 4
        AFE_TurnOn8K(DL_PATH);
        AM_Write_Idle_Delay(4); //?! not need
        VBI_Reset();
    }
    else
    {
        VBI_Reset();
        VBI_End();
    }
}

void tone_init(void)
{
    ///ktInit()
    DSP_TONE_AMP2 = 0x3FFF; // 0x82207B20
    //DSP_TONE_AMP1 = 0x3FFF;

    ////AFE_TurnOnFIR(0);

    ////toneInit()
    DSP_KT_ATT = 0;         // 0x82207B22
    DSP_TONE_CTRL1 = 0;     // 0x82207B10
    DSP_TONE_CTRL2 = 0;     // 0x82207B12
    DSP_TONE_CTRL1_AUD = 0; // 0x82207E72
    DSP_TONE_CTRL2_AUD = 0; // 0x82207E74
}

void tone_play(int f1, int a1, int f2, int a2)
{
    DSP_TONE_AMP1 = a1;
    DSP_TONE_AMP2 = a2;
    DSP_TONE_F2A = f1; // freq1
    DSP_TONE_F2B = f2; // freq2
    DSP_TONE_F1A = f1; // freq3
    DSP_TONE_F1B = f2; // freq4
    DSP_TONE_CTRL1 = DSP_TONE_START;
    DSP_TONE_CTRL2 = DSP_TONE_START;
}

void audio_init(void)
{
    dsp_hw_init();

    MD_TOPSM_Init();    // ?!
    MODEM_TOPSM_Init(); // ?!

    idma_load_genral();
    dsp_wakeup();
    dsp_wait_ready();
    dsp_dynamic_download(DDID_Internal_Header_Kernal);

    AM_Init(); // ?!

    tone_init(); 
}

#endif