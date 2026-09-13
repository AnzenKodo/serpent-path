#ifndef OS_CORE_H
#define OS_CORE_H

// ak: External Includes
//=============================================================================

#include <fcntl.h>
#include <errno.h>

// ak: Types
//=============================================================================

// ak: Private OS State ======================================================

typedef struct _Os_Core_State _Os_Core_State;
struct _Os_Core_State
{
    Str8_Array args;
    Log_Context log_context;
};

// ak: Functions
//=============================================================================

// ak: Memory =================================================================

internal size_t os_pagesize_get(void);

// ak: File ===================================================================

internal size_t os_file_read(Fs_File file, Rng1_U64 rng, void *out_data);

// ak: Exit ===================================================================

internal void os_exit(int32_t exit_code);

// ak: Sleep ==================================================================

internal void os_sleep_us(uint64_t microsec);
internal void os_sleep_ms(uint32_t millisec);

// ak: Command-Line Operations ================================================

internal Str8 *os_program_path_get(void);

// ak: Environment Variable ===================================================

internal bool os_env_is_set(Str8 name);
internal Str8 os_env_get(Str8 name);

// ak: OS Entry Points ========================================================

internal void os_main(void);

// ak: Global Variables
//=============================================================================

global _Os_Core_State _os_core_state = STRUCT_ZERO;

#endif // OS_CORE_H
