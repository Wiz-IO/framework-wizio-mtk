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

#ifndef  _DDLOAD_H_
#define  _DDLOAD_H_

#define MT6261

typedef struct
{
    int Dsp_Start_Addr;
    char *Array_Addr;
    int Array_Len;
} DLL_Func;

typedef struct
{
    DLL_Func *dspFunc;
    char *Compressed_Array_Addr;
    int Compressed_Array_Len;
} DLL_FuncCompress;

typedef struct
{
    DLL_Func dll;
    unsigned char isAlloc;
} DLL_FuncGen;


#define CM_PAGE                                 4

#define CM_PAGE_TCH_DDL                         8

#define CACHE_DSP_MEMTYPE_CM                    0
#define CACHE_DSP_MEMTYPE_PM                    1
#define CACHE_DSP_MEMTYPE_DM                    2

#define DSP_AUDIO_DM_TABLE_PAGE1                5
#define DSP_AUDIO_DM_TABLE_PAGE2                6

/* ------------------------------------------------------------------------- */

#define  DDID_NONE                      0

#define  DDID_DAF                       1
#define  DDID_WAVETABLE                 3
#define  DDID_AMR_CC_SCHEDULE           4
#define  DDID_AAC                       5
#define  DDID_HEADER                    6
#define  DDID_AMRSBC                   14

#define  DDID_EES                       7
#define  DDID_AEC                       7
#define  DDID_NR                        7
#define  DDID_SBSD                      7
#define  DDID_AWB                      11

#if DPCHIP_DDL_WMA_V2
  #define  DDID_WMA_16OB_HighRate      12
  #define  DDID_WMA_44OB               13
  #define  DDID_WMA_44QB_HighRate      15
  #define  DDID_WMA_16OB_LowRate       16
  #define  DDID_WMA_44QB_LowRate       17
#endif

#define DDID_AAC_PLUS                   8
#define DDID_AAC_PLUS_PS               18
#define DDID_AWBDEC                    19
#define DDID_MUSICAM                   20
#define DDID_AWB_PLUS                  21
#define DDID_BSAC                      22
#define DDID_SBC                       23
#define DDID_AudioHeader               24
#define DDID_I2S                       25
#define DDID_COOK_256                  26
#define DDID_COOK_512                  27 
#define DDID_COOK_1024                 28
#define DDID_IPS                       29
#define DDID_VSG                       30
#define DDID_PCM                       31
#define DDID_Internal_AAC_SBR         252
#define DDID_Internal_WMA_44OB_2      253
#define DDID_Internal_ABF             254
#define DDID_Internal_Header_Kernal   255

#define DDID_HW_HUFFMAN_COSIM        5000     // Only for Hardware Huffman cosim
#define DDID_HW_AUDMA_COSIM          5001     // Only for Hardware AuDMA cosim
#define DDID_AUDIO_CACHE_REMAP       5002     // Only for Cache-DSP cosim remapping
#define DDID_AUDIO_DM_REMAP          5003     // Only for DM remapping cosim
#define DDID_SPH_TCH_1               6001     // For SPH ENH seperate into two parts
#define DDID_SPH_TCH_2               6002     // For SPH ENH seperate into two parts
#define DDID_SPH_TCH_3               6003     // For SPH ENH seperate into two parts




/* ------------------------------------------------------------------------- */

// for WMA L2 support
void _idma_load_pm_d2(unsigned short int page, unsigned short int addr, unsigned int len, const unsigned char *image);

void FreeFuncPtr(DLL_FuncGen *dllGenPtr);

int GetPmDllFuncPtr(unsigned int fw_id, DLL_FuncGen *dllResult);

void DSP_DynamicDownload_Init(void);

int DSP_DynamicDownload(int dsp_fw_id);

void aud_7z_res_decompress(unsigned char *dest, int dest_len, unsigned char *src, int src_len);

/* ------------------------------------------------------------------------- */

typedef struct {
    int a;
    int v;
} DSP_INIT_T;

#endif  /*_DDLOAD_H_ */

