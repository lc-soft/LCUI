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
    set_group("tests")
    set_rundir("tests")
    add_files("tests/test.c")
    add_deps("ctest", "libi18n")
    add_rules("tests.runnable")
    add_tests("default", {group = "tests"})
