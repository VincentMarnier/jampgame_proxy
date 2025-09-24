#include "jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_Wrappers.hpp"
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
    ClientData_t *clTarget = &proxy.clientData[targ->s.clientNum];
    ClientData_t *clAttacker = &proxy.clientData[attacker->s.clientNum];
    clAttacker->gameStats[targ->s.clientNum].damagesGiven += damages;
    clTarget->gameStats[attacker->s.clientNum].damagesTaken += damages;
    if (
      (targ->client->sess.sessionTeam == TEAM_RED || targ->client->sess.sessionTeam == TEAM_BLUE) &&
      targ->client->sess.sessionTeam == attacker->client->sess.sessionTeam
    )
    {
      clTarget->teamDamagesTaken += damages;
      clAttacker->teamDamagesGiven += damages;
    }
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
    ClientData_t *clSelf = &proxy.clientData[self->s.clientNum];
    ClientData_t *clAttacker = &proxy.clientData[attacker->s.clientNum];
    clAttacker->gameStats[self->s.clientNum].killed++;
    clSelf->gameStats[attacker->s.clientNum].killedBy++;
    if (
      (self->client->sess.sessionTeam == TEAM_RED || self->client->sess.sessionTeam == TEAM_BLUE) &&
      self->client->sess.sessionTeam == attacker->client->sess.sessionTeam
    )
    {
      clSelf->teamKilled++;
      clAttacker->teamKills++;
    }
	}
}
