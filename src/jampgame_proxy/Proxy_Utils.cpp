#pragma once

#include "Proxy_Utils.hpp"

qboolean Q_IsValidAsciiStr(const char* string) {
	const char* s;
	int	c;

	s = string;

	while ((c = *s) != 0) {
		if (c < 0x20 || c > 0x7E) {
			return qfalse;
		}
		s++;
	}

	return qtrue;
}

/*
Q_strchrs

Description:	Find any characters in a string. Think of it as a shorthand strchr loop.
Mutates:		--
Return:			first instance of any characters found
otherwise NULL
*/

const char* Q_strchrs(const char* string, const char* search)
{
	const char* p = string;
	const char *s;

	while (*p != '\0')
	{
		for (s = search; *s; s++)
		{
			if (*p == *s)
			{
				return p;
			}
		}

		p++;
	}

	return NULL;
}

void Q_StripColor(char* text)
{
	qboolean doPass = qtrue;
	char* read;
	char* write;

	while (doPass)
	{
		doPass = qfalse;
		read = write = text;
		while (*read)
		{
			if (Q_IsColorStringExt(read))
			{
				doPass = qtrue;
				read += 2;
			}
			else
			{
				// Avoid writing the same data over itself
				if (write != read)
				{
					*write = *read;
				}
				write++;
				read++;
			}
		}
		if (write < read)
		{
			// Add trailing NUL byte if string has shortened
			*write = '\0';
		}
	}
}

float calcRatio(int kill, int death)
{
	if (kill == 0 && death == 0)
	{
		return 1.00;
	}
	else if (kill < 1 && death >= 1)
	{
		return 0.00;
	}
	else if (kill >= 1 && death <= 1)
	{
		return (float)kill;
	}
	else
	{
		return (float)kill / (float)death;
	}
}
