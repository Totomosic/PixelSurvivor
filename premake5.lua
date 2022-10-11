workspace "PixelSurvivor"
    architecture "x64"

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

OutputTemplate = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["spdlog"] = "PixelSurvivor/vendor/spdlog/include"
IncludeDirs["Glad"] = "PixelSurvivor/vendor/Glad/include"
IncludeDirs["glfw"] = "PixelSurvivor/vendor/glfw/include"
IncludeDirs["glm"] = "PixelSurvivor/vendor/glm"

group ("vendor")
include ("PixelSurvivor/vendor/Glad")
include ("PixelSurvivor/vendor/glfw")
include ("PixelSurvivor/vendor/glm")

group ("PixelSurvivor")
include ("PixelSurvivor")
