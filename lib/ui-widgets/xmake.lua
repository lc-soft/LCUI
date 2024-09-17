set_project("libui-widgets")
set_version("0.1.0-a")

target("libui-widgets")
    set_kind("$(kind)")
    add_files("src/**.c")
    add_deps("yutil", "libcss", "pandagl", "libthread", "libui", "libui-xml", "libworker", "libptk", "libtimer")
    set_configdir("include/ui_widgets")
    add_configfiles("src/config.h.in")
    add_headerfiles("include/ui_widgets.h", "include/(ui_widgets/*.h)")
    if is_kind("static") then
        set_configvar("LIBUI_WIDGETS_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LIBUI_WIDGETS_DLL_EXPORT")
    end
