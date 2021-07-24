add_repositories("local-repo ../build")
add_requires("lcui")
includes("lib/test/xmake.lua")
set_warnings("all")

target("run_tests")
    add_files("run_tests.c", "cases/*.c")
    add_packages("lcui")
    add_deps("test")
