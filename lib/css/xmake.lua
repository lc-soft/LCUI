add_rules("mode.debug", "mode.release")

target("css")
    set_kind("static")
    add_files("src/*.c")
