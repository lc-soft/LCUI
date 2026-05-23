set_version("0.1.0-a")

target("libthread")
    set_kind("$(kind)")
    add_files("src/*.c")
    set_configdir("include")
    add_configfiles("src/thread.h.in")
    add_headerfiles("include/thread.h")
    if not is_plat("windows") then
        add_syslinks("pthread", "dl")
    end
    add_deps("yutil")
    if is_kind("static") then
        set_configvar("LIBTHREAD_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LIBTHREAD_DLL_EXPORT")
    end

target("libthread-tests")
    set_default(false)
    set_kind("binary")
    set_group("tests")
    set_rundir("tests")
    add_files("tests/main.c", "tests/test_thread.c")
    add_deps("ctest", "libthread")
    add_rules("tests.runnable")
    add_tests("default", {group = "tests"})

