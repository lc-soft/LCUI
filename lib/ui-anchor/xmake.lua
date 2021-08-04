target("lcui-ui-anchor")
    set_kind("shared")
    add_files("src/**.c")
    if is_plat("windows") then
        add_options("uwp")
        if has_config("uwp") then
            add_files("src/*.cpp")
        end
    end
    add_deps("lcui-ui", "lcui-ui-builder")
