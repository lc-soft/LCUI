target("lcui-ui")
    set_kind("static")
    add_files("src/**.c")
    add_deps("lcui-util", "lcui-paint", "lcui-css", "lcui-thread")
