#include "Proxy_Header.hpp"
#include "Proxy_CVars.hpp"
#include "Proxy_Translate_SystemCalls.hpp"
#include "RuntimePatch/Engine/Proxy_Engine_Wrappers.hpp"

static cvarTable_t proxyOriginalEngineCVarTable[] =
{
	{ &proxy.originalEngineCvars.proxy_sv_pingFix, "proxy_sv_pingFix", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &proxy.originalEngineCvars.proxy_sv_enableRconCmdCooldown, "proxy_sv_enableRconCmdCooldown", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &proxy.originalEngineCvars.proxy_sv_enableNetStatus, "proxy_sv_enableNetStatus", "0", CVAR_ARCHIVE, 0, qfalse },
};
static constexpr size_t proxyOriginalEngineCVarTableSize = 3;

static cvarTable_t proxyCVarTable[] =
{
	// New cvars
	{ &proxy.cvars.proxy_sv_maxCallVoteMapRestartValue, "proxy_sv_maxCallVoteMapRestartValue", "60", CVAR_ARCHIVE, 0, qfalse },
	{ &proxy.cvars.proxy_sv_modelPathLength, "proxy_sv_modelPathLength", "64", CVAR_ARCHIVE, 0, qfalse },
	{ &proxy.cvars.proxy_sv_disableKillCmd, "proxy_sv_disableKillCmd", "0", CVAR_ARCHIVE, 0, qfalse },

	// get cvars
	{ &proxy.cvars.sv_fps, "sv_fps", "20", CVAR_SERVERINFO, 0, qfalse },
	{ &proxy.cvars.sv_gametype, "sv_gametype", "0", CVAR_SERVERINFO, 0, qfalse }
};
static constexpr size_t proxyCVarTableSize = 5;

// ==================================================
// Proxy_OriginalEngine_CVars_Registration
// --------------------------------------------------
// Those are the variables that will affect engine modifications.
// ==================================================

void Proxy_OriginalEngine_CVars_Registration(void)
{
	printf("----- Proxy: Initializing original engine Proxy CVars\n");

	unsigned int i = 0;
	cvarTable_t* currentCVarTableItem = nullptr;

	for (i = 0, currentCVarTableItem = proxyOriginalEngineCVarTable; i < proxyOriginalEngineCVarTableSize; ++i, ++currentCVarTableItem)
	{
		trap_Cvar_Register(currentCVarTableItem->vmCvar, currentCVarTableItem->cvarName, currentCVarTableItem->defaultString, currentCVarTableItem->cvarFlags);

		if (currentCVarTableItem->vmCvar)
		{
			currentCVarTableItem->modificationCount = currentCVarTableItem->vmCvar->modificationCount;
		}
	}

	printf("----- Proxy: Original engine Proxy CVars properly initialized\n");
}

// ==================================================
// Proxy_CVars_Registration
// --------------------------------------------------
// Those are the variables that won't affect engine modifications.
// ==================================================

void Proxy_CVars_Registration(void)
{
	unsigned int i = 0;
	cvarTable_t* currentCVarTableItem = nullptr;

	for (i = 0, currentCVarTableItem = proxyCVarTable; i < proxyCVarTableSize; ++i, ++currentCVarTableItem)
	{
		trap_Cvar_Register(currentCVarTableItem->vmCvar, currentCVarTableItem->cvarName, currentCVarTableItem->defaultString, currentCVarTableItem->cvarFlags);

		if (currentCVarTableItem->vmCvar)
		{
			currentCVarTableItem->modificationCount = currentCVarTableItem->vmCvar->modificationCount;
		}
	}
}

// ==================================================
// Proxy_OriginalEngine_UpdateCvars
// --------------------------------------------------
// Updates all affecting engine variables.
// ==================================================

static void Proxy_OriginalEngine_UpdateCvars(void)
{
	unsigned int i = 0;
	cvarTable_t* currentCVarTable = nullptr;

	for (i = 0, currentCVarTable = proxyOriginalEngineCVarTable; i < proxyOriginalEngineCVarTableSize; ++i, ++currentCVarTable)
	{
		if (currentCVarTable->vmCvar)
		{
			int vmCvarModificationCount = currentCVarTable->vmCvar->modificationCount;
			trap_Cvar_Update(currentCVarTable->vmCvar);

			if (currentCVarTable->modificationCount != vmCvarModificationCount)
			{
				currentCVarTable->modificationCount = vmCvarModificationCount;

				if (currentCVarTable->trackChange)
				{
					trap_SendServerCommand(-1, jampgame.functions.va("print \"Server: %s changed to %s\n\"",
						currentCVarTable->cvarName, currentCVarTable->vmCvar->string));
				}
			}
		}
	}
}

// ==================================================
// Proxy_UpdateCvars
// --------------------------------------------------
// Updates all proxy variables.
// ==================================================

static void Proxy_UpdateCvars(void)
{
	unsigned int i = 0;
	cvarTable_t* currentCVarTable = nullptr;

	for (i = 0, currentCVarTable = proxyCVarTable; i < proxyCVarTableSize; ++i, ++currentCVarTable)
	{
		if (currentCVarTable->vmCvar)
		{
			int vmCvarModificationCount = currentCVarTable->vmCvar->modificationCount;
			trap_Cvar_Update(currentCVarTable->vmCvar);

			if (currentCVarTable->modificationCount != vmCvarModificationCount)
			{
				currentCVarTable->modificationCount = vmCvarModificationCount;

				if (currentCVarTable->trackChange)
				{
					trap_SendServerCommand(-1, jampgame.functions.va("print \"Server: %s changed to %s\n\"",
						currentCVarTable->cvarName, currentCVarTable->vmCvar->string));
				}
			}
		}
	}
}

// ==================================================
// Proxy_UpdateAllCvars
// --------------------------------------------------
// Updates all the variables once per second~.
// ==================================================
// TODO: Seems like something we don't want to do every sec ...
void Proxy_UpdateAllCvars(void)
{
	// TODO: won't be once per second on sv_fps change
	static unsigned int frame = 0;

	if (frame == (unsigned int)proxy.cvars.sv_fps.integer)
	{
		Proxy_UpdateCvars();
		Proxy_OriginalEngine_UpdateCvars();

		frame = 0;
	}

	frame++;
}
