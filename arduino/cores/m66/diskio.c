/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#ifdef USE_FATFS

#include <stdio.h>
#include <string.h>
#include <debug.h>

#include <ff.h>
#include <diskio.h>

#ifndef PRINTF
#define PRINTF printf
#endif 

extern void (*__efs_ram_begin__[])(void);
extern void (*__efs_ram_size__[])(void);

extern void (*__efs_rom_begin__[])(void);
extern void (*__efs_rom_size__[])(void);

int SerialFlash_Write(uint32_t Address, const void *Data, int Size);
DRESULT efs_write(const BYTE *Data, DWORD sector, UINT count);

/* Definitions of physical drive number for each drive */
#define DISK_RAM 0
#define DISK_EFS 1

#define WORK_SIZE 4096
typedef struct
{
	int is_register;
	uint32_t address;
	uint32_t size;
	BYTE work[WORK_SIZE];
	FATFS filesystem;
} media_info;

static media_info media[2] = {
	{
		// RAM
		0,
		(uint32_t)__efs_ram_begin__,
		(uint32_t)__efs_ram_size__,
		{0} //
	},
	{
		// EFS-ROM
		0,
		(uint32_t)__efs_rom_begin__,
		(uint32_t)__efs_rom_size__,
		{0} //
	},
	//{/* SD CARD */ 0} //
};

static int is_limit(int drv, uint32_t address, uint32_t size)
{
	if (address < media[drv].address || address > media[drv].address + media[drv].size)
		return -1;
	return 0;
}

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
	BYTE pdrv /* Physical drive  to identify the drive */
)
{
	int res = STA_NOINIT;
	switch (pdrv)
	{
	case DISK_RAM:
	case DISK_EFS:
		res = media[pdrv].is_register ? RES_OK : STA_NOINIT;
		break;
	}
	if (res)
	{
		PRINTF("[ERROR] disk_status( %d )\n", (int)pdrv);
	}
	return res;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
	BYTE pdrv /* Physical drive to identify the drive */
)
{
	int res = STA_NOINIT;
	switch (pdrv)
	{
	case DISK_RAM:
	case DISK_EFS:
		res = media[pdrv].is_register ? FR_OK : STA_NOINIT;
		break;
	}
	if (res)
	{
		PRINTF("[ERROR] disk_initialize( %d )\n", (int)pdrv);
	}
	return res;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
	BYTE pdrv,	  /* Physical drive to identify the drive */
	BYTE *dst,	  /* Data buffer to store read data */
	LBA_t sector, /* Start sector in LBA */
	UINT count	  /* Number of sectors to read */
)
{
	char *src;
	int res = RES_PARERR;
	switch (pdrv)
	{
	case DISK_RAM:
	case DISK_EFS:
		src = (char *)media[pdrv].address + (sector * FF_MAX_SS);
		memcpy(dst, src, count * FF_MAX_SS);
		res = RES_OK;
		break;
	}
	if (res)
	{
		PRINTF("[ERROR] disk_read( %d )\n", (int)pdrv);
	}
	return res;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write(
	BYTE pdrv,		 /* Physical drive to identify the drive */
	const BYTE *src, /* Data to be written */
	LBA_t sector,	 /* Start sector in LBA */
	UINT count		 /* Number of sectors to write */
)
{
	int res = RES_PARERR;
	uint32_t Size = count * FF_MAX_SS;
	uint32_t Address = media[pdrv].address + (sector * FF_MAX_SS);

	if (Address < media[pdrv].address || Address + Size >= media[pdrv].address + media[pdrv].size)
	{
		PRINTF("[ERROR] disk_write( %d ) protect\n", (int)pdrv);
		return RES_WRPRT;
	}

	switch (pdrv)
	{
	case DISK_RAM:
		memcpy((void *)Address, src, Size);
		res = RES_OK;
		break;

	case DISK_EFS:
		//DUMP("SRC", (char *)src, 16);
		res = SerialFlash_Write(Address, src, Size);
		//DUMP("DST", (char *)Address, 16); //PRINTF("\n");
		break;
	}
	if (res)
	{
		PRINTF("[ERROR] disk_write( %d ) %d\n", (int)pdrv, res);
	}
	return res;
}

DWORD get_fattime(void) { return 0; }

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
	BYTE pdrv, /* Physical drive */
	BYTE cmd,  /* Control code */
	void *buff /* Buffer to send/receive control data */
)
{
	int res = RES_PARERR;
	switch (cmd)
	{
	case CTRL_SYNC:
		res = RES_OK;
		break;

	case GET_BLOCK_SIZE: // ??? erase
		*(WORD *)buff = FF_MAX_SS;
		res = RES_OK;
		break;

	case GET_SECTOR_SIZE: // 512
		*(WORD *)buff = FF_MAX_SS;
		res = RES_OK;
		break;

	case GET_SECTOR_COUNT:
		*(DWORD *)buff = media[pdrv].size / FF_MAX_SS;
		res = RES_OK;
		break;
	}

	return res;
}

/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/

void test_file(const char *file_name)
{
	FIL fd;		   /* File object */
	char line[64]; /* Line buffer */
	FRESULT res;   /* FatFs return code */
	UINT cnt;
	PRINTF("[TEST] filename: %s\n", file_name);

#if 1
	res = f_open(&fd, file_name, FA_WRITE | FA_CREATE_ALWAYS);
	PRINTF("[TEST] f_open( create, res = %d ) \n", res);
	if (res)
	{
		PRINTF("[ERROR] f_open()\n");
		return;
	}
	res = f_write(&fd, "Hello World 42                   ", 16, &cnt);
	PRINTF("[TEST] f_write( res = %d cnt = %d ) \n", res, cnt);
	f_close(&fd);
#endif

#if 1
	memset(line, 0, sizeof(line));
	res = f_open(&fd, file_name, FA_READ);
	PRINTF("[TEST] f_open( read, res = %d )\n", res);
	if (res)
	{
		PRINTF("[ERROR] f_open()\n");
		return;
	}
	res = f_read(&fd, line, 100, &cnt);
	PRINTF("[TEST] f_read( res = %d, cnt = %d ) %s\n", res, cnt, line);
	f_close(&fd);
#endif
}

void fs_init(bool format)
{
	int res;
#if 0
	PRINTF("[i] RAM A 0x%08lX\n", (uint32_t)__efs_ram_begin__);
	PRINTF("[i] RAM S 0x%08lX\n", (uint32_t)__efs_ram_size__);
	PRINTF("[i] EFS A 0x%08lX\n", (uint32_t)__efs_rom_begin__);
	PRINTF("[i] EFS S 0x%08lX\n", (uint32_t)__efs_rom_size__);
#endif
	PRINTF("[FS] INIT RAMDISK\n");
#if 0	
	PRINTF("  ADDR  : 0x%08lX\n", media[DISK_RAM].address);
	PRINTF("  SIZE  : %lu\n", media[DISK_RAM].size);
#endif

#if 1
	media[DISK_RAM].is_register = 1; // enable

	res = f_mount(&media[DISK_RAM].filesystem, "ram:", 0);
	PRINTF("[FS] RAMDISK MOUNT = %d\n", res);

	res = f_mkfs("ram:", 0, (void *)media[DISK_RAM].work, WORK_SIZE);
	PRINTF("[FS] RAMDISK FORMAT = %d\n", res);

	test_file("ram:test.txt");
#endif

	////////////////////////////////////////////////////////////////////////////////
	PRINTF("[FS] INIT FLASHDISK\n");
#if 0
	PRINTF("  ADDR  : 0x%08lX\n", media[DISK_EFS].address);
	PRINTF("  SIZE  : %lu\n", media[DISK_EFS].size);
#endif
	bool SerialFlash_Init(void);
	if (SerialFlash_Init())
	{
		media[DISK_EFS].is_register = 1; // enable

		res = f_mount(&media[DISK_EFS].filesystem, "efs:", 0);
		PRINTF("[FS] EFS MOUNT = %d\n", res);

		if (format)
		{
			res = f_mkfs("efs:", 0, (void *)media[DISK_EFS].work, WORK_SIZE);
			PRINTF("[FS] EFS FORMAT = %d\n", res);
		}

		test_file("efs:test.txt");
	}
}

#endif