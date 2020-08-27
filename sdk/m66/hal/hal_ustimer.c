// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*
* This file is part of the DZ09 project.
*
* Copyright (C) 2019 AJScorp
*
* This program is free software; you can redistribute it and/or modify 
* it under the terms of the GNU General Public License as published by 
* the Free Software Foundation; version 2 of the License.
*
* This program is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with this program; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. 
*/

#include <hal/hal_ustimer.h>

void USC_StartCounter(void)
{
    USCNT_CON = USCNT_START_CODE;
}

void USC_StopCounter(void)
{
    USCNT_CON = USCNT_STOP_CODE;
}

int32_t USC_GetCurrentTicks(void)
{
    return USCNTI_VAL;
}

void USC_Pause_us(uint32_t us)
{
    if (USCNT_CON & USC_START)
    {
        volatile int32_t PrevTime = USC_GetCurrentTicks();

        while (USC_GetCurrentTicks() - PrevTime < us)
            ;
    }
}

void delay_m(uint32_t t)
{
    USC_Pause_us(t * 1000);
}

void delay_s(uint32_t t)
{
    delay_m(t * 1000);
}