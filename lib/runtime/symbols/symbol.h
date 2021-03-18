//
// Created by Rohit Maurya on 5/18/2020.
//

#ifndef infinity_SYMBOL_H
#define infinity_SYMBOL_H

#include "../../../stdimports.h"
#include "string.h"
#include "../../grammar/DataType.h"

extern int invalidAddr;
class ClassObject;
class Symbol {
public:
    Symbol()
    {
        init();
    }

    void init() {
        address = invalidAddr;
        guid = invalidAddr;
        name.init();
        fullName.init();
        owner = NULL;
        flags = 0;
        type = UNDEFINED;
    }

    void free() {
        name.free();
        fullName.free();
    }

    int32_t address;
    int32_t guid;
    String name;
    String fullName;
    ClassObject *owner;
    uint32_t flags;
    DataType type;
};

#endif //infinity_SYMBOL_H
