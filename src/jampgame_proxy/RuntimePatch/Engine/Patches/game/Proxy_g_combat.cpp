#include "jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_Wrappers.hpp"
#include "jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_Utils.hpp"
#include "jampgame_proxy/Imports/server/sv_game.hpp"
#include "jampgame_proxy/Proxy_Header.hpp"
#include "Proxy_g_combat.hpp"

#define max(x,y) ((x)>(y)?(x):(y))

void (*Original_G_Damage)(gentity_t *targ, gentity_t *inflictor, gentity_t *attacker,
  vec3_t dir, vec3_t point, int damage, int dflags, int mod);
void Proxy_G_Damage(gentity_t *targ, gentity_t *inflictor, gentity_t *attacker,
  vec3_t dir, vec3_t point, int damage, int dflags, int mod)
{
  if (targ && targ->client && attacker && attacker->client && !targ->damageRedirect)
	{
    int total_health = max(targ->client->ps.stats[STAT_ARMOR], 0) + max(targ->client->ps.stats[STAT_HEALTH], 0);
    Original_G_Damage(targ, inflictor, attacker, dir, point, damage, dflags, mod);
    int damages = total_health - max(targ->client->ps.stats[STAT_ARMOR], 0) - max(targ->client->ps.stats[STAT_HEALTH], 0);
    proxy.clientData[attacker->s.clientNum].gameStats[targ->s.clientNum].damagesGiven += damages;
    proxy.clientData[targ->s.clientNum].gameStats[attacker->s.clientNum].damagesTaken += damages;
	}
  else
  {
    Original_G_Damage(targ, inflictor, attacker, dir, point, damage, dflags, mod);
  }
}

void (*Original_player_die)(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath);
void Proxy_player_die(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath)
{
  Original_player_die(self, inflictor, attacker, damage, meansOfDeath);

  if (self && self->client && attacker && attacker->client)
	{
    proxy.clientData[attacker->s.clientNum].gameStats[self->s.clientNum].killed++;
    proxy.clientData[self->s.clientNum].gameStats[attacker->s.clientNum].killedBy++;
	}
  else
  {
  }
}

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
          trap_SendServerCommand( clientNum, jampgame.functions.va("print \"%15s %28s %39s\n\"", 
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
        playerState_t* ps;
        jampgame.functions.Q_strncpyz(clientName, (proxy.g_entities + clientNum)->client->pers.netname, MAX_NETNAME);
        Q_StripColor(clientName);
	      ps = Proxy_GetPlayerStateByClientNum(clientNum);
        jampgame.functions.Q_strncpyz(kd, formatScore(ps->persistant[PERS_SCORE], ps->persistant[PERS_KILLED]), 28);
        trap_SendServerCommand( -1, jampgame.functions.va("print \"%15s %28s %39s\n\"", 
          clientName,
          kd,
          formatScore(damagesGiven, damagesTaken)));
      }
    }
  }
}