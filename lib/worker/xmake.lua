add_rules("mode.debug", "mode.release")

target("worker")
    set_kind("static")
    add_files("src/*.c")
