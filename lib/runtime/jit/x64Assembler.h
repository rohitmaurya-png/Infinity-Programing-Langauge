//
//Created by Rohit Maurya 8/23/2019.
//

#ifndef infinity_X64ASSEMBLER_H
#define infinity_X64ASSEMBLER_H

#include "_BaseAssembler.h"

#ifdef BUILD_JIT
class x64Assembler : public _BaseAssembler {
public:
    x64Assembler()
    :
            _BaseAssembler()
    {
        initialize();
    }

private:
    x86::Mem getMemPtr(x86int_t addr) override ;
    x86::Mem getMemPtr(x86::Gp reg, x86int_t addr) override ;
    x86::Mem getMemPtr(x86::Gp reg) override ;
    x86int_t getRegisterSize() override ;
    void initializeRegisters() override ;
};
#endif

#endif //infinity_X64ASSEMBLER_H
