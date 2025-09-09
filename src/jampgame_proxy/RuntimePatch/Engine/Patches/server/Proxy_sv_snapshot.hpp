#pragma once

#include <sdk/qcommon/qcommon.h>
#include <sdk/server/server.h>

extern void (*Original_SV_SendMessageToClient)(msg_t*, client_t*);
void Proxy_SV_SendMessageToClient(msg_t* msg, client_t* client);