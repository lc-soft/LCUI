set_project("lcui")
set_version("2.2.1")
add_rules("mode.debug", "mode.release", "c++.openmp", "mode.coverage")
add_includedirs("include")
add_rpathdirs("@loader_path/lib", "@loader_path")
add_defines("LCUI_EXPORTS", "_UNICODE")
includes("lib/**/xmake.lua")
set_warnings("all")

if is_plat("windows") then
    add_defines("_CRT_SECURE_NO_WARNINGS")
else
    add_cxflags("-fPIC")
    if is_mode("coverage") then
        add_cflags("-ftest-coverage", "-fprofile-arcs", {force = true})
        add_syslinks("gcov")
    end
end

if is_mode("release") then
    set_symbols("none")
end

target("lcui")
    set_kind("shared")
    add_files("src/*.c")
    add_configfiles("src/config.h.in")
    set_configdir("src")
    add_headerfiles("include/LCUI.h")
    add_headerfiles("include/LCUI_Build.h")
    add_headerfiles("include/(LCUI/**.h)")
    add_deps(
        "lcui-util",
        "lcui-thread",
        "lcui-css",
        "lcui-font",
        "lcui-paint",
        "lcui-image",
        "lcui-gui",
        "lcui-platform",
        "lcui-text",
        "lcui-builder",
        "lcui-timer",
        "lcui-worker"
    )

target("run-tests")
    set_kind("binary")
    set_rundir("$(projectdir)/test")
    set_default(false)
    if is_plat("linux") and is_mode("coverage") then
        on_run(function (target)
            import("core.base.option")
            local argv = {}
            local options = {{nil, "memcheck",  "k",  nil, "enable memory check."}}
            local args = option.raw_parse(option.get("arguments") or {}, options)
            if args.memcheck then
                table.insert(argv, "--leak-check=full")
                table.insert(argv, "--error-exitcode=42")
            end
            table.insert(argv, val("projectdir").."/"..target:targetfile())
            os.cd("$(projectdir)/test")
            os.execv("valgrind", argv)
        end)
    end
    add_files("./test/run_tests.c", "test/cases/*.c")
    add_deps("test", "lcui")
