add_rules("mode.debug", "mode.release")

option("with-libxml2")
    set_default(true)
    set_showmenu(true)
    set_configvar("USE_LIBXML2", 1)
option_end()

if has_config("with-libxml2") then
    add_requires("libxml2", {optional = true})
end

target("builder")
    set_kind("static")
    add_files("src/*.c")
    set_configdir("src")
    add_configfiles("src/config.h.in")
    add_options("with-libxml2")
    add_packages("libxml2")
    add_deps("lcui-util", "lcui-css", "lcui-font", "lcui-ui")
