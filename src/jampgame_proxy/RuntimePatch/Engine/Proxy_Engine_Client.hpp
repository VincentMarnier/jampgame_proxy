#pragma once

#include <sdk/qcommon/qcommon.h>
#include <sdk/server/server.h>

#include <cstddef>

void Proxy_Engine_Client_UpdateUcmdStats(size_t clientNum, usercmd_t* cmd, size_t packetIndex);
void Proxy_Engine_Client_CalcPacketsAndFPS(int clientNum, int* packets, int* fps);
void Proxy_Engine_Client_UpdateTimenudge(client_t* client, usercmd_t* cmd, int _Milliseconds);