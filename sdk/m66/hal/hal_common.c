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

#include <hal/hal.h>

bool isROMSpace(int address)
{
    return (unsigned int)(address - 0x10000000) < 0x400000 || (unsigned int)(address + 0x70000000) < 0x400000;
}

bool isCachedRam(uint32_t address)
{
    return (~address >> 28);
}

bool isNonCachedRam(uint32_t address)
{
  return (~address >> 28) != 0;
}