add_rules("mode.debug", "mode.release")

target("gui")
    set_kind("static")
    add_files("src/**.c")
