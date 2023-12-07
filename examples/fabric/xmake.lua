add_requires("cairo")

target("fabric")
    set_kind("binary")
    add_files("src/*.c")
    add_packages("cairo", "lcui")
