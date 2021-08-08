option("with-freetype", {showmenu = true, default = true})
option("with-fontconfig", {showmenu = true, default = true})

if has_config("with-freetype") then
    add_requires("freetype", {optional = true, configs = {shared = false}})
end

if has_config("with-fontconfig") then
    add_requires("fontconfig", {optional = true})
end

target("lcui-font")
    set_kind("static")
    add_files("src/**.c")
    set_configdir("src")
    add_configfiles("src/config.h.in")
    add_options("with-freetype", "with-fontconfig")
    add_packages("freetype", "fontconfig")
    add_deps("lcui-util", "lcui-paint")
    if has_package("fontconfig") then
        set_configvar("USE_FONTCONFIG", 1)
    end
    if has_package("freetype") then
        set_configvar("USE_FREETYPE", 1)
    end
