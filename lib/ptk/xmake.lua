if is_plat("linux") then
    add_requires("libx11", {optional = true})
end

option("enable-touch")
    set_showmenu(true)
    set_default(true)
    set_configvar("PTK_TOUCH_ENABLED", 1)

target("libptk")
    set_version("0.1.0-a")
    set_kind("$(kind)")
    add_files("src/*.c")
    set_configdir("include/ptk")
    add_configfiles("src/config.h.in")
    add_deps("yutil", "pandagl")
    add_headerfiles("include/ptk.h", "include/(ptk/*.h)")
    if is_kind("static") then
        set_configvar("PTK_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("PTK_DLL_EXPORT")
    end
    if is_plat("windows") then
        add_options("enable-touch")
        add_files("src/windows/*.c")
        add_links("Shell32")
    else
        add_files("src/linux/*.c")
        add_packages("libx11")
        if has_package("libx11") then
            set_configvar("PTK_HAS_LIBX11", 1)
        end
        add_syslinks("pthread", "dl")
    end
