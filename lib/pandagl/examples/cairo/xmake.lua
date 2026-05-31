add_requires("cairo")

target("pandagl-cairo-example")
    set_default(false)
    set_kind("binary")
    set_group("examples")
    add_files("src/*.c")
    add_packages("cairo")
    add_deps("pandagl", "yutil")
