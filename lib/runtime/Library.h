//
// Created by Rohit Maurya on 6/27/2020.
//

#ifndef infinity_LIBRARY_H
#define infinity_LIBRARY_H

#include "symbols/string.h"

class Library {
public:
    Library()
    {
        init();
    }

    void init() {
        handle = NULL;
        name.init();
    }
#ifdef _WIN32
    HINSTANCE handle;
#else
    void* handle;
#endif
    native_string name;
};

#endif //infinity_LIBRARY_H
