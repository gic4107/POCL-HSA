typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

// clang bug: doubles not available on x86. See bug #911911
#ifdef __x86_64
#define cles_khr_int64
#define cl_khr_fp16
#define cl_khr_fp64
#else
#define __EMBEDDED_PROFILE__ 1
#undef cles_khr_int64
#define cl_khr_fp16
#undef cl_khr_fp64
#endif

#if 64 == 64
typedef ulong size_t;
typedef long ptrdiff_t;
typedef long intptr_t;
typedef ulong uintptr_t;
#else
typedef uint size_t;
typedef int ptrdiff_t;
typedef int intptr_t;
typedef uint uintptr_t;
#endif
