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

#include <Arduino.h>
#include <hal_interface.h>

////////////////////////////////////////////////////////////////////////////////////////

static uint32_t __save_mask = 0;
INLINE void interrupts(void) { RestoreInterrupts(__save_mask); };
INLINE void noInterrupts(void) { __save_mask = DisableInterrupts(); };

////////////////////////////////////////////////////////////////////////////////////////

#define ARRAYLEN(x) (sizeof(x) / sizeof((x)[0]))
PinDescription *getPin(uint8_t arduinoPin)
{
    return (arduinoPin >= ARRAYLEN(pinsMap)) ? NULL : &pinsMap[arduinoPin];
}

void pinMode(uint8_t pin, uint8_t mode)
{
    PinDescription *n = getPin(pin);
    if (n)
    {
        GPIO_Setup(n->gpio, GPMODE(0)); // function IO
        if (mode & INPUT)
        {
            if (mode & INPUT_PULLUP)
                GPIO_SETPULLUP(n->gpio);
            if (mode & INPUT_PULLDOWN)
                GPIO_SETPULLDOWN(n->gpio);
            GPIO_SETDIRIN(n->gpio);
        }
        else
        {
            if (mode & OUTPUT_LO)
                GPIO_DATAOUT(n->gpio, 0);
            else
                GPIO_DATAOUT(n->gpio, 1);
            GPIO_SETDIROUT(n->gpio);
        }
    }
}

void digitalWrite(uint8_t pin, uint8_t val)
{
    PinDescription *n = getPin(pin);
    if (n)
        GPIO_DATAOUT(n->gpio, val ? 1 : 0);
}

int digitalRead(uint8_t pin)
{
    PinDescription *n = getPin(pin);
    if (n)
        return GPIO_DATAIN(n->gpio);
    return -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool eintMode(uint8_t pin, void (*Handler)(void), uint8_t Sense, uint16_t Debounce, bool Enable)
{
    bool res = false;
    PinDescription *n = getPin(pin);
    if (n)
    {
        if (Handler)
        {
            if ((res = NVIC_RegisterEINT(n->eint, Handler, Sense, Debounce, Enable)))
                n->eint_handler = Handler;
        }
        else
        {
            NVIC_DisableIRQ(n->eint);
            NVIC_DisableEINT(n->eint);
            res = NVIC_UnregisterEINT(n->eint);
            n->eint_handler = NULL;
        }
    }
    return res;
}

bool eintEnable(uint8_t pin)
{
    PinDescription *n = getPin(pin);
    if (n)
        return NVIC_EnableEINT(n->eint);
    return false;
}

bool eintDisable(uint8_t pin)
{
    PinDescription *n = getPin(pin);
    if (n)
        return NVIC_DisableEINT(n->eint);
    return false;
}

bool eintInterrupt(uint8_t pin)
{
    PinDescription *n = getPin(pin);
    if (n)
        return NVIC_EnableIRQ(n->eint);
    return false;
}

bool eintNoInterrupt(uint8_t pin)
{
    PinDescription *n = getPin(pin);
    if (n)
        return NVIC_DisableIRQ(n->eint);
    return false;
}