add_rules("mode.debug", "mode.release")

target("lcui-gui")
    set_kind("static")
    add_files("src/**.c")
    add_deps("lcui-util", "lcui-paint", "lcui-css", "lcui-text", "lcui-thread")
