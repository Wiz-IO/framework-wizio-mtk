////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Georgi Angelov
//      Simple UART driver, Arduino style, used for Serial class
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

/* Simple VFS */

#ifdef USE_FS

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef USE_FATFS
#include <ff.h>
#else
#include <lfs.h>
#endif

extern uint32_t GenHashStr(char *str);

#define FILES_FD_BASE 3 /* skip STDERR_FILENO */
#define MAX_OPEN_FILES 8

typedef struct vfs_node_s
{
    int fd;            // is fd
    unsigned int hash; // find open
#ifdef USE_FATFS
    FIL file;
#else
    lfs_file_t file;
#endif
} vfs_node_t;

vfs_node_t vfs_files[MAX_OPEN_FILES] = {0};

static bool vfs_is_file(int fd)
{
    return (fd >= FILES_FD_BASE && fd < FILES_FD_BASE + MAX_OPEN_FILES);
}

static bool vfs_is_open(const char *path)
{
    for (int i = 0; i < MAX_OPEN_FILES; i++)
        if (vfs_files[i].hash && vfs_is_file(vfs_files[i].fd) && GenHashStr(path) == vfs_files[i].hash)
            return true;
    return false;
}

static int vfs_get_index(int fd)
{
    if (vfs_is_file(fd))
        for (int i = 0; i < MAX_OPEN_FILES; i++)
            if (vfs_files[i].fd == fd)
                return i;
    return -1;
}

static vfs_node_t *vfs_get_context(int fd)
{
    int i = vfs_get_index(fd);
    if (-1 == i)
        return 0;
    return &vfs_files[i];
}

static int vfs_get_free_index(void)
{
    for (int i = 0; i < MAX_OPEN_FILES; i++)
        if (0 == vfs_files[i].fd && 0 == vfs_files[i].hash)
            return i;
    return -1; // no mem
}

/*


*/

int vfs_open(const char *path, int flags, int mode_not_used)
{
    (void)mode_not_used;
    int res, i = -1;
    if (vfs_is_open(path))
    {
        printf("[ERROR] vfs_open file is open\n");
        return -1;
    }

    if (-1 == (i = vfs_get_free_index()))
    {
        printf("[ERROR] vfs_open no mem\n");
        return -1;
    }

    vfs_node_t *p = &vfs_files[i];

#ifdef USE_FATFS
    if ((res = f_open(&p->file, path, flags)))
#else
    if ((res = lfs_file_open(&lfs_drive, &p->file, path, flags)))
#endif
    {
        printf("[ERROR] vfs_open( %d ) %s\n", res, path);
        memset(p, 0, sizeof(vfs_node_t));
        return -1;
    }

    p->hash = GenHashStr(path);
    return p->fd = i + FILES_FD_BASE;
}

int vfs_close(int fd)
{
    int res;
    vfs_node_t *p = vfs_get_context(fd);
    if (NULL == p)
    {
        printf("[ERROR] vfs_close vrong fd\n");
        return -1;
    }
#ifdef USE_FATFS
    f_close(&p->file);
#else
    int res = lfs_file_close(&lfs_drive, &p->file);
#endif
    memset(p, 0, sizeof(vfs_node_t));
    return res;
}

size_t vfs_write(int fd, const char *buf, size_t size)
{
    int res = 0, cnt;
    vfs_node_t *p = vfs_get_context(fd);
    if (p && buf && size && vfs_is_file(p->fd))
#ifdef USE_FATFS
        res = f_write(&p->file, buf, (UINT)size, (UINT *)&cnt);
#else
        return lfs_file_write(&lfs_drive, &p->file, buf, size);
#endif

    return res == 0 ? cnt : 0;
}

size_t vfs_read(int fd, char *buf, size_t size)
{
    int res = 0, cnt;
    vfs_node_t *p = vfs_get_context(fd);
    if (p && buf && size && vfs_is_file(p->fd))
#ifdef USE_FATFS
        res = f_read(&p->file, buf, (UINT)size, (UINT *)&cnt);
#else
        return lfs_file_read(&lfs_drive, &p->file, buf, size);
#endif
    return res == 0 ? cnt : 0;
}

_off_t vfs_seek(int fd, _off_t offset, int whence)
{
    int res = -1;
    vfs_node_t *p = vfs_get_context(fd);
    if (p && vfs_is_file(p->fd))
#ifdef USE_FATFS
        switch (whence)
        {
        case SEEK_SET:
            return f_lseek(&p->file, offset);
        case SEEK_CUR:
            return f_lseek(&p->file, f_tell(&p->file) + offset);
        case SEEK_END:
            return f_lseek(&p->file, f_size(&p->file) + offset);
        }
#else
        return lfs_file_seek(&lfs_drive, &p->file, offset, whence);
#endif
    return -1;
}

#endif