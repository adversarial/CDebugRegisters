#pragma once

#ifndef __cplusplus
#include <stdint.h>
#else
#include <cstdint>
namespace dbg {
#endif

typedef uint32_t dbgreg_t;

typedef enum { execution, data_write, io_readwrite, data_readwrite } bpcondition_t;
typedef enum { DR0, DR1, DR2, DR3 } bpindex_t;

// Can be directly copied into beginning of CONTEXT struct as members are same
typedef struct {
    dbgreg_t Reserved, // ContextFlags in CONTEXT struct
             DR0,
             DR1,
             DR2,
             DR3,
             DR6,
             DR7;
} DEBUG_CONTEXT;

typedef struct {
    void*           Address;
    bpcondition_t   Condition;
    bool            LocalSet;
    bool            GlobalSet;   // Not supported by Windows
} DebugRegister;

#ifdef __cplusplus
}
#endif