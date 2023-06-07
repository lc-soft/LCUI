set_project("libui-xml")
set_version("0.1.0-a")
add_requires("libxml2", {optional = true})

target("libui-xml")
    add_packages("libxml2")
    if has_package("libxml2") then
        set_configvar("LIBUI_XML_HAS_LIBXML2", 1)
    end
    set_kind("$(kind)")
    add_files("src/**.c")
    add_deps("yutil", "libui", "libcss", "pandagl")
    set_configdir("include/ui_xml")
    add_configfiles("src/config.h.in")
    add_headerfiles("include/ui_xml.h", "include/(ui_xml/*.h)")
    if is_kind("static") then
        set_configvar("LIBUI_XML_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LIBUI_XML_DLL_EXPORT")
    end
