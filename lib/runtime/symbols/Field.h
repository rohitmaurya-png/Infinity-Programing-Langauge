//
//Created by Rohit Maurya 2/12/2020.
//

#ifndef infinity_FIELD_H
#define infinity_FIELD_H

#include "../../../stdimports.h"
#include "string.h"
#include "../../grammar/DataType.h"
#include "symbol.h"

class ClassObject;

class Field : public Symbol {
public:
    Field()
    {
        init();
    }

    void init() {
        utype = NULL;
        isArray = false;
        threadLocal = false;
        Symbol::init();
    }

    void operator=(Field* field) {
        this->address = field->address;
        this->guid = field->guid;
        this->name = field->name;
        this->fullName = field->fullName;
        this->type = field->type;
        this->owner = field->owner;
        this->flags = field->flags;
        this->isArray=field->isArray;
        this->threadLocal = field->threadLocal;
        this->utype = field->utype;
    }

    Symbol *utype;
    bool isArray;
    bool threadLocal;
};

#define IS_STATIC(flags) ((flags >> 5) & 1UL)


#endif //infinity_FIELD_H
