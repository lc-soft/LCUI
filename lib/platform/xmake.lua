add_rules("mode.debug", "mode.release")
add_requires("libomp", {optional = true})
add_includedirs("src")

option("with-libx11")
    set_default(true)
    set_showmenu(true)
    set_configvar("USE_LIBX11", 1)
option_end()

option("uwp", {showmenu = true, default = false})

if has_config("with-libx11") then
    add_requires("libx11", {optional = true})
end

target("lcui-uwp")
    if is_plat("windows") and has_config("uwp") then
        set_default(true)
    else
        set_default(false)
    end
    set_kind("static")
    add_includedirs("src/uwp")
    add_cxxflags("/ZW", {force = true})
    add_defines("WINAPI_FAMILY=WINAPI_FAMILY_APP")
    add_links("WindowsApp")
    set_pcxxheader("src/uwp/pch.h")
    add_files("src/uwp/**.cpp")

target("lcui-windows")
    if is_plat("windows") and not has_config("uwp") then
        set_default(true)
    else
        set_default(false)
    end
    set_kind("static")
    add_files("src/windows/*.c")
    add_links("Shell32")

target("lcui-linux")
    if is_plat("linux") then
        set_default(true)
    else
        set_default(false)
    end
    set_kind("static")
    add_files("src/linux/*.c")

target("lcui-platform")
    set_kind("static")
    add_rules("c.openmp", "c++.openmp")
    set_configdir("src")
    add_configfiles("src/config.h.in")
    add_files("src/*.c")
    if is_plat("windows") then
        add_options("uwp")
        if has_config("uwp") then
            add_deps("lcui-uwp")
        else
            add_deps("lcui-windows")
        end
    else
        add_deps("lcui-linux")
    end
