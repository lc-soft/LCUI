target("lcui-worker")
    set_kind("static")
    add_files("src/*.c")
    add_deps("lcui-thread")
