set_project("lcui")
set_version("2.2.1")
add_rules("mode.debug", "mode.release", "c++.openmp", "mode.coverage")
add_includedirs("include", "include/LCUI")
add_rpathdirs("@loader_path/lib", "@loader_path")
add_defines("LCUI_EXPORTS", "UNICODE")
includes("lib/**/xmake.lua")
includes("test/xmake.lua")
set_warnings("all")
set_rundir("$(projectdir)/test")

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
        "lcui-ui",
        "lcui-ui-widgets",
        "lcui-ui-anchor",
        "lcui-ui-builder",
        "lcui-platform",
        "lcui-text",
        "lcui-timer",
        "lcui-worker"
    )
    before_build(function (target)
        os.cp("$(projectdir)/lib/*/include/*.h", "$(projectdir)/include/LCUI")
    end)
