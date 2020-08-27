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



#ifndef _TOPSM_H_
#define _TOPSM_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <hal/hal.h>

#define RM_CLK_SETTLE            (*(VU32*)(MCU_TOPSM_base+0x0000))           // Resource Manager Clock Settling Setting       
#define RM_TMR_TRG0              (*(VU32*)(MCU_TOPSM_base+0x0010))           // Resource Manager Timer Cross Trigger Setting  
#define RM_TMR_TRG1              (*(VU32*)(MCU_TOPSM_base+0x0014))           // Resource Manager Timer Cross Trigger Setting  
#define RM_TMR_PWR0              (*(VU32*)(MCU_TOPSM_base+0x0018))           // Resource Manager SUBSYS Power Setting         
#define RM_TMR_PWR1              (*(VU32*)(MCU_TOPSM_base+0x001C))           // Resource Manager SUBSYS Power Setting         
#define RM_PERI_CON              (*(VU32*)(MCU_TOPSM_base+0x0030))           // Resource Manager Peripheral Pause Control     
#define RM_TMR_SSTA              (*(VU32*)(MCU_TOPSM_base+0x0040))           // Resource Manager Timer Sleep Control State    
#define TOPSM_DBG                (*(VU32*)(MCU_TOPSM_base+0x0050))           // Sleep Manager Debug Control               

#define FRC_SYNC1_STA            (*(VU32*)(MCU_TOPSM_base+0x0070))           // TQ_CNT to FRC sync status 
#define FRC_CON                  (*(VU32*)(MCU_TOPSM_base+0x0080))           // Free Running Counter Control Register         
#define FRC_F32K_FM              (*(VU32*)(MCU_TOPSM_base+0x0084))           // F32K_CK Frequency Measurement Value           
#define FRC_VAL_R                (*(VU32*)(MCU_TOPSM_base+0x0088))           // Free Running Counter Current Value               
#define FRC_SYNC_1L              (*(VU32*)(MCU_TOPSM_base+0x0090))           // Free Running Counter sync result in 1/26 us   
#define FRC_SYNC_1H              (*(VU32*)(MCU_TOPSM_base+0x0094))           // Free Running Counter sync result in us        
#define FM_CON                   (*(VU32*)(MCU_TOPSM_base+0x00A0))           // Frequency Measurement Control Register        
#define FM_CAL                   (*(VU32*)(MCU_TOPSM_base+0x00A4))           // Frequency Measurement Calibration Control     
#define FM_T0                    (*(VU32*)(MCU_TOPSM_base+0x00A8))           // Frequency Measurement Start Time              
#define FM_T1                    (*(VU32*)(MCU_TOPSM_base+0x00AC))           // Frequency Measurement End   Time              
#define RTCCOUNT                 (*(VU32*)(MCU_TOPSM_base+0x0104))           // 32K count 

#define GPS_SYNC_CON0            (*(VU32*)(MCU_TOPSM_base+0x0300))           // GPS strobe timing value of Free Running Counter  
#define GPS_SYNC_CON1            (*(VU32*)(MCU_TOPSM_base+0x0304))           // GPS strobe timing ENABLE of Free Running Counter 

#define RM_PLL_MASK0             (*(VU32*)(MCU_TOPSM_base+0x0830))           // Power Status Register                         
#define RM_PLL_MASK1             (*(VU32*)(MCU_TOPSM_base+0x0834))           // Power Status Register                         
#define RM_SM_TRG_MASK           (*(VU32*)(MCU_TOPSM_base+0x0860))           // Sleep Manager Cross Trigger Control          

#define MCU_TOPSM_SM_SW_WAKEUP   (*(VU32*)(MCU_TOPSM_base+0x08A0))  
#define MCU_TOPSM_SM_REQ_MASK    (*(VU32*)(MCU_TOPSM_base+0x08B0))  
#define MCU_TOPSM_SM_MAS_RDY_STA (*(VU32*)(MCU_TOPSM_base+0x08F0))  

// ----------------------------------------------------------------------------------------------------------------------------- 

//MODEM2G_TOPSM_base 32 bits
#define MODEM2G_TOPSM_RM_TMR_PWR0               (*(VU32*)(MODEM2G_TOPSM_base+0x0018)) /* Resource Manager Timer Power Enable Control 0 */
#define MODEM2G_TOPSM_RM_TMR_PWR1               (*(VU32*)(MODEM2G_TOPSM_base+0x001C)) /* Resource Manager Timer Power Enable Control 1 */
#define MODEM2G_TOPSM_RM_PWR_CON(n)             (*(VU32*)(MODEM2G_TOPSM_base+0x0800+(n)*4)) /* Power Control Register n */
#define MODEM2G_TOPSM_RM_PWR_STA                (*(VU32*)(MODEM2G_TOPSM_base+0x0820))       /* Power Status Register */
#define MODEM2G_TOPSM_SW_CLK_FORCE_ON_FLAG      (*(VU32*)(MODEM2G_TOPSM_base+0x0A00))  
#define MODEM2G_TOPSM_SW_CLK_FORCE_ON_SET       (*(VU32*)(MODEM2G_TOPSM_base+0x0A04))  
#define MODEM2G_TOPSM_SW_CLK_FORCE_ON_CLR       (*(VU32*)(MODEM2G_TOPSM_base+0x0A08))  
#define MODEM2G_TOPSM_SW_PWR_FORCE_ON_SET       (*(VU32*)(MODEM2G_TOPSM_base+0x0A14)) /* software power force on set   */
#define MODEM2G_TOPSM_SW_PWR_FORCE_ON_CLR       (*(VU32*)(MODEM2G_TOPSM_base+0x0A18)) /* software power force on clear */
#define MODEM2G_TOPSM_SW_PWR_STA                (*(VU32*)(MODEM2G_TOPSM_base+0x0A24)) /* Software Power Status */
#define MODEM2G_TOPSM_PROTECT_ACK_MASK          (*(VU32*)(MODEM2G_TOPSM_base+0x0A38))  

// from mt2523
#define DSP_TOPSM_RM_CLK_SETTLE              (*(VU32*)(MODEM2G_TOPSM_base+0x0000)) /* Resource Manager Clock Settling Setting */
#define DSP_TOPSM_RM_TMR_SSTA                (*(VU32*)(MODEM2G_TOPSM_base+0x0040)) /* Resource Manager Timer Sleep Control State*/
#define DSP_TOPSM_TOPSM_DBG                  (*(VU32*)(MODEM2G_TOPSM_base+0x0050)) /* Sleep Manager Debug Control */

#define DSP_TOPSM_RM_PWR_CON0                (*(VU32*)(MODEM2G_TOPSM_base+0x0800)) /* Power Control Register0: Specify MTCMOS SUBSYS Power Control Sequence, and it is controlled by Software mode or hardware mode */
#define DSP_TOPSM_RM_TMR_PWR0                (*(VU32*)(MODEM2G_TOPSM_base+0x0018)) /* Resource Manager Timer Power Enable Control 0 */
#define DSP_TOPSM_RM_PWR_STA                 (*(VU32*)(MODEM2G_TOPSM_base+0x0820)) /* Power Status Register */
#define DSP_TOPSM_RM_PWR_PER0                (*(VU32*)(MODEM2G_TOPSM_base+0x0824)) /* Power COntrol for peripheral */
#define DSP_TOPSM_RM_PLL_MASK0               (*(VU32*)(MODEM2G_TOPSM_base+0x0830)) /* Resource Manager Timer PLL Enable Control 0 */
#define DSP_TOPSM_RM_SM_PLL_MASK0            (*(VU32*)(MODEM2G_TOPSM_base+0x0840)) /* Sleep Manager PLL Enable Control  */
#define DSP_TOPSM_RM_SM_PWR                  (*(VU32*)(MODEM2G_TOPSM_base+0x0850)) /* SLeep Manager  Power Enable Control */
#define DSP_TOPSM_RM_SM_TRG                  (*(VU32*)(MODEM2G_TOPSM_base+0x0860)) /* Sleep Manager cross trigger control */
#define DSP_TOPSM_RM_SM_MASK                 (*(VU32*)(MODEM2G_TOPSM_base+0x0890)) /* Sleep Manager Enable Control*/
#define DSP_TOPSM_SM_SW_WAKEUP	             (*(VU32*)(MODEM2G_TOPSM_base+0x08A0)) /* Sleep Manager Software Wake Up When set those bits, hardware will assert resource request to other topsm.*/
#define DSP_TOPSM_SM_SLV_REQ_IRQ             (*(VU32*)(MODEM2G_TOPSM_base+0x08C0)) /* Sleep Manager Request Interrupt */
#define DSP_TOPSM_SM_MAS_RDY_IRQ             (*(VU32*)(MODEM2G_TOPSM_base+0x08D0)) /* Sleep Manager Ready Interrupt */
#define DSP_TOPSM_SM_TRIG_SETTLE0            (*(VU32*)(MODEM2G_TOPSM_base+0x0900)) /* Sleep Manager Ready Interrupt */
#define DSP_TOPSM_DBG_RM_SM_MASK             (*(VU32*)(MODEM2G_TOPSM_base+0x0880)) /* Sleep Manager Debug Control Mask */
#define DSP_TOPSM_SM_REQ_MASK                (*(VU32*)(MODEM2G_TOPSM_base+0x08B0)) /* Sleep Manager Request Source Mask */
#define DSP_TOPSM_SM_MAS_RDY_STA             (*(VU32*)(MODEM2G_TOPSM_base+0x08F0)) /* Sleep Manager Ready Status Specify subsys topsm receive ready from other topsm */

#define DSP_TOPSM_PROTECT_ACK_MASK           (*(VU32*)(MODEM2G_TOPSM_base+0x0830)) /* Resource Manager Timer PLL Enable Control 0 */

#define DSP_TOPSM_CCF_CLK_CON                (*(VU32*)(MODEM2G_TOPSM_base+0x0200)) /* Clock Control Register */

#define DSP_TOPSM_SW_CLK_FORCE_ON_SET        (*(VU32*)(MODEM2G_TOPSM_base+0x0A04))  /* Software Clock Force On Set*/
#define DSP_TOPSM_SW_CLK_FORCE_ON_CLR        (*(VU32*)(MODEM2G_TOPSM_base+0x0A08))  /* Software Clock Force On Clear*/
#define DSP_TOPSM_INDIV_CLK_PROTECT_ACK_MASK (*(VU32*)(MODEM2G_TOPSM_base+0x0A34))

#ifdef __cplusplus
}
#endif
#endif