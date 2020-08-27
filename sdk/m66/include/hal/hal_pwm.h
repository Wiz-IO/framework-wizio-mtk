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

#ifndef _PWM_H_
#define _PWM_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <hal/hal.h>

#define	PWM4_CTRL		        (*(volatile uint16_t*)(PWM2_base+0x0000)) //PWM Control
#define	PWM4_COUNT		        (*(volatile uint16_t*)(PWM2_base+0x0004)) //PWM max counter value
#define	PWM4_THRESHOLD	        (*(volatile uint16_t*)(PWM2_base+0x0008)) //PWM threshold value

#if 0
#define PWM1_CTRL		        (*(volatile uint16_t*)(PWM_base+0x0000)) //PWM Control
#define PWM1_COUNT 	            (*(volatile uint16_t*)(PWM_base+0x0004)) //PWM max counter value
#define PWM1_THRESHOLD 		    (*(volatile uint16_t*)(PWM_base+0x0008)) //PWM threshold value

#define PWM2_CTRL		        (*(volatile uint16_t*)(PWM_base+0x0024)) //PWM Control
#define PWM2_COUNT  		    (*(volatile uint16_t*)(PWM_base+0x0028)) //PWM max counter value
#define PWM2_THRESHOLD 		    (*(volatile uint16_t*)(PWM_base+0x002C)) //PWM threshold value

#define PWM3_CTRL		        (*(volatile uint16_t*)(PWM2_base+0x0018)) //PWM3 Control
#define PWM3_COUNT  		    (*(volatile uint16_t*)(PWM2_base+0x001C)) //PWM3 max counter value
#define PWM3_THRESHOLD 		    (*(volatile uint16_t*)(PWM2_base+0x0020)) //PWM3 threshold value
#endif

#define	PWM_CTRL_EN		        0x8000
#define	PWM_CTRL_CLK_1		    0x0000
#define PWM_CTRL_CLK_2		    0x0001
#define PWM_CTRL_CLK_4		    0x0002
#define PWM_CTRL_CLK_8		    0x0003
#define PWM_CTRL_CLKSEL         0x0004


#ifdef __cplusplus
}
#endif
#endif /* _PWM_H_ */
