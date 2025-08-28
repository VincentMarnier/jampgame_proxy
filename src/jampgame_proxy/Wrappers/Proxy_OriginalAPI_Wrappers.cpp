#include "jampgame_proxy/Proxy_Header.hpp"
#include "sdk/game/g_public.hpp"
#include "Proxy_OriginalAPI_Wrappers.hpp"
#include "jampgame_proxy/Proxy_SharedAPI.hpp"
#include "jampgame_proxy/Proxy_Main.hpp"

#include <cstdarg>

intptr_t QDECL Proxy_OriginalAPI_VM_DllSyscall(intptr_t command, ...)
{
	intptr_t args[16];
	
	va_list ap;

	va_start(ap, command);

	for (std::size_t i = 0; i < sizeof(args) / sizeof(args[i]); ++i)
	{
		args[i] = va_arg(ap, intptr_t);
	}

	va_end(ap);

	switch (command)
	{
		//==================================================
		case G_LOCATE_GAME_DATA: // (sharedEntity_t* gEnts, int numGEntities, int sizeofGEntity_t, playerState_t* clients, int sizeofGameClient)
		//==================================================
		{
			Proxy_SharedAPI_LocateGameData((sharedEntity_t*)args[0], (int)args[1], (int)args[2], (playerState_t*)args[3], (int)args[4]);

			break;
		}
		//==================================================
		case G_GET_USERCMD: // (int clientNum, usercmd_t* cmd)
		//==================================================
		{
			// The server set the usercmd to the last he knows about
			int response = proxy.originalSystemCall(command, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12], args[13], args[14], args[15]);
			
			// We then alter the usercmd if needed before "sending" it back to the original game module
			Proxy_SharedAPI_GetUsercmd((int)args[0], (usercmd_t*)args[1]);
			
			return response;
		}
		default:
			break;
	}

	return proxy.originalSystemCall(command, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12], args[13], args[14], args[15]);
}

intptr_t QDECL Proxy_OriginalAPI_VM_Call(intptr_t command, ...)
{
	intptr_t args[11];

	va_list ap;

	va_start(ap, command);

	for (std::size_t i = 0; i < sizeof(args) / sizeof(args[i]); ++i)
	{
		args[i] = va_arg(ap, intptr_t);
	}

	va_end(ap);

	return vmMain(command, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10]);
}