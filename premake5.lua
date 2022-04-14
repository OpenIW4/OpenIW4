require "lua"

workspace "OpenIW4"
	location "./build/"

	targetdir "%{wks.location}/bin/%{cfg.buildcfg}/"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}/%{prj.name}/"
	buildlog "%{wks.location}/obj/%{cfg.buildcfg}/%{prj.name}.log"

	largeaddressaware "on"
	editandcontinue "off"
	staticruntime "on"

	systemversion "latest"
	characterset "mbcs"
	architecture "x86"
	warnings "extra"

	includedirs {
		"./src/",
	}

	buildoptions {
		"/Zm200",
		"/utf-8",
		"/std:c++17",
		"/bigobj",
	}

	flags {
		"noincrementallink",
		"no64bitchecks",
		"shadowedvariables",
		"undefinedidentifiers",
		"multiprocessorcompile",
	}

	defines {
		"NOMINMAX",
		"WIN32_LEAN_AND_MEAN",
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
	}

	platforms {
		"x86",
	}

	configurations {
		"Release",
		"Debug",
	}

	filter "Release"
		defines "NDEBUG"
		optimize "full"
		runtime "release"
		symbols "off"
	filter ""

	filter "Debug"
		defines "DEBUG"
		optimize "debug"
		runtime "debug"
		symbols "on"
	filter ""

	group "Main"

	project "OpenIW4"
		targetname "OpenIW4"

		language "c++"
		kind "windowedapp"
		warnings "off"
		
		pchheader "stdafx.hpp"
		pchsource "src/game/stdafx.cpp"
		forceincludes "stdafx.hpp"
		
		links {
			"Winmm",
		}

		files {
			"./src/**",
		}

		includedirs {
			"./src/",
		}

		linkoptions {
			"/NXCOMPAT:NO",
			"/IGNORE:4254",
			"/DYNAMICBASE:NO",
			"/SAFESEH:NO",
			"/LARGEADDRESSAWARE",
			"/LAST:.main",
		}