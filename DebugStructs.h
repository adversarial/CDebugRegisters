#pragma once

#ifndef __cplusplus
#include <stdint.h>
#else
#include <cstdint>
namespace dbg {
#endif

typedef uint32_t dbgreg_t;

typedef enum { execution, data_write, io_readwrite, data_readwrite } bpcondition_t;
typedef enum { Dr0, Dr1, Dr2, Dr3 } bpindex_t;

// Can be directly copied into beginning of CONTEXT struct as members are same
typedef struct {
    dbgreg_t Reserved, // ContextFlags in CONTEXT struct
             Dr0,
             Dr1,
             Dr2,
             Dr3,
             Dr6,
             Dr7;
} DEBUG_CONTEXT;

typedef struct {
    void*           Address;
    bpcondition_t   Condition;
    bool            LocalSet;
    bool            GlobalSet;   // Not supported by Windows
    bool            Triggered;
    size_t          Size;
} DebugRegister;

#ifdef __cplusplus
}
#endif

// General notes:
//  ch 16.2: http://www.intel.com/Assets/en_US/PDF/manual/253668.pdf
//
// dr7 (condition) byte order:
//
// c (condition): see { bpcondition_t } in "DebugStructs.h"
// cb (len): length = 1 << cb
// d (detect): if set, instructions that access drx are privileged (generates debug exception on access)
// lX (drX is local)
// gX (drX is global)
// e..d  c..b  a..9  8..7  6..5  4..3  2..1  0..e  d  c  b  a  9  8  7  6  5  4  3  2  1  0 
//  cb3   c3    cb2   c2    cb1   c1    cb0   c0            d              g3 l3 g2 l2 g1 l1 g0 l0

// dr6 (exception info) bit order
//
// d (detected): if d flag in dr7 is set, accessing debug registers generates a debug exception with this
//               flag set
// s (single step): single step triggered exception (trap flag)
// t (task switch single step): task switch into task where trap flag was set
// sX (status of drX): if breakpoint was hit
// e  d  c  g  b  a  9  8  7  6  5  4  3  2  1  0 
// t  s  d                             s3 s2 s1 s0