add_rules("mode.debug", "mode.release")

target("timer")
    set_kind("static")
    add_files("src/*.c")
