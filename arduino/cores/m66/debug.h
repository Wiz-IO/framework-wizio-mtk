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

#ifndef _DBG_H_
#define _DBG_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <hal_interface.h>

void dbg_retarget(TUSART *uart);

#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG

int log_printf(const char *frm, ...);
void log_buf(const char *text, const unsigned char *buf, unsigned int len);
void __attribute__((__section__(".ramfunc"))) PANIC(char *str, bool Abort);

void dump16(uint32_t address, int size_shorts);
void dump32(uint32_t address, int size_int);

#   define LOG(FORMAT, ...)          log_printf(FORMAT, ##__VA_ARGS__)
#   define DUMP(TEXT, BUFFER, LEN)   log_buf((const char*)TEXT, (unsigned char *)BUFFER, LEN)

#include <hal/hal_usart.h>

#if 0
#define DBG_BUF_LEN   256
extern char DBG_BUFFER[DBG_BUF_LEN];
#define DBG(FORMAT,...) {\
    sprintf(DBG_BUFFER,FORMAT,##__VA_ARGS__); \
    USART_SendString(USART1, DBG_BUFFER); \
}
#endif

#else
#   define LOG(FORMAT, ...) 
#   define DUMP(TEXT, BUFFER, LEN)
#endif

#ifdef __cplusplus
}
#endif
#endif