set_project("libui-cursor")
set_version("0.1.0-a")

target("libui-cursor")
    set_kind("$(kind)")
    add_files("src/**.c")
    add_deps("yutil", "pandagl", "libplatform")
    set_configdir("include/ui_cursor")
    add_configfiles("src/config.h.in")
    add_headerfiles("include/ui_cursor.h", "include/(ui_cursor/*.h)")
    if is_kind("static") then
        set_configvar("LIBUI_CURSOR_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LIBUI_CURSOR_DLL_EXPORT")
    end
