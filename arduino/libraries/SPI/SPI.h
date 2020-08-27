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

#ifndef __SPI_CLASS_H__
#define __SPI_CLASS_H__

#ifdef __cplusplus

#include "interface.h"
#include "constants.h"
#include "Stream.h"
#include "variant.h"
#include "RingBuffer.h"

typedef enum
{
    SPI_MODE0 = 0, // CPOL : 0  | CPHA : 0
    SPI_MODE1,     // CPOL : 0  | CPHA : 1
    SPI_MODE2,     // CPOL : 1  | CPHA : 0
    SPI_MODE3      // CPOL : 1  | CPHA : 1
} SPIDataMode;

class SPISettings
{
public:
    //SPISettings(uint32_t clockFrequency, BitOrder bitOrder, SPIDataMode dataMode)
    SPISettings(uint32_t clockFrequency, uint8_t bitOrder, uint8_t dataMode)
    {
        clock = clockFrequency;
        order = bitOrder;
        mode = dataMode;
    }
    SPISettings()
    {
        clock = 1000;
        order = MSBFIRST;
        mode = SPI_MODE0;
    }

private:
    uint16_t clock;
    uint8_t order;
    uint8_t mode;
    friend class SPIClass;
};

class SPIClass
{
public:
    SPIClass() { ctx = SPI_Create(); }
    SPIClass(uint32_t chnnl) { ctx = SPI_Create(); }
    SPIClass(uint32_t chnnl, int miso, int mosi, int clk, int cs) { ctx = SPI_Create(); }

    bool begin()
    {
        if (0 == SPI_Open(ctx))
            return 0 == SPI_Config(ctx);
        return false;
    }

    void beginTransaction(SPISettings settings)
    {
        setFrequency(settings.clock);
        setDataMode(settings.mode);
        setBitOrder(settings.order);
        begin();
    }

    void end() { SPI_Close(ctx); }

    byte transfer(uint8_t data)
    {
        byte rd = 0;
        SPI_Transfer(ctx, &data, &rd, 1);
        return rd;
    }

    uint16_t transfer16(uint16_t data)
    {
        union {
            uint16_t val;
            struct
            {
                uint8_t lsb;
                uint8_t msb;
            };
        } t;
        t.val = data;
        if (ctx->attr.rx_mlsb == SPI_LSB)
        {
            t.lsb = transfer(t.lsb);
            t.msb = transfer(t.msb);
        }
        else
        {
            t.msb = transfer(t.msb);
            t.lsb = transfer(t.lsb);
        }
        return t.val;
    }

    void transfer(void *buf, size_t count)
    {
        SPI_Transfer(ctx, (uint8_t *)buf, (uint8_t *)buf, count);
    }

    void setBitOrder(uint8_t order)
    {
        if (order == LSBFIRST)
        {
            ctx->attr.rx_mlsb = SPI_LSB;
            ctx->attr.tx_mlsb = SPI_LSB;
        }
        else
        {
            ctx->attr.rx_mlsb = SPI_MSB;
            ctx->attr.tx_mlsb = SPI_MSB;
        }
    }

    void setDataMode(uint8_t mode)
    {
        switch (mode)
        {
        case SPI_MODE1:
            ctx->attr.clk_polarity = SPI_CPOL_0;
            ctx->attr.clk_format = SPI_CPHA_1;
            break;
        case SPI_MODE2:
            ctx->attr.clk_polarity = SPI_CPOL_1;
            ctx->attr.clk_format = SPI_CPHA_0;
            break;
        case SPI_MODE3:
            ctx->attr.clk_polarity = SPI_CPOL_1;
            ctx->attr.clk_format = SPI_CPHA_1;
            break;
        default:
            ctx->attr.clk_polarity = SPI_CPOL_0;
            ctx->attr.clk_format = SPI_CPHA_0;
            break;
        }
    }

    void setFrequency(uint16_t kHz, bool update = false) { SPI_SetSpeed(ctx, kHz, update); }
    void setClockDivider(uint8_t div) { SPI_SetSpeed(ctx, SPI_FREQ / div, false); }

    void endTransaction(void) {}
    void usingInterrupt(int interruptNumber) {}
    void notUsingInterrupt(int interruptNumber) {}
    void attachInterrupt(){};
    void detachInterrupt(){};

protected:
    spi_context *ctx;
};

extern SPIClass SPI;

#endif // __cplusplus
#endif //__SPI_CLASS_H__