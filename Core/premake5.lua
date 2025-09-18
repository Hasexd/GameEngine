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
    }

    includedirs
    {
        "src",
        "vendor/glm",
        "vendor/glad/include",
    }

    links
    {
        "opengl32"
    }

    dependson
    {
        "opengl32"
    }

    vpaths
    {
        ["Headers"] = "src/**.h",
        ["Sources"] = "src/*.cpp",
    }

    filter "system:windows"
        systemversion "latest"

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