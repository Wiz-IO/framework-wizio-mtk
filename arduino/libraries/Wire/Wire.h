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

#ifndef __WIRE_H__
#define __WIRE_H__
#ifdef __cplusplus

#include "interface.h"
#include "Stream.h"
#include "variant.h"
#include "RingBuffer.h"

/* !!! MAX Transaction is 8 bytes !!! */
#define I2C_MAX_BUFFER_SIZE (8)
#define WIRE_MAX I2C_MAX_BUFFER_SIZE /* Adafruit SDD1306 */

class TwoWire : public Stream
{
private:
  void set_speed(uint32_t speed)
  {
    ctx->config.Mode = I2C_TRANSACTION_LO_MODE;
    speed /= 1000;
    ctx->config.HiSpeed = speed;
    ctx->config.LoSpeed = speed;
    I2C_RecalculateSpeed(ctx);
  }

public:
  TwoWire(uint8_t port, uint32_t speed_Hz = 100000)
  {
    ctx = I2C_Create();
    set_speed(speed_Hz);
    transmissionBegun = false;
  }

  ~TwoWire() { free(ctx); }

  void setIO(uint8_t io) { ctx->config.IO = io; }

  void setClock(uint32_t speed_Hz)
  {
    set_speed(speed_Hz);
    I2C_UpdateSpeed(ctx, true);
  }

  void begin() { I2C_Open(ctx); }
  void end() { I2C_Close(ctx); }

  void beginTransmission(uint8_t);
  uint8_t endTransmission(bool stopBit);
  uint8_t endTransmission(void);

  uint8_t requestFrom(uint8_t address, size_t quantity, bool stopBit);
  uint8_t requestFrom(uint8_t address, size_t quantity);

  size_t write(uint8_t data);
  size_t write(const uint8_t *data, size_t quantity);

  virtual int available(void) { return rx.available(); }
  virtual int read(void) { return rx.read_char(); }
  virtual int peek(void) { return rx.peek(); }
  virtual void flush(void) {}

  using Print::write;

  void onService(void){};
  void onReceive(void (*)(int)){};
  void onRequest(void (*)(void)){};

private:
  i2c_context_t *ctx;
  bool transmissionBegun;

  /* !!! The rings size is 1024 but max R/W transaction is 8 bytes !!! */
  RingBuffer rx;
  RingBuffer tx;
  uint8_t tx_fifo[I2C_MAX_BUFFER_SIZE];
  uint8_t rx_fifo[I2C_MAX_BUFFER_SIZE];

  uint8_t write_fifo();
};

extern TwoWire Wire;

#endif // cpp
#endif // h