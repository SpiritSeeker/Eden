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
IncludeDir["GLFW"] = "Eden/vendor/GLFW/include"
IncludeDir["Glad"] = "Eden/vendor/Glad/include"

include "Eden/vendor/GLFW"
include "Eden/vendor/Glad"

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

	excludes
	{
		"%{prj.name}/src/Platform/**.cpp",
		"%{prj.name}/src/Platform/**.h"
	}

	includedirs
	{
		"%{IncludeDir.src}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	defines
	{
		"EDEN_BUILD_DLL",
		"GLFW_INCLUDE_NONE"
	}

	links
	{
		"mpg123",
		"openal",
		"GLFW",
		"Glad"
	}

	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"

		defines
		{
			"EDEN_PLATFORM_LINUX"
		}

		files
		{
			"%{prj.name}/src/Platform/Linux/**.cpp",
			"%{prj.name}/src/Platform/Linux/**.h"
		}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"EDEN_PLATFORM_WINDOWS"
		}

		files
		{
			"%{prj.name}/src/Platform/Windows/**.cpp",
			"%{prj.name}/src/Platform/Windows/**.h"
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
