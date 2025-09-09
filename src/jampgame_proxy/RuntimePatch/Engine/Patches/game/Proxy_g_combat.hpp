#pragma once

#include <sdk/game/g_local.h>

extern void (*Original_G_Damage)(gentity_t *targ, gentity_t *inflictor, gentity_t *attacker,
  vec3_t dir, vec3_t point, int damage, int dflags, int mod);
void Proxy_G_Damage(gentity_t *targ, gentity_t *inflictor, gentity_t *attacker,
  vec3_t dir, vec3_t point, int damage, int dflags, int mod);

extern void (*Original_player_die)(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath);
void Proxy_player_die(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath);

extern void (*Original_BeginIntermission)(void);
void Proxy_BeginIntermission(void);