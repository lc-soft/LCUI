add_rules("mode.debug", "mode.release")

target("lcui-util")
    set_kind("shared")
    add_files("src/*.c")
