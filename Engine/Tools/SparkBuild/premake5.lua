-- Copyright 2020 SparkyPotato

project "SparkBuild"
	location "../../../Intermediate/ProjectFiles"

	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	runtime "Release"
	
	targetdir "../../Binaries/SparkBuild/"
	objdir "../../Intermediate/SparkBuild/"
	debugdir "../../Binaries/SparkBuild/"
	
	kind "ConsoleApp"
	
	characterset "Unicode"
		
	optimize "speed"
	symbols "on"
		
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
