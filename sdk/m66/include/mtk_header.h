#ifndef _MTK_HEADER_H_
#define _MTK_HEADER_H_

#include <stdint.h>

typedef enum
{
    GFH_FILE_INFO = 0x0,
    GFH_BL_INFO = 0x1,
    GFH_ANTI_CLONE = 0x2,
    GFH_BL_SEC_KEY = 0x3,
    GFH_SCTRL_CERT = 0x4,
    GFH_TOOL_AUTH = 0x5,
    GFH_MTK_RESERVED1 = 0x6,
    GFH_BROM_CFG = 0x7,
    GFH_BROM_SEC_CFG = 0x8,
    GFH_MTK_RESERVED2 = 0x9,
    GFH_MTK_RESERVED3 = 0xA,
    GFH_ROOT_CERT = 0xB,
    GFH_EXP_CHK = 0xC,
    GFH_EPP_PARAM = 0xD,
    GFH_CHIP_VER = 0xE,
    GFH_MTK_RESERVED4 = 0xF,
    GFH_MD_SEC_CFG = 0x10,
    GFH_EPP_INFO = 0x100,
    GFH_EMI_LIST = 0x101,
    GFH_CMEM_ID_INFO = 0x102,
    GFH_CMEM_NOR_INFO = 0x103,
    GFH_DSP_INFO = 0x104,
    GFH_MAUI_INFO = 0x200,
    GFH_MAUI_SEC = 0x201,
    GFH_MAUI_CODE_KEY = 0x202,
    GFH_MAUI_SECURE_RO_KEY = 0x203,
    GFH_MAUI_RESOURCE_KEY = 0x204,
    GFH_SECURE_RO_INFO = 0x205,
    GFH_DL_PACKAGE_INFO = 0x206,
    GFH_FLASH_INFO = 0x207,
    GFH_MACR_INFO = 0x208,
    GFH_ARM_BL_INFO = 0x209,
    GFH_EMMC_BOOTING_INFO = 0x20A,
    GFH_FOTA_INFO = 0x20B,
    GFH_CBR_RECORD_INFO = 0x20C,
    GFH_CONFIDENTIAL_BIN_INFO = 0x20D,
    GFH_CBR_INFO = 0x20E,
    GFH_MBA_INFO = 0x20F,
    GFH_BINARY_LOCATION = 0x210,
    GFH_BOOT_CERT_CTRL_CONTENT = 0x300,
    GFH_TYPE_NUM = 0x301,
    GFH_TYPE_END = 0xFFFF,
} GFH_TYPE;

typedef enum
{
    GFH_FILE_NONE = 0x0,
    ARM_BL = 0x1,
    ARM_EXT_BL = 0x2,
    DUALMAC_DSP_BL = 0x3,
    SCTRL_CERT = 0x4,
    TOOL_AUTH = 0x5,
    FILE_MTK_RESERVED1 = 0x6,
    EPP = 0x7,
    FILE_MTK_RESERVED2 = 0x8,
    FILE_MTK_RESERVED3 = 0x9,
    ROOT_CERT = 0xA,
    AP_BL = 0xB,
    V_MAUI_BINARY = 0x100,
    PRIMARY_MAUI = 0x100,
    SECONDARY_MAUI = 0x101,
    ON_DEMAND_PAGING = 0x102,
    THIRD_ROM = 0x103,
    DSP_ROM = 0x104,
    CACHED_DSP_ROM = 0x105,
    PRIMARY_FACTORY_BIN = 0x106,
    SECONDARY_FACTORY_BIN = 0x107,
    VIVA = 0x108,
    LTE_DSP_ROM = 0x109,
    TINY_SYS_ROM1 = 0x10A,
    TINY_SYS_ROM2 = 0x10B,
    IOT_WIFI_BIN = 0x10C,
    V_MAUI_BINARY_END = 0x17F,
    V_RESOURCE_BINARY = 0x180,
    CUSTOM_PACK = 0x180,
    LANGUAGE_PACK = 0x181,
    JUMP_TABLE = 0x182,
    V_RESOURCE_BINARY_END = 0x1FF,
    V_MISC_BINARY = 0x200,
    FOTA_UE = 0x200,
    ARM_EXT_BL_BACKUP = 0x201,
    V_MISC_BINARY_END = 0x202,
    V_SECURE_RO = 0x300,
    SECURE_RO_S = 0x300,
    SECURE_RO_ME = 0x301,
    V_SECURE_RO_END = 0x302,
    V_EXTERNAL_FILE = 0x400,
    CARD_DOWNLOAD_PACKAGE = 0x400,
    CONFIDENTIAL_BINARY = 0x401,
    V_EXTERNAL_FILE_END = 0x402,
    V_FILE_SYSTEM_BINARY = 0x480,
    FILE_SYSTEM = 0x480,
    V_FILE_SYSTEM_BINARY_END = 0x481,
    V_SRD = 0x500,
    BOOT_CERT_CTRL = 0x500,
    V_SRD_END = 0x501,
    V_CUSTOMIZED_FILE = 0x7000,
    CUSTOMER_BIN1 = 0x7000,
    CUSTOMER_BIN2 = 0x7001,
    CUSTOMER_BIN3 = 0x7002,
    V_CUSTOMIZED_FILE_END = 0x70FF,
    GFH_FILE_END = 0xFFFF,
} GFH_FILE_TYPE;

typedef enum
{
    FLASH_DEV_NONE = 0x0,
    F_NOR = 0x1,
    F_NAND_SEQUENTIAL = 0x2,
    F_NAND_TTBL = 0x3,
    F_NAND_FDM50 = 0x4,
    F_EMMC_BOOT_REGION = 0x5,
    F_EMMC_DATA_REGION = 0x6,
    F_SF = 0x7,
    F_XBOOT = 0x8,
    F_SPI_NAND_SEQUENTIAL = 0x9,
    F_SPI_NAND_TTBL = 0xA,
    F_SPI_NAND_FDM50 = 0xB,
    F_MSDC = 0xC,
    FLASH_DEV_END = 0xFF,
} GFH_FLASH_DEV;

typedef enum
{
    SIG_NONE = 0x0,
    SIG_PHASH = 0x1,
    SIG_SINGLE = 0x2,
    SIG_SINGLE_AND_PHASH = 0x3,
    SIG_MULTI = 0x4,
    SIG_TYPE_NUM = 0x5,
    SIG_TYPE_END = 0xFF,
} GFH_SIG_TYPE;

typedef struct
{
    uint32_t m_magic_ver;
    uint16_t m_size;
    uint16_t m_type; // uint16_t
} GFH_Header;

typedef struct
{
    GFH_Header m_gfh_hdr;
    char m_identifier[12];
    uint32_t m_file_ver;
    GFH_FILE_TYPE m_file_type; // (uint16_t)
    GFH_FLASH_DEV m_flash_dev; // (uint8_t)
    GFH_SIG_TYPE m_sig_type;   // (uint8_t)
    uint32_t m_load_addr;
    uint32_t m_file_len;
    uint32_t m_max_size;
    uint32_t m_content_offset;
    uint32_t m_sig_len;
    uint32_t m_jump_offset;
    uint32_t m_attr;
} GFH_FILE_INFO_v1;

typedef struct
{
    GFH_Header m_gfh_hdr;
    char m_identifier[12];
    uint32_t m_file_ver;
    GFH_FILE_TYPE m_file_type;
    GFH_FLASH_DEV m_flash_dev;
    GFH_SIG_TYPE m_sig_type;
    uint32_t m_load_addr;
    uint32_t m_file_len;
    uint32_t m_max_size;
    uint32_t m_content_offset;
    uint32_t m_sig_len;
    uint32_t m_jump_offset;
    uint32_t m_attr;
} GFH_FILE_INFO_v1_1;


int GFH(void);

#endif