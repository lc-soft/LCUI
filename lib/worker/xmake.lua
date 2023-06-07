target("libworker")
    set_kind("$(kind)")
    add_files("src/**.c")
    add_deps("yutil", "libthread")
    set_configdir("include/worker")
    add_configfiles("src/config.h.in")
    add_headerfiles("include/worker.h", "include/(worker/*.h)")
    if is_kind("static") then
        set_configvar("LIBWORKER_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LIBWORKER_DLL_EXPORT")
    end
