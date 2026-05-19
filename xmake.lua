-- Project configuration for Projek-Akhir-APL
set_project("Projek-Akhir-APL")
set_version("1.0.0")

-- Set C++ standard
set_languages("c++17")

-- Add compiler flags
add_cxxflags("-Wall", "-Wextra")

-- Debug mode: enable ASSERT macros
if is_mode("debug") then
    add_defines("DEBUG")
    set_symbols("debug")
end

-- Define the executable target
target("app")
    -- Set target type to console application
    set_kind("binary")
    
    -- Add include directories
    add_includedirs("includes")
    
    -- Add source files
    add_files(
        "src/main.cpp",
        "modules/core_rental.cpp",
        "modules/priority.cpp",
        "modules/reports.cpp",
        "modules/transactions.cpp",
        "modules/user_manager.cpp",
        "utils/file_io.cpp",
        "utils/my_library.cpp",
        "utils/validator.cpp"
    )
    
    -- Set output directory
    set_targetdir("bin")
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

