target("lcui-timer")
    set_kind("static")
    add_files("src/*.c")
    add_deps("lcui-util", "lcui-thread")
