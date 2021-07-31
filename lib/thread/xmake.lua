add_rules("mode.debug", "mode.release")

target("lcui-thread")
    set_kind("shared")
    add_files("src/**.c")
    set_configdir("src")
    add_configfiles("src/config.h.in")
    add_includedirs("src")
    add_deps("lcui-util")
