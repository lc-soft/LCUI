set_project("i18n")
set_version("0.1.0-a")
add_requires("libyaml", { optional = true })

target("libi18n")
    set_kind("$(kind)")
    add_files("src/*.c")
    add_deps("yutil")
    add_includedirs("include")
    set_configdir("include")
    add_configfiles("src/i18n.h.in")
    add_headerfiles("include/i18n.h")
    add_packages("libyaml")
    if has_package("libyaml") then
        set_configvar("LIBI18N_HAS_YAML", 1)
    end
    if is_kind("static") then
        set_configvar("LIBI18N_STATIC_BUILD", 1)
    elseif is_plat("windows") then
        add_defines("LIBI18N_DLL_EXPORT")
    end

target("libi18n-tests")
    set_default(false)
    set_kind("binary")
    add_files("tests/test.c")
    add_deps("libi18n", "ctest")
    set_rundir("tests")
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
                table.insert(argv, target:targetfile())
                os.execv("sudo", argv)
            end
        else
            os.execv(target:targetfile())
        end
    end)
