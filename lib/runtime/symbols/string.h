//
//Created by Rohit Maurya 2/11/2020.
//

#ifndef infinity_STRING_H
#define infinity_STRING_H

#include <string>
#include "../../../stdimports.h"
#include "../../util/File.h"

class native_string {
public:
    native_string(string value)
            :
            len(0)
    {
        if(value == "") {
            chars = NULL;
        } else {
            len=value.size();
            chars = (char*)malloc(sizeof(char)*(len+1));
            assign(value);
            chars[len]=0;
        }
    }

    native_string(const char value[])
            :
            len(0),
            chars(NULL)
    {
        set(value);
    }

    native_string(const double *value, long max)
            :
            len(0),
            chars(NULL)
    {
        set(value, max);
    }

    native_string()
    {
        init();
    }

    void set(const char value[]) {
        free();

        len=strlen(value);

        if(len==0) {
            chars = NULL;
        } else {
            chars = (char*)malloc(sizeof(char)*(len+1));
            std::memcpy(chars, value, sizeof(char)*len);
            chars[len]=0;
        }
    }

    void set(const double* value, long max) {
        free();

        if(value == NULL || max==0) {
            chars = NULL;
        } else {
            len = max;
            chars = (char*)malloc(sizeof(char)*(max+1));
            for(int64_t i = 0; i < max; i++)
                chars[i] = (char)value[i];
            chars[len]=0;

        }
    }

    void init() {
        len=0;
        chars=NULL;
    }

    void operator+=(const string &str) {

        if(str != "") {
            len += str.size();
            for(char c : str) {
                operator+=(c);
            }
        }
    }

    bool operator==(native_string str) const {
        if(str.len != len) return false;
        for(int64_t i = 0; i < len; i++) {
            if(str.chars[i] != chars[i])
                return false;
        }
        return true;
    }

    bool operator!=(const string &str) const {
        return !operator==(str);
    }

    void operator=(const string &_str) {
        free();

        if(_str.size()>0) {
            len = _str.size();
            chars = (char*)malloc(sizeof(char)*(_str.size()+1));
            if(chars != NULL) {
                std::memcpy(chars, _str.c_str(), sizeof(char) * len);
                chars[len] = 0;
            }
            else {
                len = 0;
            }
        }
    }

    native_string& operator=(const native_string _str) {
        free();

        if(_str.len>0) {
            len = _str.len;
            chars = (char*)malloc(sizeof(char)*(_str.len+1));
            if(chars != NULL) {
                std::memcpy(chars, _str.chars, sizeof(char) * len);
                chars[len] = 0;
            }
            else {
                len = 0;
            }
        }

        return *this;
    }

    int injectBuff(File::buffer &buf) {

        if(buf.size()>0) {
            len = buf.size();
            if(len) {
                chars = (char*)malloc(sizeof(char)*buf.size());
                if(chars==NULL) { len = 0; return 1; }

                std::memcpy(chars, buf.data(), sizeof(char)*len);
            } else
                chars = NULL;
        } else {
            free();
        }

        return 0;
    }

    void operator+=(const char &c) {
        if(len == 0) {
            len=1;
            chars=(char*)malloc(sizeof(char)*(len+1));
            if(chars != NULL) {
                chars[0] = c;
                chars[len]=0;
            }
            else {
                len = 0;
            }
        } else {
            char*tmp = (char*)realloc(chars,sizeof(char)*(len+2));
            if(tmp != NULL) {
                chars = tmp;
                chars[len++] = c;
                chars[len]=0;
            }
        }
    }

    CXX11_INLINE
    string str() {
        if(len==0) return "";
        return string(chars,len);
    }

    const char* c_str() {
        return chars;
    }

    void free() {
        if(chars != NULL) {
            std::free(chars);
            chars = NULL;
        }
        len = 0;
    }

    char *chars;
    int64_t len;

private:

    CXX11_INLINE
    void assign(const string &value) {
        for(int64_t i = 0; i < len; i++)
            chars[i] = value.at(i);
    }

    CXX11_INLINE
    void set(const native_string& value) {
        for(int64_t i = 0; i < len; i++)
            chars[i] = value.chars[i];
    }
};

namespace runtime {
 typedef native_string String;
}

using namespace runtime;
#endif //infinity_STRING_H
