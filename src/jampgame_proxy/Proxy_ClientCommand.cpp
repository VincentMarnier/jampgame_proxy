#include "Proxy_Header.hpp"
#include "Proxy_ClientCommand.hpp"
#include "Imports/server/sv_game.hpp"
#include "RuntimePatch/Engine/Proxy_Engine_Wrappers.hpp"
#include "Proxy_Translate_SystemCalls.hpp"
#include "Proxy_Utils.hpp"

/*
==================
Proxy_ClientCommand_MyRatio
==================
*/
static void ratioString(int kill, int death, int suicides, char* ratioString, int sizeRatioString)
{
	if (kill - death >= 0)
	{
		jampgame.functions.Com_sprintf(ratioString, sizeRatioString, "(" S_COLOR_GREEN "+%i" S_COLOR_WHITE ")", kill - death);
	}
	else
	{
		jampgame.functions.Com_sprintf(ratioString, sizeRatioString, "(" S_COLOR_RED "%i" S_COLOR_WHITE ")", kill - (death - suicides));
	}
}

void Proxy_ClientCommand_MyRatio(int clientNum)
{
	ClientData_t *currentClientData = &proxy.clientData[clientNum];

	if (currentClientData->lastTimeMyratioCheck + 1000 > proxy.proxyData.svsTime)
	{
		return;
	}

	currentClientData->lastTimeMyratioCheck = proxy.proxyData.svsTime;

	char ratioStringBuffer[16];
	playerState_t* ps;

	ps = Proxy_GetPlayerStateByClientNum(clientNum);

	float ratio = calcRatio(ps->persistant[PERS_SCORE], ps->persistant[PERS_KILLED]);

	ratioString(ps->persistant[PERS_SCORE], ps->persistant[PERS_KILLED], ps->fd.suicides, ratioStringBuffer, sizeof(ratioStringBuffer));

	if (proxy.cvars.sv_gametype.integer != GT_DUEL && proxy.cvars.sv_gametype.integer != GT_TEAM && proxy.cvars.sv_gametype.integer != GT_FFA)
	{
		trap_SendServerCommand(clientNum, "print \"Command not supported for this gametype\n\"");

		return;
	}

	trap_SendServerCommand(clientNum, jampgame.functions.va("print \"Kills" S_COLOR_BLUE ": " S_COLOR_WHITE "%i " S_COLOR_BLUE "| "
		S_COLOR_WHITE "Deaths" S_COLOR_BLUE ": " S_COLOR_WHITE "%i " S_COLOR_BLUE "(" S_COLOR_WHITE "Suicides" S_COLOR_BLUE ": " S_COLOR_WHITE "%i" S_COLOR_BLUE ") " S_COLOR_BLUE "| "
		S_COLOR_WHITE "Ratio" S_COLOR_BLUE ": " S_COLOR_WHITE "%.2f %s\n\"",
		ps->persistant[PERS_SCORE], ps->persistant[PERS_KILLED],
		ps->fd.suicides,
		ratio, ratioStringBuffer));
}
