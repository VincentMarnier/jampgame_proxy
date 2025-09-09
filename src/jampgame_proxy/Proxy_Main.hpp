#pragma once

// ==================================================
// jampgame_proxy
// Fork of JKA_YBEProxy by Yberion
// Inspired by JMPProxy from DeathSpike (https://github.com/Deathspike/JMPProxy)
// ==================================================

#include <sdk/qcommon/qcommon.h>
#include <cstdlib>

extern "C" __attribute__((visibility("default"))) int vmMain(int command, int arg0, int arg1, int arg2, int arg3, int arg4,
	int arg5, int arg6, int arg7, int arg8, int arg9, int arg10);