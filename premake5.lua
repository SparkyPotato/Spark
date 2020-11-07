-- Copyright 2020 SparkyPotato

workspace "Spark"
	architecture "x86_64"
	
	configurations
	{
		"Debug",
		"Development",
		"Release",
	}
	
	startproject "Spark"
	
group "Engine"	
project "Spark"
	location "Intermediate/ProjectFiles"

	language "C++"
	cppdialect "C++20"
	
	targetdir "Engine/Binaries/%{cfg.buildcfg}/Spark/"
	objdir "Engine/Intermediate/%{cfg.buildcfg}/Spark/"
	debugdir "Engine/Binaries/%{cfg.buildcfg}/Spark/"
	
	dependson "SparkBuild"
	
	kind "Makefile"
	
	includedirs
	{
		"Engine/Source",
		"Engine/**/Public"
	}
	
	files
	{
		"Engine/Source/**.h",
		"Engine/Source/**.cpp"
	}

	buildcommands
	{
		"..\\..\\Engine\\Tools\\SparkBuild\\Build.bat Dir=\"Engine/\""
	}
	
	rebuildcommands
	{
		'..\\..\\Engine\\Tools\\SparkBuild\\Build.bat Dir=\"Engine/\" -Rebuild'
	}

group "Tools"
project "SparkBuild"
	location "Intermediate/ProjectFiles"

	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	runtime "Release"
	
	targetdir "Engine/Binaries/SparkBuild/"
	objdir "Engine/Intermediate/SparkBuild/"
	debugdir "Engine/Binaries/SparkBuild/"
	
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
		"Engine/Tools/SparkBuild/Public",
		"Engine/Dependencies/json"
	}
	
	files
	{
		"Engine/Tools/SparkBuild/Public/**.h",
		"Engine/Tools/SparkBuild/Private/**.h",
		"Engine/Tools/SparkBuild/Private/**.cpp"
	}
