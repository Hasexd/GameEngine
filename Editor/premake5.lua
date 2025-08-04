project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/glad/src/**.c",
        "vendor/glfw/src/**.c",
        "vendor/imgui/**.h",
        "vendor/imgui/**.cpp",
    }

    includedirs
    {
        "src",
        "../Core/src",
        "../Core/vendor/glm",
        "vendor/glfw/include",
        "vendor/glad/include",
        "vendor/imgui/"
    }

    links
    {
        "Core",
        "opengl32"
    }

    defines
    {
        "_GLFW_WIN32",
        "_CRT_SECURE_NO_WARNINGS",
    }

    filter "system:windows"
        systemversion "latest"

    filter "system:linux"
        defines "_GLFW_X11"
        links
        {
            "GL",
            "X11",
            "pthread",
            "Xrandr",
            "Xi",
            "dl"
        }

    filter "configurations:Debug"
        defines "EDITOR_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "EDITOR_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "EDITOR_DIST"
        runtime "Release"
        optimize "on"