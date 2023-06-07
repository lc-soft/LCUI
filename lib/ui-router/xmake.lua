set_project("libui-router")
set_version("0.1.0-a")

target("libui-router")
    set_kind("$(kind)")
    add_files("src/*.c")
    add_deps("yutil", "libui", "libui-widgets")
    set_configdir("include/ui_router")
    add_configfiles("src/version.h.in")
    add_headerfiles("include/ui_router.h", "include/(ui_router/*.h)")
    if is_kind("static") then
        set_configvar("LIBUI_ROUTER_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LIBUI_ROUTER_DLL_EXPORT")
    end

target("libui-router-tests")
    set_kind("binary")
    add_files("tests/test.c")
    add_deps("libui-router", "ctest")
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
