add_requires("cairo", { optional = true })

target("fabric")
    set_default(false)
    set_group("lcui-examples")
    set_prefixdir("fabric", {bindir = "."})
    set_kind("binary")
    add_files("src/*.c")
    add_deps("lcui")
    add_packages("cairo")
    if has_package("cairo") then
        add_defines("HAS_CAIRO")
    end
