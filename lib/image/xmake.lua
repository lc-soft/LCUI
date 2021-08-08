option("with-libpng", {showmenu = true, default = true})
option("with-libjpeg", {showmenu = true, default = true})

if has_config("with-libpng") then
    add_requires("libpng", {optional = true})
end

if has_config("with-libjpeg") then
    add_requires("libjpeg", {optional = true})
end

target("lcui-image")
    set_kind("static")
    add_files("src/*.c")
    set_configdir("src")
    add_configfiles("src/config.h.in")
    add_options("with-libpng", "with-libjpeg")
    add_packages("libjpeg", "libpng")
    add_deps("lcui-util", "lcui-paint")
    if has_package("libjpeg") then
        set_configvar("USE_LIBJPEG", 1)
    end
    if has_package("libpng") then
        set_configvar("USE_LIBPNG", 1)
    end
