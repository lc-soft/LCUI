set_project("libplatform")
set_version("0.1.0-a")

if is_plat("linux") then
    add_requires("libx11", {optional = true})
end

option("enable-touch")
    set_showmenu(true)
    set_default(true)
    set_configvar("ENABLE_TOUCH", 1)

target("libplatform")
    set_kind("$(kind)")
    add_files("src/*.c")
    set_configdir("include/platform")
    add_configfiles("src/config.h.in")
    add_deps("yutil", "pandagl")
    add_headerfiles("include/platform.h", "include/(platform/*.h)")
    if is_kind("static") then
        set_configvar("LIBPLAT_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LIBPLAT_DLL_EXPORT")
    end
    if is_plat("windows") then
        add_options("enable-touch")
        add_files("src/windows/*.c")
        add_links("Shell32")
    else
        add_files("src/linux/*.c")
        add_packages("libx11")
        if has_package("libx11") then
            set_configvar("LIBPLAT_HAS_LIBX11", 1)
        end
        add_syslinks("pthread", "dl")
    end
