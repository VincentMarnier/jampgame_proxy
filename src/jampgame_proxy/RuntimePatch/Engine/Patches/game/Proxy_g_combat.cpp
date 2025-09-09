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
}
