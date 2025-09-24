#include <sdk/game/g_local.h>
#include "jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_Utils.hpp"
#include "Proxy_g_cmds.hpp"

char* formatScore(int w, int l)
{
  return jampgame.functions.va("%d/%d(" S_COLOR_YELLOW "%+d" S_COLOR_WHITE ") [" S_COLOR_CYAN "%.2f" S_COLOR_WHITE "]", w, l, w - l, calcRatio(w, l));
}
void (*Original_BeginIntermission)(void);
void Proxy_BeginIntermission(void)
{
  Original_BeginIntermission();
  
  char clientName[MAX_NETNAME];
  char kd[28];
  char tk[9];
  char tdmg[13];

  // Print per clients stats
  for (int clientNum = 0; clientNum < MAX_CLIENTS; ++clientNum)
  {
    ClientData_t* cl = &proxy.clientData[clientNum];
    if (cl->isConnected)
    {
      trap_SendServerCommand( clientNum, "print \""
        "========================== PERSONAL STATS ==========================\n"
        "           Name   K/D(" S_COLOR_YELLOW "diff." S_COLOR_WHITE ") [" S_COLOR_CYAN "ratio" S_COLOR_WHITE "]          Damages(" S_COLOR_YELLOW "diff." S_COLOR_WHITE ") [" S_COLOR_CYAN "ratio" S_COLOR_WHITE "]\n"
        "--------------- -------------------- -------------------------------\n\"");
      for (int i = 0; i < MAX_CLIENTS; ++i)
      {
        GameStats_t* gameStats = &(cl->gameStats[i]);
        if (gameStats->damagesGiven > 0 || gameStats->damagesTaken > 0)
        {
          gclient_s* cl = (proxy.g_entities + i)->client;
          jampgame.functions.Q_strncpyz(clientName, cl->pers.netname, MAX_NETNAME);
          Q_StripColor(clientName);
          jampgame.functions.Q_strncpyz(kd, formatScore(gameStats->killed, gameStats->killedBy), 28);
          trap_SendServerCommand( clientNum, jampgame.functions.va("print \"%15.15s %28.28s %39.39s\n\"", 
            clientName,
            kd,
            formatScore(gameStats->damagesGiven, gameStats->damagesTaken)));
        }
      }
      trap_SendServerCommand( clientNum, "print \"\n\"");
    }
  }

  // Print global stats
  int highestDmgDealt = INT_MIN;
  int highestDmgDealerClientNum = -1;
  int bestDmgDealerDiff = INT_MIN;
  int bestDmgDealerClientNum = -1;
  int bestFraggerDiff = INT_MIN;
  int bestFraggerClientNum = -1;
  trap_SendServerCommand( -1, "print \""
    "======================================= GLOBAL STATS =======================================\n"
    "           Name   K/D(" S_COLOR_YELLOW "diff." S_COLOR_WHITE ") [" S_COLOR_CYAN "ratio" S_COLOR_WHITE "]          Damages(" S_COLOR_YELLOW "diff." S_COLOR_WHITE ") [" S_COLOR_CYAN "ratio" S_COLOR_WHITE "] Teamkills  Team damages\n"
    "--------------- -------------------- ------------------------------- --------- -------------\n\"");
  for (int clientNum = 0; clientNum < MAX_CLIENTS; ++clientNum)
  {
    ClientData_t* clData = &proxy.clientData[clientNum];
    if (clData->isConnected)
    {
      int damagesGiven = 0;
      int damagesTaken = 0;
      for (int i = 0; i < MAX_CLIENTS; ++i)
      {
        damagesGiven += clData->gameStats[i].damagesGiven;
        damagesTaken += clData->gameStats[i].damagesTaken;
      }

      if (damagesGiven > 0 || damagesTaken > 0)
      {
        // Print player's stats
        damagesGiven -= clData->teamDamagesGiven;
        gclient_s* cl = (proxy.g_entities + clientNum)->client;
        jampgame.functions.Q_strncpyz(clientName, (proxy.g_entities + clientNum)->client->pers.netname, MAX_NETNAME);
        Q_StripColor(clientName);
        jampgame.functions.Q_strncpyz(kd, formatScore(cl->ps.persistant[PERS_SCORE], cl->ps.persistant[PERS_KILLED]), 28);
        jampgame.functions.Com_sprintf(tk, 9, "%d/%d", clData->teamKills, clData->teamKilled);
        jampgame.functions.Com_sprintf(tdmg, 13, "%d/%d", clData->teamDamagesGiven, clData->teamDamagesTaken);
        trap_SendServerCommand( -1, jampgame.functions.va("print \"%15.15s %28.28s %39.39s %9.9s %13.13s\n\"", 
          clientName,
          kd,
          formatScore(damagesGiven, damagesTaken),
          tk,
          tdmg));

        // Update best players
        int damagesDiff = damagesGiven - damagesTaken;
        if (damagesDiff > bestDmgDealerDiff)
        {
          bestDmgDealerDiff = damagesDiff;
          bestDmgDealerClientNum = clientNum;
        }
        if (damagesGiven > highestDmgDealt)
        {
          highestDmgDealt = damagesGiven;
          highestDmgDealerClientNum = clientNum;
        }
        int killsDiff = cl->ps.persistant[PERS_SCORE] - cl->ps.persistant[PERS_KILLED];
        if (killsDiff > bestFraggerDiff)
        {
          bestFraggerDiff = killsDiff;
          bestFraggerClientNum = clientNum;
        }
      }
    }
  }

  // Print best players
  if (bestFraggerClientNum >= 0 || bestDmgDealerClientNum >= 0 || highestDmgDealerClientNum >= 0)
  {
    // Skip line
    trap_SendServerCommand( -1, "print \"\n\"");

    if (bestDmgDealerClientNum >= 0)
    {
      jampgame.functions.Q_strncpyz(clientName, (proxy.g_entities + bestDmgDealerClientNum)->client->pers.netname, MAX_NETNAME);
      Q_StripColor(clientName);
      trap_SendServerCommand( -1, jampgame.functions.va("print \"MVP: %s (" S_COLOR_YELLOW "%+d" S_COLOR_WHITE ")\n\"", 
        clientName,
        bestDmgDealerDiff));
    }

    if (highestDmgDealerClientNum >= 0)
    {
      jampgame.functions.Q_strncpyz(clientName, (proxy.g_entities + highestDmgDealerClientNum)->client->pers.netname, MAX_NETNAME);
      Q_StripColor(clientName);
      trap_SendServerCommand( -1, jampgame.functions.va("print \"Highest damage dealer: %s (" S_COLOR_YELLOW "%+d" S_COLOR_WHITE ")\n\"", 
        clientName,
        highestDmgDealt));
    }

    if (bestFraggerClientNum >= 0)
    {
      jampgame.functions.Q_strncpyz(clientName, (proxy.g_entities + bestFraggerClientNum)->client->pers.netname, MAX_NETNAME);
      Q_StripColor(clientName);
      trap_SendServerCommand( -1, jampgame.functions.va("print \"Best fragger: %s (" S_COLOR_YELLOW "%+d" S_COLOR_WHITE ")\n\"", 
        clientName,
        bestFraggerDiff));
    }
  }
}