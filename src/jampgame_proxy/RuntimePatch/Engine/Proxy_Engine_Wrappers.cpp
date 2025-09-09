#include "Proxy_Engine_Wrappers.hpp"
#include "jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_Utils.hpp"

ProxyServer_t server = { 0 };
ProxyJampgame_t jampgame = { 0 };

void Proxy_Engine_Initialize_MemoryLayer(void)
{
	printf("----- Proxy: Initializing memory layer\n");

	// ----------- SERVER

	// vars
	server.svs = (decltype(server.svs))var_svs_addr;
	server.sv = (decltype(server.sv))var_sv_addr;

	// cvars
	server.cvars.sv_fps = *(cvar_t**)cvar_sv_fps_addr;
	server.cvars.sv_gametype = *(cvar_t**)cvar_sv_gametype_addr;
	server.cvars.sv_hostname = *(cvar_t**)cvar_sv_hostname_addr;
	server.cvars.sv_mapname = *(cvar_t**)cvar_sv_mapname_addr;
	server.cvars.sv_maxclients = *(cvar_t**)cvar_sv_maxclients_addr;
	server.cvars.sv_privateClients = *(cvar_t**)cvar_sv_privateClients_addr;
	server.cvars.sv_pure = *(cvar_t**)cvar_sv_pure_addr;
	server.cvars.sv_maxRate = *(cvar_t**)cvar_sv_maxRate_addr;
	server.cvars.sv_rconPassword = *(cvar_t**)cvar_sv_rconPassword_addr;
	server.cvars.sv_floodProtect = *(cvar_t**)cvar_sv_floodProtect_addr;
	server.cvars.sv_allowDownload = *(cvar_t**)cvar_sv_allowDownload_addr;

	// functions
	server.functions.SV_ClientEnterWorld = (decltype(server.functions.SV_ClientEnterWorld))func_SV_ClientEnterWorld_addr;
	server.functions.SV_ClientThink = (decltype(server.functions.SV_ClientThink))func_SV_ClientThink_addr;
	server.functions.SV_DropClient = (decltype(server.functions.SV_DropClient))func_SV_DropClient_addr;
	server.functions.SV_Netchan_Transmit = (decltype(server.functions.SV_Netchan_Transmit))func_SV_Netchan_Transmit_addr;
	//server.functions.SV_RateMsec = (decltype(server.functions.SV_RateMsec))func_SV_RateMsec_addr; // we directly use our own function inside "Proxy_sv_snapshot()"
	server.functions.SV_UpdateServerCommandsToClient = (decltype(server.functions.SV_UpdateServerCommandsToClient))func_SV_UpdateServerCommandsToClient_addr;
	server.functions.SV_FlushRedirect = (decltype(server.functions.SV_FlushRedirect))func_SV_FlushRedirect_addr;
	server.functions.SV_ExecuteClientCommand = (decltype(server.functions.SV_ExecuteClientCommand))func_SV_ExecuteClientCommand_addr;
	server.functions.SV_GetChallenge = (decltype(server.functions.SV_GetChallenge))func_SV_GetChallenge_addr;
	server.functions.SV_DirectConnect = (decltype(server.functions.SV_DirectConnect))func_SV_DirectConnect_addr;
	server.functions.SVC_RemoteCommand = (decltype(server.functions.SVC_RemoteCommand))func_SVC_RemoteCommand_addr;
	server.functions.SV_Netchan_Process = (decltype(server.functions.SV_Netchan_Process))func_SV_Netchan_Process_addr;
	server.functions.SV_SendMessageToClient = (decltype(server.functions.SV_SendMessageToClient))func_SV_SendMessageToClient_addr;
	server.functions.SVC_Status = (decltype(server.functions.SVC_Status))func_SVC_Status_addr;
	server.functions.SVC_Info = (decltype(server.functions.SVC_Info))func_SVC_Info_addr;
	server.functions.SV_ConnectionlessPacket = (decltype(server.functions.SV_ConnectionlessPacket))func_SV_ConnectionlessPacket_addr;
	server.functions.SV_ExecuteClientMessage = (decltype(server.functions.SV_ExecuteClientMessage))func_SV_ExecuteClientMessage_addr;
	server.functions.SV_SendClientGameState = (decltype(server.functions.SV_SendClientGameState))func_SV_SendClientGameState_addr;
	server.functions.SV_SendServerCommand = (decltype(server.functions.SV_SendServerCommand))func_SV_SendServerCommand_addr;
	server.functions.SV_UserinfoChanged = (decltype(server.functions.SV_UserinfoChanged))func_SV_UserinfoChanged_addr;
	server.functions.SV_SendClientSnapshot = (decltype(server.functions.SV_SendClientSnapshot))func_SV_SendClientSnapshot_addr;

	// ----------- COMMON

	// vars
	server.common.vars.logfile = (decltype(server.common.vars.logfile))var_common_logfile_addr;
	server.common.vars.rd_buffer = (decltype(server.common.vars.rd_buffer))var_common_rd_buffer_addr;
	server.common.vars.rd_buffersize = (decltype(server.common.vars.rd_buffersize))var_common_rd_buffersize_addr;
	server.common.vars.rd_flush = (decltype(server.common.vars.rd_flush))var_common_rd_flush_addr;
	server.common.vars.cmd_argc = (decltype(server.common.vars.cmd_argc))var_cmd_argc_addr;
	server.common.vars.cmd_argv = (decltype(server.common.vars.cmd_argv))var_cmd_argv_addr;
	server.common.vars.cmd_tokenized = (decltype(server.common.vars.cmd_tokenized))var_cmd_tokenized_addr;

	// cvars
	server.common.cvars.com_dedicated = *(cvar_t**)cvar_common_com_dedicated_addr;
	server.common.cvars.com_sv_running = *(cvar_t**)cvar_common_com_sv_running_addr;
	server.common.cvars.com_cl_running = *(cvar_t**)cvar_common_com_cl_running_addr;
	server.common.cvars.com_logfile = *(cvar_t**)cvar_common_com_logfile_addr;
	server.common.cvars.com_developer = *(cvar_t**)cvar_common_com_developer_addr;
	server.common.cvars.fs_gamedirvar = *(cvar_t**)cvar_common_fs_gamedirvar_addr;

	// functions
	server.common.functions.Com_DPrintf = (decltype(server.common.functions.Com_DPrintf))func_Com_DPrintf_addr;
	server.common.functions.Com_HashKey = (decltype(server.common.functions.Com_HashKey))func_Com_HashKey_addr;
	server.common.functions.Com_Printf = (decltype(server.common.functions.Com_Printf))func_Com_Printf_addr;
	server.common.functions.Com_BeginRedirect = (decltype(server.common.functions.Com_BeginRedirect))func_Com_BeginRedirect_addr;
	server.common.functions.Com_EndRedirect = (decltype(server.common.functions.Com_EndRedirect))func_Com_EndRedirect_addr;
	server.common.functions.Cvar_VariableString = (decltype(server.common.functions.Cvar_VariableString))func_Cvar_VariableString_addr;
	server.common.functions.Cvar_VariableValue = (decltype(server.common.functions.Cvar_VariableValue))func_Cvar_VariableValue_addr;
	server.common.functions.FS_FOpenFileWrite = (decltype(server.common.functions.FS_FOpenFileWrite))func_FS_FOpenFileWrite_addr;
	server.common.functions.FS_ForceFlush = (decltype(server.common.functions.FS_ForceFlush))func_FS_ForceFlush_addr;
	server.common.functions.FS_Initialized = (decltype(server.common.functions.FS_Initialized))func_FS_Initialized_addr;
	server.common.functions.FS_Write = (decltype(server.common.functions.FS_Write))func_FS_Write_addr;
	server.common.functions.FS_FileIsInPAK = (decltype(server.common.functions.FS_FileIsInPAK))func_FS_FileIsInPAK_addr;
	server.common.functions.FS_LoadedPakPureChecksums = (decltype(server.common.functions.FS_LoadedPakPureChecksums))func_FS_LoadedPakPureChecksums_addr;
	server.common.functions.FS_ReferencedPakNames = (decltype(server.common.functions.FS_ReferencedPakNames))func_FS_ReferencedPakNames_addr;
	server.common.functions.FS_SV_FOpenFileRead = (decltype(server.common.functions.FS_SV_FOpenFileRead))func_FS_SV_FOpenFileRead_addr;
	server.common.functions.FS_Read = (decltype(server.common.functions.FS_Read))func_FS_Read_addr;
	server.common.functions.FS_FCloseFile = (decltype(server.common.functions.FS_FCloseFile))func_FS_FCloseFile_addr;
	server.common.functions.Netchan_TransmitNextFragment = (decltype(server.common.functions.Netchan_TransmitNextFragment))func_Netchan_TransmitNextFragment_addr;
	server.common.functions.NET_AdrToString = (decltype(server.common.functions.NET_AdrToString))func_NET_AdrToString_addr;
	server.common.functions.NET_OutOfBandPrint = (decltype(server.common.functions.NET_OutOfBandPrint))func_NET_OutOfBandPrint_addr;
	server.common.functions.NET_CompareBaseAdr = (decltype(server.common.functions.NET_CompareBaseAdr))func_NET_CompareBaseAdr_addr;
	server.common.functions.MSG_Init = (decltype(server.common.functions.MSG_Init))func_MSG_Init_addr;
	server.common.functions.MSG_ReadByte = (decltype(server.common.functions.MSG_ReadByte))func_MSG_ReadByte_addr;
	server.common.functions.MSG_ReadDeltaUsercmdKey = (decltype(server.common.functions.MSG_ReadDeltaUsercmdKey))func_MSG_ReadDeltaUsercmdKey_addr;
	server.common.functions.MSG_ReadShort = (decltype(server.common.functions.MSG_ReadShort))func_MSG_ReadShort_addr;
	server.common.functions.MSG_ReadLong = (decltype(server.common.functions.MSG_ReadLong))func_MSG_ReadLong_addr;
	server.common.functions.MSG_ReadString = (decltype(server.common.functions.MSG_ReadString))func_MSG_ReadString_addr;
	server.common.functions.MSG_ReadStringLine = (decltype(server.common.functions.MSG_ReadStringLine))func_MSG_ReadStringLine_addr;
	server.common.functions.MSG_Bitstream = (decltype(server.common.functions.MSG_Bitstream))func_MSG_Bitstream_addr;
	server.common.functions.MSG_BeginReadingOOB = (decltype(server.common.functions.MSG_BeginReadingOOB))func_MSG_BeginReadingOOB_addr;
	server.common.functions.MSG_WriteBigString = (decltype(server.common.functions.MSG_WriteBigString))func_MSG_WriteBigString_addr;
	server.common.functions.MSG_WriteByte = (decltype(server.common.functions.MSG_WriteByte))func_MSG_WriteByte_addr;
	server.common.functions.MSG_WriteDeltaEntity = (decltype(server.common.functions.MSG_WriteDeltaEntity))func_MSG_WriteDeltaEntity_addr;
	server.common.functions.MSG_WriteLong = (decltype(server.common.functions.MSG_WriteLong))func_MSG_WriteLong_addr;
	server.common.functions.MSG_WriteShort = (decltype(server.common.functions.MSG_WriteShort))func_MSG_WriteShort_addr;
	server.common.functions.MSG_WriteString = (decltype(server.common.functions.MSG_WriteString))func_MSG_WriteString_addr;
	server.common.functions.MSG_WriteData = (decltype(server.common.functions.MSG_WriteData))func_MSG_WriteData_addr;
	server.common.functions.Sys_IsLANAddress = (decltype(server.common.functions.Sys_IsLANAddress))func_Sys_IsLANAddress_addr;
	server.common.functions.Sys_Print = (decltype(server.common.functions.Sys_Print))func_Sys_Print_addr;
	server.common.functions.Cmd_Argv = (decltype(server.common.functions.Cmd_Argv))func_Cmd_Argv_addr;
	server.common.functions.Cmd_ExecuteString = (decltype(server.common.functions.Cmd_ExecuteString))func_Cmd_ExecuteString_addr;
	server.common.functions.Cmd_TokenizeString = (decltype(server.common.functions.Cmd_TokenizeString))func_Cmd_TokenizeString_addr;
	server.common.functions.Huff_Decompress = (decltype(server.common.functions.Huff_Decompress))func_Huff_Decompress_addr;
	server.common.functions.Z_Free = (decltype(server.common.functions.Z_Free))func_Z_Free_addr;
	server.common.functions.Z_Malloc = (decltype(server.common.functions.Z_Malloc))func_Z_Malloc_addr;

	// jampgame
	jampgame.functions.Com_Error = (decltype(jampgame.functions.Com_Error))proxy.jampgameAddress + jampgame_func_Com_Error_addr;
	jampgame.functions.Com_sprintf = (decltype(jampgame.functions.Com_sprintf))proxy.jampgameAddress + jampgame_func_Com_sprintf_addr;
	jampgame.functions.Info_SetValueForKey = (decltype(jampgame.functions.Info_SetValueForKey))proxy.jampgameAddress + jampgame_func_Info_SetValueForKey_addr;
	jampgame.functions.Info_ValueForKey = (decltype(jampgame.functions.Info_ValueForKey))proxy.jampgameAddress + jampgame_func_Info_ValueForKey_addr;
	jampgame.functions.Q_strcat = (decltype(jampgame.functions.Q_strcat))proxy.jampgameAddress + jampgame_func_Q_strcat_addr;
	jampgame.functions.Q_stricmp = (decltype(jampgame.functions.Q_stricmp))proxy.jampgameAddress + jampgame_func_Q_stricmp_addr;
	jampgame.functions.Q_stricmpn = (decltype(jampgame.functions.Q_stricmpn))proxy.jampgameAddress + jampgame_func_Q_stricmpn_addr;
	jampgame.functions.Q_strncmp = (decltype(jampgame.functions.Q_strncmp))proxy.jampgameAddress + jampgame_func_Q_strncmp_addr;
	jampgame.functions.Q_strncpyz = (decltype(jampgame.functions.Q_strncpyz))proxy.jampgameAddress + jampgame_func_Q_strncpyz_addr;
	jampgame.functions.va = (decltype(jampgame.functions.va))proxy.jampgameAddress + jampgame_func_va_addr;
	
	printf("----- Proxy: Memory layer properly initialized\n");
}