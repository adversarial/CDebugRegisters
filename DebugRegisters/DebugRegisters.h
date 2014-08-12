#pragma once

#include "..\DebugStructs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CDebugRegisters CDebugRegisters;

CDebugRegisters* NewDebugRegisters();

int DebugRegisters_Internalize(CDebugRegisters* Self, DEBUG_CONTEXT* In);
int DebugRegisters_Serialize(CDebugRegisters* Self, DEBUG_CONTEXT* Out);

// Returns pointer to array of 4 debug registers (in order DRx)
DebugRegister* DebugRegisters_Get(CDebugRegisters* Self);

#ifdef __cplusplus
}
#endif