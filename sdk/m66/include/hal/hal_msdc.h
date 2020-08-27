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

#ifndef _MSDC_H_
#define _MSDC_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <hal/hal.h>

#define MSDC_base       (MSDC2_base)

#define MSDC_CFG		(*(volatile uint32_t*)(MSDC_base + 0x00))
#   define MSDC_CFG_MSDC		0x00000001
#   define MSDC_CFG_RST		    0x00000002
#   define MSDC_CFG_NOCRC		0x00000004
#   define MSDC_CFG_CLKSRC		0x00000018
#   define MSDC_CFG_STDBY		0x00000020
#   define MSDC_CFG_CRED		0x00000040
#   define MSDC_CFG_SCKON		0x00000080
#   define MSDC_CFG_SCLKF		0x0000ff00
#   define MSDC_CFG_INTEN		0x00010000
#   define MSDC_CFG_DRQEN		0x00020000
#   define MSDC_CFG_PINEN		0x00040000
#   define MSDC_CFG_DIRQE		0x00080000
#   define MSDC_CFG_RCDEN		0x00100000
#   define MSDC_CFG_VDDPD		0x00200000

#define MSDC_STA		(*(volatile uint32_t*)(MSDC_base + 0x04))
#define MSDC_STA_BF             1
#define MSDC_STA_BE             2
#define MSDC_STA_DRQ            4
#define MSDC_STA_FIFOCLR        (1 << 14) 


#define MSDC_INTR		(*(volatile uint32_t*)(MSDC_base + 0x08))
#define MSDC_PS			(*(volatile uint32_t*)(MSDC_base + 0x0C))
#define MSDC_DAT		(*(volatile uint32_t*)(MSDC_base + 0x10))

#define MSDC_IOCON		(*(volatile uint32_t*)(MSDC_base + 0x14))
#   define MSDC_IOCON_ODCCFG0		0x0007	    // driving capacity of CMD/BS and SCLK
#   define MSDC_IOCON_ODCCFG1		0x0038	    // driving capacity of data lines
#   define MSDC_IOCON_SCRCFG0		0x0040	    // slew rate of the CMD/BS and SCLK
#   define MSDC_IOCON_SCRCFG1		0x0080	    // slew rate of the data lines
#   define MSDC_IOCON_DMABURST		0x0300	    // select burst type when data transfer by DMA
#   define MSDC_IOCON_CMDRE		    0x8000	    // 0: latch the command at rising edge, 1: at falling edge
#   define MSDC_IOCON_DSW			0x00010000
#   define MSDC_IOCON_INTLH		    0x00060000
#   define MSDC_IOCON_CMDSEL		0x00080000
#   define MSDC_IOCON_CRCDIS		0x00100000
#   define MSDC_IOCON_SAMPON		0x00200000
#   define MSDC_IOCON_FIXDLY		0x00C00000
#   define MSDC_IOCON_SAMPLEDLY	    0x03000000

#define MSDC_IOCON1 (*(volatile uint32_t*)(MSDC_base + 0x18))
#   define MSDC_IOCON1_PRVAL_INS		0x00000003
#   define MSDC_IOCON1_PRCFG_INS		0x00000004
#   define MSDC_IOCON1_PRVAL_DA		    0x00000030
#   define MSDC_IOCON1_PRCFG_DA		    0x00000040
#   define MSDC_IOCON1_PRVAL_CM		    0x00000300
#   define MSDC_IOCON1_PRCFG_CM		    0x00000400
#   define MSDC_IOCON1_PRVAL_CK		    0x00003000
#   define MSDC_IOCON1_PRCFG_CK		    0x00004000
#   define MSDC_IOCON1_PRVAL_RST_WP	    0x00030000
#   define MSDC_IOCON1_PRCFG_RST_WP	    0x00040000
#   define MSDC_IOCON1_ALL_PULLUP_47K	0x00022222


//SDC_STA
#   define SDC_STA_SDCBUSY		0x0001
#   define SDC_STA_CMDBUSY		0x0002
#   define SDC_STA_DATBUSY		0x0004
#   define SDC_STA_FECMDBUSY	0x0008
#   define SDC_STA_FEDATBUSY	0x0010
#   define SDC_STA_R1BSY		0x0010
#   define SDC_STA_WP           0x8000

//SDC_DAT_STA
#   define SDC_DAT_STA_BLKDONE  1
#   define SDC_DAT_STA_DATTO    2

#define MSDC_CLKACB_CFG (*(volatile uint32_t*)(MSDC_base +0x80))
#   define MSDC_CLKACB_CFG_ACB_MODE		    0x80000000
#   define MSDC_CLKACB_CFG_CMD_PHASE_RED	0x20000000
#   define MSDC_CLKACB_CFG_CMD_PHASE		0x1F000000
#   define MSDC_CLKACB_CFG_TUN_BLOCK_NUM	0x00FF0000
#   define MSDC_CLKACB_CFG_DAT_PHASE_RED	0x00002000
#   define MSDC_CLKACB_CFG_DAT_PHASE		0x00001F00
#   define MSDC_CLKACB_CFG_CLKPAD_RED		0x00000080
#   define MSDC_CLKACB_CFG_CLK_LATCH		0x00000040
#   define MSDC_CLKACB_CFG_SAMPLE_PERIOD	0x00000030
#   define MSDC_CLKACB_CFG_SCANMODE		    0x00000008
#   define MSDC_CLKACB_CFG_FINE_EN			0x00000006
#   define MSDC_CLKACB_CFG_COARSE_EN		0x00000002
#   define MSDC_CLKACB_CFG_CLKTUN_ON		0x00000001
#define MSDC_CLKACB_STA		        (*(volatile uint32_t*)(MSDC_base +0x84))
#define MSDC_CLKACB_CRCSTA1	        (*(volatile uint32_t*)(MSDC_base +0x88))
#define MSDC_CLKACB_CRCSTA2	        (*(volatile uint32_t*)(MSDC_base +0x8C))

#define MSDC_CLOCK		26000 //97000	// (kHz)
#define MSDC_OP_CLOCK	15300
#define MSDC_30M		30700

typedef struct {
    uint32_t CFG;           /* 0 base+0x00, */
    uint32_t STA;           /* 1 base+0x04, */
    uint32_t INT;           /* 2 base+0x08, */
    uint32_t PS;            /* 3 base+0x0C, */
    uint32_t DAT;           /* 4 base+0x10, */
    uint32_t IOCON;         /* 5 base+0x14, */
    uint32_t IOCON1;        /* 6 base+0x18, */
    uint32_t res_1;         /* 7 base+0x1C, */
    uint32_t SDC_CFG;       /* 8 base+0x20, */
    uint32_t SDC_CMD;       /* 9 base+0x24, */
    uint32_t SDC_ARG;       /*10 base+0x28, */
    uint32_t SDC_STA;       /*11 base+0x2C, */
    uint32_t SDC_RES0;      /*12 base+0x30, */
    uint32_t SDC_RES1;      /*13 base+0x34, */
    uint32_t SDC_RES2;      /*14 base+0x38, */
    uint32_t SDC_RES3;      /*15 base+0x3C, */
    uint32_t SDC_CMD_STA;   /*16 base+0x40, */
    uint32_t SDC_DAT_STA;   /*17 base+0x44, */
    uint32_t SDC_CSTA;      /*18 base+0x48, */
    uint32_t SDC_IRQ0;      /*19 base+0x4C, */
    uint32_t SDC_IRQ1;      /*20 base+0x50, */
    uint32_t SDIO_CFG;      /*   base+0x54, */
    uint32_t SDIO_STA;      /*   base+0x55, */
    uint32_t res_2[9];      
    uint32_t CLKACB_CFG;    /*   base+0x80, */
    uint32_t CLKACB_STA;
    uint32_t CLKACB_CRCSTA1;
    uint32_t CLKACB_CRCSTA2;
} msdc_register_t;
#define MSDC                ((volatile msdc_register_t*)MSDC_base)

#define SDC_NO_ARG			    0
#define SDC_CMD8_ARG			0x000001AA // the argument of the CMD8
#define SDC_OCR_DEFAULT		    0x00FF8000
#define SD_ACMD41_HCS		    0x40000000 // indicate host support high capacity card.
#define SD_OCR_VOLTAGE_ARGUMENT 0x00FF8000
#define SD_OCR_CAPACITY_STATUS  0x40000000
#define SD_OCR_BUSY_STATUS      0x80000000
#define SD_RCA_DEFAULT          0x00


/* MMC bus commands */
#define CMD0_GO_IDLE_STATE          0x00    /* Resets the MMC */
#define CMD1_SEND_OP_COND           0x01    /* Activates the card's initialization process */
#define CMD2_ALL_SEND_CID           0x02    /* Asks all cards in ready state to send their CID */
#define CMD3_SET_RELATIVE_ADDR      0x03    /* Assigns relative address to the card in identification state */
#define CMD4_SET_DSR                0x04    /* Sets drive stage register DSR */
#define CMD7_SELECT_CARD            0x07
#define CMD9_SEND_CSD               0x09    /* Asks the selected card to send its CSD */
#define CMD10_SEND_CID              0x0A    /* Asks the selected card to send its CID */
#define CMD11_READ_DAT_UNTIL_STOP_MMC   0x0B
#define CMD12_STOP_TRANSMISSION     0x0C    /* Stop data transmission */
#define CMD13_SEND_STATUS           0x0D    /* Asks the selected card to send its status register */
#define CMD15_GO_INACTIVE_STATE     0x0F
#define CMD16_SET_BLOCKLEN          0x10    /* Selects a block length for all following block commands */
#define CMD17_READ_SINGLE_BLOCK     0x11    /* Reads a block of the size selected by the SET_BLOCKLEN command */
#define CMD18_READ_MULTIPLE_BLOCK   0x12
#define CMD20_WRITE_DAT_UNTIL_STOP_MMC  0x14
#define CMD24_WRITE_SINGLE_BLOCK    0x18    /* Writes a block of the size selected by SET_BLOCKLEN command */
#define CMD25_WRITE_MULTIPLE_BLOCK  0x19     /* Writes multiple block until CMD 12 */
#define CMD26_PROGRAM_CID           0x1A
#define CMD27_PROGRAM_CSD           0x1B    /* Programming of the programmable bits of the CSD */
#define CMD28_SET_WRITE_PROT        0x1C    /* If the card has write protection features, sets the write protection bit */
#define CMD29_CLR_WRITE_PROT        0x1D    /* If with write protection features, clears the write protection bit */
#define CMD30_SEND_WRITE_PROT       0x1E    /* If with write protection features, asks the card to send status of write protection bit */
#define CMD32_TAG_SECTOR_START      0x20    /* Sets the address of the first sector */
#define CMD33_TAG_SECTOR_END        0x21    /* Sets the address of the last sectore */
#define CMD34_UNTAG_SECTOR          0x22    /* Removes one previously selected sector */
#define CMD35_TAG_ERASE_GROUP_START 0x23    /* Sets the address of the first erase group */
#define CMD36_TAG_ERASE_GROUP_END   0x24    /* Sets the address of the last erase group */
#define CMD37_UNTAG_ERASE_GROUP     0x25    /* Removes on previously selected erase group */
#define CMD38_ERASE                 0x26    /* Erases all previously selected sectors */
#define CMD39_FAST_IO_MMC           0x27
#define CMD40_GO_IRQ_STATE          0x28    
#define CMD42_LOCK_UNLOCK           0x2A    /* Used to set/reset the password or lock/unlock the card */
#define CMD55_APP					0x37	/* Indicates to the card the next cmd is an application specific command */
#define CMD56_GEN					0x38	/* Used either to transfer a data block or to get a data block form the card for general purpse */

// commnad value for MSDC controller SDC_CMD
#define SDC_CMD_CMD0			0x0000
#define SDC_CMD_CMD1			0x0181
#define SDC_CMD_CMD2			0x0502
#define SDC_CMD_CMD3_SD			0x0303
#define SDC_CMD_CMD3_MMC		0x0083
#define SDC_CMD_CMD4			0x0004
#define SDC_CMD_CMD7			0x0387
//#define SDC_CMD_CMD7			0x0087
#define SDC_CMD_CMD8			0x0088
#define SDC_CMD_CMD9			0x0109
#define SDC_CMD_CMD10			0x010a	// the response time delay is not NID(5 cycles)
#define SDC_CMD_CMD11_MMC		0x188b 
#define SDC_CMD_CMD12			0x438c	
//#define SDC_CMD_CMD12			0x038c	// remove STOP bit 
#define SDC_CMD_CMD13			0x008d
#define SDC_CMD_CMD15			0x000f
#define SDC_CMD_CMD16			0x0090
#define SDC_CMD_CMD17			0x0891
#define SDC_CMD_CMD18			0x1092
#define SDC_CMD_CMD20_MMC		0x3894
//#define SDC_CMD_CMD24			0x2898
#if defined(R1B_INTERRUPT_MODE)
#define SDC_CMD_CMD24			0x2b98	// use R1b to replace R1 write single block
#else
#define SDC_CMD_CMD24			0x2898	// use R1b to replace R1 write single block
#endif
#define SDC_CMD_CMD25			0x3099
//#define SDC_CMD_CMD25			0x3399	// use R1b to replace R1 write single block
#define SDC_CMD_CMD26			0x009a
#define SDC_CMD_CMD27			0x009b
#define SDC_CMD_CMD28			0x039c
#define SDC_CMD_CMD29			0x039d
#define SDC_CMD_CMD30			0x089e
#define SDC_CMD_CMD32			0x00a0
#define SDC_CMD_CMD33			0x00a1
#define SDC_CMD_CMD34_MMC		0x00a2
#define SDC_CMD_CMD35_MMC		0x00a3
#define SDC_CMD_CMD36_MMC		0x00a4
#define SDC_CMD_CMD37_MMC		0x00a5
#define SDC_CMD_CMD38			0x03a6
#define SDC_CMD_CMD39_MMC		0x0227
#define SDC_CMD_CMD40_MMC		0x82a8
#define SDC_CMD_CMD41_SD		0x01a9     
#define SDC_CMD_CMD42			0x2baa     
#define SDC_CMD_CMD55			0x00b7     
#define SDC_CMD_CMD56			0x00b8
#define SDC_CMD_ACMD6			0x0086
#define SDC_CMD_ACMD13			0x088d
#define SDC_CMD_ACMD22			0x0896
#define SDC_CMD_ACMD23			0x0097	
#define SDC_CMD_ACMD42			0x00aa
#define SDC_CMD_ACMD51			0x08b3	

// for MMC4.0
#define SDC_CMD_CMD6_MMC40		0x0386	// SWITCH
#define SDC_CMD_CMD8_MMC40		0x0888	// SEND_EXT_CSD

#define SDC_CMD_CMD6_SD11		0x0886	// SWITCH
/* SDIO command*/
#define SDC_CMD_CMD5			0x0185	// use R3 to replace R4
#define SDC_CMD_CMD52			0x02b4	// STOp field will be set while used to Stop data transfer
#define SDC_CMD_CMD53			0x01b5	// the RWand DTYPE will be changed according request

typedef enum {
    SD_COMMAND8_RESPONSE_NO_RESPONSE,        /* before SD2.0 version or MMC*/
    SD_COMMAND8_RESPONSE_INVALID,            /* SD 2.0 or higher compliant but voltage condition is not allow*/
    SD_COMMAND8_RESPONSE_VALID               /* SD 2.0 or higher compliant*/
} sd_command8_response_t;

typedef enum {
    HAL_SD_TYPE_SD_CARD            = 0,                          /** SD card. */
    HAL_SD_TYPE_MMC_CARD           = 1,                          /** MMC card. */
    HAL_SD_TYPE_SD20_LCS_CARD      = 2,                          /** SD 2.0 card and capacity less than 2GB. */
    HAL_SD_TYPE_SD20_HCS_CARD      = 3,                          /** SD 2.0 card and capacity larger than 2GB. */
    HAL_SD_TYPE_MMC42_CARD         = 4,                          /** EMMC 4.2 and above card. */
    HAL_SD_TYPE_CARD_TYPE_MAX      = 5,
    HAL_SD_TYPE_UNKNOWN_CARD       = 0xff,                       /** Unknown card. */
} hal_sd_card_type_t;

/* Card Specific Data(CSD) register structure */
typedef enum {
    CSD_VERSION_1_0 = 0,
    CSD_VERSION_1_1 = 0,
    CSD_VERSION_2_0 = 1,
    CSD_VERSION_EXT
} csd_version_t;

typedef struct {
    uint8_t mmc44_reserved0[136];    /*[135:0]*/
    uint32_t enh_start_addr;         /*[139:136]*/
    uint8_t enh_size_mult[3];        /*[142:140]*/
    uint8_t gp_size_mult[12];        /*[154:143]*/
    uint8_t partition_settig;        /*[155]*/
    uint8_t partition_attr;          /*[156]*/
    uint8_t max_enh_size_mult[3];    /*[159:157]*/
    uint8_t partition_support;       /*[160]*/
    uint8_t mmc44_reserved1;         /*[161]*/
    uint8_t rst_function;            /*[162]*/
    uint8_t mmc44_rev2[5];           /*[167:163]*/
    uint8_t rpmb_size_mul;           /*[168]*/
    uint8_t fw_config;               /*[169]*/
    uint8_t mmc44_reserved3;         /*[170]*/
    uint8_t user_wp;                 /*[171]*/
    uint8_t mmc44_reserved4;         /*[172]*/
    uint8_t boot_wp;                 /*[173]*/
    uint8_t mmc44_reserved5;         /*[174]*/
    uint8_t erase_grp_def;           /*[175]*/
    uint8_t mmc44_reserved6;         /*[176]*/
    uint8_t boot_bus_width;          /*[177]*/
    uint8_t boot_config_prot;        /*[178]*/
    uint8_t partition_config;        /*[179]*/
    uint8_t mmc44_reserved7;         /*[180]*/
    uint8_t erased_mem_cont;         /*[181]*/
    uint8_t mmc44_reserved8;         /*[182]*/
    uint8_t bus_width;               /*[183]*/
    uint8_t reserved2;               /*[184]*/
    uint8_t high_speed;              /*[185]*/
    uint8_t reserved3;               /*[186]*/
    uint8_t power_class;             /*[187]*/
    uint8_t reserved4;               /*[188]*/
    uint8_t cmd_set_rev;             /*[189]*/
    uint8_t reserved5;               /*[190]*/
    uint8_t cmd_set;                 /*[191]*/
    uint8_t ext_csd_rev;             /*[192]*/
    uint8_t reserved7;               /*[193]*/
    uint8_t csd_structure;           /*[194]*/
    uint8_t reserved8;               /*[195]*/
    uint8_t card_type;               /*[196]*/
    uint8_t reserved9;               /*[197]*/
    uint8_t out_of_interrupt_time;   /*[198]*/
    uint8_t partition_switch_time;   /*[199]*/
    uint8_t pwr_52_195;              /*[200]*/
    uint8_t pwr_26_195;              /*[201]*/
    uint8_t pwr_52_360;              /*[202]*/
    uint8_t pwr_26_360;              /*[203]*/
    uint8_t reserved10;              /*[204]*/
    uint8_t min_perf_r_4_26;         /*[205]*/
    uint8_t min_perf_w_4_26;         /*[206]*/
    uint8_t min_perf_r_8_26_4_52;    /*[207]*/
    uint8_t min_perf_w_8_26_4_52;    /*[208]*/
    uint8_t min_perf_r_8_52;         /*[209]*/
    uint8_t min_perf_w_8_52;         /*[210]*/
    uint8_t reserved11;              /*[211]*/
    uint32_t sec_count;              /*[212], newly defined in MMC42*/
    uint8_t mmc44_reserved9[5];      /*[220:216]*/
    uint8_t hc_wp_grp_size;          /*221*/
    uint8_t rel_wr_sec_c;            /*222*/
    uint8_t erase_timeout_mult;      /*223*/
    uint8_t hc_erase_grp_size;       /*224*/
    uint8_t acc_size;                /*225*/
    uint8_t boot_size_mul;           /*[226]*/
    uint8_t mmc44_reserved10;        /*[227]*/
    uint8_t boot_info;               /*[228]*/
    uint8_t mmc44_reserved11[275];   /*[503:229]*/
    uint8_t s_cmd_set;
    uint8_t reserved13[7];
} extended_csd_information_t;


typedef struct {
    csd_version_t     csd_structure;        /* the version of CSD structure */
    uint8_t   tacc;                         /* read access time-1 */
    uint8_t   nsac;                         /* read access time-2 */
    uint8_t   tran_speed;                   /* max. data transfer rate */
    uint16_t  ccc;                          /* card command classes */
    uint32_t  write_bl_len;                 /* max write data block length */
    uint32_t  read_bl_len;                  /* max read data block length */
    uint8_t   write_blk_misalign;           /* write block misalighment */
    uint8_t   read_blk_misalign;            /* read block misalighment */
    uint8_t   write_bl_partial;             /* partial blocks for write allowed */
    uint8_t   read_bl_partial;              /* partial blocks for write allowed */
    uint8_t   dsr_imp;                      /* DSR implemented*/
    uint32_t  c_size;                       /* device size*/
    uint8_t   erase_blk_en;                 /* erase single block enable*/
    uint32_t  sector_size;                  /* erase sector size */
    uint32_t  wp_grg_size;                  /* write protect block size*/
    uint8_t   wp_grp_enable;                /* write protect group enable */
    uint8_t   tmp_write_protect;            /* temporary write protection  */
    uint8_t   perm_write_protect;           /* permanent write protection  */
    uint32_t  wp_grp_size_mmc;              /* write protect group size */
    uint32_t  erase_sec_size_mmc;           /* erase sector size */
    uint32_t  erase_grp_size_mmc;           /* erase group size */
    uint8_t   spec_vers;                    /* defne the MMC system spec. */
    extended_csd_information_t *ext_csd;    /* pointer to EXT_CSD */
    uint64_t  capacity;                     /* capacity in bytes */
} csd_information_t;

typedef struct
{
    uint32_t op_clock;
    uint32_t cmd_sta;
    uint32_t dat_sta;
    uint32_t sd_int;
    uint32_t rca;

    int error_status;
    sd_command8_response_t command8_response; /* receive response after CMD8 (SD2.0 or higher)*/
    uint32_t sd_ocr;                          /* OCR register*/
    hal_sd_card_type_t card_type;

    csd_information_t csd;

    volatile bool is_inactive;
    volatile bool is_write_protection;
} sd_ctx_t;

void MSDC_Init(void);
int SD_Init(void);
int SD_Send_Cmd(uint32_t cmd, uint32_t arg);
int SD_Card_Type(void);

int SD_ReadBlock(uint32_t sector, void *buffer, uint32_t num);
int SD_WriteBlock(uint32_t sector, const void *buffer, uint32_t num);

#define SECTOR_SIZE 512

#ifdef __cplusplus
}
#endif
#endif /* _MSDC_H_ */
