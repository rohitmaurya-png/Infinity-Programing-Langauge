//
//Created by Rohit Maurya 11/14/2019.
//

#ifndef infinity_EXPRESSION_H
#define infinity_EXPRESSION_H

#include "../../../stdimports.h"

class Utype;
class Ast;

enum expression_type
{
    exp_null,
    exp_var,
    exp_object,
    exp_class,
    exp_nil,
    exp_undefined
};

class Expression {
public:
    Expression()
    {
        init();
    }

    void init();
    void copy(Expression *expr);

    Utype* utype;
    expression_type type;
    Ast* ast;
};


#endif //infinity_EXPRESSION_H
