-- Copyright 2020 SparkyPotato

project "Spark"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
	characterset "Unicode"
	
	targetdir "../Binaries/Spark/%{cfg.buildcfg}"
	objdir "../Intermediate/Spark/%{cfg.buildcfg}"
	debugdir "../Binaries/Spark/%{cfg.buildcfg}"
	
	pchheader "SparkPCH.h"
	pchsource "PCH/SparkPCH.cpp"
 
    filter "system:macosx"
        pchheader "PCH/SparkPCH.h"
	
	filter "system:windows"
		systemversion "latest"
		entrypoint "wWinMainCRTStartup"
		
	filter "configurations:*Editor*"
		kind "WindowedApp"
		defines "IS_EDITOR"
		
	filter "configurations:*Lib*"
		kind "StaticLib"
		defines "IS_LIB"
		
	filter "configurations:*Release*"
		optimize "speed"
		symbols "off"
		runtime "Release"
		defines "IS_RELEASE"
		
	filter "configurations:*Development*"
		optimize "speed"
		symbols "off"
		runtime "Release"
		defines "IS_DEBUG"
		
	filter "configurations:*Debug*"
		optimize "off"
		symbols "on"
		runtime "Debug"
		defines "IS_DEBUG"
		
	filter {}
	
	includedirs
	{
		"Source",
		"PCH",
	}
	
	files
	{
		"Source/**.h",
		"Source/**.cpp",
		"PCH/**"
	}
	
	filter { "files:**.cpp", "files:not **.build.cpp", "files:not PCH/SparkPCH.cpp" }
		flags "ExcludeFromBuild"
		
	filter "action:vs*"
		files { "**.natvis" }
	
