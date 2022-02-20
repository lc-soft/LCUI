target("libcss")
    set_kind("$(kind)")
    add_files("src/**.c")
    add_deps("yutil")

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
