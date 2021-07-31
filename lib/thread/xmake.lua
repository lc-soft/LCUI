target("lcui-thread")
    set_kind("static")
    if is_plat("windows") then
        add_files("src/windows/*.c")
    else
        add_files("src/pthread/*.c")
        add_syslinks("pthread", "dl")
    end
    add_deps("lcui-util")
