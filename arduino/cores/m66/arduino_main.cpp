/*
    Created on: 25.06.2020
    Author: Georgi Angelov
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA   
 */

#include <interface.h>

extern "C" int GFH_Dummy(void);
extern "C" void STD_Dummy(void);
extern "C" void NVIC_Dummy(void);
extern "C" void __sinit(struct _reent *s);
extern "C" void __libc_init_array(void);

extern void setup();
extern void loop();

void system_init(void)
{
    GFH_Dummy();
    STD_Dummy();
    NVIC_Dummy();

#if BOOT_SIZE > 0
    USART_SendString((TUSART *)USART1, (char *)"\n[SYS] INIT BEGIN\n");
    dbg_retarget((TUSART *)USART1);
#endif

    __libc_init_array(); // init cpp
MPU_Initialize(); /* Setup system cache */


#if BOOT_SIZE == 0
    EMI_MemoryRemap(MR_FB1RB0); /* Remap Flash to Bank1, RAM to Bank0. Now ROM starts from 0x10000000 */
    PLL_Initialize();
    PCTL_Initialize();
    GPIO_Initialize();
#endif

#ifdef LINKIT // for LinkitONE
    PCTL_Initialize();
    GPIO_Initialize();
    RGU_DisableWDT(); 
#endif
   
    RTC_Initialize();
    RTC_LockBBPower();
    USC_StartCounter();
    GPT_InitializeTimers();
    NVIC_Initialize();
    EnableInterrupts();

#if BOOT_SIZE > 0
    uint32_t *info = (uint32_t *)0x80000000;
    printf("[SYS] CPU_HW: %04lX\n", *info++);
    printf("[SYS] CPU_SW: %04lX\n", *info++);
    printf("[SYS] CPU_ID: %04lX\n", *info++);
    printf("[SYS] CPU_SB: %04lX\n", *info++);
    //USART_SendString((TUSART *)USART1, (char *)"[SYS] INIT END\n");
#endif
}

extern "C" int main(void)
{
    system_init();
    initVariant();
    setup();
    while (1)
        loop();
}