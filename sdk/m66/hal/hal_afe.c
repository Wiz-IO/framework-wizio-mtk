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

#include <hal/hal_afe.h>
#include <hal/hal_ustimer.h>
#include <hal/inc_dpram.h>
#include <hal/hal_pmu.h>

void AFE_Switch_IntAmp(bool on) // 1: turn on, 0: turn off, HS
{
    unsigned short restore;
    if (on)
    {
#if !defined(__CLASSK_CP_SUPPORT__)
        //off -> bypass, spk initial setting
        SPK_CON3 = 0x4800;
        SPK_CON6 = 0x0988; // bypass mode
        SPK_CON7 = 0x0180; // vcm fast startup
        delay_u(2000);     // wait 2ms
        SPK_CON7 = 0x0100;
#endif
        ABBA_AUDIODL_CON0 |= RG_AUDHSPWRUP; // HS buffer power on
        delay_u(10);
        ABBA_AUDIODL_CON1 |= 0x0004;                       // spk mode
        ABBA_AUDIODL_CON12 &= (~audzcdenable);             // for adjust gain with ramp
        restore = ABBA_AUDIODL_CON13;                      // set to 0dB
        ABBA_AUDIODL_CON13 = (restore & ~0x7C00) | 0x2000; // Default 0dB => [14:10] = 01000
#if __SPK_DC_COMPENSATION__
////removed
#endif
        SPK_CON7 |= 0x0080;    // fast VCM track
        SPK_CON0 |= RG_SPK_EN; // 0x0001; // enable spk
        delay_u(2000);
        SPK_CON3 |= RG_SPK_IN_FLOAT_B; // 0x4000;
        SPK_CON7 &= (~0x0080);         // vcm high PSRR mode

        {
            short targ_gain = (short)(restore & 0x7C00);
            short curr_gain = (short)(ABBA_AUDIODL_CON13 & 0x7C00);
            short gain_step = targ_gain >= curr_gain ? 0x0400 : -0x0400;
            while (curr_gain != targ_gain)
            {
                curr_gain += gain_step;
                ABBA_AUDIODL_CON13 = (unsigned short)curr_gain | (restore & ~0x7C00);
                delay_u(500);
            }
        }

        ABBA_AUDIODL_CON1 |= 0x6000; // ZCD: handset mode
        ABBA_AUDIODL_CON12 |= (audzcdenable);
    }
    else
    {
        ABBA_AUDIODL_CON1 &= (~0x0004);
        SPK_CON3 |= 0x0800;
        SPK_CON0 &= (~0x0001);
        SPK_CON3 &= (~0x4000);
        SPK_CON7 &= (~0x0400);
#if __SPK_DC_COMPENSATION__
////removed
#endif
    }
#if __AFE_CLASSK_SUPPORT_INTERNAL__
////removed
#endif
}

void AFE_DCLK_CtrlSeq(bool on, bool audio, int path)
{
    uint32_t savedMask = DisableInterrupts();

    if (on)
    {
        REG_16(0xA001022C) |= 0x0013; // HW_MISC |= 0x0013;

        ABBA_AUDIODL_CON8 |= (RG_LCLDO_TBST_EN);
        ABBA_AUDIODL_CON10 = 0x1406; //LDO:2.4v
        //ABBA_AUDIODL_CON9 |= 0x0001;
        ABBA_AUDIODL_CON11 |= 0x0003;
        delay_u(1);
        if (audio)
            ABBA_AUDIODL_CON7 |= 0x0003;
        else
            ABBA_AUDIODL_CON7 |= 0x0002;

        ABBA_AUDIODL_CON0 |= (RG_AUDDACRPWRUP | RG_AUDDACLPWRUP);
        ABBA_AUDIODL_CON15 = 0x11A1;
        ABBA_AUDIODL_CON14 |= 0x0001;

        MD2GSYS_CG_CLR2 = PDN_CON2_VAFE; // power on, REG_32(0x82C00028) = 0x100;

        if (audio)
        {
            AFE_MCU_CON1 |= 0x000C;  // A_IF_DL_ON | UDSP_DL_ON
            AFE_AMCU_CON0 |= 0x0001; // AIRQON
        }
        else
        {
            switch (path)
            {
            case DL_PATH:
                AFE_MCU_CON1 |= 0x000C;
                break;
            case UL_PATH:
                AFE_MCU_CON1 |= 0x0003;
            default: // DL_UL_BOTH_PATH:
                AFE_MCU_CON1 |= 0x000F;
                break;
            }
            AFE_VMCU_CON0 |= 0x0001; // // Turns on 8k interrupt
        }
        AFE_MCU_CON0 = 0x0001; // Turns on the audio front end
    }
    else // off
    {
        ABBA_AUDIODL_CON0 &= (~(RG_AUDDACRPWRUP | RG_AUDDACLPWRUP));
        ABBA_AUDIODL_CON7 &= (~0x0003);
        ABBA_AUDIODL_CON11 &= (~0x0003);
        AFE_MCU_CON0 &= (~0x0001); // Turns off the audio front end
        if (audio)
        {
            AFE_AMCU_CON0 &= (~0x0001);
        }
        else
        {
            AFE_VMCU_CON0 &= (~0x0001);
        }
        AFE_MCU_CON1 &= (~0x000F);

        MD2GSYS_CG_SET2 = PDN_CON2_VAFE; // power off, REG_32(0x82C00018) = 0x100;

        ABBA_AUDIODL_CON14 &= (~0x0001);
        ABBA_AUDIODL_CON15 = 0x1021;

        //*ABBA_AUDIODL_CON9 &= (~0x0001);
        REG_16(0xA001022C) &= (~0x0010);
    }

    RestoreInterrupts(savedMask);
}

void AFE_SetSamplingRate(uint32_t freq)
{
    uint16_t SampleRate;
    switch (freq)
    {
    case ASP_FS_8K:
        SampleRate = 0;
        break;
    case ASP_FS_11K:
        SampleRate = 1;
        break;
    case ASP_FS_12K:
        SampleRate = 2;
        break;
    case ASP_FS_16K:
        SampleRate = 4;
        break;
    case ASP_FS_22K:
        SampleRate = 5;
        break;
    case ASP_FS_24K:
        SampleRate = 6;
        break;
    case ASP_FS_32K:
        SampleRate = 8;
        break;
    case ASP_FS_44K:
        SampleRate = 9;
        break;
    case ASP_FS_48K:
        SampleRate = 10;
        break;
    default:
        SampleRate = 0;
        break;
    }
    short tmp = AFE_AMCU_CON1 & (~0x03C0);
    AFE_AMCU_CON1 = tmp | (SampleRate << 6);
}

// run voice
void AFE_TurnOn8K(int path)
{
    if ((AFE_AMCU_CON0 & 0x0001) == 0) // AIRQON
        AFE_VMCU_CON3 |= 0x0020;       // VSDN_DATA_MONO
    AFE_DCLK_CtrlSeq(true, false, path);
    DSP_8K_LIMITER_CTRL &= (~0x1);
}

void AFE_TurnOff8K(void)
{
    //if(afe.voice_clk_ena)
    {
        AFE_MCU_CON0 = 0x0000;
        AFE_VMCU_CON0 &= (~0x0001);
        AFE_MCU_CON1 &= (~0x000F);
        //if(!afe.audio_clk_ena)
        {
            REG_32(0x82C00018) |= 0x100; // MD2GSYS_CG_SET2 |= PDN_CON2_VAFE;
        }
    }
}

// run audio
void AFE_TurnOnAudioClock(int clock, int path)
{
    AFE_SetSamplingRate(clock);
    AFE_DCLK_CtrlSeq(true, true, path);
}

// default init for all MT62xx
void AFE_Chip_Init(void)
{
    //Digital part Initialization
    AFE_AMCU_CON1 = 0x0E00; // 24
    AFE_AMCU_CON5 = 0x0002; // 180
    AFE_VMCU_CON3 = 0x0002; // 1C
    AFE_VMCU_CON2 = 0x083C; // 10 //// VTX_CK_PHASE, VSDM_GAIN=3C
    AFE_AMCU_CON2 = 0x3C;   // 2C //// ASDM_GAIN = 3C
    AFE_VMCU_CON1 = 0x80;   // 0C //// VRSDON

    //Uplink PGA Gain : 6dB
    ABBA_VBITX_CON0 |= (0x2 << 6); //0x80

    //Analog part Initialization and power-on control sequence
    ABBA_AUDIODL_CON8 |= (RG_LCLDO_TBST_EN);
    ABBA_AUDIODL_CON4 = 0x01B9;
    delay_u(10);
    ABBA_AUDIODL_CON4 |= 0x0040;

    // should wait 2s~3s to charge cap
    ABBA_AUDIODL_CON10 = 0x1406; // LDO:2.4v
    ABBA_AUDIODL_CON9 |= 0x0001;

    //Speaker Amp setting
    SPK_CON0 = ((SPK_CON0 & 0xFFCF) | (3 << 4)); // SpkAmpGain << 4

    //printf("[AFE] INIT\n");
}

void AFE_HardwareMuteCtrl(bool mute)
{
    if (mute)
    {
        AFE_AMCU_CON1 |= 0x3C;
    }
    else
    {
        AFE_AMCU_CON1 &= ~0x0C;
    }
}

void AFE_SwitchHSon(void) //???? AFE_Switch_VoiceDAC_VoiceAMP
{
    unsigned short restore;
    ABBA_AUDIODL_CON11 |= 0x0003;
    ABBA_AUDIODL_CON7 |= 0x0003;
    ABBA_AUDIODL_CON0 |= (RG_AUDDACRPWRUP | RG_AUDDACLPWRUP);
    ABBA_AUDIODL_CON1 &= (~0x6000);
    ABBA_AUDIODL_CON1 |= 0x6000; // enable HS zcd

    restore = ABBA_AUDIODL_CON13;
    ABBA_AUDIODL_CON13 = (restore & ~0x7C00) | 0x2000; // set to 0dB and enable zcd
    ABBA_AUDIODL_CON12 |= (audzcdenable);    
    ABBA_AUDIODL_CON13 = restore;

    ABBA_AUDIODL_CON1 &= (~0x1FF0);
    ABBA_AUDIODL_CON1 |= 0x0800;
    ABBA_AUDIODL_CON0 |= RG_AUDHSPWRUP; // HS buffer power on

    // force to turn on earphone under spk
    {
        ABBA_AUDIODL_CON0 |= 0x000D; // x80
        ABBA_AUDIODL_CON1 |= 0x0090; // x84
    }
}

void AFE_SwitchHPon(void) // AFE_Switch_VoiceDAC_AudAMP
{
    short restore, tmp;
    ABBA_AUDIODL_CON11 |= 0x0003;
    ABBA_AUDIODL_CON7 |= 0x0003;
    ABBA_AUDIODL_CON0 |= (RG_AUDDACRPWRUP | RG_AUDDACLPWRUP);
    ABBA_AUDIODL_CON1 &= (~0x6000);
    ABBA_AUDIODL_CON1 |= 0x4000; // enable HP zcd

    restore = ABBA_AUDIODL_CON13;
    ABBA_AUDIODL_CON13 = (restore & ~0x03FF) | 0x02F7; // set to 0dB and enable zcd
    ABBA_AUDIODL_CON12 |= (audzcdenable);

    ABBA_AUDIODL_CON0 |= RG_AUDHSPWRUP; // HS buffer power on [must]
    delay_m(10);
    ABBA_AUDIODL_CON5 |= 0x0001;
    delay_m(10);
    ABBA_AUDIODL_CON0 |= 0x0001;
    delay_m(10);
    ABBA_AUDIODL_CON2 |= 0x0001;
    ABBA_AUDIODL_CON0 |= 0x000C;
    ABBA_AUDIODL_CON2 &= (~0x0001);
    //delay_m(5);
    tmp = ABBA_AUDIODL_CON1 & (~0x1FF0); // to avoid set path pop
    ABBA_AUDIODL_CON1 = tmp | 0x0120;
    ABBA_AUDIODL_CON5 &= (~0x0001);
    ABBA_AUDIODL_CON0 &= (~RG_AUDHSPWRUP);
    ABBA_AUDIODL_CON4 &= (~(RG_ADEPOP_EN | RG_DEPOP_VCM_EN));
    ABBA_AUDIODL_CON13 = restore;
}
