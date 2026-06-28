target("yutil-tests")
    set_default(false)
    set_kind("binary")
    set_group("tests")
    set_rundir(".")
    add_deps("yutil")
    add_files("*.c")
    if is_plat("windows") then
        add_defines("_CRT_SECURE_NO_WARNINGS")
    else
        add_cxflags("-Wno-error=unused-variable", "-Wno-error=unused-but-set-variable")
    end
