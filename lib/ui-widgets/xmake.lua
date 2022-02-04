target("lcui-ui-widgets")
    set_kind("static")
    add_files("src/**.c")
    add_deps("lcui-util", "lcui-ui", "lcui-platform", "lcui-ui-builder", "lcui-worker")
