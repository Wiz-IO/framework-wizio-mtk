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

#include <Arduino.h>
#include "Wire.h"
#include <hal/hal_i2c.h>

/* !!! MAX Transaction is 8 bytes !!! */
uint8_t TwoWire::requestFrom(uint8_t address, size_t quantity, bool stopBit)
{
    ctx->config.SlaveAddress = address;
    int res = -1;
    if (quantity == 0)
        return 0;
    if (!stopBit)
        return 0;
    rx.clear();
    res = I2C_BeginTransaction(ctx, I2C_TRANSACTION_READ, NULL, 0, rx._aucBuffer, quantity);
    if (res < 0)
        quantity = 0;
    rx._iHead = quantity;
    return rx.available();
}

uint8_t TwoWire::requestFrom(uint8_t address, size_t quantity)
{
    return requestFrom(address, quantity, true);
}

void TwoWire::beginTransmission(uint8_t address)
{
    ctx->config.SlaveAddress = address;
    tx.clear();
    transmissionBegun = true;
}

/* !!! MAX Transaction is 8 bytes !!! */
uint8_t TwoWire::endTransmission(bool stopBit)
{
    if (!stopBit)
        return 1;
    transmissionBegun = false;
    int res = -1, count = 0, size = tx.available();
    if (size > I2C_MAX_BUFFER_SIZE)
        size = I2C_MAX_BUFFER_SIZE;
    for (int i = 0; i < size; i++, count++)
        tx_fifo[i] = tx.read_char();
    res = I2C_BeginTransaction(ctx, I2C_TRANSACTION_WRITE, tx_fifo, count, NULL, 0);
    return res ? 4 : 0; // 0:success or 4:other error
}

uint8_t TwoWire::endTransmission()
{
    return endTransmission(true);
}

size_t TwoWire::write(uint8_t ucData)
{
    if (!transmissionBegun || tx.isFull())
        return 0;
    tx.store_char(ucData);
    return 1;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
    for (size_t i = 0; i < quantity; ++i)
    {
        if (!write(data[i]))
            return i;
    }
    return quantity;
}

TwoWire Wire(0); // hard port