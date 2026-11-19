add_rules("mode.debug", "mode.release")
add_includedirs("include")

target("ctest")
    set_version("1.0.0")
    set_kind("static")
    add_files("src/*.c")
    add_headerfiles("include/ctest.h")

if os.isdir("tests") then
    includes("tests/xmake.lua")
end
