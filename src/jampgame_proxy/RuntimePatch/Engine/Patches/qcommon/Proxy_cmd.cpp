#include "jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_Wrappers.hpp"
#include "Proxy_cmd.hpp"

static char cmd_cmd[BIG_INFO_STRING]; // the original command we received (no token processing)

/*
============
Cmd_TokenizeString

Parses the given string into command line tokens.
The text is copied to a seperate buffer and 0 characters
are inserted in the appropriate place, The argv array
will point into this temporary buffer.
============
*/
// NOTE TTimo define that to track tokenization issues
//#define TKN_DBG
static void Cmd_TokenizeString2(const char* text_in, qboolean ignoreQuotes) {
	const char* text;
	char* textOut;

#ifdef TKN_DBG
	// FIXME TTimo blunt hook to try to find the tokenization of userinfo
	server.common.functions.Com_DPrintf("Cmd_TokenizeString: %s\n", text_in);
#endif

	// clear previous args
	*server.common.vars.cmd_argc = 0;

	if (!text_in) {
		return;
	}

	jampgame.functions.Q_strncpyz(cmd_cmd, text_in, sizeof(cmd_cmd));

	text = text_in;
	textOut = *server.common.vars.cmd_tokenized;

	while (1) {
		if (*server.common.vars.cmd_argc == MAX_STRING_TOKENS) {
			return;			// this is usually something malicious
		}

		while (1) {
			// skip whitespace
			while (*text && *(const unsigned char* /*eurofix*/)text <= ' ') {
				text++;
			}
			if (!*text) {
				return;			// all tokens parsed
			}

			// skip // comments
			if (text[0] == '/' && text[1] == '/') {
				return;			// all tokens parsed
			}

			// skip /* */ comments
			if (text[0] == '/' && text[1] == '*') {
				while (*text && (text[0] != '*' || text[1] != '/')) {
					text++;
				}
				if (!*text) {
					return;		// all tokens parsed
				}
				text += 2;
			}
			else {
				break;			// we are ready to parse a token
			}
		}

		// handle quoted strings
	// NOTE TTimo this doesn't handle \" escaping
		if (!ignoreQuotes && *text == '"') {
			(* server.common.vars.cmd_argv)[* server.common.vars.cmd_argc] = textOut;
			(* server.common.vars.cmd_argc)++;
			text++;
			while (*text && *text != '"') {
				*textOut++ = *text++;
			}
			*textOut++ = 0;
			if (!*text) {
				return;		// all tokens parsed
			}
			text++;
			continue;
		}

		// regular token
		(* server.common.vars.cmd_argv)[* server.common.vars.cmd_argc] = textOut;
		(* server.common.vars.cmd_argc)++;

		// skip until whitespace, quote, or command
		while (*(const unsigned char* /*eurofix*/)text > ' ') {
			if (!ignoreQuotes && text[0] == '"') {
				break;
			}

			if (text[0] == '/' && text[1] == '/') {
				break;
			}

			// skip /* */ comments
			if (text[0] == '/' && text[1] == '*') {
				break;
			}

			*textOut++ = *text++;
		}

		*textOut++ = 0;

		if (!*text) {
			return;		// all tokens parsed
		}
	}
}

void (*Original_Cmd_TokenizeString)(const char* text_in);
void Proxy_Cmd_TokenizeString(const char* text_in) {
	Cmd_TokenizeString2(text_in, qfalse);
}

/*
============
Cmd_TokenizeStringIgnoreQuotes
============
*/
void Proxy_Cmd_TokenizeStringIgnoreQuotes(const char* text_in) {
	Cmd_TokenizeString2(text_in, qtrue);
}

/*
============
Cmd_ArgsFrom

Returns a single string containing argv(arg) to argv(argc()-1)

/!\ not used in the original engine, it is safe to use it without a trampoline
============
*/
char* Proxy_Cmd_ArgsFrom(int arg) {
	static	char	cmd_args[BIG_INFO_STRING];
	int		i;

	cmd_args[0] = '\0';
	if (arg < 0)
		arg = 0;
	for (i = arg; i < *server.common.vars.cmd_argc; i++) {
		jampgame.functions.Q_strcat(cmd_args, sizeof(cmd_args), (*server.common.vars.cmd_argv)[i]);
		if (i != (*server.common.vars.cmd_argc) - 1) {
			jampgame.functions.Q_strcat(cmd_args, sizeof(cmd_args), " ");
		}
	}

	return cmd_args;
}

/*
============
Cmd_Cmd

Retrieve the unmodified command string
For rcon use when you want to transmit without altering quoting
https://zerowing.idsoftware.com/bugzilla/show_bug.cgi?id=543
============
*/
char* Proxy_Cmd_Cmd(void)
{
	return cmd_cmd;
}

/*
============
Cmd_Update_Argv
============
*/
void Cmd_Update_Argv(int argNumber, char* newArg)
{
	if ((unsigned)argNumber >= (unsigned)*server.common.vars.cmd_argc)
	{
		return;
	}

	if (!newArg)
	{
		return;
	}

	(*server.common.vars.cmd_argv)[argNumber] = newArg;
}
