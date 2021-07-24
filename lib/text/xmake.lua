add_rules("mode.debug", "mode.release")

target("text")
    set_kind("static")
    add_files("src/*.c")
