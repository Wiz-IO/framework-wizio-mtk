////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Georgi Angelov ver 2.0
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
////////////////////////////////////////////////////////////////////////////////////////

#include "debug.h"

void __attribute__((__section__(".ramfunc"))) PANIC(char *Str, bool Abort)
{
    while (Str)
        USART1->TXB = *Str++;
    while (Abort)
        ;
}

static int dbg_write_r(struct _reent *r, _PTR ctx, const char *buf, int len)
{
    TUSART_CONTEXT p;
    if (len)
    {
        p.USART = stdout->_cookie;
        p.USART_TX_Buffer = buf;
        p.USART_TX_Count = len;
        p.USART_TX_Index = 0;
        USART_SendBufferPoll(&p);
    }
    return len;
}

static TUSART *log_uart = 0;
void dbg_retarget(TUSART *uart)
{
    extern void __sinit(struct _reent * s);
    __sinit(_impure_ptr);

    stdout->_cookie = uart;
    stdout->_file = STDOUT_FILENO;
    stdout->_flags = __SWID | __SWR | __SNBF;
    stdout->_write = dbg_write_r; // only write
    setvbuf(stdout, NULL, _IONBF, 0);
    //printf("[SYS] PRINTF DEBUG\n");

    log_uart = uart;
}

#ifdef ENABLE_DEBUG

static char dbgBuffer[1024];
int log_printf(const char *frm, ...)
{
    if (log_uart)
    {
        va_list arg;
        va_start(arg, frm);
        int n = vsnprintf(dbgBuffer, sizeof(dbgBuffer), frm, arg);
        va_end(arg);
        if (n > 0)
            USART_SendString(log_uart, dbgBuffer);
    }
    return 0;
}

static char bufferDump[1024];

void log_buf(const char *text, const unsigned char *buf, unsigned int len)
{
    if (0 == log_uart)
        return;
    char txt[17];
    unsigned int i, idx = 0;
    snprintf(bufferDump + idx, sizeof(bufferDump) - idx, "[DMP] %s 0x%08X (%u bytes)\n", text, (int)buf, len);
    log_printf(bufferDump);
    idx = 0;
    memset(txt, 0, sizeof(txt));
    for (i = 0; i < len; i++)
    {
        if (i >= 4096)
            break;
        if (i % 16 == 0)
        {
            if (i > 0)
            {
                snprintf(bufferDump + idx, sizeof(bufferDump) - idx, "  %s\n", txt);
                log_printf(bufferDump);
                idx = 0;
                memset(txt, 0, sizeof(txt));
            }
            idx += snprintf(bufferDump + idx, sizeof(bufferDump) - idx, "%04X: ", (unsigned int)i);
        }
        idx += snprintf(bufferDump + idx, sizeof(bufferDump) - idx, " %02X", (unsigned int)buf[i]);
        txt[i % 16] = (buf[i] > 31 && buf[i] < 127) ? buf[i] : '.';
    }
    if (len > 0)
    {
        for (/* i = i */; i % 16 != 0; i++)
            idx += snprintf(bufferDump + idx, sizeof(bufferDump) - idx, "   ");
        snprintf(bufferDump + idx, sizeof(bufferDump) - idx, "  %s\n", txt);
        log_printf(bufferDump);
    }
}

void dump16(uint32_t address, int size_shorts)
{
    char hex[32];
    uint16_t *ptr = (uint16_t *)address;
    printf("[DUMP-16] Address = %lX\n", address);
    while (size_shorts > 0)
    {
        memset(bufferDump, 0, sizeof(bufferDump));
        for (int i = 0; i < 16; i++)
        {
            snprintf(hex, 32, "%04X, ", (int)*ptr++);
            strcat(bufferDump, hex);
            if (--size_shorts < 1)
                break;
        }
        printf("%s\n", bufferDump);
    }
}

void dump32(uint32_t address, int size_int)
{
    char hex[32];
    int *ptr = (int *)address;
    printf("[DUMP-32] %lX\n", address);
    while (size_int > 0)
    {
        memset(bufferDump, 0, sizeof(bufferDump));
        for (int i = 0; i < 8; i++)
        {
            snprintf(hex, 32, "%08X, ", *ptr++);
            strcat(bufferDump, hex);
            if (--size_int < 1)
                break;
        }
        printf("%s\n", bufferDump);
    }
}
#endif