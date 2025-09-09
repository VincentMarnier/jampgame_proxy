#pragma once

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

  // Print per clients stats
  for (int clientNum = 0; clientNum < MAX_CLIENTS; ++clientNum)
  {
    ClientData_t* cl = &proxy.clientData[clientNum];
    if (cl->isConnected)
    {
      trap_SendServerCommand( clientNum, "print \""
        "========================== PERSONAL STATS ==========================\n"
        "           name   K/D(" S_COLOR_YELLOW "diff." S_COLOR_WHITE ") [" S_COLOR_CYAN "ratio" S_COLOR_WHITE "]          Damages(" S_COLOR_YELLOW "diff." S_COLOR_WHITE ") [" S_COLOR_CYAN "ratio" S_COLOR_WHITE "]\n"
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
  trap_SendServerCommand( -1, "print \""
    "=========================== GLOBAL STATS ===========================\n"
    "           name   K/D(" S_COLOR_YELLOW "diff." S_COLOR_WHITE ") [" S_COLOR_CYAN "ratio" S_COLOR_WHITE "]          Damages(" S_COLOR_YELLOW "diff." S_COLOR_WHITE ") [" S_COLOR_CYAN "ratio" S_COLOR_WHITE "]\n"
    "--------------- -------------------- -------------------------------\n\"");
  for (int clientNum = 0; clientNum < MAX_CLIENTS; ++clientNum)
  {
    ClientData_t* cl = &proxy.clientData[clientNum];
    if (cl->isConnected)
    {
      int damagesGiven = 0;
      int damagesTaken = 0;
      for (int i = 0; i < MAX_CLIENTS; ++i)
      {
        damagesGiven += cl->gameStats[i].damagesGiven;
        damagesTaken += cl->gameStats[i].damagesTaken;
      }

      if (damagesGiven > 0 || damagesTaken > 0)
      {
        gclient_s* cl = (proxy.g_entities + clientNum)->client;
        jampgame.functions.Q_strncpyz(clientName, (proxy.g_entities + clientNum)->client->pers.netname, MAX_NETNAME);
        Q_StripColor(clientName);
        jampgame.functions.Q_strncpyz(kd, formatScore(cl->ps.persistant[PERS_SCORE], cl->ps.persistant[PERS_KILLED]), 28);
        trap_SendServerCommand( -1, jampgame.functions.va("print \"%15.15s %28.28s %39.39s\n\"", 
          clientName,
          kd,
          formatScore(damagesGiven, damagesTaken)));
      }
    }
  }
}