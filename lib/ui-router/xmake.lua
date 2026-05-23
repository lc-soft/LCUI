set_project("libui-router")
set_version("0.1.0-a")

target("libui-router")
    set_kind("$(kind)")
    add_files("src/**.c")
    add_deps("yutil", "libui", "librouter")
    set_configdir("include/ui_router")
    add_configfiles("src/config.h.in")
    add_headerfiles("include/ui_router.h", "include/(ui_router/*.h)")
    if is_kind("static") then
        set_configvar("LIBUI_ROUTER_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LIBUI_ROUTER_DLL_EXPORT")
    end
