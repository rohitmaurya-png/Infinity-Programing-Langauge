//
// Created by rohit  on 7/24/2020.
//

#ifndef infinity_RANDOM_H
#define infinity_RANDOM_H
#include "../../../stdimports.h"
#include "../../util/time.h"

void setSeed(unsigned int seed);

Int randInt();

Int randInt(Int min, Int max);

double randDouble();

double randDouble(double min, double max);

#endif //infinity_RANDOM_H
