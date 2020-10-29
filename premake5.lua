-- Copyright 2020 SparkyPotato

workspace "Spark"
	architecture "x86_64"
	
	configurations
	{
		"DebugLib",
		"DebugEditor",
		"DevelopmentLib",
		"DevelopmentEditor",
		"ReleaseLib",
		"ReleaseEditor",
	}
	
	startproject "Spark"
	
	include "Engine/Tools/SparkBuild"
	include "Engine"
