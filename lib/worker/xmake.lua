add_rules("mode.debug", "mode.release")

target("lcui-worker")
    set_kind("static")
    add_files("src/*.c")
