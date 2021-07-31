includes("lib/test/xmake.lua")
set_rundir("$(projectdir)/test")
add_deps("lcui")

target("run_tests")
    set_default(false)
    add_files("run_tests.c", "cases/*.c")
    add_deps("test")

target("helloworld")
    set_default(false)
    add_files("helloworld.c")
