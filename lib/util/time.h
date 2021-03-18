//
//Created by Rohit Maurya 1/22/2020.
//

#ifndef infinity_TIME_H
#define infinity_TIME_H


#include <cstdint>

class Clock {
public:
    static int64_t realTimeInNSecs();
    static int __os_time(int ty);


};

#define NANO_TOMICRO(x) ((x)/1000L)

#define NANO_TOMILL(x) (NANO_TOMICRO(x)/1000L)

#define NANO_TOSEC(x) (NANO_TOMILL(x)/1000L)

#endif //infinity_TIME_H
