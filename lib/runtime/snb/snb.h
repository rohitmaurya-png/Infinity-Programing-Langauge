//
// Created by Rohit Maurya on 6/27/2020.
//

#ifndef infinity_SNB_H
#define infinity_SNB_H

#include "../../../stdimports.h"
#include "util.h"

typedef short (*lib_handshake)(void *lib_funcs[], int);

bool setupLibrary(lib_handshake lhand);

#ifdef _WIN32
#define load_func(handle, name) GetProcAddress(handle, name)
#else
#define load_func(handle, name) dlsym(handle, name)
#endif

#ifdef _WIN32
#define load_lib(lib) LoadLibrary(lib.c_str())
#else
#define load_lib(lib) dlopen(lib.c_str(), RTLD_LAZY)
#endif

#ifdef _WIN32
#define free_lib(handle) FreeLibrary(handle)
#else
#define free_lib(handle) dlclose(handle)
#endif


#endif //infinity_SNB_H
