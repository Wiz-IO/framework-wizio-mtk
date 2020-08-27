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

#include <Arduino.h>
#include <hal/hal_afe.h>

static void tone_init(void)
{
    static int once = 0;
    if (0 == once)
    {
        AFE_Chip_Init();
        AFE_TurnOnAudioClock(ASP_FS_8K, DL_PATH);
        AFE_SwitchHPon();
        //AFE_Switch_Amp(true);
        once = 1;
    }
}

void noTone(uint8_t _pin)
{
    AFE_DAC_TEST = 0;
}

/* 31 tones, step125,  125-3875Hz */
void tone(uint8_t _pin, unsigned int frequency, unsigned long duration)
{
    tone_init();
    // Audio frequency = Sampling rate / 64 * FREQ_DIV
    AFE_DAC_TEST = ADAC_SINUS | AMP_DIV(7) | FREQ_DIV((frequency * 64) / 8000);
    if (duration)
    {
        delay(duration);
        noTone(0);
    }
}

void beep(char f)
{
    tone_init();
    AFE_DAC_TEST = ADAC_SINUS | AMP_DIV(7) | FREQ_DIV(f);
    delay(100);
    AFE_DAC_TEST = 0;
}