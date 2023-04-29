set_project("libcss")
set_version("0.1.0-a")

target("libcss")
    set_kind("$(kind)")
    add_files("src/**.c")
    set_configdir("include/css")
    add_configfiles("src/config.h.in")
    add_headerfiles("include/css.h", "include/(css/*.h)")
    add_deps("yutil")
    if is_kind("static") then
        set_configvar("LIBCSS_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LIBCSS_DLL_EXPORT")
    end

target("libcss_tests")
    set_kind("binary")
    set_rundir("tests")
    add_files("tests/*.c")
    add_deps("ctest", "libcss")
    on_run(function (target)
        import("core.base.option")
        local argv = {}
        local options = {{nil, "memcheck",  "k",  nil, "enable memory check."}}
        local args = option.raw_parse(option.get("arguments") or {}, options)
        os.cd("$(scriptdir)/tests")
        if args.memcheck then
            if is_plat("windows") then
                table.insert(argv, target:targetfile())
                os.execv("drmemory", argv)
            else
                table.insert(argv, "--leak-check=full")
                table.insert(argv, "--error-exitcode=42")
                table.insert(argv, target:targetfile())
                os.execv("valgrind", argv)
            end
        else
            os.execv(target:targetfile())
        end
    end)
