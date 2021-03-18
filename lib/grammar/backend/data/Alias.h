//
// Created by Rohit Maurya on 2/26/2020.
//

#ifndef infinity_ALIAS_H
#define infinity_ALIAS_H

#include "DataEntity.h"

class Utype;

class Alias : public DataEntity {
public:
    Alias()
            :
            DataEntity(),
            utype(NULL)
    {
    }

    Alias(long guid, string name, ClassObject* owner, List<AccessFlag>& flags,
          Meta& meta)
            :
            DataEntity(),
            utype(NULL)
    {
        this->type = type;
        this->guid = guid;
        this->name = name;
        this->owner = owner;
        this->flags.addAll(flags);
    }

    void free();

    Utype *utype;
};


#endif //infinity_ALIAS_H
