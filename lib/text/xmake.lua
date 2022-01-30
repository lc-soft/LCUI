target("lcui-text")
    set_kind("static")
    add_files("src/*.c")
    add_deps("lcui-util", "lcui-font", "lcui-paint", "lcui-css")
