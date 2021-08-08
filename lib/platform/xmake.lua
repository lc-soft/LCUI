add_requires("libomp", {optional = true})
add_includedirs("src", "include")
set_configdir("src")
add_configfiles("src/config.h.in")
option("with-libx11", {showmenu = true, default = true})

option("enable-touch")
    set_showmenu(true)
    set_default(true)
    set_configvar("ENABLE_TOUCH", 1)

option("use-linux-input-event")
    set_showmenu(true)
    set_configvar("USE_LINUX_INPUT_EVENT", 1)

option("uwp", {showmenu = true, default = false})

if has_config("with-libx11") then
    add_requires("libx11", {optional = true})
end

target("lcui-platform-uwp")
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

target("lcui-platform-windows")
    if is_plat("windows") and not has_config("uwp") then
        set_default(true)
    else
        set_default(false)
    end
    set_kind("static")
    add_files("src/windows/*.c")
    add_links("Shell32")

target("lcui-platform-linux")
    if is_plat("linux") then
        set_default(true)
    else
        set_default(false)
    end
    add_options("use-linux-input-event")
    add_packages("libx11")
    if has_package("libx11") then
        set_configvar("USE_LIBX11", 1)
    end
    set_kind("static")
    add_files("src/linux/*.c")

target("lcui-platform")
    set_kind("static")
    add_files("src/*.c")
    if is_plat("windows") then
        add_options("uwp", "enable-touch")
        if has_config("uwp") then
            add_deps("lcui-uwp")
        else
            add_deps("lcui-platform-windows")
        end
    else
        add_deps("lcui-platform-linux")
    end
    add_deps("lcui-thread")
