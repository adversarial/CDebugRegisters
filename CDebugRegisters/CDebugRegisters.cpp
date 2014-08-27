#include "CDebugRegisters.hpp"

using namespace dbg;

#include <sstream>

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
    
    for (dbgreg_t *Reg = &In.Dr0, i = 0; i < 4; ++Reg, ++i) {
        // Seems like something that should be done in a constructor... nah
        Registers[i].Address = (void*)(*Reg);
        Registers[i].LocalSet = In.Dr7 & (1 << (i * 2));
        Registers[i].GlobalSet = In.Dr7 & (1 << (i * 2 + 1));
        Registers[i].Condition = static_cast<bpcondition_t>((In.Dr7 >> (16 + i * 4)) & 3);
        switch (In.Dr7 >> (16 + 2 + i * 4) & 3) {
        case 0:
            Registers[i].Size = 1;
            break;
        case 1:
            Registers[i].Size = 2;
            break;
        case 2:
            // Only supported on some processors, else undefined
            // TODO: implement cpuid based check
            Registers[i].Size = 8;
            break;
        case 3:
            Registers[i].Size = 4;
            break;
        }
        Registers[i].Triggered = In.Dr6 & (1 << i);
    }
    IsTrapException = In.Dr6 & (1 << 14);
}

void CDebugRegisters::Serialize(DEBUG_CONTEXT& Out) const {

    std::memset(static_cast<void*>(&Out), 0, sizeof(Out));

    for (dbgreg_t *Reg = &Out.Dr0, i = 0; i < 4; ++Reg, ++i) {
        *Reg = (dbgreg_t)Registers[i].Address;

        if (Registers[i].LocalSet)
            Out.Dr7 |= 1 << (i * 2);

        if (Registers[i].GlobalSet)
            Out.Dr7 |= 1 << (i * 2 + 1);

        Out.Dr7 |= (static_cast<dbgreg_t>(Registers[i].Condition) & 3) << (16 + i * 4);
        // Intel guides: "If the corresponding RW field in register DR7 is 00 (instruction execution), then the LEN field should also be 00."
        //               "The effect of using other lengths is undefined"
        if (Registers[i].Condition != execution) {
            switch (Registers[i].Size) {
            case 1:
                // encoding 0
                break;
            case 2:
                // encoding 1
                Out.Dr7 |= 1 << (16 + 2 + i * 4);
                break;
            case 4:
                // encoding 2
                // Only supported on some processors, else undefined
                // TODO: implement cpuid based check
                Out.Dr7 |= 2 << (16 + 2 + i * 4);
                break;
            case 8:
                // encoding 3
                Out.Dr7 |= 3 << (16 + 2 + i * 4);
                break;
            default:
                std::stringstream ErrText;
                ErrText << "Invalid breakpoint size " << Registers[i].Size << " used to trigger Dr" << i;
                throw std::runtime_error(ErrText.str().c_str());
            }
        }

        Out.Dr6 |= Registers[i].Triggered << i;
        Out.Dr6 |= IsTrapException << 14;

        Out.Reserved |= DEBUGREG_CONTEXT_FLAGS;
    }
}