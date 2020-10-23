-- Copyright 2020 SparkyPotato

workspace "Spark"
	architecture "x86_64"
	configurations
	{
		"Debug Lib",
		"Debug Editor",
		"Development Lib",
		"Development Editor",
		"Release Lib",
		"Release Editor",
	}
	
	flags "FatalWarnings"
	
	include "Engine"
