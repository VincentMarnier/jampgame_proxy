#pragma once

#include <sdk/qcommon/qcommon.h>

const char* Proxy_FS_GetCurrentGameDir(bool emptybase = false);

extern void (QDECL *Original_Com_Printf)(const char*, ...);
void QDECL Proxy_Com_Printf(const char* fmt, ...);