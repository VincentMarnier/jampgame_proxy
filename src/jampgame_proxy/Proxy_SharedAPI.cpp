#include "Proxy_Header.hpp"
#include "sdk/server/server.hpp"
#include "Proxy_ClientCommand.hpp"
#include "sdk/server/sv_client.hpp"
#include "Proxy_SharedAPI.hpp"
#include "Imports/game/g_cmds.hpp"
#include "jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_ClientCommand.hpp"

// ==================================================
// IMPORT TABLE
// ==================================================

void Proxy_SharedAPI_LocateGameData(sharedEntity_t* gEnts, int numGEntities, int sizeofGEntity_t, playerState_t* clients, int sizeofGameClient)
{
	proxy.locatedGameData.g_entities = gEnts;
	proxy.locatedGameData.num_entities = numGEntities;
	proxy.locatedGameData.g_entitySize = sizeofGEntity_t;

	proxy.locatedGameData.g_clients = clients;
	proxy.locatedGameData.g_clientSize = sizeofGameClient;
}

void Proxy_SharedAPI_GetUsercmd(int clientNum, usercmd_t* ucmd)
{
	if (ucmd->forcesel == FP_LEVITATION || ucmd->forcesel >= NUM_FORCE_POWERS)
	{
		ucmd->forcesel = 0xFFu;
	}

	ucmd->angles[ROLL] = 0;
}

// ==================================================
// EXPORT TABLE
// ==================================================

void Proxy_SharedAPI_ClientConnect(int clientNum, qboolean firstTime, qboolean isBot)
{
	// Doesn't work on the new API
	if (firstTime && !isBot)
	{
		proxy.trap->SendServerCommand(clientNum, va("print \"^5%s (^7%s^5)^7\n\"", JAMPGAMEPROXY_NAME, JAMPGAMEPROXY_VERSION));
	}
}

void Proxy_SharedAPI_ClientDisconnect(int clientNum)
{
	if (clientNum >= 0 && clientNum < MAX_CLIENTS && proxy.clientData[clientNum].isConnected) {
		// Reset client's data on disconnect
		std::memset(&proxy.clientData[clientNum], 0, sizeof(*proxy.clientData));
	}
}

void Proxy_SharedAPI_ClientBegin(int clientNum, qboolean allowTeamReset)
{
	if (clientNum >= 0 && clientNum < MAX_CLIENTS)
	{
		proxy.clientData[clientNum].isConnected = true;
	}
}

qboolean Proxy_SharedAPI_ClientCommand(int clientNum)
{
	ClientData_t *currentClientData = &proxy.clientData[clientNum];

	if (!currentClientData->isConnected)
	{
		return qfalse;
	}

	char cmd[MAX_TOKEN_CHARS] = { 0 };
	qboolean sayCmd = qfalse;

	proxy.trap->Argv(0, cmd, sizeof(cmd));

	if (!Q_stricmpn(cmd, "jkaDST_", 7))
	{
		proxy.trap->SendServerCommand(-1, va("chat \"^3(Anti-Cheat system) ^7%s^3 got kicked cause of cheating^7\"", currentClientData->cleanName));
		proxy.trap->DropClient(clientNum, "(Anti-Cheat system) you got kicked cause of cheating");

		return qfalse;
	}

	// Todo (not sure): Check in the entier command + args?
	const char* argsConcat = ConcatArgs(1);

	if (!Q_stricmpn(cmd, "say", 3) || !Q_stricmpn(cmd, "say_team", 8) || !Q_stricmpn(cmd, "tell", 4))
	{
		sayCmd = qtrue;

		// 256 because we don't need more, the chat can handle 150 max char
		// and allowing 256 prevent a message to not be sent instead of being truncated
		// if this is a bit more than 150
		if (std::strlen(argsConcat) > 256)
		{
			return qfalse;
		}
	}

	char cmd_arg1[MAX_TOKEN_CHARS] = { 0 };
	char cmd_arg2[MAX_TOKEN_CHARS] = { 0 };

	proxy.trap->Argv(1, cmd_arg1, sizeof(cmd_arg1));
	proxy.trap->Argv(2, cmd_arg2, sizeof(cmd_arg2));

	// Fix: crach gc
	if (!Q_stricmpn(cmd, "gc", 2) && atoi(cmd_arg1) >= proxy.trap->Cvar_VariableIntegerValue("sv_maxclients"))
	{
		return qfalse;
	}

	// Fix: crash npc spawn
	if (!Q_stricmpn(cmd, "npc", 3) && !Q_stricmpn(cmd_arg1, "spawn", 5) && (!Q_stricmpn(cmd_arg2, "ragnos", 6) || !Q_stricmpn(cmd_arg2, "saber_droid", 6)))
	{
		return qfalse;
	}

	// Fix: team crash
	if (!Q_stricmpn(cmd, "team", 4) && (!Q_stricmpn(cmd_arg1, "follow1", 7) || !Q_stricmpn(cmd_arg1, "follow2", 7)))
	{
		return qfalse;
	}

	// Disable: callteamvote, useless in basejka and can lead to a bugged UI on custom client
	if (!Q_stricmpn(cmd, "callteamvote", 12))
	{
		return qfalse;
	}

	if (proxy.cvars.proxy_sv_disableKillCmd.integer && !Q_stricmpn(cmd, "kill", 4))
	{
		return qfalse;
	}

	if (!Q_stricmpn(cmd, "callvote", 8))
	{
		const int cmdArg2NumberValue = atoi(cmd_arg2);

		int minArg2Value = 0;
		int maxArg2Value = 1;
		qboolean checkNeeded = qfalse;

		// Fix: callvote long string length crash
		if (std::strlen(cmd_arg2) >= MAX_CVAR_VALUE_STRING)
		{
			return qfalse;
		}

		// Fix: callvote capturelimit with wrong values
		if (!Q_stricmpn(cmd_arg1, "capturelimit", 12))
		{
			minArg2Value = 0;
			maxArg2Value = 0x7FFFFFFF;
			checkNeeded = qtrue;
		}

		// Fix: callvote fraglimit with wrong values
		if (!Q_stricmpn(cmd_arg1, "fraglimit", 9))
		{
			minArg2Value = 0;
			maxArg2Value = 0x7FFFFFFF;
			checkNeeded = qtrue;
		}

		// Fix: callvote g_doWarmup with wrong values
		if (!Q_stricmpn(cmd_arg1, "g_doWarmup", 10))
		{
			minArg2Value = 0;
			maxArg2Value = 1;
			checkNeeded = qtrue;
		}

		// Fix: callvote map_restart with wrong values
		if (!Q_stricmpn(cmd_arg1, "map_restart", 11))
		{
			minArg2Value = 0;
			maxArg2Value = proxy.cvars.proxy_sv_maxCallVoteMapRestartValue.integer;
			checkNeeded = qtrue;
		}

		// Fix: callvote timelimit with wrong values
		if (!Q_stricmpn(cmd_arg1, "timelimit", 9))
		{
			minArg2Value = 0;
			maxArg2Value = 35790;
			checkNeeded = qtrue;
		}

		if (checkNeeded && (cmdArg2NumberValue < minArg2Value || cmdArg2NumberValue > maxArg2Value))
		{
			return qfalse;
		}
	}

	if (Q_strchrs(argsConcat, "\r\n"))
	{
		return qfalse;
	}

	if (!sayCmd && Q_strchrs(argsConcat, ";"))
	{
		return qfalse;
	}

	if (!Q_stricmpn(cmd, "myratio", 7))
	{
		Proxy_ClientCommand_MyRatio(clientNum);

		return qfalse;
	}

	// Only work on default engine since it require memory hook
	if (proxy.isOriginalEngine)
	{
		if (proxy.originalEngineCvars.proxy_sv_enableNetStatus.integer && (!Q_stricmpn(cmd, "netStatus", 9) || !Q_stricmpn(cmd, "showNet", 7)))
		{
			Proxy_Engine_ClientCommand_NetStatus(clientNum);

			return qfalse;
		}
	}

	return qtrue;
}

/*
// If we want to do something with the usercmd then call manually GetUsercmd
void Proxy_SharedAPI_ClientThink(int clientNum, usercmd_t* ucmd)
{}
*/

void Proxy_SharedAPI_ClientUserinfoChanged(int clientNum)
{
	char userinfo[MAX_INFO_STRING];
	
	proxy.trap->GetUserinfo(clientNum, userinfo, sizeof(userinfo));

	if (std::strlen(userinfo) == 0)
	{
		return;
	}

	std::size_t len = 0;
	const char* val = NULL;

	val = Info_ValueForKey(userinfo, "name");

	ClientData_t *currentClientData = &proxy.clientData[clientNum];

	// Fix bad names
	Proxy_ClientCleanName(val, currentClientData->cleanName, sizeof(currentClientData->cleanName));
	Info_SetValueForKey(userinfo, "name", currentClientData->cleanName);

	val = Info_ValueForKey(userinfo, "model");

	// Fix bugged models
	// Fix model length crash on some custom clients
	// There's also a check done in SV_UserinfoChanged
	if (val)
	{
		len = std::strlen(val);

		if (!Q_stricmpn(val, "darksidetools", len))
		{
			proxy.trap->SendServerCommand(-1, va("chat \"^3(Anti-Cheat system) ^7%s^3 got kicked cause of cheating^7\"", currentClientData->cleanName));
			proxy.trap->DropClient(clientNum, "(Anti-Cheat system) you got kicked cause of cheating");
		}

		if ((!Q_stricmpn(val, "jedi_", 5) && (!Q_stricmpn(val, "jedi_/red", len) ||
			!Q_stricmpn(val, "jedi_/blue", len))) ||
			!Q_stricmpn(val, "rancor", len) ||
			!Q_stricmpn(val, "wampa", len) ||
			!Q_IsValidAsciiStr(val) ||
			len >= /*MAX_QPATH*/ (std::size_t)proxy.cvars.proxy_sv_modelPathLength.integer)
		{
			Info_SetValueForKey(userinfo, "model", "kyle");
		}
	}

	//Fix forcepowers crash
	char forcePowers[30];
	
	Q_strncpyz(forcePowers, Info_ValueForKey(userinfo, "forcepowers"), sizeof(forcePowers));

	len = std::strlen(forcePowers);

	qboolean badForce = qfalse;

	if (len >= 22 && len <= 24)
	{
		byte seps = 0;

		for (std::size_t i = 0; i < len; i++)
		{
			if (forcePowers[i] != '-' && (forcePowers[i] < '0' || forcePowers[i] > '9'))
			{
				badForce = qtrue;
				break;
			}

			if ((i < 1 || i > 5) && forcePowers[i] == '-')
			{
				badForce = qtrue;
				break;
			}

			if (i && forcePowers[i - 1] == '-' && forcePowers[i] == '-')
			{
				badForce = qtrue;
				break;
			}

			if (forcePowers[i] == '-')
			{
				seps++;
			}
		}

		if (seps != 2)
		{
			badForce = qtrue;
		}
	}
	else
	{
		badForce = qtrue;
	}

	if (badForce)
	{
		Q_strncpyz(forcePowers, "7-1-030000000000003332", sizeof(forcePowers));
	}

	Info_SetValueForKey(userinfo, "forcepowers", forcePowers);

	proxy.trap->SetUserinfo(clientNum, userinfo);

	return;
}