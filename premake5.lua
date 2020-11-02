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
		include "Engine"
		
	group "Tools"
		include "Engine/Tools/SparkBuild"
