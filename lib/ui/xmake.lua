set_project("libui")
set_version("0.1.0-a")

target("libui")
    set_kind("$(kind)")
    add_files("src/**.c")
    add_deps("yutil", "pandagl", "libcss")
    set_configdir("include/ui")
    add_configfiles("src/config.h.in")
    add_headerfiles("include/ui.h", "include/(ui/*.h)")
    if is_kind("static") then
        set_configvar("LIBUI_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LIBUI_DLL_EXPORT")
    end
