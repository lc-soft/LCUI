set_project("lcui")
set_version("3.0.0-alpha.2")
set_languages("c11")
set_warnings("all", "error")
set_policy("package.requires_lock", true)
add_rules("mode.debug", "mode.release", "mode.coverage")
add_rpathdirs("@loader_path/lib", "@loader_path")
add_defines("UNICODE", "_CRT_SECURE_NO_WARNINGS", "YUTIL_EXPORTS")
if not is_plat("windows") then
    -- Strict C11 hides POSIX/GNU extensions behind feature macros.
    add_defines("_GNU_SOURCE=1")
end
add_includedirs(
    "lib/ctest/include",
    "lib/yutil/include",
    "lib/pandagl/include",
    "lib/thread/include",
    "lib/css/include",
    "lib/i18n/include",
    "lib/ptk/include",
    "lib/router/include",
    "lib/ui/include",
    "lib/ui-server/include",
    "lib/ui-xml/include",
    "include",
    {public = true}
)

option("memcheck")
    set_default(false)
    set_showmenu(true)
    set_description("Enable memory check tools (drmemory on Windows, valgrind on Linux)")
option_end()

option("merge-archive")
    set_default(false)
    set_showmenu(true)
    set_description("Merge the sub-library archives into lcui for standalone distribution")
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
includes("examples/xmake.lua")

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
        "pandagl",
        "libcss",
        "libi18n",
        "libui",
        "libui-server",
        "librouter",
        "libui-xml"
    )
    add_headerfiles("include/LCUI.h", "include/(LCUI/**.h)")
    -- lcui is a self-contained package: the umbrella header LCUI.h includes each
    -- sub-library's flat root header, so those headers must be installed together
    -- with lcui for consumers to be able to #include <LCUI.h>.
    -- Iterate over lib/* instead of listing every sub-library: the set of
    -- sub-libraries changes over time (ui-cursor, ui-router and worker were
    -- merged away) and the loop adapts on its own. A directory counts as a
    -- sub-library only when it has its own xmake.lua -- lib/ui-router is a
    -- leftover holding nothing but a generated config header.
    for _, subdir in ipairs(os.filedirs("lib/*")) do
        local incdir = subdir:gsub("\\", "/") .. "/include"
        if os.isfile(subdir .. "/xmake.lua") and os.isdir(incdir) then
            add_headerfiles(incdir .. "/(**.h)")
        end
    end
    -- Merge the sub-library archives into lcui.lib for releases that ship a
    -- single standalone library. Development and test builds keep the archives
    -- separate so add_deps() can export each sub-library's own links.
    if has_config("merge-archive") then
        set_policy("build.merge_archive", true)
    end
