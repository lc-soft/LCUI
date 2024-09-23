set_project("lcui")
set_version("3.0.0-a")
set_warnings("all", "error")
set_policy("package.requires_lock", true)
add_rules("mode.debug", "mode.release", "mode.coverage")
add_rpathdirs("@loader_path/lib", "@loader_path")
add_defines("UNICODE", "_CRT_SECURE_NO_WARNINGS", "YUTIL_EXPORTS")
add_includedirs(
    "lib/ctest/include",
    "lib/yutil/include",
    "lib/pandagl/include",
    "lib/thread/include",
    "lib/css/include",
    "lib/i18n/include",
    "lib/ptk/include",
    "lib/worker/include",
    "lib/ui/include",
    "lib/ui-server/include",
    "lib/ui-widgets/include",
    "lib/ui-cursor/include",
    "lib/ui-xml/include",
    "lib/ui-router/include",
    "include",
    {public = true}
)
includes("lib/*/xmake.lua")
includes("tests/xmake.lua")

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

target("lcui_tests")
    set_default(false)
    set_kind("binary")
    set_rundir("tests")
    add_includedirs("tests/include")
    add_files("tests/run_tests.c", "tests/cases/*.c")
    add_deps("ctest", "lcui")
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
                table.insert(argv, "valgrind")
                table.insert(argv, "--leak-check=full")
                table.insert(argv, "--error-exitcode=42")
                table.insert(argv, "--num-callers=20")
                table.insert(argv, target:targetfile())
                os.execv("sudo", argv)
            end
        else
            os.execv(target:targetfile())
        end
    end)

target("lcui")
    set_kind("$(kind)")
    add_files("src/*.c")
    if is_kind("static") then
        set_configvar("LCUI_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LCUI_DLL_EXPORT")
    end
    add_configfiles("src/config.h.in")
    set_configdir("include/LCUI")
    add_deps(
        "yutil",
        "libthread",
        "libptk",
        "libworker",
        "pandagl",
        "libcss",
        "libi18n",
        "libui",
        "libui-widgets",
        "libui-cursor",
        "libui-server",
        "libui-router",
        "libui-xml"
    )
    add_headerfiles("include/LCUI.h", "include/(LCUI/*.h)")
