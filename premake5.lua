workspace "Socked"
    architecture "x64"
    configurations { "Debug", "Release" }

    targetdir "build/%{cfg.buildcfg}/"
    targetname "%{prj.name}-%{cfg.system}-%{cfg.architecture}"

-- Build configurations
filter "configurations:Debug"
    defines "DEBUG"
    symbols "On"

filter "configurations:Release"
    defines "NDEBUG"
    optimize "On"

project "Socked"
    location "./"
    kind "Utility"
    language "C++"

    files { 
        "./**.h",
        "./**.hpp",
        "./**.c",
        "./**.cpp"
    }

    includedirs {
        -- Project source code
        "./",
    }


-- Windows system
filter "system:windows"
    system "windows"
    cppdialect "C++17"
    systemversion "latest"
    -- flags { "MultiProcessorCompile" }

-- Linux system
filter "system:linux"
    system "linux"
    cppdialect "gnu++17"
    -- buildoptions { "-fopenmp" }
    -- links { "omp", "glfw", "GL", "m", "X11", "pthread", "Xi", "Xrandr", "dl" }

