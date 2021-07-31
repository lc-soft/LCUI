add_rules("mode.debug", "mode.release")

target("lcui-text")
    set_kind("shared")
    add_files("src/*.c")
    add_deps("lcui-util", "lcui-font", "lcui-paint")
