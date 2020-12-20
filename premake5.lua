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
	location "Engine/Intermediate/ProjectFiles"

	language "C++"
	cppdialect "C++17"
	
	targetdir "Engine/Binaries/%{cfg.buildcfg}/Spark/"
	objdir "Engine/Intermediate/%{cfg.buildcfg}/Spark/"
	
	dependson "SparkBuild"
	
	kind "Makefile"
	
	includedirs
	{
		"Engine/**/Public",
		"Engine/Intermediate/GeneratedFiles"
	}
	
	files
	{
		"Engine/Source/**.h",
		"Engine/Source/**.cpp",
		"Engine/Source/**.json"
	}

	buildcommands
	{
		"..\\..\\Source\\Tools\\SparkBuild\\Build.bat Build Engine %{cfg.buildcfg}"
	}
	
	rebuildcommands
	{
		"..\\..\\Source\\Tools\\SparkBuild\\Build.bat Rebuild Engine %{cfg.buildcfg}"
	}
	
	cleancommands
	{
		"..\\..\\Source\\Tools\\SparkBuild\\Build.bat Clean Engine %{cfg.buildcfg}"
	}

group "Tools"
project "SparkBuild"
	location "Engine/Intermediate/ProjectFiles"

	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	runtime "Release"
	
	targetdir "Engine/Binaries/%{cfg.buildcfg}/SparkBuild/"
	objdir "Engine/Intermediate/%{cfg.buildcfg}/SparkBuild/"
	
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
		"Engine/Source/Tools/SparkBuild/Private",
		"Engine/Dependencies/json/Public"
	}
	
	files
	{
		"Engine/Source/Tools/SparkBuild/Public/**.h",
		"Engine/Source/Tools/SparkBuild/Private/**.h",
		"Engine/Source/Tools/SparkBuild/Private/**.cpp"
	}
