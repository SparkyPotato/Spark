-- Copyright 2020 SparkyPotato

project "Spark"
	location "../Intermediate/ProjectFiles"

	language "C++"
	cppdialect "C++20"
	
	targetdir "Binaries/Build/%{cfg.buildcfg}/Spark/"
	objdir "Intermediate/Build/%{cfg.buildcfg}/Spark/"
	debugdir "Binaries/%{cfg.buildcfg}/Spark/"
	
	dependson "SparkBuild"
	
	kind "Makefile"
	
	includedirs
	{
		"Source",
		"Source/*/Public"
	}
	
	files
	{
		"Source/**.h",
		"Source/**.cpp"
	}

	buildcommands
	{
		"..\\..\\Engine\\Tools\\SparkBuild\\Build.bat Dir=\"Engine/\""
	}
	
	rebuildcommands
	{
		'..\\..\\Engine\\Tools\\SparkBuild\\Build.bat Dir=\"Engine/\" -Rebuild'
	}
		