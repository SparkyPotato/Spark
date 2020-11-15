-- Copyright 2020 SparkyPotato

workspace "Spark"
	architecture "x86_64"
	
	configurations
	{
		"Debug",
		"Development",
		"Release"
	}
	
	startproject "Spark"
	
group "Engine"	
project "Spark"
	location "Intermediate"

	language "C++"
	cppdialect "C++17"
	
	targetdir "Binaries/%{cfg.buildcfg}/Spark/"
	objdir "Intermediate/%{cfg.buildcfg}/Spark/"
	
	dependson "SparkBuild"
	
	kind "Makefile"
	
	includedirs
	{
		"Engine/**/Public"
	}
	
	files
	{
		"Engine/Source/**.h",
		"Engine/Source/**.cpp"
	}

	buildcommands
	{
		"..\\Engine\\Tools\\SparkBuild\\Build.bat Dir=Engine Config=\"%{cfg.buildcfg}\""
	}
	
	rebuildcommands
	{
		"..\\Engine\\Tools\\SparkBuild\\Build.bat Dir=Engine Config=\"%{cfg.buildcfg}\" -Rebuild"
	}

group "Tools"
project "SparkBuild"
	location "Intermediate"

	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	runtime "Release"
	
	targetdir "Binaries/%{cfg.buildcfg}/SparkBuild/"
	objdir "Intermediate/%{cfg.buildcfg}/SparkBuild/"
	
	kind "ConsoleApp"
	
	characterset "Unicode"
		
	filter "configurations: Release"
		optimize "speed"
		symbols "off"
		
	filter "configurations: Development"
		optimize "speed"
		symbols "on"
		
	filter "configurations: Debug"
		optimize "off"
		symbols "on"
		
	filter {}
	
	flags
	{
		"MultiProcessorCompile"
	}
	
	buildoptions
	{
		"/utf-8"
	}
	
	includedirs
	{
		"Engine/Tools/SparkBuild/Public",
		"Engine/Dependencies/json/Public"
	}
	
	files
	{
		"Engine/Tools/SparkBuild/Public/**.h",
		"Engine/Tools/SparkBuild/Private/**.h",
		"Engine/Tools/SparkBuild/Private/**.cpp"
	}
