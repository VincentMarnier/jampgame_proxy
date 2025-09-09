#include <sdk/qcommon/qcommon.h>
#include "jampgame_proxy/Proxy_Header.hpp"
#include "g_cmds.hpp"

char* ConcatArgs(int start) {
	int		i;
	int		c;
	static char	line[MAX_STRING_CHARS];
	size_t	len;
	char	arg[MAX_STRING_CHARS];

	len = 0;
	c = trap_Argc();

	for (i = start; i < c; i++)
	{
		trap_Argv(i, arg, sizeof(arg));
		size_t tlen = strlen(arg);

		if (len + tlen >= MAX_STRING_CHARS - 1)
		{
			break;
		}

		memcpy(line + len, arg, tlen);
		len += tlen;

		if (i != c - 1)
		{
			line[len] = ' ';
			len++;
		}
	}

	line[len] = 0;

	return line;
}