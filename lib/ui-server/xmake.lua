set_project("libui-server")
set_version("0.1.0-a")
add_requires("libomp", {optional = true})

option("with-openmp", {showmenu = true, default = true})

target("libui-server")
    set_kind("$(kind)")
    add_files("src/**.c")
    add_packages("libomp")
    add_options("with-openmp")
    add_deps("yutil", "pandagl", "libplatform", "libui", "libui-cursor")
    set_configdir("include/ui_server")
    add_configfiles("src/config.h.in")
    add_headerfiles("include/ui_server.h", "include/(ui_server/*.h)")
    if is_kind("static") then
        set_configvar("LIBUI_SERVER_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LIBUI_SERVER_DLL_EXPORT")
    end
    if has_package("libomp") and has_config("with-openmp") then
        set_configvar("LIBUI_SERVER_HAS_OPENMP", 1)
    end
