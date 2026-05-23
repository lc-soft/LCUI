set_project("librouter")
set_version("0.1.0-a")

target("librouter")
    set_kind("$(kind)")
    add_files("src/*.c")
    add_deps("yutil", "libui")
    set_configdir("include/router")
    add_configfiles("src/version.h.in")
    add_headerfiles("include/router.h", "include/(router/*.h)")
    if is_kind("static") then
        set_configvar("LIBROUTER_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LIBROUTER_DLL_EXPORT")
    end

target("librouter-tests")
    set_default(false)
    set_kind("binary")
    set_group("tests")
    set_rundir("tests")
    add_files("tests/test.c")
    add_deps("ctest", "librouter")
    add_rules("tests.runnable")
    add_tests("default", {group = "tests"})
