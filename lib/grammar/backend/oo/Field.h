//
//Created by Rohit Maurya 10/22/2019.
//

#ifndef infinity_FIELD_H
#define infinity_FIELD_H

#include "../data/DataEntity.h"

class Param;
class Method;
class Expression;
class Utype;

class Field : public DataEntity {
public:
    Field()
        :
            DataEntity(),
            isArray(false),
            nullField(false),
            utype(NULL),
            locality(stl_stack),
            inlineCheck(false),
            local(false),
            getter(NULL),
            setter(NULL),
            scopeLevel(0),
            initialized(true),
            initialized_pc(invalidAddr)
    {
    }

    Field(DataType type, long guid, string name, ClassObject* owner, List<AccessFlag>& flags,
           Meta& meta, StorageLocality stl, long addr)
        :
        DataEntity(),
        locality(stl),
        isArray(false),
        nullField(false),
        utype(NULL),
        inlineCheck(false),
        local(false),
        getter(NULL),
        setter(NULL),
        scopeLevel(0),
        initialized(true),
        initialized_pc(invalidAddr)
    {
        this->type = type;
        this->guid = guid;
        this->name = name;
        this->address = addr;
        this->owner = owner;
        this->flags.addAll(flags);
        this->meta.copy(meta);
    }

    void free();
    bool equals(Field& f);
    bool isRelated(Field &f);
    string toString();

public:
    bool isArray;
    bool nullField;
    bool inlineCheck;
    bool local;
    bool initialized;
    Int scopeLevel;
    uInt initialized_pc; // for local variable use ONLY
    StorageLocality locality;
    Method* getter, *setter;
    Utype *utype;
};


#endif //infinity_FIELD_H
