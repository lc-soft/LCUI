option("with-freetype")
    set_showmenu(true)
    set_default(true)
    set_configvar("USE_FREETYPE", 1)
option_end()

option("with-fontconfig")
    set_showmenu(true)
    set_default(true)
    set_configvar("USE_FONTCONFIG", 1)
option_end()

if has_config("with-freetype") then
    add_requires("freetype", {optional = true})
end

if has_config("with-fontconfig") then
    add_requires("fontconfig", {optional = true})
end

target("lcui-font")
    set_kind("shared")
    add_files("src/**.c")
    set_configdir("src")
    add_configfiles("src/config.h.in")
    add_options("with-freetype", "with-fontconfig")
    add_packages("freetype", "fontconfig")
    add_deps("lcui-util", "lcui-paint")
