project "Editor"
   location "src"
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
    }

    includedirs
    {
        "src",
        "../Core/src",
        "../Core/vendor/glm",
        "../Core/vendor/glfw/include",
        "../Core/vendor/glad/include",
        "../Core/vendor/imgui",
    }

    links
    {
        "Core",
        "opengl32"
    }

    vpaths
    {
        ["Headers"] = "src/**.h",
        ["Sources"] = "src/*.cpp"
    }

     dependson
    {
        "Core",
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