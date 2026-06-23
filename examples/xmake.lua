if is_plat("windows") and is_mode("release") then
    add_rules("win.sdk.application")
end
includes("*/xmake.lua")
