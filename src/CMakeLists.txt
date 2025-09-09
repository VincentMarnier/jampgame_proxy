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
	"${JampgameProxyDir}/jampgame_proxy/Proxy_Translate_SystemCalls.hpp"
	"${JampgameProxyDir}/jampgame_proxy/Proxy_Translate_SystemCalls.cpp"
	"${JampgameProxyDir}/jampgame_proxy/Proxy_Utils.hpp"
	"${JampgameProxyDir}/jampgame_proxy/Proxy_Utils.cpp"
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
# src/jampgame_proxy/RuntimePatch/Engine/Patches/game
#============================================================================

set(JampgameProxyRuntimePatchEnginePatchesGameFiles
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/game/Proxy_g_cmds.cpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/game/Proxy_g_cmds.hpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/game/Proxy_g_combat.cpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/game/Proxy_g_combat.hpp"
	)
source_group("src/jampgame_proxy/RuntimePatch/Engine/Patches/game" FILES ${JampgameProxyRuntimePatchEnginePatchesGameFiles})
set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxyRuntimePatchEnginePatchesGameFiles})

#============================================================================
# src/jampgame_proxy/RuntimePatch/Engine/Patches/qcommon
#============================================================================

set(JampgameProxyRuntimePatchEnginePatchesQcommonFiles
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/qcommon/Proxy_cmd.cpp"
	"${JampgameProxyDir}/jampgame_proxy/RuntimePatch/Engine/Patches/qcommon/Proxy_cmd.hpp"
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
# src/jampgame_proxy/Wrappers
#============================================================================

set(JampgameProxyWrapperFiles
	"${JampgameProxyDir}/jampgame_proxy/Wrappers/Proxy_OriginalAPI_Wrappers.cpp"
	"${JampgameProxyDir}/jampgame_proxy/Wrappers/Proxy_OriginalAPI_Wrappers.hpp"
	)
source_group("src/jampgame_proxy/Wrapper" FILES ${JampgameProxyWrapperFiles})
set(JampgameProxyFiles ${JampgameProxyFiles} ${JampgameProxyWrapperFiles})

# Add Zydis
option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
add_subdirectory(third_party/zydis)

# Create library
add_library(${JampgameProxy} SHARED ${JampgameProxyFiles})

target_include_directories(${JampgameProxy} PRIVATE "${JampgameProxyDir}/sdk/")

# Add Zydis static lib
target_link_libraries(${JampgameProxy} PRIVATE "Zydis")

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
