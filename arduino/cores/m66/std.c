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

#include "interface.h"
void STD_Dummy(void) {} /* load this file */

////////////////////////////////////////////////////////////////////////////
// INIT CPP
////////////////////////////////////////////////////////////////////////////

extern void (*__preinit_array_start[])(void) __attribute__((weak));
extern void (*__preinit_array_end[])(void) __attribute__((weak));
extern void (*__init_array_start[])(void) __attribute__((weak));
extern void (*__init_array_end[])(void) __attribute__((weak));
extern void (*__fini_array_start[])(void) __attribute__((weak));
extern void (*__fini_array_end[])(void) __attribute__((weak));
extern void _init(void) __attribute__((weak));
extern void _fini(void) __attribute__((weak));

void __libc_init_array(void)
{
    size_t count;
    size_t i;
    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; i++)
        __preinit_array_start[i]();
    _init();
    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++)
        __init_array_start[i]();
}

void __libc_fini_array(void)
{
    size_t count;
    size_t i;
    count = __fini_array_end - __fini_array_start;
    for (i = count; i > 0; i--)
        __fini_array_start[i - 1]();
    _fini();
}

extern void __cxa_finalize(void *handle) {}
extern void __cxa_pure_virtual(void) { abort(); }
extern void __cxa_deleted_virtual(void) { abort(); }
void __sync_synchronize(void) {}

////////////////////////////////////////////////////////////////////////////
// SYSCALL
////////////////////////////////////////////////////////////////////////////
int vfs_open(const char *path, int flags, int mode);
int vfs_close(int fd);
size_t vfs_write(int fd, const char *buf, size_t size);
size_t vfs_read(int fd, char *dst, size_t size);
_off_t vfs_seek(int fd, _off_t where, int whence);
////////////////////////////////////////////////////////////////////////////

void abort(void)
{
    while (1)
        ;
}

extern void *_sbrk(int incr) { return (void *)-1; }

void _exit(int status) { abort(); }
void _kill(int pid, int sig) { return; }
int _getpid(void) { return -1; }
int _getpid_r(struct _reent *r) { return -1; }
void _raise(void) { return; }
int __attribute__((weak)) _link(const char *__path1 __attribute__((unused)), const char *__path2 __attribute__((unused))) { return 0; }

int _unlink(const char *path)
{
    return 0; //?
}

int _stat(const char *path, struct stat *st)
{
#ifdef USE_FS
// TODO
#endif
    return -1;
}

int _fstat_r(struct _reent *ignore, int fd, struct stat *st)
{
#ifdef USE_FS
// TODO
#endif
    return -1;
}

int _isatty(int fd) { return ((fd == STDOUT_FILENO) || (fd == STDERR_FILENO) || (fd == STDIN_FILENO)); }

int _open_r(struct _reent *ignore, const char *path, int flags, int mode)
{
#ifdef USE_FS
    return vfs_open(path, flags, mode);
#endif
    return -1;
}

int _close_r(struct _reent *ignore, int fd)
{
#ifdef USE_FS
    return vfs_close(fd);
#endif
    return -1;
}

_ssize_t _write_r(struct _reent *ignore, int fd, const void *buf, size_t len)
{
    if (fd < 0 || !buf || !len)
        return 0;
    if ((fd == STDOUT_FILENO) || (fd == STDERR_FILENO)) // for printf
        if (stdout->_cookie && stdout->_write)
            return stdout->_write(0, stdout->_cookie, buf, len);            
#ifdef USE_FS
    return vfs_write(fd, buf, len);
#endif
    return 0;
}

_ssize_t _read_r(struct _reent *ignore, int fd, void *buf, size_t len)
{
    if (fd < 0 || !buf || !len)
        return 0;

    if (fd == STDIN_FILENO) // is not set, by default
        if (stdin->_cookie && stdout->_write)
            return stdout->_read(0, stdout->_cookie, buf, len);
#ifdef USE_FS
    return vfs_read(fd, buf, len);
#endif
    return 0;
}

_off_t _lseek_r(struct _reent *ignore, int fd, _off_t where, int whence)
{
#ifdef USE_FS
    return vfs_seek(fd, where, whence);
#endif
    return -1;
}

////////////////////////////////////////////////////////////////////////////////////////
static unsigned int rand_next = 0;
void srand(unsigned int seed) { rand_next = seed; }

int rand(void)
{
    if (0 == rand_next)
        rand_next = HAL_SEED;
    rand_next = rand_next * 1103515245 + 12345;
    return ((unsigned int)(rand_next / 65536) % 32768);
}

////////////////////////////////////////////////////////////////////////////////////////
// TIME #include <sys/time.h>
////////////////////////////////////////////////////////////////////////////////////////

time_t now(void)
{
    extern time_t RTC_getDateTime(void);
    return RTC_getDateTime();
}

time_t _Time(time_t *timer) { return now(); }

time_t _time(time_t *tod)
{
    time_t t = _Time(NULL);
    if (tod)
        *tod = t;
    return (t);
}

clock_t _times(struct tms *buf)
{
    return millis(); // clock() is uint64_t, ... use MILLIS(), millis() or disable nano.specs
}

int _gettimeofday_r(struct _reent *ignore, struct timeval *tv, void *tz)
{
    /* time() */
    (void)tz;
    if (tv)
    {
        tv->tv_sec = now();
        tv->tv_usec = 0;
        return 0;
    }
    return -1;
}

//////////////////////
