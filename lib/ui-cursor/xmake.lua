add_configfiles("src/config.h.in")
set_configdir("src")

target("lcui-ui-cursor")
    set_kind("static")
    add_files("src/**.c")
    add_deps("lcui-util", "lcui-platform", "pandagl")
