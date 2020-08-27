////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////

#ifndef __VARIANT_H__
#define __VARIANT_H__

#include <interface.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /* C ware          */

#define LED 0

#define MODE_EINT(n) (n)

    typedef struct
    {
        uint8_t gpio;
        uint8_t eint; 
        void *eint_handler;
    } PinDescription;
    extern PinDescription pinsMap[21];

    void sim1_to_gpio(bool volt_3);

#ifdef __cplusplus
}

/* CPP ware       */

#include <Uart.h>
extern Uart Serial;
extern Uart Serial1;
extern Uart Serial2;

#endif //__cplusplus

#endif /* __VARIANT_H__ */
