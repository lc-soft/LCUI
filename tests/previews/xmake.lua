local previews = {
    "flex_layout",
    "block_layout",
    "widget_opacity",
    "scrollbar"
}

for _, topic in ipairs(previews) do
    target("preview_" .. topic)
        set_default(false)
        set_kind("binary")
        set_group("previews")
        set_rundir("../fixtures")
        add_includedirs("../helpers")
        add_files("preview_" .. topic .. ".c",
                  "../scenes/" .. topic .. "_scene.c")
        add_deps("lcui")
end
