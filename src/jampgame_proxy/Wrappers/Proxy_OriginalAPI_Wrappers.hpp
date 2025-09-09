#pragma once

#include <sdk/qcommon/qcommon.h>

// VM_DllSyscall can handle up to 1 (command) + 16 args
int QDECL Proxy_OriginalAPI_VM_DllSyscall(int command, ...);
// VM_Call can handle up to 1 (command) + 11 args
int QDECL Proxy_OriginalAPI_VM_Call(int command, ...);