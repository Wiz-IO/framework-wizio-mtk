////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Georgi Angelov
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

#ifndef INTERFACE_H_
#define INTERFACE_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <reent.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include <sys/time.h>

#include <hal_interface.h>

#include <debug.h>

#define INLINE inline __attribute__((always_inline))

    INLINE unsigned int millis(void)
    {
        return USCNTI_VAL / 1000;
    } // TODO
    INLINE unsigned int micros(void) { return USCNTI_VAL; }
    INLINE unsigned int seconds(void) { return millis() / 1000; }

    INLINE void delay(unsigned int ms) { delay_m(ms); }
    INLINE void delayMicroseconds(unsigned int us) { delay_u(us); };
    INLINE void delaySeconds(unsigned int s) { delay_s(s); };

    time_t now(void); // get from rtc

    char *utoa(unsigned int value, char *buffer, int radix);
    INLINE char *ltoa(long value, char *result, int base) { return utoa(value, result, base); }
    INLINE char *ultoa(unsigned long value, char *result, int base) { return utoa(value, result, base); }

    uint32_t clockCyclesPerMicrosecond(void);       // todo
    uint32_t clockCyclesToMicroseconds(uint32_t a); // todo
    uint32_t microsecondsToClockCycles(uint32_t a); // todo

    int analogRead(uint8_t pin);            // adc
    void analogWrite(uint8_t pin, int val); // pwm, must be called pwmInit()

    typedef enum
    {
        OSC_13M_DIV_1 = 0,
        OSC_13M_DIV_2 = 1,
        OSC_13M_DIV_4 = 2,
        OSC_13M_DIV_8 = 3,

        OSC_32K_DIV_1 = 4,
        OSC_32K_DIV_2 = 5,
        OSC_32K_DIV_4 = 6,
        OSC_32K_DIV_8 = 7,
    } pwm_osc_enum;
    // init PWM before analogWrite()
    void pwmInit(int channel, uint32_t count, pwm_osc_enum osc, bool start);
    void pwmStart(uint8_t channel, int val);
    void pwmStop(uint8_t channel, int val);

    bool eintMode(uint8_t pin, void (*Handler)(void), uint8_t Sense, uint16_t Debounce, bool Enable);
    bool eintEnable(uint8_t pin);
    bool eintDisable(uint8_t pin);
    bool eintInterrupt(uint8_t pin);
    bool eintNoInterrupt(uint8_t pin);

    //enable/disable IRQs 
    void interrupts(void);
    void noInterrupts(void);

    void initVariant(void) __attribute__((weak));

#ifndef SERIAL_BUFFER_SIZE
#define SERIAL_BUFFER_SIZE 1024
#endif

#define digitalPinToPort(p)
#define digitalPinToBitMask(p)
#define digitalPinToClkid(p)
#define digitalPinSPIAvailiable(p)
#define digitalPinToSPIDevice(p)
#define digitalPinToSPIClockId(p)

#ifdef __cplusplus
}

void beep(char f = 8);

#endif

#endif /* INTERFACE_H_ */