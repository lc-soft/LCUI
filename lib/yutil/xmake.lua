set_warnings("all", "error")

-- set language: c99
stdc = "c99"
set_languages(stdc)

-- disable some compiler errors
add_cxflags("-Wno-error=deprecated-declarations", "-fno-strict-aliasing", "-Wno-error=expansion-to-defined")

-- add build modes
add_rules("mode.release", "mode.debug")

add_defines("_UNICODE", "YUTIL_EXPORTS")

if is_plat("windows") then
    add_defines("_CRT_SECURE_NO_WARNINGS")
else
    add_defines("_GNU_SOURCE=1")
end

if is_mode("release") then
    set_symbols("none")
end

-- 'xmake f --optionname=test'
option("yutil_test")
    set_default(true)
    set_showmenu(true)
    set_category("test")
    set_description("Enable or disable yutil_test option")
option_end()

-- include project sources
includes("tests")

target("yutil")
    -- make as a static/shared library
    set_kind("$(kind)")
    if is_kind("shared") then
        -- export all symbols for windows/dll
        if is_plat("windows") then
            if is_mode("release") then
                set_optimize("fastest")
            end
            add_rules("utils.symbols.export_all")
        end
    else
        set_configvar("YUTIL_STATIC_BUILD", 1)
    end

    -- add include directories
    add_includedirs("include", {public = true})

    -- add the header files for installing
    add_headerfiles("include/yutil.h")
    add_headerfiles("include/(yutil/*.h)")
    set_configdir("include/yutil")
    add_configfiles("include/yutil/config.h.in")

    -- add the common source files
    add_files("src/*.c")
