target("libtimer")
    set_kind("$(kind)")
    add_files("src/*.c")
    add_deps("yutil", "libthread")
    set_configdir("include")
    add_configfiles("src/timer.h.in")
    add_headerfiles("include/timer.h")
    if is_kind("static") then
        set_configvar("LIBTIMER_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LIBTIMER_DLL_EXPORT")
    end
