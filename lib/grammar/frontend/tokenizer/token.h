//
// Created by rohit  on 11/13/2017.
//

#ifndef infinity_TOK_H
#define infinity_TOK_H


#include "../../../../stdimports.h"
#include "tokentype.h"

enum token_id
{
    IDENTIFIER=0,
    NATIVE_TYPE=1,
    TYPE_IDENTIFIER=2,
    INTEGER_LITERAL=3,
    HEX_LITERAL=4,
    MODULENAME=5,
    STRING_LITERAL=6,
    CHAR_LITERAL=7,
    LITERAL=8,
    VALUE=9,
    ACCESSTYPE=10,
    SINGLELINE_COMMENT=11,
    RETURN_STATEMENT=12,

    SINGLE=13, // single token
    NOENTITY=1999 // not an entity
};

/**
 * Token entity is a class that represents a single entity (not a group of token types).
 * For instance a token entity could be an identifier or native type but not a module name.
 */
class Token
{
public:
    Token(string val, token_id tokenid, int col, int line, token_type type = NONE)
            :
            tok(val),
            type(type),
            id(tokenid),
            col(col),
            line(line)
    {
    }

    Token()
            :
            tok(""),
            type(NONE),
            id(NOENTITY),
            col(0),
            line(0)
    {
    }
    ~Token()
    {
        tok.clear();
    }

    bool operator!=(const token_id& id)
    {
        return this->id != id;
    }

    bool operator==(const token_id& id)
    {
        return this->id == id;
    }
    void operator=(const Token& token)
    {
        tok = token.tok;
        line = token.line;
        col = token.col;
        id = token.id;
        type = token.type;
    }
    bool operator!=(const token_type& ty)
    {
        return this->type == ty;
    }
    bool operator==(const string& s)
    {
        return this->tok == s;
    }
    bool operator!=(const string& s)
    {
        return this->tok != s;
    }

    token_id getId();
    void setId(token_id id);
    string &getValue();
    int getColumn();
    int getLine();
    bool isSingle();
    token_type getType();

private:
    int line;
    int col;
    token_id id;
    token_type type;
    string tok;
};


#endif //infinity_TOKEN_H
