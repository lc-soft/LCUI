target("doc-viewer")
    set_default(false)
    set_group("lcui-examples")
    set_prefixdir("doc-viewer", {bindir = "."})
    set_kind("binary")
    set_rundir("dist")
    add_deps("lcui")
    add_files("app/**.c")
    add_installfiles("dist/(assets/*)")
    after_build(function (target)
        local src = path.join(os.projectdir(), "docs", "examples", "assets", "attachment")
        local dst = path.join(os.projectdir(), "examples", "doc-viewer", "dist", "assets", "attachment")
        if os.isdir(src) then
            os.mkdir(dst)
            for _, file in ipairs(os.files(path.join(src, "*"))) do
                os.cp(file, dst)
            end
        end
    end)
    set_warnings("all")
