// ==================================================
// jampgame_proxy
// Fork of JKA_YBEProxy by Yberion
// Inspired by JMPProxy from DeathSpike (https://github.com/Deathspike/JMPProxy)
// ==================================================

#include "Proxy_Main.hpp"
#include "Proxy_Header.hpp"
#include "Proxy_CVars.hpp"
#include "Proxy_Files.hpp"
#include "RuntimePatch/Engine/Proxy_Engine_Patch.hpp"
#include "RuntimePatch/Engine/Proxy_Engine_Wrappers.hpp"
#include "Proxy_SharedAPI.hpp"
#include "Proxy_Translate_SystemCalls.hpp"
#include "Wrappers/Proxy_OpenJKAPI_Wrappers.hpp"
#include "Wrappers/Proxy_OriginalAPI_Wrappers.hpp"

Proxy_t proxy = { 0 };

static void Proxy_GetOriginalGameAPI(void)
{
	// 1) Original Engine -> Load the Proxy dllEntry (store the original server's systemCall function pointer in originalSystemCall)
	// 2) The Proxy get the Original dllEntry
	// 3) The Proxy send our own Proxy systemCall function pointer to the Original dllEntry
	// 4) If there is a call to our Proxy's systemCall function it will call the Original systemCall function at the end of it
	proxy.originalDllEntry = (dllEntryFuncPtr_t)JampgameProxy_GetFunctionAddress(proxy.jampgameHandle, "dllEntry");

	if (!proxy.originalDllEntry)
	{
		proxy.trap->Print("================================================================\n");
		proxy.trap->Print("----- Proxy: Failed to find dllEntry(), exiting\n");
		proxy.trap->Print("================================================================\n");

		exit(EXIT_FAILURE);
	}

	// Engine -> Proxy vmMain -> Original vmMain
	proxy.originalVmMain = (vmMainFuncPtr_t)JampgameProxy_GetFunctionAddress(proxy.jampgameHandle, "vmMain");

	if (!proxy.originalVmMain)
	{
		proxy.trap->Print("================================================================\n");
		proxy.trap->Print("----- Proxy: Failed to find vmMain(), exiting\n");
		proxy.trap->Print("================================================================\n");

		exit(EXIT_FAILURE);
	}

	// "Send our own Proxy systemCall function pointer to the Original dllEntry"
	proxy.originalDllEntry(Proxy_OriginalAPI_VM_DllSyscall);
}

Q_CABI Q_EXPORT intptr_t vmMain(intptr_t command, intptr_t arg0, intptr_t arg1, intptr_t arg2, intptr_t arg3, intptr_t arg4,
	intptr_t arg5, intptr_t arg6, intptr_t arg7, intptr_t arg8, intptr_t arg9, intptr_t arg10)
{
	switch (command)
	{
		//==================================================
		case GAME_INIT: // (int levelTime, int randomSeed, int restart)
		//==================================================
		{
			proxy.trap->Print("================================================================\n");
			proxy.trap->Print("----- Proxy: %s %s\n", JAMPGAMEPROXY_NAME, JAMPGAMEPROXY_VERSION);
			proxy.trap->Print("================================================================\n");

			proxy.trap->Print("----- Proxy: Loading original game library %s\n", PROXY_LIBRARY_NAME PROXY_LIBRARY_DOT PROXY_LIBRARY_EXT);

			Proxy_LoadGameLibrary();

			Proxy_GetOriginalGameAPI();

			proxy.trap->Print("----- Proxy: %s properly loaded\n", PROXY_LIBRARY_NAME PROXY_LIBRARY_DOT PROXY_LIBRARY_EXT);
			
			proxy.trap->Print("----- Proxy: Initializing Proxy CVars\n");

			Proxy_CVars_Registration();

			proxy.trap->Print("----- Proxy: Proxy CVars properly initialized\n");

			char version[MAX_STRING_CHARS];

			proxy.trap->Cvar_VariableStringBuffer("version", version, sizeof(version));

			if (!Q_stricmpn(version, ORIGINAL_ENGINE_VERSION, sizeof(ORIGINAL_ENGINE_VERSION) - 1))
			{
				proxy.isOriginalEngine = true;
			}

			if (proxy.isOriginalEngine)
			{
				proxy.trap->Print("----- Proxy: Original engine detected\n");

				Proxy_Engine_Initialize_MemoryLayer();
				Proxy_OriginalEngine_CVars_Registration();
				
				proxy.trap->Print("----- Proxy: Patching engine\n");

				Proxy_Engine_Inline_Patches();
				Proxy_Engine_Attach_Patches();

				proxy.trap->Print("----- Proxy: Engine properly patched\n");
			}

			break;
		}
		//==================================================
		case GAME_SHUTDOWN: // (int restart)
		//==================================================
		{
			if (proxy.isOriginalEngine)
			{
				// On "rcon map XXX" or "rcon map_restart 0" it directly goes there from SVC_RemoteCommand
				// the problem here is that Com_EndRedirect() isn't called after the
				// Cmd_ExecuteString() of the map change
				// We need to manually call the Com_EndRedirect()
				server.common.functions.Com_EndRedirect();

				proxy.trap->Print("----- Proxy: Unpatching engine\n");

				Proxy_Engine_Detach_Patches();

				proxy.trap->Print("----- Proxy: Engine properly unpatched\n");
			}

			if (proxy.jampgameHandle)
			{
				// Send the shutdown signal to the original game module and store the response
				proxy.originalVmMainResponse = proxy.originalVmMain(command, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);

				proxy.trap->Print("----- Proxy: Unloading original game library %s\n", PROXY_LIBRARY_NAME PROXY_LIBRARY_DOT PROXY_LIBRARY_EXT);

				// We can close the original game library
				JampgameProxy_CloseLibrary(proxy.jampgameHandle);

				proxy.trap->Print("----- Proxy: %s properly unloaded\n", PROXY_LIBRARY_NAME PROXY_LIBRARY_DOT PROXY_LIBRARY_EXT);

				// Return the response of the original game module after the shutdown
				return proxy.originalVmMainResponse;
			}

			break;
		}
		// ==================================================
		case GAME_RUN_FRAME:
		// ==================================================
		// The server is going to active its new frame, which
		// often is around 20 times per second. Active some
		// of our own active functions.
		// ==================================================
		{
			Proxy_UpdateAllCvars();
			proxy.proxyData.svsTime = (int)arg0;

			break;
		}
		//==================================================
		case GAME_CLIENT_CONNECT: // (int clientNum, qboolean firstTime, qboolean isBot)
		//==================================================
		{
			Proxy_SharedAPI_ClientConnect((int)arg0, (qboolean)arg1, (qboolean)arg2);

			break;
		}
		//==================================================
		case GAME_CLIENT_DISCONNECT: // (int clientNum, qboolean firstTime, qboolean isBot)
		//==================================================
		{
			Proxy_SharedAPI_ClientDisconnect((int)arg0);

			break;
		}
		//==================================================
		case GAME_CLIENT_BEGIN: // (int clientNum, qboolean allowTeamReset)
		//==================================================
		{
			Proxy_SharedAPI_ClientBegin((int)arg0, (qboolean)arg1);

			break;
		}
		//==================================================
		case GAME_CLIENT_COMMAND: // (int clientNum)
		//==================================================
		{
			if (!Proxy_SharedAPI_ClientCommand((int)arg0))
			{
				return 0;
			}

			break;
		}
		/*
		//==================================================
		case GAME_CLIENT_THINK: // (int clientNum)
		//==================================================
		{
			int response = proxy.originalVmMain(command, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);

			//Proxy_SharedAPI_ClientThink((int)arg0, (usercmd_t*)arg1);

			return response;
		}
		*/
		//==================================================
		case GAME_CLIENT_USERINFO_CHANGED: // (int clientNum)
		//==================================================
		{
			Proxy_SharedAPI_ClientUserinfoChanged((int)arg0);

			break;
		}
		default:
			break;
	}

	return proxy.originalVmMain(command, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
}

// The engine sends the system call function pointer to the game module through dllEntry
Q_CABI Q_EXPORT void dllEntry(systemCallFuncPtr_t systemCallFuncPtdr) {
	proxy.originalSystemCall = systemCallFuncPtdr;

	// Create trap calls available directly within the proxy (proxy.trap->*)
	Proxy_Translate_SystemCalls();
}

Q_CABI Q_EXPORT gameExport_t* QDECL GetModuleAPI(int apiVersion, gameImport_t* import)
{
	assert(import);

	// Needed for trap_... calls inside of the proxy
	proxy.trap = import;

	Com_Printf = proxy.trap->Print;
	Com_Error = proxy.trap->Error;

	if (apiVersion != GAME_API_VERSION)
	{
		proxy.trap->Print("=========================================================================\n");
		proxy.trap->Print("----- Proxy: Mismatched GAME_API_VERSION: expected %i, got %i, exiting\n", GAME_API_VERSION, apiVersion);
		proxy.trap->Print("=========================================================================\n");

		return nullptr;
	}

	Proxy_LoadGameLibrary();

	GetGameAPI_t jampGameGetModuleAPI = (GetGameAPI_t)JampgameProxy_GetFunctionAddress(proxy.jampgameHandle, "GetModuleAPI");

	if (!jampGameGetModuleAPI)
	{
		proxy.trap->Print("==================================================================================\n");
		proxy.trap->Print("----- Proxy: Failed to find GetModuleAPI function, loading vmMain and dllEntry\n");
		proxy.trap->Print("==================================================================================\n");

		Proxy_GetOriginalGameAPI();

		return nullptr;
	}

	static gameImport_t _copyOpenJKAPIGameImportTable = { 0 };
	static gameExport_t _copyOpenJKAPIGameExportTable = { 0 };

	proxy.originalOpenJKAPIGameImportTable = import;
	std::memcpy(&_copyOpenJKAPIGameImportTable, import, sizeof(gameImport_t));
	proxy.copyOpenJKAPIGameImportTable = &_copyOpenJKAPIGameImportTable;

	proxy.originalOpenJKAPIGameExportTable = jampGameGetModuleAPI(apiVersion, &_copyOpenJKAPIGameImportTable);
	std::memcpy(&_copyOpenJKAPIGameExportTable, proxy.originalOpenJKAPIGameExportTable, sizeof(gameExport_t));
	proxy.copyOpenJKAPIGameExportTable = &_copyOpenJKAPIGameExportTable;

	Proxy_OpenJKAPI_InitLayerExportTable();
	Proxy_OpenJKAPI_InitLayerImportTable();

	return proxy.copyOpenJKAPIGameExportTable;
}
