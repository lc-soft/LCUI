set_project("lcui")
set_version("3.0.0-a")
set_warnings("all")
add_rules("mode.debug", "mode.release", "mode.coverage")
add_rpathdirs("@loader_path/lib", "@loader_path")
add_requires("libomp", "libxml2", {optional = true})
add_defines("LCUI_EXPORTS", "YUTIL_EXPORTS", "UNICODE", "_CRT_SECURE_NO_WARNINGS")
add_includedirs(
    "lib/ctest/include",
    "lib/yutil/include",
    "lib/pandagl/include",
    "lib/css/include",
    "lib/platform/include",
    "lib/ui/include",
    "lib/ui-server/include",
    "lib/ui-widgets/include",
    "lib/ui-cursor/include",
    "lib/ui-builder/include",
    "include"
)
includes("lib/*/xmake.lua")
includes("examples/*/xmake.lua")
includes("tests/xmake.lua")

option("ci-env", {showmenu = true, default = false})

option("enable-openmp", {showmenu = true, default = true})

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
    set_kind("binary")
    set_rundir("tests")
    add_files("tests/run_tests.c", "tests/cases/*.c")
    add_deps("ctest", "lcui")
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

target("lcui")
    set_kind("$(kind)")
    add_files(
        "src/*.c",
        "lib/worker/src/**.c",
        "lib/ui-builder/src/**.c",
        "lib/ui-cursor/src/**.c",
        "lib/ui-server/src/**.c",
        "lib/ui-widgets/src/**.c"
    )
    add_configfiles("src/config.h.in")
    set_configdir("include/LCUI")
    add_packages("libomp", "libxml2")
    add_options("enable-openmp")
    add_deps("yutil", "pandagl", "libcss", "libui", "libthread", "libtimer", "libplatform")

    if has_package("libomp") and has_config("enable-openmp") then
        set_configvar("ENABLE_OPENMP", 1)
    end

    if has_package("libxml2") then
        set_configvar("WITH_LIBXML2", 1)
    end

target("headers")
    set_kind("phony")
    set_default(false)
    before_build(function (target)
        -- Copy the header file of the internal library to the LCUI header file directory
        os.cp("$(projectdir)/lib/thread/include/*.h", "$(projectdir)/include/LCUI/")
        os.cp("$(projectdir)/lib/timer/include/*.h", "$(projectdir)/include/LCUI/")
        os.cp("$(projectdir)/lib/worker/include/*.h", "$(projectdir)/include/LCUI/")
    end)
