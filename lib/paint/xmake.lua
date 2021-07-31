add_rules("mode.debug", "mode.release")

target("lcui-paint")
    set_kind("shared")
    add_files("src/*.c")
    add_deps("lcui-util")
