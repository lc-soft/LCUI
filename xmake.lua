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
        -- Copy the header file of the internal library to the LCUI header file directory
        os.cp("$(projectdir)/lib/util/include/*.h", "$(projectdir)/include/LCUI/util")
        os.cp("$(projectdir)/lib/thread/include/*.h", "$(projectdir)/include/LCUI/thread")
        os.cp("$(projectdir)/lib/css/include/*.h", "$(projectdir)/include/LCUI/css")
        os.cp("$(projectdir)/lib/font/include/*.h", "$(projectdir)/include/LCUI/font")
        os.cp("$(projectdir)/lib/paint/include/*.h", "$(projectdir)/include/LCUI/paint")
        os.cp("$(projectdir)/lib/image/include/*.h", "$(projectdir)/include/LCUI/image")
        os.cp("$(projectdir)/lib/ui/include/*.h", "$(projectdir)/include/LCUI")
        os.cp("$(projectdir)/lib/ui-widgets/include/*.h", "$(projectdir)/include/LCUI/ui")
        os.cp("$(projectdir)/lib/ui-anchor/include/*.h", "$(projectdir)/include/LCUI/ui")
        os.cp("$(projectdir)/lib/ui-builder/include/*.h", "$(projectdir)/include/LCUI/ui")
        os.cp("$(projectdir)/lib/platform/include/*.h", "$(projectdir)/include/LCUI/")
        os.cp("$(projectdir)/lib/text/include/*.h", "$(projectdir)/include/LCUI/")
        os.cp("$(projectdir)/lib/timer/include/*.h", "$(projectdir)/include/LCUI/")
        os.cp("$(projectdir)/lib/worker/include/*.h", "$(projectdir)/include/LCUI/")
    end)
