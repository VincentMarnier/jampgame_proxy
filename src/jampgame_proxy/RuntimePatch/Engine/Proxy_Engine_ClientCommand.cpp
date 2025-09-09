#include "Proxy_Engine_ClientCommand.hpp"
#include "jampgame_proxy/Proxy_Header.hpp"
#include "jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_Wrappers.hpp"
#include "jampgame_proxy/Imports/server/sv_game.hpp"
#include "Proxy_Engine_Client.hpp"

/*
==================
Proxy_ClientCommand_NetStatus

Display net settings of all players
==================
*/
void Proxy_Engine_ClientCommand_NetStatus(int clientNum)
{
	ClientData_t *currentClientData = &proxy.clientData[clientNum];

	if (currentClientData->lastTimeNetStatus + 500 > server.svs->time)
	{
		return;
	}

	char			status[4096];
	int				i;
	client_t* 		cl;
	playerState_t* 	ps;
	int				ping;
	char			state[32];

	status[0] = 0;

	jampgame.functions.Q_strcat(status, sizeof(status), "score ping rate   fps  packets timeNudge snaps id name \n");
	jampgame.functions.Q_strcat(status, sizeof(status), "----- ---- ------ ---- ------- --------- ----- -- ---------------\n");

	for (i = 0, cl = server.svs->clients; i < server.cvars.sv_maxclients->integer; i++, cl++)
	{
		int			fps = 0;
		int			packets = 0;
		int			snaps = 0;

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
		
		// If not a bot
		if (cl->ping >= 1)
		{
			Proxy_Engine_Client_CalcPacketsAndFPS(i, &packets, &fps);
		}
		else {
			// if a bot then snapshotMsec will = 0, which is bad for "1000 / cl->snapshotMsec"
			cl->snapshotMsec = 1;
		}

		if (1000 / cl->snapshotMsec > server.cvars.sv_fps->integer)
		{
			snaps = server.cvars.sv_fps->integer;
		}
		else
		{
			snaps = 1000 / cl->snapshotMsec;
		}

		// No need for truncation "feature" if we move name to the end
		jampgame.functions.Q_strcat(status, sizeof(status), jampgame.functions.va("%5i %s %6i %4i %7i %9i %5i %2i %s^7\n", ps->persistant[PERS_SCORE], state, cl->rate, fps, packets, proxy.clientData[i].timenudge, snaps, i, cl->name));
	}

	currentClientData->lastTimeNetStatus = server.svs->time;

	char buffer[1012] = { 0 };
	size_t statusLength = strlen(status);
	size_t currentProgress = 0;

	while (currentProgress < statusLength)
	{
		jampgame.functions.Q_strncpyz(buffer, &status[currentProgress], sizeof(buffer));

		if (strlen(buffer) < 1012)
		{
			jampgame.functions.Q_strcat(buffer, sizeof(buffer), "\n");
		}

		trap_SendServerCommand(clientNum, jampgame.functions.va("print \"%s", buffer));
		currentProgress += strlen(buffer);
	}
}