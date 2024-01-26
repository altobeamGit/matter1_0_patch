/* internal use only -- mapping of "system calls" for libraries that lose
   and only provide C names, so that we end up in violation of ANSI */
#ifndef __SYSLIST_H
#define __SYSLIST_H

#ifdef MISSING_SYSCALL_NAMES
#define _close close
#define _execve execve
#define _fcntl fcntl
#define _fork fork
#define _fstat fstat
#define _getpid getpid
#define _gettimeofday gettimeofday
#define _isatty isatty
#define _kill kill
#define _link link
#define _lseek lseek
#define _mkdir mkdir
#define _open open
#define _read read
#define _sbrk sbrk
#define _stat stat
#define _times times
#define _unlink unlink
#define _wait wait
#define _write write
#else
#define close(...) _close(__VA_ARGS__)
#define execve(...) _execve(__VA_ARGS__)
#define fcntl(...) _fcntl(__VA_ARGS__)
#define fork(...) _fork(__VA_ARGS__)
#define fstat(...) _fstat(__VA_ARGS__)
#define getpid(...) _getpid(__VA_ARGS__)
#define gettimeofday(...) _gettimeofday(__VA_ARGS__)
#define isatty(...) _isatty(__VA_ARGS__)
#define kill(...) _kill(__VA_ARGS__)
#define link(...) _link(__VA_ARGS__)
#define lseek(...) _lseek(__VA_ARGS__)
#define mkdir(...) _mkdir(__VA_ARGS__)
#define open(...) _open(__VA_ARGS__)
#define read(...) _read(__VA_ARGS__)
#define sbrk(...) _sbrk(__VA_ARGS__)
#define stat(...) _stat(__VA_ARGS__)
#define times(...) _times(__VA_ARGS__)
#define unlink(...) _unlink(__VA_ARGS__)
#define wait(...) _wait(__VA_ARGS__)
#define write(...) _write(__VA_ARGS__)
#endif /* MISSING_SYSCALL_NAMES */

#if defined MISSING_SYSCALL_NAMES || !defined HAVE_OPENDIR
/* If the system call interface is missing opendir, readdir, and
   closedir, there is an implementation of these functions in
   libc/posix that is implemented using open, getdents, and close. 
   Note, these functions are currently not in the libc/syscalls
   directory.  */
#define _opendir opendir
#define _readdir readdir
#define _closedir closedir
#endif /* MISSING_SYSCALL_NAMES || !HAVE_OPENDIR */

#endif /* !__SYSLIST_H_ */
