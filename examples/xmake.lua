add_repositories("lcui-repo ../build")
add_requires("lcui")
if is_plat("windows") then
    add_rules("win.sdk.application")
end
includes("*/xmake.lua")
