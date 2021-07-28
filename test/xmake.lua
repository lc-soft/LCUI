includes("lib/test/xmake.lua")

target("run_tests")
    set_default(false)
    set_rundir("$(projectdir)/test")
    add_files("run_tests.c", "cases/*.c")
    add_deps("LCUI", "test")

target("helloworld")
    set_default(false)
    set_rundir("$(projectdir)/test")
    add_files("helloworld.c")
    add_deps("LCUI")
