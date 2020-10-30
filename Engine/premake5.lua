-- Copyright 2020 SparkyPotato

project "Spark"
	location "../Intermediate/ProjectFiles"

	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
	targetdir "../Binaries/%{cfg.buildcfg}/Spark/"
	objdir "../Intermediate/Int/%{cfg.buildcfg}/Spark/"
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
		"..\\..\\Binaries\\%{cfg.buildcfg}\\SparkBuild\\SparkBuild.exe -BE Source=../../Engine"
	}
