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

#include <variant.h>
#include <hal/hal_adc.h>
#include <hal/hal_pwm.h>

static int adc_once = 0;

int analogRead(uint8_t channel)
{
    if (0 == adc_once)
    {
        adc_once = 1;
        AUXADC_Enable();
    }
    return AUXADC_MeasureChannel(channel & 0xF);
}

void analogWrite(uint8_t channel, int val)
{
    switch (channel)
    {
    case 4:
        PWM4_THRESHOLD = val & 0xFFF;
        break;
    }
}

void pwmInit(int channel, uint32_t count, pwm_osc_enum osc, bool start)
{
    switch (channel)
    {
    case 4:
        GPIO_Setup(GPIO3, GPMODE(GPIO03_MODE_PWM1)); // GPIO3 ( QL Netligh )
        PWM4_CTRL = osc;
        PWM4_COUNT = count & 0xFFF;      
        PWM4_THRESHOLD = PWM4_COUNT / 2; // duty 50%
        if (start)
            PCTL_PowerUp(PD_PWM4); // start
        break;
    }
}

void pwmStart(uint8_t channel, int val)
{
    switch (channel)
    {
    case 4:
        PCTL_PowerUp(PD_PWM4);
        break;
    }
}

void pwmStop(uint8_t channel, int val)
{
    switch (channel)
    {
    case 4:
        PCTL_PowerDown(PD_PWM4);
        break;
    }
}
