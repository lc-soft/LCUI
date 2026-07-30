rule("wayland.protocol")
    on_load(function(target)
        if target:sourcebatches()["wayland.protocol"] then
            local generatedir = path.join(target:autogendir(), "rules", "wayland")
            target:add("includedirs", generatedir)
        end
    end)

    before_buildcmd_file(function(target, batchcmds, sourcefile, opt)
        local generatedir = path.join(target:autogendir(), "rules", "wayland")
        local basename = path.basename(sourcefile)
        local headerfile = path.join(generatedir, basename .. "-client-protocol.h")
        local sourcefile_c = path.join(generatedir, basename .. "-protocol.c")
        local objectfile = target:objectfile(sourcefile_c)
        local scanner = os.getenv("WAYLAND_SCANNER") or "/usr/bin/wayland-scanner"
        local code_mode = target:kind() == "shared" and "public-code" or "private-code"

        table.insert(target:objectfiles(), objectfile)
        batchcmds:show_progress(opt.progress, "${color.build.object}wayland-scanner %s", sourcefile)
        batchcmds:mkdir(generatedir)
        batchcmds:vrunv(scanner, {"client-header", path(sourcefile), path(headerfile)})
        batchcmds:vrunv(scanner, {code_mode, path(sourcefile), path(sourcefile_c)})
        batchcmds:compile(sourcefile_c, objectfile)
        batchcmds:add_depfiles(sourcefile)
        batchcmds:set_depmtime(os.mtime(objectfile))
        batchcmds:set_depcache(target:dependfile(objectfile))
    end)
