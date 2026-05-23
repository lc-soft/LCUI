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
    "lib/router/include",
    "lib/ui/include",
    "lib/ui-server/include",
    "lib/ui-cursor/include",
    "lib/ui-xml/include",
    "lib/ui-router/include",
    "include",
    {public = true}
)

option("memcheck")
    set_default(false)
    set_showmenu(true)
    set_description("Enable memory check tools (drmemory on Windows, valgrind on Linux)")
option_end()

rule("tests.runnable")
    on_test(function (target, opt)
        import("core.base.option")
        opt = opt or {}
        local args = opt.runargs or {}
        local rundir = opt.rundir or target:rundir()
        local envs = opt.runenvs
        local exec_opt = {curdir = rundir, envs = envs}
        local exepath = path.absolute(target:targetfile())
        if has_config("memcheck") then
            local cmd
            if is_plat("windows") then
                cmd = {"drmemory", "--", exepath}
            else
                cmd = {"valgrind",
                       "--leak-check=full",
                       "--error-exitcode=42",
                       "--num-callers=20",
                       exepath}
            end
            for _, a in ipairs(args) do
                table.insert(cmd, a)
            end
            local ok = try { function ()
                os.execv(cmd[1], table.slice(cmd, 2), exec_opt)
                return true
            end }
            return ok ~= nil, ok == nil and "memcheck failed" or nil
        end
        local ok = try { function ()
            os.execv(exepath, args, exec_opt)
            return true
        end }
        return ok ~= nil, ok == nil and "test failed" or nil
    end)
rule_end()

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

target("lcui")
    set_kind("$(kind)")
    add_files("src/**.c")
    if is_kind("static") then
        set_configvar("LCUI_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LCUI_DLL_EXPORT")
    end
    if has_package("fontconfig") then
        add_defines("HAVE_FONTCONFIG")
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
        "libui-cursor",
        "libui-server",
        "librouter",
        "libui-router",
        "libui-xml"
    )
    add_headerfiles("include/LCUI.h", "include/(LCUI/**.h)")
