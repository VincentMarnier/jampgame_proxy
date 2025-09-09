#pragma once

// ==================================================
// INCLUDE
// ==================================================

#include "jampgame_proxy/Proxy_Header.hpp"
#include <sdk/qcommon/qcommon.h>
#include <sdk/server/server.h>

// ==================================================
// DEFINE
// ==================================================

// --------------------------------
// Address to NOPs
// --------------------------------

// We want to NOP mostly all bytes related to the timer check, including the call to Com_Milliseconds()
constexpr intptr_t func_SVC_RemoteCommand_timer_start_block_addr = 0x8056b26;
// from this address, we want to NOP 25 bytes
constexpr size_t func_SVC_RemoteCommand_timer_NOP_amount = 25;

// --------------------------------
// Address to patch a single byte
// --------------------------------

// the address to the byte that will allow use to change the value from 49136 to 1008
constexpr intptr_t func_SVC_RemoteCommand_Com_BeginRedirect_SV_OUTPUTBUF_LENGTH_addr = 0x8056c7d;

// --------------------------------
// Function address to hook
// --------------------------------

constexpr intptr_t func_Com_Printf_addr = 0x8072ca4;
constexpr intptr_t func_SV_CalcPings_addr = 0x8057204;
constexpr intptr_t func_SV_SendMessageToClient_addr = 0x8058c84;
constexpr intptr_t func_SV_SendClientGameState_addr = 0x804cee4;
constexpr intptr_t func_SV_Status_f_addr = 0x804f7f4;
constexpr intptr_t func_SV_SvEntityForGentity_addr = 0x804ffb4;
constexpr intptr_t func_SV_ConnectionlessPacket_addr = 0x8056d64;
constexpr intptr_t func_SVC_Status_addr = 0x8056574;
constexpr intptr_t func_SVC_Info_addr = 0x8056784;
constexpr intptr_t func_SVC_RemoteCommand_addr = 0x8056b14;
constexpr intptr_t func_Cmd_TokenizeString_addr = 0x812c454;
constexpr intptr_t func_SV_UserinfoChanged_addr = 0x804e144;
constexpr intptr_t func_SV_ExecuteClientMessage_addr = 0x804e8c4;
constexpr intptr_t func_SV_PacketEvent_addr = 0x8057024;
constexpr intptr_t func_SV_ExecuteClientCommand_addr = 0x804e3d4;
constexpr intptr_t func_SV_BeginDownload_f_addr = 0x804d714;
constexpr intptr_t func_SV_DoneDownload_f_addr = 0x804eaa4;
constexpr intptr_t func_SV_WriteDownloadToClient_addr = 0x804d764;

// --------------------------------
// Function address to call
// --------------------------------

constexpr intptr_t func_SV_ClientEnterWorld_addr = 0x804d444;
constexpr intptr_t func_SV_ClientThink_addr = 0x804e634;
constexpr intptr_t func_SV_DropClient_addr = 0x804cb84;
constexpr intptr_t func_SV_Netchan_Transmit_addr = 0x8057db4;
//constexpr intptr_t func_SV_RateMsec_addr = 0x8058c04;
constexpr intptr_t func_SV_FlushRedirect_addr = 0x8057b44;
constexpr intptr_t func_SV_UpdateServerCommandsToClient_addr = 0x80582c4;
constexpr intptr_t func_SV_GetChallenge_addr = 0x804b9a4;
constexpr intptr_t func_SV_DirectConnect_addr = 0x804c014;
constexpr intptr_t func_SV_Netchan_Process_addr = 0x8057e14;
constexpr intptr_t func_SV_SendServerCommand_addr = 0x8056214;
constexpr intptr_t func_SV_SendClientSnapshot_addr = 0x8058e04;
constexpr intptr_t func_Com_DPrintf_addr = 0x8072ed4;
constexpr intptr_t func_Com_HashKey_addr = 0x8073b14;
constexpr intptr_t func_Com_BeginRedirect_addr = 0x8072c34;
constexpr intptr_t func_Com_EndRedirect_addr = 0x8072c74;
constexpr intptr_t func_Cvar_VariableString_addr = 0x80756f4;
constexpr intptr_t func_Cvar_VariableValue_addr = 0x8075694;
constexpr intptr_t func_FS_FOpenFileWrite_addr = 0x812d2a4;
constexpr intptr_t func_FS_ForceFlush_addr = 0x812c8a4;
constexpr intptr_t func_FS_Initialized_addr = 0x812b754;
constexpr intptr_t func_FS_Write_addr = 0x812e074;
constexpr intptr_t func_FS_FileIsInPAK_addr = 0x812e354;
constexpr intptr_t func_FS_LoadedPakPureChecksums_addr = 0x8130d44;
constexpr intptr_t func_FS_ReferencedPakNames_addr = 0x8131024;
constexpr intptr_t func_FS_SV_FOpenFileRead_addr = 0x812cd74;
constexpr intptr_t func_FS_Read_addr = 0x812df44;
constexpr intptr_t func_FS_FCloseFile_addr = 0x812d1b4;
constexpr intptr_t func_Netchan_TransmitNextFragment_addr = 0x807ab74;
constexpr intptr_t func_NET_AdrToString_addr = 0x807b314;
constexpr intptr_t func_NET_OutOfBandPrint_addr = 0x807b744;
constexpr intptr_t func_NET_CompareBaseAdr_addr = 0x807b284;
constexpr intptr_t func_MSG_Init_addr = 0x80774a4;
constexpr intptr_t func_MSG_ReadByte_addr = 0x8077df4;
constexpr intptr_t func_MSG_ReadDeltaUsercmdKey_addr = 0x8078b34;
constexpr intptr_t func_MSG_ReadShort_addr = 0x8077e34;
constexpr intptr_t func_MSG_ReadLong_addr = 0x8077e74;
constexpr intptr_t func_MSG_ReadString_addr = 0x8077ee4;
constexpr intptr_t func_MSG_ReadStringLine_addr = 0x8077ff4;
constexpr intptr_t func_MSG_Bitstream_addr = 0x80775d4;
constexpr intptr_t func_MSG_BeginReadingOOB_addr = 0x8077614;
constexpr intptr_t func_MSG_WriteBigString_addr = 0x8077c04;
constexpr intptr_t func_MSG_WriteByte_addr = 0x8077a24;
constexpr intptr_t func_MSG_WriteDeltaEntity_addr = 0x8078d74;
constexpr intptr_t func_MSG_WriteLong_addr = 0x8077ad4;
constexpr intptr_t func_MSG_WriteShort_addr = 0x8077aa4;
constexpr intptr_t func_MSG_WriteString_addr = 0x8077b34;
constexpr intptr_t func_MSG_WriteData_addr = 0x8077a54;
constexpr intptr_t func_Sys_IsLANAddress_addr = 0x80c5f84;
// Windows: directly Conbuf_AppendText()
constexpr intptr_t func_Sys_Print_addr = 0x80c57a4;
constexpr intptr_t func_Cmd_Argv_addr = 0x812c264;
constexpr intptr_t func_Cmd_ExecuteString_addr = 0x8124144;
constexpr intptr_t func_Huff_Decompress_addr = 0x807c224;
constexpr intptr_t func_Z_Free_addr = 0x8080474;
constexpr intptr_t func_Z_Malloc_addr = 0x80800f4;

// --------------------------------
// Variable address
// --------------------------------

constexpr intptr_t var_svs_addr = 0x83121e0;
constexpr intptr_t var_svsClients_addr = 0x83121ec;
constexpr intptr_t var_sv_addr = 0x8273ec0;
constexpr intptr_t var_common_rd_buffer_addr = 0x81e90c0;
constexpr intptr_t var_common_rd_buffersize_addr = 0x81e90c4;
constexpr intptr_t var_common_rd_flush_addr = 0x81e90c8;
constexpr intptr_t var_common_logfile_addr = 0x831f24c;
constexpr intptr_t var_cmd_argc_addr = 0x8260e20;
constexpr intptr_t var_cmd_argv_addr = 0x8260e40;
constexpr intptr_t var_cmd_tokenized_addr = 0x8264440;

// --------------------------------
// Cvars
// for 'server' addr check function:
//   - Windows: 0x442f60
//   - Linux: 0x8055824
// 
// for 'com' addr check function:
//   - Windows: 0x410fc0
//   - Linux: 0x8074614
// --------------------------------

constexpr intptr_t cvar_sv_fps_addr = 0x8273e84;
constexpr intptr_t cvar_sv_gametype_addr = 0x83121cc;
constexpr intptr_t cvar_sv_hostname_addr = 0x8273e9c;
constexpr intptr_t cvar_sv_mapname_addr = 0x8273e90;
constexpr intptr_t cvar_sv_maxclients_addr = 0x8273ea4;
constexpr intptr_t cvar_sv_privateClients_addr = 0x8273e80;
constexpr intptr_t cvar_sv_pure_addr = 0x83121a8;
constexpr intptr_t cvar_sv_maxRate_addr = 0x831219c;
constexpr intptr_t cvar_sv_rconPassword_addr = 0x83121d4;
constexpr intptr_t cvar_sv_floodProtect_addr = 0x8273e88;
constexpr intptr_t cvar_sv_allowDownload_addr = 0x83121b0;
constexpr intptr_t cvar_common_com_dedicated_addr = 0x831f254;
constexpr intptr_t cvar_common_com_sv_running_addr = 0x831f300;
constexpr intptr_t cvar_common_com_cl_running_addr = 0x831f400;
constexpr intptr_t cvar_common_com_logfile_addr = 0x831f41c;
constexpr intptr_t cvar_common_com_developer_addr = 0x831e204;
constexpr intptr_t cvar_common_fs_gamedirvar_addr = 0x838aaec;

// ==================================================
// STRUCT
// ==================================================

struct serverFunctions_t
{
	void		(*SV_ClientEnterWorld)							(client_t*, usercmd_t*);
	void		(*SV_ClientThink)								(client_t*, usercmd_t*);
	void		(*SV_DropClient)								(client_t*, const char*);
	void		(*SV_Netchan_Transmit)							(client_t*, msg_t*);
	//int		(*SV_RateMsec)									(client_t*, int);
	void		(*SV_UpdateServerCommandsToClient)				(client_t*, msg_t*);
	void		(*SV_FlushRedirect)								(char*);
	void		(*SV_ExecuteClientCommand)						(client_t*, const char*, qboolean);
	void		(*SV_GetChallenge)								(netadr_t);
	void		(*SV_DirectConnect)								(netadr_t);
	void		(*SVC_RemoteCommand)							(netadr_t, msg_t*);
	qboolean	(*SV_Netchan_Process)							(client_t*, msg_t*);
	void		(*SV_SendMessageToClient)						(msg_t*, client_t*);
	void		(*SVC_Status)									(netadr_t);
	void		(*SVC_Info)										(netadr_t);
	void		(*SV_ConnectionlessPacket)						(netadr_t, msg_t*);
	void		(*SV_ExecuteClientMessage)						(client_t*, msg_t*);
	void		(*SV_SendClientGameState)						(client_t*);
	void		(QDECL* SV_SendServerCommand)					(client_t*, const char*, ...);
	void		(*SV_UserinfoChanged)							(client_t*);
	void		(*SV_SendClientSnapshot)						(client_t*);
};

struct serverCvars_t
{
	cvar_t* sv_fps;
	cvar_t* sv_gametype;
	cvar_t* sv_hostname;
	cvar_t* sv_mapname;
	cvar_t* sv_maxclients;
	cvar_t* sv_privateClients;
	cvar_t* sv_pure;
	cvar_t* sv_maxRate;
	cvar_t* sv_rconPassword;
	cvar_t* sv_floodProtect;
	cvar_t* sv_allowDownload;
};

struct vars_t
{
	char**			rd_buffer;
	int*			rd_buffersize;
	void			(* *rd_flush) (char*);
	fileHandle_t*	logfile;
	int*			cmd_argc;
	char*			(*cmd_argv)[MAX_STRING_TOKENS]; // char* cmd_argv[MAX_STRING_TOKENS];
	char			(*cmd_tokenized)[BIG_INFO_STRING + MAX_STRING_TOKENS]; // char cmd_tokenized[BIG_INFO_STRING+MAX_STRING_TOKENS];
};

struct Cvars_t
{
	cvar_t* com_dedicated;
	cvar_t* com_sv_running;
	cvar_t* com_cl_running;
	cvar_t* com_logfile;
	cvar_t* com_developer;
	cvar_t* fs_gamedirvar;
};

struct Functions_t
{
	void			(QDECL* Com_DPrintf)							(const char*, ...);
	int				(*Com_HashKey)									(char*, int);
	void			(QDECL *Com_Printf)								(const char*, ...);
	void			(*Com_BeginRedirect)							(char*, int, void (*)(char*));
	void			(*Com_EndRedirect)								(void);
	char*			(*Cvar_VariableString)							(const char*);
	float			(*Cvar_VariableValue)							(const char*);
	fileHandle_t	(*FS_FOpenFileWrite)							(const char*);
	void			(*FS_ForceFlush)								(fileHandle_t);
	qboolean		(*FS_Initialized)								();
	int				(*FS_Write)										(const void*, int, fileHandle_t);
	int				(*FS_FileIsInPAK)								(const char*, int*);
	const char*		(*FS_LoadedPakPureChecksums)					(void);
	const char*		(*FS_ReferencedPakNames)						(void);
	int				(*FS_SV_FOpenFileRead)							(const char*, fileHandle_t*);
	int				(*FS_Read)										(void*, int, fileHandle_t);
	void			(*FS_FCloseFile)								(fileHandle_t);
	void			(*Netchan_TransmitNextFragment)					(netchan_t*);
	const char*		(*NET_AdrToString)								(netadr_t);
	void			(QDECL* NET_OutOfBandPrint)						(netsrc_t, netadr_t, const char*, ...);
	qboolean		(*NET_CompareBaseAdr)							(netadr_t, netadr_t);
	void			(*MSG_Init)										(msg_t*, byte*, int);
	int				(*MSG_ReadByte)									(msg_t*);
	void			(*MSG_ReadDeltaUsercmdKey)						(msg_t*, int, usercmd_t*, usercmd_t*);
	int				(*MSG_ReadShort)								(msg_t*);
	int				(*MSG_ReadLong)									(msg_t*);
	char*			(*MSG_ReadString)								(msg_t*);
	char*			(*MSG_ReadStringLine)							(msg_t*);
	void			(*MSG_Bitstream)								(msg_t*);
	void			(*MSG_BeginReadingOOB)							(msg_t*);
	void			(*MSG_WriteBigString)							(msg_t*, const char*);
	void			(*MSG_WriteByte)								(msg_t*, int);
	void			(*MSG_WriteDeltaEntity)							(msg_t*, struct entityState_s*, struct entityState_s*, qboolean);
	void			(*MSG_WriteLong)								(msg_t*, int);
	void			(*MSG_WriteShort)								(msg_t*, int);
	void			(*MSG_WriteString)								(msg_t*, const char*);
	void			(*MSG_WriteData)								(msg_t*, const void*, int);
	qboolean		(*Sys_IsLANAddress)								(netadr_t);
	void			(*Sys_Print)									(const char*);
	char*			(*Cmd_Argv)                                     (int);
	void			(*Cmd_ExecuteString)							(const char*);
	void			(*Cmd_TokenizeString)							(const char*);
	void			(*Huff_Decompress)								(msg_t*, int);
	void			(*Z_Free)										(void*);
	void*			(*Z_Malloc)										(int, memtag_t, qboolean, int);
};

struct Common_t
{
	vars_t 		vars;
	Cvars_t 	cvars;
	Functions_t	functions;
};

struct ProxyServer_t
{
	serverStatic_t*		svs;
	server_t*			sv;
	serverFunctions_t	functions;
	serverCvars_t		cvars;
	Common_t			common;
};

// ==================================================
// FUNCTION
// ==================================================

void Proxy_Engine_Initialize_MemoryLayer(void);

// ==================================================
// EXTERN VARIABLE
// ==================================================
extern ProxyServer_t server;



// --------------------------------
// jampgame functions addresses to call
// --------------------------------
constexpr intptr_t jampgame_func_Com_Error_addr = 0x000868a4;
constexpr intptr_t jampgame_func_Com_Printf_addr = 0x000868e4;
constexpr intptr_t jampgame_func_Com_sprintf_addr = 0x001a5524;
constexpr intptr_t jampgame_func_Info_SetValueForKey_addr = 0x001a5b54;
constexpr intptr_t jampgame_func_Info_ValueForKey_addr = 0x001a5604;
constexpr intptr_t jampgame_func_Q_strcat_addr = 0x001a53e4;
constexpr intptr_t jampgame_func_Q_stricmp_addr = 0x001a5184;
constexpr intptr_t jampgame_func_Q_stricmpn_addr = 0x001a5184;
constexpr intptr_t jampgame_func_Q_strncmp_addr = 0x001a5284;
constexpr intptr_t jampgame_func_Q_strncpyz_addr = 0x001a50f4;
constexpr intptr_t jampgame_func_va_addr = 0x001a55b4;

constexpr intptr_t jampgame_func_G_Damage_addr = 0x00138554;
constexpr intptr_t jampgame_func_BeginIntermission_addr = 0x00087d14;
constexpr intptr_t jampgame_func_player_die_addr = 0x00133b44;

// --------------------------------
// jampgame variables addresses
// --------------------------------
constexpr intptr_t jampgame_var_G_WeaponLogDamage_addr = 0x0099b640;

struct JampgameFunctions_t
{
	void			(*Com_Error)												(int, const char *, ...);
	void			(*Com_Printf)												( const char *, ... );
	void			(*Com_sprintf) 											(char *, int, const char *, ...);
	void			(*Info_SetValueForKey)							(char *, const char *, const char *);
	char*			(*Info_ValueForKey)									(const char *, const char *);
	void			(*Q_strcat)													(char *, int, const char *);
	int				(*Q_stricmp)												(const char *, const char *);
	int				(*Q_stricmpn)												(const char *, const char *, int );
	int				(*Q_strncmp) 												(const char *, const char *, int);
	void 			(*Q_strncpyz)												(char *, const char *, int);
	char*  		(*va)																(const char *, ...);
};

struct JampgameVariables_t
{
	int*	G_WeaponLogDamage; // int G_WeaponLogDamage[MAX_CLIENTS][MOD_MAX]
};

struct ProxyJampgame_t
{
	JampgameFunctions_t	functions;
	JampgameVariables_t variables;
};

extern ProxyJampgame_t jampgame;