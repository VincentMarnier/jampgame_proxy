#include "Proxy_Header.hpp"
#include "Proxy_Files.hpp"

void Proxy_LoadGameLibrary(void)
{
	// Todo: Check for fs_basegame
	char fs_gameBuffer[MAX_OSPATH];
	std::size_t pathLength = 0;
	std::size_t tmpLength = 0;

	fs_gameBuffer[0] = 0;

	proxy.trap->Cvar_VariableStringBuffer(FS_GAME_CVAR, fs_gameBuffer, sizeof(fs_gameBuffer));

	pathLength = std::strlen(fs_gameBuffer);

	// length 0 means that fs_game is set to the default value which is the default base game folder name
	if (pathLength == 0)
	{
		pathLength += std::strlen(DEFAULT_BASE_GAME_FOLDER_NAME);
		std::memcpy(fs_gameBuffer, DEFAULT_BASE_GAME_FOLDER_NAME, pathLength);
	}

	tmpLength = std::strlen(PROXY_LIBRARY);
	std::memcpy(fs_gameBuffer + pathLength, PROXY_LIBRARY, tmpLength);
	pathLength += tmpLength;

	fs_gameBuffer[pathLength] = '\0';

	proxy.jampgameHandle = JampgameProxy_OpenLibrary(fs_gameBuffer);

	if (proxy.jampgameHandle == NULL)
	{
		proxy.trap->Print("================================================================\n");
		proxy.trap->Print("----- Proxy: Failed to open %s, exiting\n", PROXY_LIBRARY_NAME PROXY_LIBRARY_DOT PROXY_LIBRARY_EXT);
		proxy.trap->Print("================================================================\n");

		exit(EXIT_FAILURE);
	}
}