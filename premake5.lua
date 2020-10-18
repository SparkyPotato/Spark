-- Copyright 2020 SparkyPotato

workspace "Spark"
	architecture "x86_64"
	configurations
	{
		"Debug Lib",
		"Debug Editor",
		"Debug Editor Console",
		"Release Lib",
		"Release Editor",
		"Release Editor Console"
	}
	
	flags "FatalWarnings"
	
	include "Engine"
