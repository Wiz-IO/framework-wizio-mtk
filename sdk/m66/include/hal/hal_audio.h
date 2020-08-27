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

#ifndef _AUDIO_H_
#define _AUDIO_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <hal/hal.h>
#include <hal/hal_topsm.h>
#include <hal/hal_afe.h>
#include <hal/inc_dpram.h>
#include <hal/inc_dsp_ram.h>

// reset and unlock set FD216
#define SHARE_DSPCON                (*(volatile uint16_t*)(SHAREG2_base+0x0000)) /* DSP Control Register */
#   define DSP_RESET_BEGIN             0x0006
#   define DSP_RESET_END               0x0007
#define SHARE_M2DI1                 (*(volatile uint16_t*)(SHAREG2_base+0x0004)) /* MCU-to-DSP Interrupt1 Register */
#define SHARE_M2DI2                 (*(volatile uint16_t*)(SHAREG2_base+0x0008)) /* MCU-to-DSP Interrupt2 Register  */
#define VBI_Reset()                 SHARE_M2DI1 = 0x0838  
#define VBI_End()                   SHARE_M2DI1 = 0x0839 
#define HW_FD216_RESET()                                     \
   {                                                         \
      SHARE_DSPCON = DSP_RESET_BEGIN; /* reset FD216 */      \
      delay_m(1);                                            \
      SHARE_DSPCON = DSP_RESET_END; /* unlock reset FD216 */ \
   }

void audio_init(void);
void AM_DSP_ToneOn(bool on);
void AM_DSP_AfeOn(void);



void tone_init(void);
void tone_play(int f1, int a1, int f2, int a2);

#ifdef __cplusplus
}
#endif
#endif /* _AUDIO_H_ */