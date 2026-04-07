set_project("Telly")
set_languages("cxx20")
set_warnings("all")
set_toolchains("mingw")

-- =========================
-- Engine (Static Library)
-- =========================
target("TellyEngine")
set_kind("static")
-- Include all engine headers
add_includedirs("Engine", { public = true })
add_includedirs("Thirdparty", { public = true })
-- Engine source files only
add_files("Engine/**.cpp")


-- =========================
-- Helper: Configure Example Targets
-- =========================
function add_example(name, path)
    target(name)
    set_kind("binary")

    -- Source files
    add_files(path .. "/**.cpp")

    -- Include paths
    add_includedirs(
        "Engine",          -- engine headers
        path,              -- root (for main.cpp)
        path .. "/headers" -- your MenuScene.hpp
    )

    -- Link engine
    add_deps("TellyEngine")

    -- Output
    set_targetdir("Runtime/bin")
    set_rundir("Runtime/bin")

    -- SDL
    add_linkdirs("Runtime/dll")
    add_links("SDL3", "SDL3_image", "SDL3_ttf")

    -- Copy DLLs after build
    after_build(function(target)
        os.cp("Runtime/dll/*.dll", target:targetdir())
    end)
end

-- =========================
-- Examples
-- =========================
add_example("MemoryGame", "Examples/Memory-Game")
-- add_example("SnakeGame", "Examples/Snake-Game") -- ready for later
