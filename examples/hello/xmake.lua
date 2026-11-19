target("hello")
    set_default(false)
    set_group("lcui-examples")
    set_prefixdir("hello", {bindir = "."})
    set_kind("binary")
    set_rundir("app")
    add_deps("lcui")
    add_files("src/*.c")
    add_installfiles("app/*")
    if is_plat("windows") and is_mode("release") then
        add_rules("win.sdk.application")
    end
