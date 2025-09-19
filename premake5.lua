workspace "GameEngine"
    architecture "universal"
    startproject "Editor"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["glm"] = "Core/vendor/glm"
IncludeDir["glfw"] = "Editor/vendor/glfw/include"

include "Core"
include "Editor"