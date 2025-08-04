project "Core"
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
        "vendor/glm/*/**.inl"
    }

    includedirs
    {
        "src",
        "vendor/glm"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "CORE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CORE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "CORE_DIST"
        runtime "Release"
        optimize "on"