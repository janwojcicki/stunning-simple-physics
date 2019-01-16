#!lua

workspace "fiziks"
	configurations {"Debug"}

project "fiziks"
  kind "ConsoleApp"
  language "C++"
  files { "src/**.h", "src/**.cpp" }
  targetdir "bin/"

  configuration "Debug"
  	buildoptions {'-g'}
  	links {'sfml-graphics', 'sfml-window', 'sfml-system','sfml-audio','sfml-network'}

