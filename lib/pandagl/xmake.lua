set_project("pandagl")
set_version("0.1.0-a")
add_requires("libpng", "libjpeg", {optional = true})
add_requires("freetype", {optional = true, configs = {shared = false}})

if is_plat("linux") then
    add_requires("fontconfig", {optional = true})
end

option("with-pandagl-font")
    set_default(true)
    set_showmenu(true)
    set_configvar("PANDAGL_HAS_FONT", 1)

option("with-pandagl-image")
    set_default(true)
    set_showmenu(true)
    set_configvar("PANDAGL_HAS_IMAGE", 1)

option("with-pandagl-text")
    set_default(true)
    set_showmenu(true)
    set_configvar("PANDAGL_HAS_TEXT", 1)

target("pandagl")
    set_kind("$(kind)")
    add_files("src/*.c")
    add_deps("yutil")
    add_includedirs("include")
    set_configdir("include/pandagl")
    add_configfiles("src/config.h.in")
    add_headerfiles("include/pandagl.h", "include/(pandagl/*.h)")
    add_packages("libpng", "libjpeg", "freetype", "fontconfig")
    if is_kind("static") then
        set_configvar("PANDAGL_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("PANDAGL_DLL_EXPORT")
    end
    add_options("with-pandagl-font", "with-pandagl-image", "with-pandagl-text")
    if has_config("with-pandagl-font") then
        add_files("src/font/*.c")
        if has_package("freetype") then
            set_configvar("PANDAGL_HAS_FREETYPE", 1)
        end
        if has_package("fontconfig") then
            set_configvar("PANDAGL_HAS_FONTCONFIG", 1)
        end
    end
    if has_config("with-pandagl-font") then
        add_files("src/text/*.c")
    end
    if has_config("with-pandagl-image") then
        add_files("src/image/*.c")
        if has_package("libpng") then
            set_configvar("PANDAGL_HAS_LIBPNG", 1)
        end
        if has_package("libjpeg") then
            set_configvar("PANDAGL_HAS_LIBJPEG", 1)
        end
    end

target("pandagl_tests")
    set_default(false)
    set_kind("binary")
    add_files("test/*.c")
    add_deps("ctest", "pandagl")
