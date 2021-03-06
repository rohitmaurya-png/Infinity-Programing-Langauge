//
//Created by Rohit Maurya 6/19/2020.
//

#ifndef infinity_PROFILER_H
#define infinity_PROFILER_H

#include "../../stdimports.h"
#include "symbols/Method.h"

#ifdef infinity_PROF_

struct funcProf {
    Method *func;
    size_t hits;  // total hits for function
    float time;  // time used per call
    float avgtm; // average time user for call
    int64_t past, ir;

    funcProf(Method *f) {
        hits=0;
        time=0;
        avgtm=0;
        ir=0;
        func = f;
    }

    funcProf() {
        hits=0;
        time=0;
        avgtm=0;
        ir=0;
        func = NULL;
    }

    void init() {
        hits=0;
        time=0;
        avgtm=0;
        ir=0;
        func = NULL;
    }

    ~funcProf() {
        hits=0;
        time=0;
        avgtm=0;
        ir=0;
        func = NULL;
    }

    void operator=(funcProf prof) {
        func=prof.func;
        hits=prof.hits;
        time = prof.time;
        ir=prof.ir;
        avgtm = prof.avgtm;
    }

    void free() {
        func = NULL;
    }
};

class Profiler {
public:
    Profiler(std::string &out)
    :
        functions(),
        totalHits(0),
        starttm(0),
        endtm(0)
    {
    }

    Profiler()
            :
            functions(),
            totalHits(0),
            starttm(0),
            endtm(0)
    {
    }

    void init(size_t stack) {
        new (&calls) std::vector<funcProf>();
        calls.reserve(stack/2);
        functions.init();
        totalHits=0;
        starttm=0;
        endtm=0;
    }
    void hit(Method *func);
    void profile();

    size_t totalHits; // total ammount of function calls
    int64_t starttm, endtm;
    size_t lastHit;
    std::vector<funcProf> calls;
    _List<funcProf> functions;

    void dump();

    void free();
};


#endif

#endif //infinity_PROFILER_H
