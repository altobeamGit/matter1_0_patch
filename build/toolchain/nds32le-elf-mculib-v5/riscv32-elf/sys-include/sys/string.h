#ifndef _SYS_STRING_H
#define _SYS_STRING_H

static __inline unsigned long __libc_detect_null(unsigned long w)
{
#ifdef __nds_v5
  unsigned long r;
  __asm__ volatile("ffb %0, %1, zero" : "=r"(r) : "r"(w));
  return r != 0;
#else
  unsigned long mask = 0x7f7f7f7f;
  if (sizeof (long) == 8)
    mask = ((mask << 16) << 16) | mask;
  return ~(((w & mask) + mask) | w | mask);
#endif
}

#endif
