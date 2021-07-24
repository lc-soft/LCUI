add_rules("mode.debug", "mode.release")

target("util")
    set_kind("static")
    add_files("src/*.c")
