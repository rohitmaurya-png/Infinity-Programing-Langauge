//
// Created by Rohit Maurya on 6/27/2020.
//

#ifndef infinity_UTIL_H
#define infinity_UTIL_H

// Define EXPORTED for any platform
#if defined _WIN32 || defined __CYGWIN__
#define EXPORTED __attribute__ ((dllexport))
#define NOT_EXPORTED
#elif defined(__GNUC__)
//  GCC
    #define EXPORTED __attribute__((visibility("default")))
    #define NOT_EXPORTED
#else
    //  do nothing and hope for the best?
    #define EXPORTED
    #define NOT_EXPORTED
    #pragma warning Unknown dynamic link import/export semantics.
#endif

typedef void* object;

#endif //infinity_UTIL_H
