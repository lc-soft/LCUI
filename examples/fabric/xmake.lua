add_requires("cairo", { optional = true })

target("fabric")
    set_kind("binary")
    add_files("src/*.c")
    add_packages("cairo", "lcui")
    if has_package("cairo") then
        add_defines("HAS_CAIRO")
    end
