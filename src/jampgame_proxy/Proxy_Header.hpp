// ==================================================
// jampgame_proxy
// Fork of JKA_YBEProxy by Yberion
// Inspired by JMPProxy from DeathSpike (https://github.com/Deathspike/JMPProxy)
// ==================================================

#pragma once

#include <sdk/game/b_local.h>
#include <sdk/game/b_public.h>
#include <sdk/qcommon/qcommon.h>

#include <cstdint>
#include <cstddef>

#include "Proxy_Translate_SystemCalls.hpp"

// ==================================================
// DEFINE
// ==================================================

#define FS_GAME_CVAR "fs_game"

#define DEFAULT_BASE_GAME_FOLDER_NAME "base"

#define PROXY_LIBRARY_SLASH "/"
#define PROXY_LIBRARY_NAME "jampgame_original.so"

#define PROXY_LIBRARY PROXY_LIBRARY_SLASH PROXY_LIBRARY_NAME

#define ORIGINAL_ENGINE_VERSION "JAmp: v1.0.1.1 linux-i386 Nov 10 2003"

#define JAMPGAMEPROXY_NAME "jampgame_proxy"
#define JAMPGAMEPROXY_VERSION "0.0.4"

// ==================================================
// TYPEDEF
// ==================================================

using systemCallFuncPtr_t =	int (QDECL *)(int command, ...);
using vmMainFuncPtr_t = 	int (*)(int command, int, int, int, int, int, int, int, int, int, int, int);
using dllEntryFuncPtr_t = 	void (*)(systemCallFuncPtr_t);

// ==================================================
// STRUCTS
// ==================================================

struct timenudgeData_t
{
	int             delayCount;
	int             delaySum;
	int             pingSum;
	int             lastTimeTimeNudgeCalculation;
};

struct ucmdStat_t
{
	int				serverTime;
	size_t		packetIndex;
};

#define CMD_MASK 1024

struct ProxyData_t
{
	int					svsTime;
};

struct LocatedGameData_t
{
	sharedEntity_t*		g_entities;
	int					g_entitySize;
	int					num_entities;

	playerState_t*		g_clients;
	int					g_clientSize;
};

struct GameStats_t
{
	int killed;
	int killedBy;
	int damagesGiven;
	int damagesTaken;
};

struct ClientData_t
{
	bool				isConnected;
	char				cleanName[MAX_NETNAME];

	timenudgeData_t		timenudgeData;
	int					timenudge; // Approximation (+- 7 with stable connection)

	int					lastTimeNetStatus;
	int					lastTimeMyratioCheck;

	ucmdStat_t			cmdStats[CMD_MASK];
	size_t			cmdIndex;

	GameStats_t gameStats[MAX_CLIENTS];
	int teamKills;
	int teamKilled;
	int teamDamagesGiven;
	int teamDamagesTaken;
};

struct Proxy_OriginalEngine_CVars_t
{
	// New cvars
	vmCvar_t proxy_sv_pingFix;
	vmCvar_t proxy_sv_enableRconCmdCooldown;
	vmCvar_t proxy_sv_enableNetStatus;
};

struct Proxy_CVars_t
{
	// New cvars
	vmCvar_t proxy_sv_maxCallVoteMapRestartValue;
	vmCvar_t proxy_sv_modelPathLength;
	vmCvar_t proxy_sv_disableKillCmd;

	// get cvars
	vmCvar_t sv_fps;
	vmCvar_t sv_gametype;
};

struct Proxy_t
{
	void					*jampgameHandle;
	uint 		jampgameAddress;

	vmMainFuncPtr_t			originalVmMain;
	dllEntryFuncPtr_t		originalDllEntry;
	systemCallFuncPtr_t		originalSystemCall;

	int				originalVmMainResponse;

	ProxyData_t				proxyData;
	LocatedGameData_t	locatedGameData;
	ClientData_t			clientData[MAX_CLIENTS];
	
	Proxy_OriginalEngine_CVars_t originalEngineCvars;
	Proxy_CVars_t			cvars;

	level_locals_t 		*level;
	gentity_t					*g_entities;
};

// ==================================================
// EXTERN VARIABLE
// ==================================================

extern Proxy_t proxy;
