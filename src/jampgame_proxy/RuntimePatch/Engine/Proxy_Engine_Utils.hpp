#pragma once

#include "jampgame_proxy/Proxy_Header.hpp"
#include "Proxy_Engine_Wrappers.hpp"
#include <sdk/server/server.h>
#include "jampgame_proxy/Proxy_Utils.hpp"

#include <cstddef>

inline constexpr size_t getClientNumFromAddr(client_t* client)
{
	return client - (*(client_t**)var_svsClients_addr);
}

/*
===========
SV_ClientCleanName

Gamecode to engine port (from OpenJK)
============
*/
inline constexpr void Proxy_ClientCleanName(const char* in, char* out, int outSize)
{
	int outpos = 0;
	int colorlessLen = 0;

	// discard leading spaces
	for (; *in == ' '; in++)
	{

	}

	// discard leading asterisk's (fail raven for using * as a skipnotify)
	// apparently .* causes the issue too so... derp
	for (; *in == '*'; in++)
	{

	}

	for (; *in && outpos < outSize - 1; in++)
	{
		out[outpos] = *in;

		if (*(in + 1) && *(in + 2))
		{
			if (*in == ' ' && *(in + 1) == ' ' && *(in + 2) == ' ') // don't allow more than 3 consecutive spaces
			{
				continue;
			}

			if (*in == '@' && *(in + 1) == '@' && *(in + 2) == '@') // don't allow too many consecutive @ signs
			{
				continue;
			}
		}

		if ((byte)*in < 0x20)
		{
			continue;
		}

		switch ((byte)* in)
		{
		default:
			break;
		case 0x81:
		case 0x8D:
		case 0x8F:
		case 0x90:
		case 0x9D:
		case 0xA0:
		case 0xAD:
			continue;
			break;
		}

		if (outpos > 0 && out[outpos - 1] == Q_COLOR_ESCAPE)
		{
			if (Q_IsColorStringExt(&out[outpos - 1]))
			{
				colorlessLen--;
			}
			else
			{
				//spaces = ats = 0;
				colorlessLen++;
			}
		}
		else
		{
			//spaces = ats = 0;
			colorlessLen++;
		}
		outpos++;
	}

	out[outpos] = '\0';

	// don't allow empty names
	if (*out == '\0' || colorlessLen == 0)
	{
		jampgame.functions.Q_strncpyz(out, "Padawan", outSize);
	}
}