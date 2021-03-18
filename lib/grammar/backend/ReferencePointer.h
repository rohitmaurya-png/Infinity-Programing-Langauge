//
//Created by Rohit Maurya 11/5/2019.
//

#ifndef infinity_REFRENCEPOINTER_H
#define infinity_REFRENCEPOINTER_H

#include "../../../stdimports.h"
#include "../List.h"

class ReferencePointer {
public:
    ReferencePointer()
    :
        mod("")
    {
        classes.init();
    }

    void free() {
        classes.free();
    }

    string mod;
    List<string> classes;
};

#endif //infinity_REFRENCEPOINTER_H
