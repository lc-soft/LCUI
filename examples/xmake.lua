add_repositories("lcui-repo ../build")
add_requires("lcui")
add_rules("mode.debug", "mode.release")
if is_plat("windows") then
    add_rules("win.sdk.application")
end
includes("*/xmake.lua")
