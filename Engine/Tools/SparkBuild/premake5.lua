-- Copyright 2020 SparkyPotato

project "SparkBuild"
	location "../../../Intermediate/ProjectFiles"

	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	runtime "Release"
	
	targetdir "../../../Binaries/SparkBuild/%{cfg.buildcfg}/"
	objdir "../../../Intermediate/SparkBuild/%{cfg.buildcfg}/"
	debugdir "../../../Binaries/SparkBuild/%{cfg.buildcfg}/"
	
	kind "ConsoleApp"
	
	characterset "MBCS"

	filter "configurations: Debug"
		defines "IS_DEBUG"
		optimize "off"
		symbols "on"
		
	filter "configurations: Development"
		defines "IS_RELEASE"
		optimize "speed"
		symbols "on"
		
	filter "configurations: Release"
		defines "IS_RELEASE"
		optimize "speed"
		symbols "off"
		
	filter {}
	
	buildoptions
	{
		"/utf-8"
	}
	
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
