#include "CDebugRegisters.hpp"

using namespace dbg;

#ifdef _WIN32
#include <Windows.h>
#   define DEBUGREG_CONTEXT_FLAGS CONTEXT_DEBUG_REGISTERS
#else
//  CONTEXT_I386 = 0x00010000
//  CONTEXT_DEBUG_REGISTERS = 0x00000010;
#   define DEBUGREG_CONTEXT_FLAGS 0x00010010L
#endif

// Modify context of currently running thread
CDebugRegisters::CDebugRegisters(DEBUG_CONTEXT& In) {
    Internalize(In);
}

void CDebugRegisters::Internalize(DEBUG_CONTEXT& In) {
    
    for (dbgreg_t *Reg = &In.DR0, i = 0; i < 4; ++Reg, ++i) {
        Registers[i].Address = (void*)(*Reg);

        if (In.DR7 & (1 << (i * 2)))
            Registers[i].LocalSet = true;

        if (In.DR7 & (1 << (i * 2 + 1))) {
            Registers[i].GlobalSet = true;
        }

        Registers[i].Condition = static_cast<bpcondition_t>((In.DR7 >> (16 + i * 4)) & 3);
    }
}

void CDebugRegisters::Serialize(DEBUG_CONTEXT& Out) const {

    for (dbgreg_t *Reg = &Out.DR0, i = 0; i < 4; ++Reg, ++i) {
        *Reg = (dbgreg_t)Registers[i].Address;

        if (Registers[i].LocalSet)
            Out.DR7 |= 1 << (i * 2);

        if (Registers[i].GlobalSet)
            Out.DR7 |= 1 << (i * 2 + 1);

        Out.DR7 |= (static_cast<uintptr_t>(Registers[i].Condition) & 3) << (16 + i * 4);

        Out.Reserved |= DEBUGREG_CONTEXT_FLAGS;
    }
}