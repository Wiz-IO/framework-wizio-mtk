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

#ifndef _AFE_H_
#define _AFE_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <hal/hal.h>
#include <hal/hal_topsm.h>
#include <hal/hal_pmu.h>

#define ASP_FS_8K                        0x00
#define ASP_FS_11K                       0x11
#define ASP_FS_12K                       0x12
#define ASP_FS_16K                       0x20
#define ASP_FS_22K                       0x21
#define ASP_FS_24K                       0x22
#define ASP_FS_32K                       0x40
#define ASP_FS_44K                       0x41
#define ASP_FS_48K                       0x42

#define DL_PATH                           0 // output
#define UL_PATH                           1 // input
#define DL_UL_BOTH_PATH                   2


//=============================================================================================
// Digital AFE
//=============================================================================================

#define AFE_VMCU_CON0       (*(volatile uint16_t*)(AFE_base+0x0000)) /* AFE MCU Control Register                     */
#   define VIRQON                   (1) // Turns on 8k interrupt

#define AFE_VMCU_CON1       (*(volatile uint16_t*)(AFE_base+0x000C)) /* AFE Voice Analog Circuit Control Register 1  */
#   define VRSDON                   (1 << 7)
#   define VMODE4K                  (1 << 9)
#   define VMODE32K                 (1 << 10)
#   define DUAL_MIC                 (1 << 12)

#define AFE_VMCU_CON2       (*(volatile uint16_t*)(AFE_base+0x0010))    
#   define VSDM_GAIN(v)             (((v) & 0x3F) << 0)
#   define VTX_CK_PHASE             (1 << 11)
#   define VDC_COMP_EN              (1 << 15)   

#define AFE_VMCU_CON3       (*(volatile uint16_t*)(AFE_base+0x001C)) 
#   define SDM_CK_PHASE             (1 << 0)
#   define SDMLP_DLTOUL             (1 << 4)
#   define VSDN_DATA_MONO           (1 << 5)
#   define VSDMLP_ULTODL            (1 << 8)

#define AFE_VDB_CON         (*(volatile uint16_t*)(AFE_base+0x0014)) /* AFE Voice DAI Blue Tooth Control Register    */
#   define VBTSLEN(v)               (((v) & 0x07) << 0)
#   define VBTSYNC                  (1 << 3)
#   define VBTON                    (1 << 4)
#   define VDAION                   (1 << 5)
#   define VBT_LOOP                 (1 << 10)
#   define VBT_LOOP_BACK            (1 << 12)
#   define PCM_CK_MODE_256K         (0 << 14)
#   define PCM_CK_MODE_512K         (1 << 14)
#   define PCM_CK_MODE_1024K        (2 << 14)
#   define PCM_CK_MODE_2048K        (3 << 14)

#define AFE_VLB_CON         (*(volatile uint16_t*)(AFE_base+0x0018)) /* AFE Voice Loop-back Mode Control Register    */
#   define VDECINMODE               (1 << 0)
#   define VINTINMODE               (1 << 1)
#   define VDAPIN_CH0               (1 << 2)
#   define VDAPIN_CH1               (1 << 3)
#   define VDSPCSMODE               (1 << 4)
#   define VDSPBYPASS               (1 << 5)
#   define VININSEL                 (1 << 6)
#   define ENGEN_OPT                (1 << 7)

#define AFE_AMCU_CON0       (*(volatile uint16_t*)(AFE_base+0x0020)) /* AFE Audio MCU Control Register Register      */
#   define AIRQON                   (1 << 0)

#define AFE_AMCU_CON1       (*(volatile uint16_t*)(AFE_base+0x0024)) // HW Mute
#   define AMUTEL                   (1 << 2)
#   define AMUTER                   (1 << 3)
#   define ARAMPSP(v)               (((v) & 0x03) << 4)
#   define AFS(v)                   (((v) & 0x0F) << 6)
#   define I2S_1XOUT_SEL            (1 << 12)
#   define MONO_SEL                 (1 << 14)
#define AFE_SET_MONO()              AFE_AMCU_CON1 |= MONO_SEL

#define AFE_EDI_CON         (*(volatile uint16_t*)(AFE_base+0x0028)) /* AFE Audio EDI(I2S , EIAJ) Control Register   */
#   define EDI_EN                      (1 << 0)
#   define EDI_FMT                     (1 << 1)
#   define EDI_WCYCLE(v)               (((v) & 0x1F) << 2)
#   define EDI_DIR                     (1 << 8)
#   define EDI_LPBK_MODE               (1 << 9)
#   define ULTOEDI                     (1 << 10)
#   define I2S_OUT_MODE_1X             (0 << 12)
#   define I2S_OUT_MODE_2X             (1 << 12)
#   define I2S_OUT_MODE_4X             (2 << 12)
#   define UL_TOI2SDSP                 (1 << 14)
#   define EDI_PAD_EN                  (1 << 15)

#define AFE_AMCU_CON2       (*(volatile uint16_t*)(AFE_base+0x002C))
#   define ASDM_GAIN(v)                (((v) & 0x3F) << 0)
#   define EDI_SEL                     (1 << 6)
#   define PREDIT_EN                   (! << 10)
#   define EDI_WS_OPTION               (1 << 14)                                                       
#   define ADC_COMP_EN                 (! << 15)

#define AFE_DAC_TEST        (*(volatile uint16_t*)(AFE_base+0x0030)) /* AUDIO/VOICE sinewave generator */
#   define FREQ_DIV(v)                 (((v) & 0x00FF) << 0)
#   define AMP_DIV(v)                  (((v) & 0x07) << 8)
#   define SIN_OUT_MUTE                (1 << 13)
#   define ADAC_AUDIO                  (0 << 14)
#   define ADAC_SINUS                  (1 << 14)
#   define VDAC_VOICE                  (0 << 15)
#   define VDAC_SINUS                  (1 << 15)

#define AFE_VAM_SET        (*(volatile uint16_t*)(AFE_base+0x0034)) /* Audio/Voice Interactive Mode Setting */
#   define PER_VAL(v)                  (((v) & 0x07) << 0)
#   define AIRQ2VIRQ                   (1 << 15) // 0: Voice interrupt/audio interrupt, 1: Audio interrupt/no interrupt

#define AFE_AMCU_CON3      (*(volatile uint16_t*)(AFE_base+0x0038))
#   define ARE_A2(v)                   (((v) & 0x0FFF) << 0)

#define AFE_AMCU_CON4      (*(volatile uint16_t*)(AFE_base+0x003C))
#   define ARE_A3(v)                   (((v) & 0x0FFF) << 0)      

#define AFE_AMCU_CON5      (*(volatile uint16_t*)(AFE_base+0x0180))
#   define SDM_CK_PHASE                (1 << 0)
#   define ASDMLP_ULTODL               (1 << 4)
#   define ASDM_DATA_MONO              (1 << 5)
#   define SDMLP_ULTODL                (1 << 8)

#define AFE_AMCU_CON6      (*(volatile uint16_t*)(AFE_base+0x0184))
#   define RCH_AUDIO_DCOFFSET_VALUE(v) (v)

#define AFE_AMCU_CON7      (*(volatile uint16_t*)(AFE_base+0x0188))
#   define LCH_AUDIO_DCOFFSET_VALUE(v) (v)

#define AFE_VMCU_CON4      (*(volatile uint16_t*)(AFE_base+0x01A0))
#   define DC_OFFSET_VALUE(v)          (v)

#define AFE_VMCU_CON6      (*(volatile uint16_t*)(AFE_base+0x01A8)) 

#define AFE_MCU_CON0       (*(volatile uint16_t*)(AFE_base+0x0210)) 
#   define AFE_ON                      (1 << 0)

#define AFE_MCU_CON1       (*(volatile uint16_t*)(AFE_base+0x0214))  
#   define A_IF_UL_ON                  (1 << 0)
#   define UDSP_UL_ON                  (1 << 1)
#   define A_IF_DL_ON                  (1 << 2)
#   define UDSP_DL_ON                  (1 << 3)

#define AFE_DBG_MD_CON0    (*(volatile uint16_t*)(AFE_base+0x0194)) 

//=============================================================================================
// Analog AFE
//=============================================================================================

#define ABBA_WR_PATH0      (*(volatile uint16_t*)(ABBSYS_base+0x0000))
#define ABBA_VBITX_CON0    (*(volatile uint16_t*)(ABBSYS_base+0x0050))
#   define RG_VCFG_1                  (0x1 << 10) 
#   define RG_VPWDB_PGA               (0x1 << 5)   

#define ABBA_VBITX_CON1    (*(volatile uint16_t*)(ABBSYS_base+0x0054))
#   define RG_VPWDB_ADC               (0x1 << 6)
#   define RG_VREF24_EN               (0x1 << 2)  
#   define RG_VCM14_EN                (0x1 << 1) 

#define ABBA_VBITX_CON2    (*(volatile uint16_t*)(ABBSYS_base+0x0058))
#define ABBA_VBITX_CON3    (*(volatile uint16_t*)(ABBSYS_base+0x005C))
#define ABBA_VBITX_CON4    (*(volatile uint16_t*)(ABBSYS_base+0x0060)) 
#   define RG_VPWDB_MBIAS             (0x1 << 1)

#define ABBA_VBITX_CON5    (*(volatile uint16_t*)(ABBSYS_base+0x0064)) 

#define ABBA_AUDIODL_CON0  (*(volatile uint16_t*)(ABBSYS_base+0x0080)) // HS buffer power on
#   define RG_AUDIBIASPWRUP           (0x1)
#   define RG_AUDHPRPWRUP             (0x1 << 2)
#   define RG_AUDHPLPWRUP             (0x1 << 3)              
#   define RG_AUDHSPWRUP              (0x1 << 4)
#   define RG_AUDDACRPWRUP            (0x1 << 5)
#   define RG_AUDDACLPWRUP            (0x1 << 6)

#define ABBA_AUDIODL_CON1  (*(volatile uint16_t*)(ABBSYS_base+0x0084)) 
#define ABBA_AUDIODL_CON2  (*(volatile uint16_t*)(ABBSYS_base+0x0088))
#define ABBA_AUDIODL_CON3  (*(volatile uint16_t*)(ABBSYS_base+0x008C)) 
#define ABBA_AUDIODL_CON4  (*(volatile uint16_t*)(ABBSYS_base+0x0090)) 
#   define RG_DEPOP_VCM_EN            (0x1)
#   define RG_DEPOP_CHARGEOPTION      (0x1 << 5)
#   define RG_ADEPOP_EN               (0x1 << 6)

#define ABBA_AUDIODL_CON5  (*(volatile uint16_t*)(ABBSYS_base+0x0094)) 
#define ABBA_AUDIODL_CON6  (*(volatile uint16_t*)(ABBSYS_base+0x0098))  
#define ABBA_AUDIODL_CON7  (*(volatile uint16_t*)(ABBSYS_base+0x009C)) 
#define ABBA_AUDIODL_CON8  (*(volatile uint16_t*)(ABBSYS_base+0x00A0)) 
#   define RG_LCLDO_TBST_EN           (0x1 << 1)  //Audio/Voice TOP bias current from ABB TOP

#define ABBA_AUDIODL_CON9  (*(volatile uint16_t*)(ABBSYS_base+0x00A4)) 
#define ABBA_AUDIODL_CON10 (*(volatile uint16_t*)(ABBSYS_base+0x00A8))
#   define RG_AUDHSBIAS               (0x1 << 12)

#define ABBA_AUDIODL_CON11 (*(volatile uint16_t*)(ABBSYS_base+0x00AC))
#define ABBA_AUDIODL_CON12 (*(volatile uint16_t*)(ABBSYS_base+0x00B0))
#   define audzcdenable               (0x1)

#define ABBA_AUDIODL_CON13 (*(volatile uint16_t*)(ABBSYS_base+0x00B4))
//HS Default 0dB => [14:10] = 01000b
//HP Default 0dB => [4:0] = 10111'b for L, [9:5] = 10111'b for R

#define ABBA_AUDIODL_CON14 (*(volatile uint16_t*)(ABBSYS_base+0x00B8))
#define ABBA_AUDIODL_CON15 (*(volatile uint16_t*)(ABBSYS_base+0x00BC))
#define ABBA_AUDIODL_CON16 (*(volatile uint16_t*)(ABBSYS_base+0x00C0)) // mute R and L at digital
#   define RG_AMUTER                  (0x1 << 1)
#   define RG_AMUTEL                  (0x1) 

#define ABBA_AUDIODL_CON17 (*(volatile uint16_t*)(ABBSYS_base+0x00C4))
#define ABBA_AUDIODL_CON18 (*(volatile uint16_t*)(ABBSYS_base+0x00C8))

#define MD2GSYS_CG_CON0             (*(volatile uint16_t*)(MD2GCONFG_base + 0x000))                 // Power Down Control 0 Register
#define MD2GSYS_CG_CON2             (*(volatile uint16_t*)(MD2GCONFG_base + 0x008))                 // Power Down Control 2 Register
#   define PDN_CON2_VAFE               (1 << 8)  // Controls the Audio Front End of VBI Power Down
#   define PDN_CON2_AAFE               (1 << 12) // Controls the Audio Front End of MP3 Power Down 
#define MD2GSYS_CG_CON4             (*(volatile uint16_t*)(MD2GCONFG_base + 0x030))                 // Power Down Control 4 Register

#define MD2GSYS_CG_SET0             (*(volatile uint16_t*)(MD2GCONFG_base + 0x010))                 // Power Down Disable 0 Register
#define MD2GSYS_CG_SET2             (*(volatile uint16_t*)(MD2GCONFG_base + 0x018))                 // Power Down Disable 2 Register
#define MD2GSYS_CG_SET4             (*(volatile uint16_t*)(MD2GCONFG_base + 0x034))                 // Power Down Disable 4 Register

#define MD2GSYS_CG_CLR0             (*(volatile uint16_t*)(MD2GCONFG_base + 0x020))                 // Power Down Enable 0 Register
#define MD2GSYS_CG_CLR2             (*(volatile uint16_t*)(MD2GCONFG_base + 0x028))                 // Power Down Enable 2 Register
#define MD2GSYS_CG_CLR4             (*(volatile uint16_t*)(MD2GCONFG_base + 0x038))                 // Power Down Enable 4 Register

void AFE_Chip_Init(void);
void AFE_TurnOn8K(int path);
void AFE_TurnOff8K(void);
void AFE_TurnOnAudioClock(int clock, int path);
void AFE_SetSamplingRate(uint32_t freq);

void AFE_Switch_IntAmp(bool on);

void AFE_SwitchHSon(void); // AFE_Switch_VoiceDAC_VoiceAMP
void AFE_SwitchHPon(void); // AFE_Switch_VoiceDAC_AudAMP

void AFE_HardwareMuteCtrl(bool mute);

#ifdef __cplusplus
}
#endif
#endif /* _AFE_H_ */