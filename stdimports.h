//
// Created by rohit  on 1/6/2017.
//

#ifndef infinity_STDIMPORTS_H
#define infinity_STDIMPORTS_H

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <list>
#include <stdexcept>
#include <string>
#include <cstdio>
#include <cstring>
#include <thread>
#include "lib/runtime/jit/architecture.h"

using namespace std;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define WIN32_
#else
#define POSIX_
#endif

#ifdef WIN32_

#define __os_yield() std::this_thread::yield();
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#ifndef __WINDEF_
#include <windef.h>
#endif
#endif
#ifdef POSIX_
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdarg.h>
#include <dlfcn.h>

#define POSIX_USEC_INTERVAL 1000

#define MUTEX pthread_mutex_t
#define __os_yield() sched_yield();
#endif

//#define infinity_PROF_
#ifdef infinity_PROF_
#define PROFILER_NAME "tanto"
#endif

#ifndef DEBUGGING
#define DEBUGGING
#endif

#if _ARCH_BITS == 32
typedef int32_t Int;
typedef uint32_t uInt;
#define _INT_MAX INT32_MAX
#else
typedef int64_t Int;
typedef uint64_t uInt;
#define _INT_MAX INT64_MAX
#endif

typedef void (*fptr)(void *);

struct infinity {
    /**
     * Jan 13, 2017 Initial build release of infinity 0.1.3, this contains
     * all the base level support for running infinity. infinity was
     * developed to support both windows and linux operating systems
     *
     * Base is no longer supported as it is too old to support
     */
    int BASE  = 1;

    /**
     * Jan 18, 2018 Build release of infinity 0.2.0, this contains all the base level
     * support for infinity as well as major performance enhancments and improvements
     * throughout the platform.
     */
    int ALPHA = 2;
};

extern infinity versions;

void* __malloc(uInt bytes);
void* __calloc(uInt n, uInt bytes);
void* __realloc(void *ptr, uInt bytes, uInt);
void __os_sleep(Int);
void setupSigHandler();

#define CXX11_INLINE inline


#endif //infinity_STDIMPORTS_H
