workspace "Eden"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	platforms
	{
		"linux",
		"windows"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["src"] = "Eden/src"
IncludeDir["spdlog"] = "Eden/vendor/spdlog/include"

project "Eden"
	location "Eden"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "edenpch.h"
	pchsource "Eden/src/edenpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.src}",
		"%{IncludeDir.spdlog}"
	}

	defines
	{
		"EDEN_BUILD_DLL"
	}

	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"

		defines
		{
			"EDEN_PLATFORM_LINUX"
		}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"EDEN_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "EDEN_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "EDEN_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "EDEN_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.src}",
		"%{IncludeDir.spdlog}"
	}

	links
	{
		"Eden"
	}

	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"

		defines
		{
			"EDEN_PLATFORM_LINUX"
		}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"EDEN_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "EDEN_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "EDEN_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "EDEN_DIST"
		optimize "On"
