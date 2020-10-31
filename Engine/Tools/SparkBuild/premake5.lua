-- Copyright 2020 SparkyPotato

project "SparkBuild"
	location "../../../Intermediate/ProjectFiles"

	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
	targetdir "../../../Binaries/SparkBuild/"
	objdir "../../../Intermediate/Build/SparkBuild/"
	debugdir "../../../Binaries/SparkBuild/"
	
	kind "ConsoleApp"
		
	optimize "speed"
	symbols "on"
		
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
