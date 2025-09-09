#include "jampgame_proxy/Proxy_Header.hpp"
#include "jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_Wrappers.hpp"
#include <sdk/qcommon/qcommon.h>
#include "jampgame_proxy/Imports/server/sv_game.hpp"
#include "jampgame_proxy/RuntimePatch/Engine/Patches/qcommon/Proxy_files.hpp"
#include "jampgame_proxy/Proxy_Utils.hpp"
#include "Proxy_sv_ccmds.hpp"

/*
================
SV_Status_f
================
*/

void (*Original_SV_Status_f)(void);
void Proxy_SV_Status_f(void)
{
	int				i;
	int				humans;
	int				bots;
	client_t*		cl;
	playerState_t*	ps;
	const char*		s;
	int				ping;
	char			state[32];

	// make sure server is running
	if (!server.common.cvars.com_sv_running->integer)
	{
		server.common.functions.Com_Printf("Server is not running.\n");

		return;
	}

	humans = bots = 0;

	for (i = 0; i < server.cvars.sv_maxclients->integer; i++)
	{
		if (server.svs->clients[i].state >= CS_CONNECTED)
		{
			if (server.svs->clients[i].netchan.remoteAddress.type != NA_BOT)
			{
				humans++;
			}
			else
			{
				bots++;
			}
		}
	}


	#define STATUS_OS "Linux"

	const char* ded_table[] = { "Listen", "LAN dedicated", "Public dedicated" };
	const char* gametypeNames[] = { "FFA", "Holocron", "Jedimaster", "Duel", "PowerDuel", "Single", "TFFA", "Siege", "CTF", "CTY" };
	char hostname[MAX_HOSTNAMELENGTH] = { 0 };

	jampgame.functions.Q_strncpyz(hostname, server.cvars.sv_hostname->string, sizeof(hostname));
	Q_StripColor(hostname);

	server.common.functions.Com_Printf("hostname: %s^7\n", hostname);
	server.common.functions.Com_Printf("server  : %s:%i, %s, %s\n", server.common.functions.Cvar_VariableString("net_ip"), trap_Cvar_VariableIntegerValue("net_port"), STATUS_OS, ded_table[server.common.cvars.com_dedicated->integer]);
	server.common.functions.Com_Printf("game    : 1.0.1.0 %i, %s\n", PROTOCOL_VERSION, Proxy_FS_GetCurrentGameDir());
	server.common.functions.Com_Printf("map     : ^7%s^7, %s(%i)\n", server.cvars.sv_mapname->string, gametypeNames[server.cvars.sv_gametype->integer], server.cvars.sv_gametype->integer);//Do we need to validate sv_gametype is 0-9? don't think so
	server.common.functions.Com_Printf("players : %i %s, %i %s(%i max)\n", humans, (humans == 1 ? "human" : "humans"), bots, (bots == 1 ? "bot" : "bots"), server.cvars.sv_maxclients->integer - server.cvars.sv_privateClients->integer);

	server.common.functions.Com_Printf("score ping rate   address                id name \n");
	server.common.functions.Com_Printf("----- ---- ------ ---------------------- -- ---------------\n");

	for (i = 0, cl = server.svs->clients; i < server.cvars.sv_maxclients->integer; i++, cl++)
	{
		if (!cl->state)
			continue;

		if (cl->state == CS_CONNECTED)
			jampgame.functions.Q_strncpyz(state, "CON ", sizeof(state));
		else if (cl->state == CS_ZOMBIE)
			jampgame.functions.Q_strncpyz(state, "ZMB ", sizeof(state));
		else
		{
			ping = cl->ping < 9999 ? cl->ping : 9999;
			jampgame.functions.Com_sprintf(state, sizeof(state), "%4i", ping);
		}

		ps = Proxy_GetPlayerStateByClientNum(i);
		s = server.common.functions.NET_AdrToString(cl->netchan.remoteAddress);

		//No need for truncation "feature" if we move name to end
		server.common.functions.Com_Printf("%5i %4s %6i %22s %2i %s^7\n", ps->persistant[PERS_SCORE], state, cl->rate, s, i, cl->name);
	}

	server.common.functions.Com_Printf("\n");
}

/*
===============================================================================

OPERATOR CONSOLE ONLY COMMANDS

These commands can only be entered from stdin or by a remote operator datagram
===============================================================================
*/

const char* Proxy_SV_GetStringEdString(const char* refSection, const char* refName)
{
	//Well, it would've been lovely doing it the above way, but it would mean mixing
	//languages for the client depending on what the server is. So we'll mark this as
	//a stringed reference with @@@ and send the refname to the client, and when it goes
	//to print it will get scanned for the stringed reference indication and dealt with
	//properly.
	static char text[1024] = { 0 };
	jampgame.functions.Com_sprintf(text, sizeof(text), "@@@%s", refName);
	return text;
}