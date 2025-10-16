project "Core"
    location "src"
    kind "StaticLib"
    language "C++"
    cppdialect "C++23"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/glm/*/**.hpp",
        "vendor/glm/*/**.h",
        "vendor/glm/*/**.inl",
        "vendor/glad/src/**.c",
        "vendor/glfw/src/**.c",
        "vendor/imgui/**.h",
        "vendor/imgui/**.cpp",
        "vendor/obj-loader/Source/**.h",
        "shaders/**.glsl",
    }

    includedirs
    {
        "src",
        "vendor/glm",
        "vendor/glad/include",
        "vendor/glfw/include",
        "vendor/imgui",
        "vendor/obj-loader/Source"
    }

    links
    {
        "opengl32"
    }

    dependson
    {
        "opengl32",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "_GLFW_WIN32",                        
        "IMGUI_IMPL_OPENGL_LOADER_GLAD"  
    }

    vpaths
    {
        ["Headers"] = "src/**.h",
        ["Sources"] = "src/*.cpp",
        ["Shaders"] = "shaders/**.glsl"
    }

    postbuildcommands
    {
        "{MKDIR} %{cfg.targetdir}/shaders",
        "{COPY} %{prj.location}/../shaders/* %{cfg.targetdir}/shaders/"
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
        defines "DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "DIST"
        runtime "Release"
        optimize "on"