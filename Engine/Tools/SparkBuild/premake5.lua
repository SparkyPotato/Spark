-- Copyright 2020 SparkyPotato

project "SparkBuild"
	location "../../../Intermediate/ProjectFiles"

	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
	targetdir "../../../Binaries/SparkBuild/%{cfg.buildcfg}"
	objdir "../../../Intermediate/SparkBuild/%{cfg.buildcfg}"
	debugdir "../../../Binaries/SparkBuild/%{cfg.buildcfg}"
	
	kind "ConsoleApp"
		
	filter "configurations:*Release*"
		optimize "speed"
		symbols "off"
		defines "IS_RELEASE"
		
	filter "configurations:*Development*"
		optimize "speed"
		symbols "off"
		defines "IS_DEBUG"
		
	filter "configurations:*Debug*"
		optimize "off"
		symbols "on"
		defines "IS_DEBUG"
		
	filter {}
	
	includedirs
	{
		"Source"
	}
	
	files
	{
		"Source/**.h",
		"Source/**.cpp"
	}
