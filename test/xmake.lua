includes("lib/ctest/xmake.lua")
add_deps("lcui")
add_includedirs("lib/ctest/include/")

target("helloworld")
    add_files("helloworld.c")

target("run-tests")
    set_kind("binary")
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
    add_files("run_tests.c", "cases/*.c")
    add_deps("ctest", "lcui")

target("test_block_layout")
    add_files("test_block_layout.c")
    add_deps("ctest")

target("test_border")
    add_files("test_border.c")
    add_deps("ctest")

target("test_box_shadow")
    add_files("test_box_shadow.c")

target("test_char_render")
    add_files("test_char_render.c")

target("test_fill_rect")
    add_files("test_fill_rect.c")
    add_deps("ctest")

target("test_fill_rect_with_rgba")
    add_files("test_fill_rect_with_rgba.c")

target("test_flex_layout")
    add_files("test_flex_layout.c")
    add_deps("ctest")

target("test_image_scaling_bench")
    add_files("test_image_scaling_bench.c")

target("test_mix_rect_with_opacity")
    add_files("test_mix_rect_with_opacity.c")

target("test_paint_background")
    add_files("test_paint_background.c")

target("test_paint_border")
    add_files("test_paint_border.c")

target("test_paint_boxshadow")
    add_files("test_paint_boxshadow.c")

target("test_pixel_manipulation")
    add_files("test_pixel_manipulation.c")

target("test_render")
    add_files("test_render.c")

target("test_scaling_support")
    add_files("test_scaling_support.c")

target("test_scrollbar")
    add_files("test_scrollbar.c")
    add_deps("ctest")

target("test_string_render")
    add_files("test_string_render.c")

target("test_textview_resize")
    add_files("test_textview_resize.c")
    add_deps("ctest")

target("test_touch")
    add_files("test_touch.c")

target("test_widget")
    add_files("test_widget.c")

target("test_widget_opacity")
    add_files("test_widget_opacity.c")
    add_deps("ctest")

target("test_widget_render")
    add_files("test_widget_render.c")
