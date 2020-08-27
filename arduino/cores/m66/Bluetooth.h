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

#ifndef _Bluetooth_h
#define _Bluetooth_h

#ifdef __cplusplus

#include <HardwareSerial.h>
#include <RingBuffer.h>
#include <hal_interface.h>
//#include "debug.h"

class SerialBluetooth : public HardwareSerial
{
private:
    static RingBuffer bt_rxBuffer; // [1024]
    static void bt_irq_handler()
    {
        uint32_t flags = DisableInterrupts();
        while (BTIF_LSR & MASK_LSR_DR)
        {
            if (bt_rxBuffer.availableForStore())
            {
                uint8_t byte = BTIF_RBR;
                bt_rxBuffer.store_char(byte);
                ::printf(".%02X", (int)byte & 0xFF);
            }
            else
                break;
        }
        RestoreInterrupts(flags);
    }

public:
    SerialBluetooth() { bt_rxBuffer.clear(); }
    ~SerialBluetooth() {}

    void begin(unsigned long brg)
    {
        bt_rxBuffer.clear();
        BT_Init(1, bt_irq_handler);
    }
    void end(){}
    int available(void) { return bt_rxBuffer.available(); }
    int peek(void) { return bt_rxBuffer.peek(); }
    int read() { return bt_rxBuffer.read_char(); }
    void flush() {}

    operator bool() { return true; } 
    using Print::write;

    size_t write(uint8_t byte)
    {
        while (!(BTIF_LSR & MASK_LSR_TEMP)) // 1 is empty
        {
        }
        BTIF_THR = byte;
        ::printf("%02X ", (int)byte & 0xFF);
        return 1;
    }
};

#endif
#endif //_Bluetooth_h