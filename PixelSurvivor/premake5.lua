project ("PixelSurvivor")
    location ""
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("../bin/" .. OutputTemplate .. "/PixelSurvivor")
    objdir ("../bin-int/" .. OutputTemplate .. "/PixelSurvivor")

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "../%{IncludeDirs.glm}",
        "../%{IncludeDirs.spdlog}",
        "../%{IncludeDirs.Glad}",
        "../%{IncludeDirs.glfw}",
        "src",
    }

    links {
        "glm",
        "glfw",
        "Glad",
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "PIX_PLATFORM_WINDOWS",
            "PIX_BUILD_STATIC",
            "_CRT_SECURE_NO_WARNINGS",
        }

        links {}

    filter "system:linux"
        systemversion "latest"

        links
        {
            "stdc++fs",
            "dl",
            "X11",
            "pthread",
        }

    filter "configurations:Debug"
        defines "PIX_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "PIX_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "PIX_DIST"
        runtime "Release"
        optimize "on"
