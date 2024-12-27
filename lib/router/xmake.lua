set_project("librouter")
set_version("0.1.0-a")

target("librouter")
    set_kind("$(kind)")
    add_files("src/*.c")
    add_deps("yutil", "libui")
    set_configdir("include/router")
    add_configfiles("src/version.h.in")
    add_headerfiles("include/router.h", "include/(router/*.h)")
    if is_kind("static") then
        set_configvar("LIBROUTER_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LIBROUTER_DLL_EXPORT")
    end

target("librouter_tests")
    set_default(false)
    set_kind("binary")
    add_files("tests/test.c")
    add_deps("librouter", "ctest")
    on_run(function (target)
        import("core.base.option")
        local argv = {}
        local options = {{nil, "memcheck",  "k",  nil, "enable memory check."}}
        local args = option.raw_parse(option.get("arguments") or {}, options)
        os.cd("$(projectdir)/tests")
        if args.memcheck then
            if is_plat("windows") then
                table.insert(argv, target:targetfile())
                os.execv("drmemory", argv)
            else
                table.insert(argv, "valgrind")
                table.insert(argv, "--leak-check=full")
                table.insert(argv, "--error-exitcode=42")
                table.insert(argv, target:targetfile())
                os.execv("sudo", argv)
            end
        else
            os.execv(target:targetfile())
        end
    end)
