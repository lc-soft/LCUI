add_requires("cairo 1.17.6")

target("fabric")
    set_kind("binary")
    add_files("src/*.c")
    add_packages("cairo", "lcui")
