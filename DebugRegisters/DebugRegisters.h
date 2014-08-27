#pragma once

#include "..\DebugStructs.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
using namespace dbg;
#endif

typedef struct CDebugRegisters CDebugRegisters;

CDebugRegisters* NewDebugRegisters();

int DebugRegisters_Internalize(CDebugRegisters* Self, DEBUG_CONTEXT* In);
int DebugRegisters_Serialize(CDebugRegisters* Self, DEBUG_CONTEXT* Out);

// Returns pointer to array of 4 debug registers (in order Drx)
DebugRegister* DebugRegisters_Get(CDebugRegisters* Self);

// If trap flag was triggered in dr6
bool DebugRegisters_IsTrapException(CDebugRegisters* Self);
// Setter
void DebugRegisters_SetTrapException(CDebugRegisters, bool Set);

#ifdef __cplusplus
}
#endif