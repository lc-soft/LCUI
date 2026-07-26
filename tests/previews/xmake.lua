local previews = {
    "flex_layout",
    "block_layout",
    "widget_opacity",
    "scrollbar"
}

for _, topic in ipairs(previews) do
    target("preview-" .. topic:gsub("_", "-"))
        set_default(false)
        set_kind("binary")
        set_group("previews")
        set_rundir("../fixtures")
        add_includedirs("../helpers")
        add_files("preview_" .. topic .. ".c",
                  "../scenes/" .. topic .. "_scene.c")
        add_deps("lcui")
end

target("preview-flex-layout-percentage-wrap")
    set_default(false)
    set_kind("binary")
    set_group("previews")
    set_rundir("../fixtures")
    add_includedirs("../helpers")
    add_defines("LCUI_FLEX_LAYOUT_PERCENTAGE_WRAP_PREVIEW")
    add_files("preview_flex_layout_percentage_wrap.c",
              "../integration/test_flex_layout_percentage_wrap.c")
    add_deps("lcui")
