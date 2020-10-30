-- Copyright 2020 SparkyPotato

project "SparkBuild"
	location "../../../Intermediate/ProjectFiles"

	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
	targetdir "../../../Binaries/%{cfg.buildcfg}/SparkBuild/"
	objdir "../../../Intermediate/%{cfg.buildcfg}/SparkBuild/"
	debugdir "../../../Binaries/%{cfg.buildcfg}/SparkBuild/"
	
	kind "ConsoleApp"
		
	filter "configurations:*Release*"
		optimize "speed"
		symbols "off"
		defines "IS_RELEASE"
		
	filter "configurations:*Development*"
		optimize "speed"
		symbols "on"
		defines "IS_DEBUG"
		
	filter "configurations:*Debug*"
		optimize "off"
		symbols "on"
		defines "IS_DEBUG"
		
	filter {}
	
	includedirs
	{
		"Source",
		"../../Dependencies/json"
	}
	
	files
	{
		"Source/**.h",
		"Source/**.cpp"
	}
