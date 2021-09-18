add_rules("mode.debug", "mode.release")
add_includedirs("include")

target("ctest")
    set_kind("static")
    add_files("src/*.c")
    add_headerfiles("include/ctest.h")
