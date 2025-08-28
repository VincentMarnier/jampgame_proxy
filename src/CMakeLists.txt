#============================================================================
# Copyright (C) 2013 - 2018, OpenJK contributors
#
# This file is part of the OpenJK source code.
#
# OpenJK is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.
#============================================================================

# Make sure the user is not executing this script directly
if(NOT InJampgameProxy)
	message(FATAL_ERROR "Use the top-level cmake script!")
endif(NOT InJampgameProxy)

set(MPSharedDefines ${SharedDefines})

set(JampgameProxyIncludeDirectories "${JampgameProxyDir}")

if(WIN32)
	set(JampgameProxyLibraries "winmm")
endif(WIN32)

#============================================================================
# src/JampgameProxy
#============================================================================

set(JampgameProxyDefines ${MPSharedDefines} "_GAME" )
set(JampgameProxyMainFiles
	"${JampgameProxyDir}/jampgame_proxy/Proxy_ClientCommand.cpp"
	"${JampgameProxyDir}/jampgame_proxy/Proxy_ClientCommand.hpp"
	"${JampgameProxyDir}/jampgame_proxy/Proxy_CVars.cpp"
	"${JampgameProxyDir}/jampgame_proxy/Proxy_CVars.hpp"
	"${JampgameProxyDir}/jampgame_proxy/Proxy_Files.cpp"
	"${JampgameProxyDir}/jampgame_proxy/Proxy_Files.hpp"
	"${JampgameProxyDir}/jampgame_proxy/Proxy_Header.hpp"
	"${JampgameProxyDir}/jampgame_proxy/Proxy_Main.cpp"
	"${JampgameProxyDir}/jampgame_proxy/Proxy_Main.hpp"
	"${JampgameProxyDir}/jampgame_proxy/Proxy_SharedAPI.cpp"
	"${JampgameProxyDir}/jampgame_proxy/Proxy_SharedAPI.hpp"
	"${JampgameProxyDir}/jampgame_proxy/Proxy_Shell.hpp"
	"${JampgameProxyDir}/jampgame_proxy/Proxy_Translate_SystemCalls.cpp"
	"${JampgameProxyDir}/jampgame_proxy/Proxy_Translate_SystemCalls.hpp"
	)
source_group("src/JampgameProxy" FILES ${JampgameProxyMainFiles})
set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxyMainFiles})

#============================================================================
# src/jampgame_proxy/Imports/game
#============================================================================

set(JampgameProxyImportsGameFiles
	"${JampgameProxyDir}/jampgame_proxy/Imports/game/g_cmds.cpp"
	"${JampgameProxyDir}/jampgame_proxy/Imports/game/g_cmds.hpp"
	)
source_group("src/jampgame_proxy/Imports/game" FILES ${JampgameProxyImportsGameFiles})
set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxyImportsGameFiles})

#============================================================================
# src/jampgame_proxy/Imports/qcommon
#============================================================================

set(JampgameProxyImportsQcommonFiles
	"${JampgameProxyDir}/jampgame_proxy/Imports/qcommon/cvar.cpp"
	"${JampgameProxyDir}/jampgame_proxy/Imports/qcommon/cvar.hpp"
	)
source_group("src/jampgame_proxy/Imports/qcommon" FILES ${JampgameProxyImportsQcommonFiles})
set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxyImportsQcommonFiles})

#============================================================================
# src/jampgame_proxy/Imports/server
#============================================================================

set(JampgameProxyImportsServerFiles
	"${JampgameProxyDir}/jampgame_proxy/Imports/server/sv_game.cpp"
	"${JampgameProxyDir}/jampgame_proxy/Imports/server/sv_game.hpp"
	)
source_group("src/jampgame_proxy/Imports/server" FILES ${JampgameProxyImportsServerFiles})
set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxyImportsServerFiles})

#============================================================================
# src/jampgame_proxy/RuntimePatch/Engine
#============================================================================

set(JampgameProxyRuntimePatchEngineFiles
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_Client.cpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_Client.hpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_ClientCommand.cpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_ClientCommand.hpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_Patch.cpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_Patch.hpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_Utils.hpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_Wrappers.cpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Proxy_Engine_Wrappers.hpp"
	)
source_group("src/jampgame_proxy/RuntimePatch/Engine" FILES ${JampgameProxyRuntimePatchEngineFiles})
set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxyRuntimePatchEngineFiles})

#============================================================================
# src/jampgame_proxy/RuntimePatch/Engine/Patches/qcommon
#============================================================================

set(JampgameProxyRuntimePatchEnginePatchesQcommonFiles
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/qcommon/Proxy_cmd.cpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/qcommon/Proxy_cmd.hpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/qcommon/Proxy_common.cpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/qcommon/Proxy_common.hpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/qcommon/Proxy_files.cpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/qcommon/Proxy_files.hpp"
	)
source_group("src/jampgame_proxy/RuntimePatch/Engine/Patches/qcommon" FILES ${JampgameProxyRuntimePatchEnginePatchesQcommonFiles})
set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxyRuntimePatchEnginePatchesQcommonFiles})

#============================================================================
# src/jampgame_proxy/RuntimePatch/Engine/Patches/server
#============================================================================

set(JampgameProxyRuntimePatchEnginePatchesServerFiles
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/server/Proxy_sv_ccmds.cpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/server/Proxy_sv_ccmds.hpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/server/Proxy_sv_client.cpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/server/Proxy_sv_client.hpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/server/Proxy_sv_game.cpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/server/Proxy_sv_game.hpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/server/Proxy_sv_main.cpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/server/Proxy_sv_main.hpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/server/Proxy_sv_snapshot.cpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/server/Proxy_sv_snapshot.hpp"
	)
source_group("src/jampgame_proxy/RuntimePatch/Engine/Patches/server" FILES ${JampgameProxyRuntimePatchEnginePatchesServerFiles})
set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxyRuntimePatchEnginePatchesServerFiles})

#============================================================================
# src/jampgame_proxy/RuntimePatch/HookUtils
#============================================================================

set(JampgameProxyHookUtilsFiles
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/HookUtils/HookUtils.cpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/HookUtils/HookUtils.hpp"
	)
source_group("src/jampgame_proxy/RuntimePatch/HookUtils" FILES ${JampgameProxyHookUtilsFiles})
set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxyHookUtilsFiles})

#============================================================================
# src/jampgame_proxy/RuntimePatch/HookUtils/LengthDisassembler/
#============================================================================

set(JampgameProxyLengthDisassemblerFiles
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/HookUtils/LengthDisassembler/hde.hpp"
	)
source_group("src/jampgame_proxy/RuntimePatch/HookUtils/LengthDisassembler" FILES ${JampgameProxyLengthDisassemblerFiles})
set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxyLengthDisassemblerFiles})

if (CMAKE_SIZEOF_VOID_P EQUAL 4 OR "${TARGET_ARCH}" MATCHES "x86")
	#============================================================================
	# src/jampgame_proxy/RuntimePatch/HookUtils/LengthDisassembler/x86
	#============================================================================

	set(JampgameProxyLengthDisassemblerX86Files
		"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/HookUtils/LengthDisassembler/x86/hde32.cpp"
		"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/HookUtils/LengthDisassembler/x86/hde32.hpp"
		"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/HookUtils/LengthDisassembler/x86/table32.hpp"
		)
	source_group("src/jampgame_proxy/RuntimePatch/HookUtils/LengthDisassembler/x86" FILES ${JampgameProxyLengthDisassemblerX86Files})
	set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxyLengthDisassemblerX86Files})
else ()
	#============================================================================
	# src/jampgame_proxy/RuntimePatch/HookUtils/LengthDisassembler/x64
	#============================================================================

	set(JampgameProxyLengthDisassemblerX64Files
		"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/HookUtils/LengthDisassembler/x64/hde64.cpp"
		"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/HookUtils/LengthDisassembler/x64/hde64.hpp"
		"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/HookUtils/LengthDisassembler/x64/table64.hpp"
		)
	source_group("src/jampgame_proxy/RuntimePatch/HookUtils/LengthDisassembler/x64" FILES ${JampgameProxyLengthDisassemblerX64Files})
	set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxyLengthDisassemblerX64Files})
endif ()

#============================================================================
# src/jampgame_proxy/Wrappers
#============================================================================

set(JampgameProxyWrapperFiles
	"${JampgameProxyDir}/jampgame_proxy/Wrappers/Proxy_OpenJKAPI_Wrappers.cpp"
	"${JampgameProxyDir}/jampgame_proxy/Wrappers/Proxy_OpenJKAPI_Wrappers.hpp"
	"${JampgameProxyDir}/jampgame_proxy/Wrappers/Proxy_OriginalAPI_Wrappers.cpp"
	"${JampgameProxyDir}/jampgame_proxy/Wrappers/Proxy_OriginalAPI_Wrappers.hpp"
	)
source_group("src/jampgame_proxy/Wrapper" FILES ${JampgameProxyWrapperFiles})
set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxyWrapperFiles})

#============================================================================
# src/sdk/server
#============================================================================

set(JampgameProxyServerFiles
	"${JampgameProxyDir}/sdk/server/server.hpp"
	"${JampgameProxyDir}/sdk/server/sv_client.cpp"
	"${JampgameProxyDir}/sdk/server/sv_client.hpp"
	)
source_group("src/sdk/server" FILES ${JampgameProxyServerFiles})
set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxyServerFiles})

#============================================================================
# src/sdk/sys
#============================================================================

set(JampgameProxySysFiles
	"${JampgameProxyDir}/sdk/sys/sys_public.hpp"
	)
source_group("src/sdk/sys" FILES ${JampgameProxySysFiles})
set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxySysFiles})

#============================================================================
# src/sdk/qcommon
#============================================================================

set(JampgameProxyDSKCommonFiles
	"${JampgameProxyDir}/sdk/qcommon/disablewarnings.hpp"
	"${JampgameProxyDir}/sdk/qcommon/files.cpp"
	"${JampgameProxyDir}/sdk/qcommon/game_version.hpp"
	"${JampgameProxyDir}/sdk/qcommon/q_color.hpp"
	"${JampgameProxyDir}/sdk/qcommon/q_math.hpp"
	"${JampgameProxyDir}/sdk/qcommon/q_platform.hpp"
	"${JampgameProxyDir}/sdk/qcommon/q_shared.cpp"
	"${JampgameProxyDir}/sdk/qcommon/q_shared.hpp"
	"${JampgameProxyDir}/sdk/qcommon/q_string.cpp"
	"${JampgameProxyDir}/sdk/qcommon/q_string.hpp"
	"${JampgameProxyDir}/sdk/qcommon/qcommon.hpp"
	"${JampgameProxyDir}/sdk/qcommon/tags.hpp"
	)
source_group("src/sdk/qcommon" FILES ${JampgameProxyDSKCommonFiles})
set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxyDSKCommonFiles})

#============================================================================
# src/sdk/game
#============================================================================

set(JampgameProxySDKGameFiles
	"${JampgameProxyDir}/sdk/game/ai.hpp"
	"${JampgameProxyDir}/sdk/game/anims.hpp"
	"${JampgameProxyDir}/sdk/game/b_public.hpp"
	"${JampgameProxyDir}/sdk/game/bg_public.hpp"
	"${JampgameProxyDir}/sdk/game/bg_vehicles.hpp"
	"${JampgameProxyDir}/sdk/game/bg_weapons.hpp"
	"${JampgameProxyDir}/sdk/game/g_local.hpp"
	"${JampgameProxyDir}/sdk/game/g_public.hpp"
	"${JampgameProxyDir}/sdk/game/g_team.hpp"
	"${JampgameProxyDir}/sdk/game/g_xcvar.hpp"
	"${JampgameProxyDir}/sdk/game/surfaceflags.hpp"
	"${JampgameProxyDir}/sdk/game/teams.hpp"
	)
source_group("src/sdk/game" FILES ${JampgameProxySDKGameFiles})
set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxySDKGameFiles})

add_library(${JampgameProxy} SHARED ${JampgameProxyFiles})

if(NOT MSVC)
	# remove "lib" prefix for .so/.dylib files
	set_target_properties(${JampgameProxy} PROPERTIES PREFIX "")
endif()
set_target_properties(${JampgameProxy} PROPERTIES COMPILE_DEFINITIONS "${JampgameProxyDefines}")

# Hide symbols not explicitly marked public.
set_property(TARGET ${JampgameProxy} APPEND PROPERTY COMPILE_OPTIONS ${JampgameProxy_VISIBILITY_FLAGS})

set_target_properties(${JampgameProxy} PROPERTIES INCLUDE_DIRECTORIES "${JampgameProxyIncludeDirectories}")
set_target_properties(${JampgameProxy} PROPERTIES PROJECT_LABEL "JampgameProxy Library")
# no libraries used
if(JampgameProxyLibraries)
	target_link_libraries(${JampgameProxy} ${JampgameProxyLibraries})
endif(JampgameProxyLibraries)

set(JampgameProxyLibsBuilt)
if(BuildJampgameProxy)
	set(JampgameProxyLibsBuilt ${JampgameProxyLibsBuilt} ${JampgameProxy})
endif()

if(WIN32)
	set(JampgameProxyLibFullPaths)
	if(MSVC)
		foreach(JampgameProxyLib ${JampgameProxyLibsBuilt})
			set(JampgameProxyLibFullPaths
				${JampgameProxyLibFullPaths}
				${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${JampgameProxyLib}${CMAKE_SHARED_LIBRARY_SUFFIX})
		endforeach(JampgameProxyLib)
	else()
		foreach(JampgameProxyLib ${JampgameProxyLibsBuilt})
			set(JampgameProxyLibFullPaths
				${JampgameProxyLibFullPaths}
				${CMAKE_BINARY_DIR}/${JampgameProxyLib}${CMAKE_SHARED_LIBRARY_SUFFIX})
		endforeach(JampgameProxyLib)
	endif()
endif()
