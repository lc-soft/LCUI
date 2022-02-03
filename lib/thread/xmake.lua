target("lcui-thread")
    set_kind("static")
    add_files("src/*.c")
    if not is_plat("windows") then
        add_syslinks("pthread", "dl")
    end
    add_deps("lcui-util")
