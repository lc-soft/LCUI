set_project("libcss")
set_version("0.1.0-a")

target("libcss")
    set_kind("$(kind)")
    add_files("src/**.c")
    set_configdir("include/css")
    add_configfiles("src/config.h.in")
    add_headerfiles("include/css.h", "include/(css/*.h)")
    add_deps("yutil")
    if is_kind("static") then
        set_configvar("LIBCSS_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LIBCSS_DLL_EXPORT")
    end

target("libcss-tests")
    set_default(false)
    set_kind("binary")
    set_group("tests")
    set_rundir("tests")
    add_includedirs("tests")
    add_files("tests/*.c")
    add_deps("ctest", "libcss")
    add_rules("tests.runnable")
    add_tests("default", {group = "tests"})
