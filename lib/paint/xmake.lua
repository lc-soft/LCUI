add_rules("mode.debug", "mode.release")

target("paint")
    set_kind("static")
    add_files("src/*.c")
