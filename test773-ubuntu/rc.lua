-- Standard awesome library
local gears = require("gears")
local awful = require("awful")
local autofocus = require("awful.autofocus")

-- Widget and layout library
local wibox = require("wibox")

-- Theme handling library
local beautiful = require("beautiful")

-- Notification library
local naughty = require("naughty")
local menubar = require("menubar")
local hotkeys_popup = require("awful.hotkeys_popup")

-- Mouse/key codes
local MOUSE_LEFT = 1
local MOUSE_RIGHT = 3
local MOUSE_SCROLL_UP = 4
local MOUSE_SCROLL_DOWN = 5

-- Themes define colours, icons, font and wallpapers.
beautiful.init(awful.util.getdir("config") .. "theme.lua")

-- This is used later as the default terminal to run.
terminal = os.getenv("XTERM") or "xterm"
launcher = "thingylaunch"

-- Use Windows key as the modifier.
modkey = "Mod4"

-- Table of layouts to cover with awful.layout.inc, order matters.
awful.layout.layouts = {
    awful.layout.suit.tile,
    awful.layout.suit.tile.left,
    awful.layout.suit.tile.bottom,
    awful.layout.suit.tile.top,
    awful.layout.suit.fair,
    awful.layout.suit.fair.horizontal,
    awful.layout.suit.spiral,
    awful.layout.suit.spiral.dwindle,
    awful.layout.suit.magnifier,
    awful.layout.suit.max,
    awful.layout.suit.max.fullscreen,
}


-- Tag list
local taglist_buttons = gears.table.join(
    -- Left-clicking a tag changes the view
    awful.button({ }, MOUSE_LEFT, function(t)
        t:view_only()
    end),

    -- Left-clicking a tag with mod key moves the focused window to the tag
    awful.button({ modkey }, MOUSE_LEFT, function(t)
        if client.focus then
            client.focus:move_to_tag(t)
        end
    end),

    -- Scrolling on the tag list rotates the view
    awful.button({ }, MOUSE_SCROLL_DOWN, function(t) awful.tag.viewnext(t.screen) end),
    awful.button({ }, MOUSE_SCROLL_UP, function(t) awful.tag.viewprev(t.screen) end)
)

-- Task list
local tasklist_buttons = gears.table.join(
    -- Left-clicking a task activates or minimizes the window
    awful.button({ }, MOUSE_LEFT, function(c)
        if c == client.focus then
            c.minimized = true
        else
            c:emit_signal(
                "request::activate",
                "tasklist",
                { raise = true }
            )
        end
    end)
)


-- Define screens
mytextclock = wibox.widget.textclock(" %F (%a) %H:%M ", 15)
mysystray = wibox.widget.systray()

awful.screen.connect_for_each_screen(function(s)
    -- Each screen has its own tag table.
    awful.tag({ "1", "2", "3", "4", "5", "6", "7", "8", "9" }, s, awful.layout.layouts[1])

    -- Create an imagebox widget which will contain an icon indicating which layout we're using.
    -- We need one layoutbox per screen.
    s.mylayoutbox = awful.widget.layoutbox(s)
    s.mylayoutbox:buttons(
        gears.table.join(
            awful.button({ }, MOUSE_LEFT, function() awful.layout.inc(1) end),
            awful.button({ }, MOUSE_RIGHT, function() awful.layout.inc(-1) end),
            awful.button({ }, MOUSE_SCROLL_DOWN, function() awful.layout.inc(1) end),
            awful.button({ }, MOUSE_SCROLL_UP, function() awful.layout.inc(-1) end)
        )
    )

    -- Create a taglist widget
    -- FIXME: Requires awesome 4.3. Works in disco, not in bionic.
    s.mytaglist = awful.widget.taglist {
        screen = s,
        filter = awful.widget.taglist.filter.all,
        buttons = taglist_buttons
    }

    -- Create a tasklist widget
    -- FIXME: Requires awesome 4.3. Works in disco, not in bionic.
    s.mytasklist = awful.widget.tasklist {
        screen = s,
        filter = awful.widget.tasklist.filter.currenttags,
        buttons = tasklist_buttons
    }

    -- Create the wibox
    s.mywibox = awful.wibar({ position = "top", screen = s })

    left_widgets = {
        layout = wibox.layout.fixed.horizontal,
        s.mytaglist,
        s.mypromptbox,
    }

    middle_widgets = s.mytasklist

    right_widgets = {
        layout = wibox.layout.fixed.horizontal,
        mysystray,
        mytextclock,
        s.mylayoutbox
    }

    -- Show systray only in the last screen
    mysystray:set_screen(s)

    s.mywibox:setup {
        layout = wibox.layout.align.horizontal, left_widgets, middle_widgets, right_widgets
    }
end)

-- Global key bindings
globalkeys = gears.table.join(
    -- Awesome control
    awful.key({ modkey, "Control" }, "r", awesome.restart, { description = "restart awesome", group = "awesome" }),
    awful.key({ modkey, "Shift" }, "q", awesome.quit, { description = "quit awesome", group = "awesome" }),

    -- Use j and k keys for moving around windows and screens
    awful.key({ modkey }, "j", function() awful.client.focus.byidx(1) end, { description = "focus next by index", group = "client" }),
    awful.key({ modkey }, "k", function() awful.client.focus.byidx(-1) end, { description = "focus previous by index", group = "client "}),
    awful.key({ modkey, "Shift" }, "j", function() awful.screen.focus_relative(1) end, { description = "focus the next screen", group = "screen" }),
    awful.key({ modkey, "Shift" }, "k", function() awful.screen.focus_relative(-1) end, { description = "focus the previous screen", group = "screen" }),

    -- Use arrow keys for moving around tags
    awful.key({ modkey }, "Left", function() awful.tag.viewprev() end, { description = "move to next tag", group = "tag" }),
    awful.key({ modkey }, "Right", function() awful.tag.viewnext() end, { description = "move to previous tag", group = "tag" }),

    -- Use space key to select layout
    awful.key({ modkey }, "space", function() awful.layout.inc(1) end, { description = "select next", group = "layout" }),
    awful.key({ modkey, "Shift" }, "space", function() awful.layout.inc(-1) end, { description = "select previous", group = "layout" }),

    -- Launcher
    awful.key({ modkey }, "Return", function() awful.spawn(terminal) end, { description = "open a terminal", group = "launcher" }),
    awful.key({ modkey }, "r", function() awful.spawn(launcher) end, { description = "open a launcher", group = "launcher" })
)

root.keys(globalkeys)

-- Client key bindings
clientkeys = gears.table.join(
    awful.key({ modkey }, "f", function (c) c.fullscreen = not c.fullscreen; c:raise() end, { description = "toggle fullscreen", group = "client" }),
    awful.key({ modkey, "Shift" }, "c", function (c) c:kill() end, { description = "close", group = "client" }),
    awful.key({ modkey, "Control" }, "space", function (c) awful.client.floating.toggle(c) end, { description = "toggle floating", group = "client" })
)

clientbuttons = gears.table.join(
    awful.button({ }, MOUSE_LEFT, function (c) c:emit_signal("request::activate", "mouse_click", { raise = true }) end),
    awful.button({ modkey }, MOUSE_LEFT, function (c) c:emit_signal("request::activate", "mouse_click", { raise = true }); awful.mouse.client.move(c) end),
    awful.button({ modkey }, MOUSE_RIGHT, function (c) c:emit_signal("request::activate", "mouse_click", { raise = true }); awful.mouse.client.resize(c) end)
)

-- Rules

awful.rules.rules = {
    -- Default properties.
    {
        rule = { },
        properties = {
            border_width = beautiful.border_width,
            border_color = beautiful.border_normal,
            focus = awful.client.focus.filter,
            raise = true,
            keys = clientkeys,
            buttons = clientbuttons,
            screen = awful.screen.preferred,
            placement = awful.placement.no_overlap + awful.placement.no_offscreen
        }
    },

    -- Make clients matching these rules floating by default.
    {
        rule_any = {
            instance = {
                "pinentry",
            },

            class = {
            },

            name = {
            },

            role = {
                "pop-up",
            }
        },
        properties = { floating = true }
    },

    -- Add titlebars to dialogs
    {
        rule_any = {
            type = { "dialog" }
        },
        properties = { titlebars_enabled = true }
    },
}

-- Signal function to execute when a new client appears.
client.connect_signal("manage", function (c)
    if awesome.startup
        and not c.size_hints.user_position
        and not c.size_hints.program_position then
        -- Prevent clients from being unreachable after screen count changes.
        awful.placement.no_offscreen(c)
    end
end)


-- Add a titlebar if titlebars_enabled is set to true in the rules.
client.connect_signal("request::titlebars", function(c)
    -- buttons for the titlebar
    local buttons = gears.table.join(
        awful.button({ }, MOUSE_LEFT, function()
            c:emit_signal("request::activate", "titlebar", { raise = true })
            awful.mouse.client.move(c)
        end),
        awful.button({ }, MOUSE_RIGHT, function()
            c:emit_signal("request::activate", "titlebar", { raise = true })
            awful.mouse.client.resize(c)
        end)
    )

    awful.titlebar(c) : setup {
        -- Left widgets
        {
            awful.titlebar.widget.iconwidget(c),
            buttons = buttons,
            layout = wibox.layout.fixed.horizontal
        },
        -- Middle widget
        {
            -- Title
            {
                align = "center",
                widget = awful.titlebar.widget.titlewidget(c)
            },
            buttons = buttons,
            layout = wibox.layout.flex.horizontal
        },
        -- Right widgets
        {
            awful.titlebar.widget.floatingbutton(c),
            awful.titlebar.widget.maximizedbutton(c),
            awful.titlebar.widget.stickybutton(c),
            awful.titlebar.widget.ontopbutton(c),
            awful.titlebar.widget.closebutton(c),
            layout = wibox.layout.fixed.horizontal()
        },
        layout = wibox.layout.align.horizontal
    }
end)

-- Enable sloppy focus, so that focus follows mouse.
client.connect_signal("mouse::enter", function(c)
    c:emit_signal("request::activate", "mouse_enter", { raise = false })
end)

-- Change border color of focused window.
client.connect_signal("focus", function(c) c.border_color = beautiful.border_focus end)
client.connect_signal("unfocus", function(c) c.border_color = beautiful.border_normal end)
