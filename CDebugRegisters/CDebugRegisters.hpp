#pragma once

#include "..\DebugStructs.h"
#include <array>
#include <cstdint>

#ifndef _M_IX86
#   error "Only compatible with x86 machines."
#endif

namespace dbg {

class CDebugRegisters {

public:
    CDebugRegisters();
    CDebugRegisters(DEBUG_CONTEXT&);

    void Internalize(DEBUG_CONTEXT& In);
    void Serialize(DEBUG_CONTEXT& Out) const;

    // public to allow editing (values are all converted on Serialize)
    std::array<DebugRegister, 4> Registers;
    bool IsTrapException,
         IsTaskSwitchTrapException;
};

}