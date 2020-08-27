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
#include <stdint.h>

#if BOOT_SIZE > 0
#include <mtk_header.h>
extern uint32_t __app_header_base;
extern uint32_t __rom_image_limit;
volatile const GFH_FILE_INFO_v1_1 app_header __attribute__((section(".app_header"))) = {
	{
		0x014D4D4D,								// MMM
		(uint16_t)(sizeof(GFH_FILE_INFO_v1_1)), // 0x0038
		(uint16_t)GFH_FILE_INFO,				// 0x0000
	},											//
	"FILE_INFO",								//
	1,											// 0x00000001			m_file_ver
	////////////////////////////////////////////////////////////////////////////////////////////////////
	(uint16_t)V_MAUI_BINARY,					 // 0x0100 				m_file_type = V_MAUI_BINARY
	(uint8_t)F_SF,								 // 0x07 				m_flash_dev
	(uint8_t)SIG_NONE,							 // 0x00 				m_sig_type
	(uint32_t)(&__app_header_base),				 // 0x10020000 			m_load_addr
	(uint32_t)(&__rom_image_limit) - 0x10020000, // 0x00000000 ??? 		m_file_len
	0xFFFFFFFF,									 // 0xFFFFFFFF			m_max_size
	sizeof(GFH_FILE_INFO_v1_1),					 // 0x000005B0 			m_content_offset
	0x00000000,									 // 0x00000000 			m_sig_len
	sizeof(GFH_FILE_INFO_v1_1),					 // 0x000005B0 			m_jump_offset = 0x100205B0
	0x00000003									 // 0x00000003 			m_attr = 3
};

int GFH_Dummy(void) {
	return app_header.m_gfh_hdr.m_magic_ver; // linker use app_header
}

#else

#include <mtktypes.h>

typedef struct tag_APP_HEADER
{
	FILE_INFO_v1 file_info;
	GFH_BROM_CFG_v3 brom_cfg;
	GFH_BL_INFO_v1 bl_info;
	GFH_ARM_BL_INFO_v1 arm_bl_info;
} APP_HEADER;

extern uint32_t __boot_header_base;
extern uint32_t __boot_header_size;

volatile const APP_HEADER app_header __attribute__ ((section (".boot_header"))) =
{
    {
        {GFH_HDR_MAGIC, sizeof(FILE_INFO_v1), GFH_FILE_INFO}, 
        "FILE_INFO",
        1,
        ARM_BL,
        F_SF,
        1,
        (uint32_t)(&__boot_header_base),
        0xffffffff,
        0xffffffff,
        (uint32_t)(&__boot_header_size),
        0x00000020,
        (uint32_t)(&__boot_header_size),
        1
    },
    {   {GFH_HDR_MAGIC, sizeof(GFH_BROM_CFG_v3), GFH_BROM_CFG},
        {0x02, 0x00, 0x00, 0x00, 0xB8, 0x0B}
    },
    {
        {GFH_HDR_MAGIC, sizeof(GFH_BL_INFO_v1), GFH_BL_INFO},
        1
    },
    {
        {GFH_HDR_MAGIC, sizeof(GFH_ARM_BL_INFO_v1), GFH_ARM_BL_INFO},
        0x00000001,
        0x00000560,
        0x00000000,
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
    }
};

int GFH_Dummy(void) {
	return app_header.file_info.jump_offset; // linker use app_header
}

#endif
