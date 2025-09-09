#include "jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_Wrappers.hpp"
#include "Proxy_files.hpp"

const char* Proxy_FS_GetCurrentGameDir(bool emptybase)
{
	if (server.common.cvars.fs_gamedirvar->string[0])
		return server.common.cvars.fs_gamedirvar->string;

	return emptybase ? "" : "base";
}

/*
=============
Com_Printf

Both client and server can use this, and it will output
to the apropriate place.

A raw string should NEVER be passed as fmt, because of "%f" type crashers.
=============
*/

void (QDECL *Original_Com_Printf)(const char*, ...);
void QDECL Proxy_Com_Printf(const char* fmt, ...)
{
	static qboolean opening_qconsole = qfalse;
	va_list			argptr;
	char			msg[4096];

	va_start(argptr, fmt);
	// Proxy -------------->
	vsnprintf(msg, sizeof(msg), fmt, argptr);
	// Proxy <--------------
	va_end(argptr);

	if (*server.common.vars.rd_buffer)
	{
		if ((strlen(msg) + strlen(*server.common.vars.rd_buffer)) > (size_t)(*server.common.vars.rd_buffersize - 1))
		{
			(*server.common.vars.rd_flush)(*server.common.vars.rd_buffer);
			**server.common.vars.rd_buffer = 0;
		}

		jampgame.functions.Q_strcat(*server.common.vars.rd_buffer, *server.common.vars.rd_buffersize, msg);

		// TTimo nooo .. that would defeat the purpose
		//(*server.common.vars.rd_flush)(*server.common.vars.rd_buffer);
		//*server.common.vars.rd_buffer = 0;
		
		return;
	}

	// echo to dedicated console and early console
	server.common.functions.Sys_Print(msg);

	// logfile
	if (server.common.cvars.com_logfile && server.common.cvars.com_logfile->integer)
	{
		// TTimo: only open the qconsole.log if the filesystem is in an initialized state
		// also, avoid recursing in the qconsole.log opening (i.e. if fs_debug is on)
		
		// Proxy -------------->
		// ( !logfile && FS_Initialized() )
		// Proxy <--------------

		if (!*server.common.vars.logfile && server.common.functions.FS_Initialized() && !opening_qconsole)
		{
			struct tm* newtime;
			time_t aclock;

			opening_qconsole = qtrue;

			time(&aclock);
			newtime = localtime(&aclock);

			*server.common.vars.logfile = server.common.functions.FS_FOpenFileWrite("qconsole.log");

			// Proxy -------------->
			if (*server.common.vars.logfile)
			{
				server.common.functions.Com_Printf("logfile opened on %s\n", asctime(newtime));
				
				if (server.common.cvars.com_logfile->integer > 1)
				{
					// force it to not buffer so we get valid
					// data even if we are crashing
					server.common.functions.FS_ForceFlush(*server.common.vars.logfile);
				}
			}
			else
			{
				server.common.functions.Com_Printf("Opening qconsole.log failed!\n");
				
				trap_Cvar_Set("logfile", 0);
			}
		}
		// Proxy <--------------

		opening_qconsole = qfalse;

		if (*server.common.vars.logfile && server.common.functions.FS_Initialized())
		{
			server.common.functions.FS_Write(msg, (int)strlen(msg), *server.common.vars.logfile);
		}
	}

#if defined(_WIN32) && defined(_DEBUG)
	if (*msg)
	{
		OutputDebugString(Q_CleanStr(msg));
		OutputDebugString("\n");
	}
#endif
}