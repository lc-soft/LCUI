target("libthread")
    set_default(false)
    set_kind("$(kind)")
    add_files("src/*.c")
    if not is_plat("windows") then
        add_syslinks("pthread", "dl")
    end
    add_deps("yutil")
