set_project("lcui")
set_version("2.2.1")
add_rules("mode.debug", "mode.release", "mode.coverage")
add_includedirs("include")
add_rpathdirs("@loader_path/lib", "@loader_path")
add_defines("LCUI_EXPORTS", "YUTIL_EXPORTS", "UNICODE")
includes("lib/**/xmake.lua")
includes("test/lib/ctest/xmake.lua")
set_warnings("all")
set_rundir("$(projectdir)/test")

option("ci-env", {showmenu = true, default = false})

if has_config("ci-env") then
    add_defines("CI_ENV")
end

if is_plat("windows") then
    add_defines("_CRT_SECURE_NO_WARNINGS")
else
    add_cxflags("-fPIC")
    if is_mode("coverage") then
        add_cflags("-ftest-coverage", "-fprofile-arcs", {force = true})
        add_syslinks("gcov")
    end
end

target("run-tests")
    set_kind("binary")
    add_files("$(projectdir)/test/run_tests.c", "$(projectdir)/test/cases/*.c")
    add_includedirs("test/lib/ctest/include/")
    add_deps("ctest", "lcui")
    on_run(function (target)
        import("core.base.option")
        local argv = {}
        local options = {{nil, "memcheck",  "k",  nil, "enable memory check."}}
        local args = option.raw_parse(option.get("arguments") or {}, options)
        os.cd("$(projectdir)/test")
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

target("lcui")
    set_kind("shared")
    add_files("src/*.c")
    add_configfiles("src/config.h.in")
    set_configdir("src")
    add_headerfiles("include/LCUI.h")
    add_headerfiles("include/(LCUI/**.h)")
    add_deps(
        "lcui-util",
        "lcui-thread",
        "lcui-css",
        "lcui-font",
        "lcui-paint",
        "lcui-image",
        "lcui-ui",
        "lcui-ui-widgets",
        "lcui-ui-cursor",
        "lcui-ui-builder",
        "lcui-ui-server",
        "lcui-platform",
        "lcui-text",
        "lcui-timer",
        "lcui-worker"
    )

target("headers")
    set_kind("phony")
    set_default(false)
    before_build(function (target)
        -- Copy the header file of the internal library to the LCUI header file directory
        os.cp("$(projectdir)/lib/util/include/*.h", "$(projectdir)/include/LCUI/util/")
        os.cp("$(projectdir)/lib/yutil/include/yutil/*.h", "$(projectdir)/include/LCUI/util/")

        os.cp("$(projectdir)/lib/thread/include/*.h", "$(projectdir)/include/LCUI/")

        os.cp("$(projectdir)/lib/css/include/*.h", "$(projectdir)/include/LCUI/")
        os.cp("$(projectdir)/lib/css/include/css/*.h", "$(projectdir)/include/LCUI/css/")

        os.cp("$(projectdir)/lib/font/include/*.h", "$(projectdir)/include/LCUI/")
        os.cp("$(projectdir)/lib/font/include/font/*.h", "$(projectdir)/include/LCUI/font/")

        os.cp("$(projectdir)/lib/paint/include/*.h", "$(projectdir)/include/LCUI/paint/")
        os.cp("$(projectdir)/lib/image/include/*.h", "$(projectdir)/include/LCUI/image/")
        os.cp("$(projectdir)/lib/ui/include/*.h", "$(projectdir)/include/LCUI/")
        os.cp("$(projectdir)/lib/ui-widgets/include/*.h", "$(projectdir)/include/LCUI/ui/widgets/")
        os.cp("$(projectdir)/lib/ui-cursor/include/*.h", "$(projectdir)/include/LCUI/ui/")
        os.cp("$(projectdir)/lib/ui-builder/include/*.h", "$(projectdir)/include/LCUI/ui/")
        os.cp("$(projectdir)/lib/ui-server/include/*.h", "$(projectdir)/include/LCUI/ui/")
        os.cp("$(projectdir)/lib/platform/include/*.h", "$(projectdir)/include/LCUI/")
        os.cp("$(projectdir)/lib/text/include/*.h", "$(projectdir)/include/LCUI/")
        os.cp("$(projectdir)/lib/timer/include/*.h", "$(projectdir)/include/LCUI/")
        os.cp("$(projectdir)/lib/worker/include/*.h", "$(projectdir)/include/LCUI/")
    end)
