//
//Created by Rohit Maurya 11/15/2019.
//

#ifndef infinity_LITERAL_H
#define infinity_LITERAL_H

#include "../../../../stdimports.h"
#include "DataEntity.h"

enum literal_type
{
    numeric_literal,
    string_literal,
    unknown_literal
};

class Literal : public DataEntity {
public:
    Literal()
    :
        DataEntity(),
        literalType(unknown_literal),
        stringData(""),
        numericData(0)
    {
    }

    Literal(string &data, long address)
            :
            DataEntity(),
            literalType(string_literal),
            stringData(data)
    {
        this->address = address;
        type = _INT8;
    }

    explicit Literal(double data, DataType type = VAR)
            :
            DataEntity(),
            literalType(numeric_literal),
            stringData(""),
            numericData(data)
    {
        this->type = type;
    }
    void free();

    literal_type literalType;
    string stringData;
    double numericData;
};

#endif //infinity_LITERAL_H
