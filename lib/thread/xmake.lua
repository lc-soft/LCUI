add_rules("mode.debug", "mode.release")

target("thread")
    set_kind("static")
    add_files("src/**.c")
    set_configdir("src")
    add_configfiles("src/config.h.in")
    add_includedirs("src")
