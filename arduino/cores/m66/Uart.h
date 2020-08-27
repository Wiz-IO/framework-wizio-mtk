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

#ifndef _Uart_h_
#define _Uart_h_

#ifdef __cplusplus

#include "HardwareSerial.h"
#include <RingBuffer.h>
#include <hal_interface.h>
#include "debug.h"

extern TUSART_CONTEXT USARTINFO[3];

static void u1_rx_handler(void);
static void u2_rx_handler(void);
static void u3_rx_handler(void);

class Uart : public HardwareSerial
{
private:
    TUSART *u;
    pUSART_CONTEXT ctx;
    RingBuffer rx_ring; // [1024]
    uint32_t _brg;

public:
    Uart(TUSART *uart)
    {
        u = uart;
        if (uart == USART1)
        {
            ctx = &USARTINFO[0];
            memset(ctx, 0, sizeof(TUSART_CONTEXT));
            ctx->rx_handler = u1_rx_handler;
        }
        else if (uart == USART1)
        {
            ctx = &USARTINFO[1];
            memset(ctx, 0, sizeof(TUSART_CONTEXT));
            ctx->rx_handler = u2_rx_handler;
        }
        else
        {
            ctx = &USARTINFO[2];
            memset(ctx, 0, sizeof(TUSART_CONTEXT));
            ctx->rx_handler = u3_rx_handler;
        }
        ctx->user = this;
        _brg = 0;
    }

    ~Uart()
    {
        USART_Initialize(u, 0, 0, 0);
        rx_ring.clear();
    }

    void begin(unsigned long brg, bool dbg = false)
    {
        end();
        _brg = brg;
        USART_Initialize(u, brg, ctx->rx_handler, USART_RXInt_EnFlag | USART_EnFlag);
        if (dbg)
            dbg_retarget(u);
    }

    void end(void) { USART_Initialize(u, 0, 0, 0); }

    size_t write(uint8_t c)
    {
        USART_SendChar(u, c); // blocked
        return 1;
    }

    size_t write(uint8_t *buf, size_t size)
    {
        ctx->USART_TX_Buffer = buf;
        ctx->USART_TX_Count = size;
        ctx->USART_TX_Index = 0;
        USART_SendBufferPoll(ctx); // blocked
        return size;
    }

    int read()
    {
        if (!rx_ring.available())
            return 0;
        uint32_t flags = DisableInterrupts();
        uint8_t byte = rx_ring.read_char();
        RestoreInterrupts(flags);
        return byte;
    }

    int read(uint8_t *buf, size_t size)
    {
        int cnt = 0;
        while (available() > 0 && size)
        {
            *buf++ = read();
            size--;
            cnt++;
        }
        return cnt;
    }

    int available(void) { return rx_ring.available(); }

    int peek(void) { return rx_ring.peek(); }

    void flush(void) {} // TODO

    int setSpeed(int brg)
    {
        _brg = brg;
        uint32_t BR = ((2 * USARTDEFCLOCK / USARTDEFOVS) / brg + 1) >> 1; //oversampling by 16, baud rate
        if (BR <= 1)
            return false; //Divider can not be lower or equal 1
        u->LCR |= U_DLAB;
        u->DLM = (BR >> 8) & 0xFF; //Set baud rate
        u->DLL = BR & 0xFF;
        u->LCR &= ~U_DLAB;
        return 0;
    }

    int getSpeed() { return _brg; }

    void save()
    {
        while (u->LSR & 1) // DR
        {
            if (rx_ring.availableForStore())
                rx_ring.store_char(u->RXB);
            else
                break;
        }
    }

    operator bool() { return true; }
    using Print::write;
};

static void u_rx_handler(Uart *s)
{
    uint32_t flags = DisableInterrupts();
    s->save();
    RestoreInterrupts(flags);
}
static void u1_rx_handler(void) { u_rx_handler((Uart *)USARTINFO[0].user); }
static void u2_rx_handler(void) { u_rx_handler((Uart *)USARTINFO[1].user); }
static void u3_rx_handler(void) { u_rx_handler((Uart *)USARTINFO[2].user); }

extern Uart Serial;
extern Uart Serial1;
extern Uart Serial2;

#endif
#endif // _Uart_h_