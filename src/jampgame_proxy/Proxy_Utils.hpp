#pragma once

#include <sdk/qcommon/qcommon.h>

#define Q_IsColorStringExt(p)	((p) && *(p) == Q_COLOR_ESCAPE && *((p)+1) && *((p)+1) >= '0' && *((p)+1) <= '9') // ^[0-9]
#define MAX_HOSTNAMELENGTH		256

qboolean Q_IsValidAsciiStr(const char* string);
const char* Q_strchrs(const char* string, const char* search);
void Q_StripColor(char* text);
float calcRatio(int kill, int death);