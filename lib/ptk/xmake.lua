includes("rules/xmake.lua")

local wayland_protocol_root = os.getenv("WAYLAND_PROTOCOLS_DIR") or "/usr/share/wayland-protocols"
local xdg_shell_xml = path.join(wayland_protocol_root, "stable", "xdg-shell", "xdg-shell.xml")
local xdg_decoration_xml = path.join(wayland_protocol_root, "unstable", "xdg-decoration", "xdg-decoration-unstable-v1.xml")
local viewporter_xml = path.join(wayland_protocol_root, "stable", "viewporter", "viewporter.xml")
local fractional_scale_xml = path.join(wayland_protocol_root, "staging", "fractional-scale", "fractional-scale-v1.xml")

if is_plat("linux") then
    add_requires("libx11", "wayland", "xkbcommon", "wayland-cursor", {optional = true})
end

option("enable-touch")
    set_showmenu(true)
    set_default(true)
    set_configvar("PTK_TOUCH_ENABLED", 1)

target("libptk")
    set_version("0.1.0-a")
    set_kind("$(kind)")
    add_files("src/*.c")
    set_configdir("include/ptk")
    add_configfiles("src/config.h.in")
    add_deps("yutil", "pandagl")
    add_headerfiles("include/ptk.h", "include/(ptk/*.h)")
    if is_kind("static") then
        set_configvar("PTK_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("PTK_DLL_EXPORT")
    end
    if is_plat("windows") then
        add_options("enable-touch")
        add_files("src/windows/*.c")
        add_links("Shell32")
    else
        add_files("src/linux/*.c")
        add_packages("libx11", "wayland", "xkbcommon", "wayland-cursor")
        if has_package("libx11") then
            set_configvar("PTK_HAS_LIBX11", 1)
        end
        if has_package("wayland") then
            add_rules("wayland.protocol")
            add_files(xdg_shell_xml, {rule = "wayland.protocol"})
            add_files(xdg_decoration_xml, {rule = "wayland.protocol"})
            add_files(viewporter_xml, {rule = "wayland.protocol"})
            add_files(fractional_scale_xml, {rule = "wayland.protocol"})
            set_configvar("PTK_HAS_WAYLAND", 1)
            add_files("src/linux/waylandapp_core.c")
            add_files("src/linux/waylandapp_input.c")
            add_files("src/linux/waylandapp_output.c")
            add_files("src/linux/waylandapp_window.c")
        end
        add_syslinks("pthread", "dl")
    end

target("libptk-tests")
    set_default(false)
    set_kind("binary")
    set_group("tests")
    set_rundir("tests")
    add_includedirs("src")
    add_files("tests/main.c", "tests/test_steptimer.c",
              "tests/test_timer_scheduler.c")
    add_deps("ctest", "libptk")
    add_rules("tests.runnable")
    add_tests("default", {group = "tests"})

