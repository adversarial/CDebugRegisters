
#include "..\CDebugRegisters\CDebugRegisters.hpp"

using namespace dbg;

extern "C" {

    CDebugRegisters* NewDebugRegisters() {
        return new CDebugRegisters();
    }

    int DebugRegisters_Internalize(CDebugRegisters* Self, DEBUG_CONTEXT* In) {
        if (!In)
            return 0;
    
        Self->Internalize((DEBUG_CONTEXT&)In);
        return 1;
    }

    int DebugRegisters_Serialize(CDebugRegisters* Self, DEBUG_CONTEXT* Out) {
        if (!Out)
            return 0;

        Self->Serialize((DEBUG_CONTEXT&)Out);
        return 1;
    }

    DebugRegister* DebugRegisters_Get(CDebugRegisters* Self) {
        return &Self->Registers[0];
    }

    bool DebugRegisters_IsTrapException(CDebugRegisters* Self) {
        return Self->IsTrapException;
    }

    void DebugRegisters_SetTrapException(CDebugRegisters* Self, bool Set) {
        Self->IsTrapException = Set;
    }
}