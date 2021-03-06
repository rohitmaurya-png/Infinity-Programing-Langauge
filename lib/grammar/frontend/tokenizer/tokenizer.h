//
// Created by rohit  on 11/13/2017.
//

#ifndef infinity_TOKENIZER_H
#define infinity_TOKENIZER_H

#include <list>
#include "../../../../stdimports.h"
#include "token.h"
#include "../ErrorManager.h"
#include "../../List.h"

class tokenizer
{
public:
    tokenizer(const string tokens, const string file)
            :
            toks(""),
            empty(""),
            len(tokens.size()),
            cursor(0),
            col(0),
            line(1),
            file(file),
            dynamicString(false),
            brackets(0)
    {
        this->tokens.init();
        lines.init();

        if(!tokens.empty()) {
            toks = tokens;
            data = tokens.c_str();
        } else {
            toks = "";
            data = NULL;
            len = 0;
        }

        parse();
    }

    unsigned long getEntityCount();
    List<Token>& getTokens();
    ErrorManager* getErrors();
    List<string>& getLines();
    string &getData();
    Int size() { return tokens.size(); }

    string file;
    Token* EOF_token;
    void free();

    static string tokenTypeToString(token_type);

private:
    void parse();
    void parse_lines();
    bool ismatch(char i, char current);
    string get_escaped_string(string msg) const;

    List<Token> tokens;
    ErrorManager* errors;
    List<string> lines;
    string toks, empty;
    const char* data;
    uInt len;
    uInt cursor;
    int line;
    int col;
    bool dynamicString;
    long brackets;

    CXX11_INLINE void saveString(const stringstream &message, bool escaped_found);

    CXX11_INLINE void parseIdentifier();
};

#endif //infinity_TOKENIZER_H
