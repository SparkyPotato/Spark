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
		"Engine/**/Public",
		"Engine/Intermediate/GeneratedFiles"
	}
	
	defines
	{
		"BUILD_Launch",
		"BUILD_Core",
		"BUILD_OSAL",
		"BUILD_SparkEditor",
		"BUILD_SparkBuild",
	}
	
	filter "system:windows"
		defines 
		{
			"PLATFORM_WINDOWS"
		}
	
	filter {}
	
	files
	{
		"Engine/Source/**.h",
		"Engine/Source/**.cpp",
		"Engine/Source/**.json"
	}

	buildcommands
	{
		"..\\Engine\\Source\\Tools\\SparkBuild\\Build.bat Dir=Engine Config=\"%{cfg.buildcfg}\""
	}
	
	rebuildcommands
	{
		"..\\Engine\\Source\\Tools\\SparkBuild\\Build.bat Dir=Engine Config=\"%{cfg.buildcfg}\" -Rebuild"
	}
	
	debugcommand "%{cfg.targetdir}\\..\\..\\..\\Engine\\Binaries\\%{cfg.buildcfg}\\SparkEditor\\Executable\\SparkEditor.exe"

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
		
	filter "configurations:Release"
		optimize "speed"
		symbols "off"
		
	filter "configurations:Development"
		optimize "speed"
		symbols "on"
		
	filter "configurations:Debug"
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
		"Engine/Source/Tools/SparkBuild/Public",
		"Engine/Source/Dependencies/json/Public"
	}
	
	files
	{
		"Engine/Source/Tools/SparkBuild/Public/**.h",
		"Engine/Source/Tools/SparkBuild/Private/**.h",
		"Engine/Source/Tools/SparkBuild/Private/**.cpp"
	}
