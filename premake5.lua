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
	
	group "Engine"
		include "Engine"
		
	group "Tools"
		include "Engine/Tools/SparkBuild"
