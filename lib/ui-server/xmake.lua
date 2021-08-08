add_requires("libomp", {optional = true})
add_configfiles("src/config.h.in")
set_configdir("src")

option("enable-openmp", {showmenu = true, default = true})

target("lcui-ui-server")
    set_kind("static")
    add_files("src/**.c")
    add_packages("libomp")
    if has_package("libomp") and has_config("enable-openmp") then
        set_configvar("ENABLE_OPENMP", 1)
    end
    add_deps("lcui-util", "lcui-ui", "lcui-ui-cursor", "lcui-platform")
