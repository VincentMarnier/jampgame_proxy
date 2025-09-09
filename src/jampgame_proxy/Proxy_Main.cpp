// ==================================================
// jampgame_proxy
// Fork of JKA_YBEProxy by Yberion
// Inspired by JMPProxy from DeathSpike (https://github.com/Deathspike/JMPProxy)
// ==================================================

#include <dlfcn.h>
#include "Proxy_Main.hpp"
#include "Proxy_Header.hpp"
#include "Proxy_CVars.hpp"
#include "Proxy_Files.hpp"
#include "RuntimePatch/Engine/Proxy_Engine_Patch.hpp"
#include "RuntimePatch/Engine/Proxy_Engine_Wrappers.hpp"
#include "Proxy_SharedAPI.hpp"
#include "Wrappers/Proxy_OriginalAPI_Wrappers.hpp"

Proxy_t proxy = { 0 };

static void Proxy_GetOriginalGameAPI(void)
{
	// 1) Original Engine -> Load the Proxy dllEntry (store the original server's systemCall function pointer in originalSystemCall)
	// 2) The Proxy get the Original dllEntry
	// 3) The Proxy send our own Proxy systemCall function pointer to the Original dllEntry
	// 4) If there is a call to our Proxy's systemCall function it will call the Original systemCall function at the end of it
	proxy.originalDllEntry = (dllEntryFuncPtr_t)dlsym(proxy.jampgameHandle, "dllEntry");

	if (!proxy.originalDllEntry)
	{
		fprintf(stderr, "================================================================\n");
		fprintf(stderr, "----- Proxy: Failed to find dllEntry(), exiting\n");
		fprintf(stderr, "================================================================\n");

		exit(EXIT_FAILURE);
	}

	Dl_info info;
	if (!dladdr((const void *)proxy.originalDllEntry, &info)) {
		fprintf(stderr, "================================================================\n");
		fprintf(stderr, "----- Proxy: Failed to find jampgame's address, exiting\n");
		fprintf(stderr, "================================================================\n");

		exit(EXIT_FAILURE);
	}
	proxy.jampgameAddress = (uintptr_t)info.dli_fbase;

	// Engine -> Proxy vmMain -> Original vmMain
	proxy.originalVmMain = (vmMainFuncPtr_t)dlsym(proxy.jampgameHandle, "vmMain");

	if (!proxy.originalVmMain)
	{
		fprintf(stderr, "================================================================\n");
		fprintf(stderr, "----- Proxy: Failed to find vmMain(), exiting\n");
		fprintf(stderr, "================================================================\n");

		exit(EXIT_FAILURE);
	}

	// Engine -> Proxy level -> Original level
	proxy.level = (level_locals_t *)dlsym(proxy.jampgameHandle, "level");
	if (!proxy.level)
	{
		fprintf(stderr, "================================================================\n");
		fprintf(stderr, "----- Proxy: Failed to find level, exiting\n");
		fprintf(stderr, "================================================================\n");

		exit(EXIT_FAILURE);
	}

	proxy.g_entities = (gentity_t *)dlsym(proxy.jampgameHandle, "g_entities");
	if (!proxy.g_entities)
	{
		fprintf(stderr, "================================================================\n");
		fprintf(stderr, "----- Proxy: Failed to find g_entities, exiting\n");
		fprintf(stderr, "================================================================\n");

		exit(EXIT_FAILURE);
	}
	
	// "Send our own Proxy systemCall function pointer to the Original dllEntry"
	proxy.originalDllEntry(Proxy_OriginalAPI_VM_DllSyscall);
}

extern "C" __attribute__((visibility("default"))) int vmMain(int command, int arg0, int arg1, int arg2, int arg3, int arg4,
	int arg5, int arg6, int arg7, int arg8, int arg9, int arg10)
{
	switch (command)
	{
		//==================================================
		case GAME_INIT: // (int levelTime, int randomSeed, int restart)
		//==================================================
		{
			printf("================================================================\n");
			printf("----- Proxy: " JAMPGAMEPROXY_NAME " " JAMPGAMEPROXY_VERSION "\n");
			printf("================================================================\n");

			char version[MAX_STRING_CHARS];
			trap_Cvar_VariableStringBuffer("version", version, sizeof(version));
			if (strncmp(version, ORIGINAL_ENGINE_VERSION, sizeof(ORIGINAL_ENGINE_VERSION) - 1))
			{
				// This is not the original engine
				fprintf(stderr, "================================================================\n");
				fprintf(stderr, "----- Trying to run " JAMPGAMEPROXY_NAME " on a modified engine, exiting\n");
				fprintf(stderr, "================================================================\n");

				exit(EXIT_FAILURE);
			}

			printf("----- Proxy: Loading original game library " PROXY_LIBRARY_NAME "\n");

			Proxy_LoadGameLibrary();

			Proxy_GetOriginalGameAPI();

			printf("----- Proxy: " PROXY_LIBRARY_NAME " properly loaded\n");
			
			printf("----- Proxy: Initializing Proxy CVars\n");

			Proxy_CVars_Registration();

			printf("----- Proxy: Proxy CVars properly initialized\n");

			printf("----- Proxy: Original engine detected\n");

			Proxy_Engine_Initialize_MemoryLayer();
			Proxy_OriginalEngine_CVars_Registration();
			
			printf("----- Proxy: Patching engine\n");

			Proxy_Engine_Inline_Patches();
			Proxy_Engine_Attach_Patches();

			printf("----- Proxy: Engine properly patched\n");

			break;
		}
		//==================================================
		case GAME_SHUTDOWN: // (int restart)
		//==================================================
		{
			// On "rcon map XXX" or "rcon map_restart 0" it directly goes there from SVC_RemoteCommand
			// the problem here is that Com_EndRedirect() isn't called after the
			// Cmd_ExecuteString() of the map change
			// We need to manually call the Com_EndRedirect()
			server.common.functions.Com_EndRedirect();

			printf("----- Proxy: Unpatching engine\n");

			Proxy_Engine_Detach_Patches();

			printf("----- Proxy: Engine properly unpatched\n");

			if (proxy.jampgameHandle)
			{
				// Send the shutdown signal to the original game module and store the response
				proxy.originalVmMainResponse = proxy.originalVmMain(command, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);

				printf("----- Proxy: Unloading original game library " PROXY_LIBRARY_NAME "\n");

				// We can close the original game library
				dlclose(proxy.jampgameHandle);

				printf("----- Proxy: " PROXY_LIBRARY_NAME " properly unloaded\n");

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
extern "C" __attribute__((visibility("default"))) void dllEntry(systemCallFuncPtr_t systemCallFuncPtdr) {
	proxy.originalSystemCall = systemCallFuncPtdr;
}
