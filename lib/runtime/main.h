//
//Created by Rohit Maurya 2/15/2020.
//

#ifndef infinity_MAIN_H
#define infinity_MAIN_H

#include "../../stdimports.h"

int runtimeStart(int argc, const char* argv[]);

void error(string message);

#define progname "infinity"
#define rev "r6"
#define progvers "2.6.0" rev

#ifdef infinity_PROF_
enum profilerSort {
    tm,
    avgt,
    calls,
    ir
};
#endif

struct options {

    bool debugMode = false;

    /**
     * JIT Compiler enabled by default to boost infinity
     * speeds of 25-50%+ faster than the original 
     * program speed
     */
    bool jit = true;

    /**
     * Slow boot allows you to not care about the initial startup time
     * gained by not compiling all the user code. This option will force
     * the JIT to imediatley compile all functions at startup-time to allow
     * for faster processing at runtime once completed.
     */
    bool slowBoot = true;

#ifdef infinity_PROF_
    int sortBy = profilerSort::tm;
#endif
};

extern options c_options;

#endif //infinity_MAIN_H
