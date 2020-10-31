-- Copyright 2020 SparkyPotato

project "Spark"
	location "../Intermediate/ProjectFiles"

	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
	targetdir "../Binaries/%{cfg.buildcfg}/Spark/"
	objdir "../Intermediate/Build/%{cfg.buildcfg}/Spark/"
	debugdir "../Binaries/%{cfg.buildcfg}/Spark/"
	
	dependson "SparkBuild"
	
	kind "Makefile"
	
	includedirs
	{
		"Source"
	}
	
	files
	{
		"Source/**.h",
		"Source/**.cpp"
	}

	buildcommands
	{
		"..\\..\\Engine\\Tools\\SparkBuild\\Build.bat Engine\\Source -be"
	}
	
	rebuildcommands
	{
		"..\\..\\Engine\\Tools\\SparkBuild\\Rebuild.bat Engine\\Source -be"
	}
