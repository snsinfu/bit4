local gfs = require("gears.filesystem")
local themes_path = gfs.get_themes_dir()

-- inherit default theme
local theme = dofile(themes_path .. "default/theme.lua")

theme.font = "sans 10"

theme.bg_normal = "#1b1b2b"
theme.bg_focus = "#deeff9"
theme.bg_urgent = "#bb1111"
theme.bg_minimize = "#adacaf"
theme.bg_systray = theme.bg_normal

theme.fg_normal = "#aaaaaa"
theme.fg_focus = "#1b1b2b"
theme.fg_urgent = "#ffffff"
theme.fg_minimize = "#1b1b2b"

return theme
