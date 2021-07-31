add_rules("mode.debug", "mode.release")

option("with-libpng")
    set_showmenu(true)
    set_default(true)
    set_configvar("USE_LIBPNG", 1)
option_end()

option("with-libjpeg")
    set_showmenu(true)
    set_default(true)
    set_configvar("USE_LIBJPEG", 1)
option_end()

if has_config("with-libpng") then
    add_requires("libpng", {optional = true})
end

if has_config("with-libjpeg") then
    add_requires("libjpeg", {optional = true})
end

target("lcui-image")
    set_kind("shared")
    add_files("src/*.c")
    set_configdir("src")
    add_configfiles("src/config.h.in")
    add_options("with-libpng", "with-libjpeg")
    add_packages("libjpeg", "libpng")
    add_deps("lcui-util", "lcui-paint")
