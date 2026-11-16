--!nocheck
-- SHARDS.CC // CS:GO Styled Roblox UI Library (Gamesense / Skeet & Modern CS Cheat Aesthetic)
-- Features:
--   - Crisp 1px double-bordered dark matte panels + accent top bar
--   - Lucide icon support (built-in rbxassetid registry + custom asset fallback)
--   - Square CS:GO toggles that light up with gradient fill & outer glow + inline Keybind & Colorpicker attachments
--   - Smooth TweenService animations across every state transition
--   - Sliders (suffix, min/max/rounding, click & drag, inline value label)
--   - Dropdowns (Single, Multi-select, Searchable, Dynamic refresh, Player list mode)
--   - Scrollable Listbox, Buttons, Textboxes, Dividers, Labels, Sub-tabs
--   - Floating Watermark, Live Keybind List window, Toast Notifications, Theme & Config system

local TweenService = game:GetService("TweenService")
local UserInputService = game:GetService("UserInputService")
local RunService = game:GetService("RunService")
local Players = game:GetService("Players")
local HttpService = game:GetService("HttpService")
local CoreGui = game:GetService("CoreGui")
local TextService = game:GetService("TextService")

local LocalPlayer = Players.LocalPlayer

local Library = {
    Version = "2.4.0",
    Title = "shards.cc",
    ToggleKey = Enum.KeyCode.Insert,
    Open = true,
    Unloaded = false,
    Flags = {},
    Options = {},
    Connections = {},
    ThemeObjects = {},
    TextObjects = {},
    LayoutRefreshers = {},
    ActiveKeybinds = {},
    Notifications = {},
    OpenPopups = {},
    Folder = "ShardsUI",
    Font = Enum.Font.Arcade,
    FontBold = Enum.Font.Arcade,
    FontScale = 0.85,
    TextStroke = true,
    Fonts = {
        ["Pixelated (CS Bitmap)"] = { Font = Enum.Font.Arcade,     Bold = Enum.Font.Arcade,     Scale = 0.85 },
        ["CS2 Crisp (RobotoMono)"] = { Font = Enum.Font.RobotoMono, Bold = Enum.Font.RobotoMono, Scale = 1.00 },
        ["Skeet Mono (Code)"]      = { Font = Enum.Font.Code,       Bold = Enum.Font.GothamBold, Scale = 1.00 },
        ["Classic (Tahoma)"]       = { Font = Enum.Font.Legacy,     Bold = Enum.Font.Legacy,     Scale = 0.92 },
    },
    Theme = {
        Background       = Color3.fromRGB(11, 11, 14),
        Sidebar          = Color3.fromRGB(14, 14, 18),
        Groupbox         = Color3.fromRGB(16, 16, 21),
        Element          = Color3.fromRGB(22, 22, 29),
        ElementHover     = Color3.fromRGB(28, 28, 37),
        OuterBorder      = Color3.fromRGB(5, 5, 7),
        InnerBorder      = Color3.fromRGB(38, 38, 48),
        HighlightBorder  = Color3.fromRGB(58, 58, 72),
        Accent           = Color3.fromRGB(155, 195, 55), -- Classic CS:GO Skeet lime
        AccentDark       = Color3.fromRGB(98, 128, 28),
        Text             = Color3.fromRGB(228, 228, 235),
        TextDim          = Color3.fromRGB(142, 142, 156),
        TextMuted        = Color3.fromRGB(92, 92, 106),
        Risky            = Color3.fromRGB(235, 75, 75),
        Success          = Color3.fromRGB(110, 215, 95),
        Warning          = Color3.fromRGB(240, 180, 65),
    },
    Presets = {
        ["Skeet Lime"]   = { Accent = Color3.fromRGB(155, 195, 55),  AccentDark = Color3.fromRGB(98, 128, 28)  },
        ["Neverlose"]    = { Accent = Color3.fromRGB(55, 165, 245),  AccentDark = Color3.fromRGB(25, 105, 175) },
        ["Fatality"]     = { Accent = Color3.fromRGB(225, 45, 95),   AccentDark = Color3.fromRGB(145, 20, 58)  },
        ["Onetap"]       = { Accent = Color3.fromRGB(235, 155, 45),  AccentDark = Color3.fromRGB(165, 98, 18)  },
        ["Primordial"]   = { Accent = Color3.fromRGB(185, 115, 245), AccentDark = Color3.fromRGB(115, 62, 168) },
        ["Aimware"]      = { Accent = Color3.fromRGB(220, 50, 50),   AccentDark = Color3.fromRGB(140, 25, 25)  },
    }
}

-- ============================================================================
-- LUCIDE ICON REGISTRY (Built-in Lucide sprite IDs + custom asset fallback)
-- ============================================================================
local LucideIcons = {
    ["crosshair"]       = "rbxassetid://10709818534",
    ["target"]          = "rbxassetid://10734977012",
    ["eye"]             = "rbxassetid://10723346959",
    ["eye-off"]         = "rbxassetid://10723346871",
    ["shield"]          = "rbxassetid://10734951847",
    ["shield-alert"]    = "rbxassetid://10734951173",
    ["zap"]             = "rbxassetid://10747384394",
    ["sliders"]         = "rbxassetid://10734963400",
    ["settings"]        = "rbxassetid://10734950309",
    ["user"]            = "rbxassetid://10747373176",
    ["users"]           = "rbxassetid://10747373426",
    ["skull"]           = "rbxassetid://10734962068",
    ["flame"]           = "rbxassetid://10723376114",
    ["swords"]          = "rbxassetid://10734975692",
    ["sword"]           = "rbxassetid://10734975486",
    ["box"]             = "rbxassetid://10709782497",
    ["layers"]          = "rbxassetid://10723424505",
    ["compass"]         = "rbxassetid://10709811445",
    ["globe"]           = "rbxassetid://10723404337",
    ["cpu"]             = "rbxassetid://10709818399",
    ["terminal"]        = "rbxassetid://10734982144",
    ["code"]            = "rbxassetid://10709810463",
    ["folder"]          = "rbxassetid://10723387563",
    ["save"]            = "rbxassetid://10734941499",
    ["trash"]           = "rbxassetid://10747362393",
    ["refresh-cw"]      = "rbxassetid://10734933222",
    ["search"]          = "rbxassetid://10734943674",
    ["check"]           = "rbxassetid://10709790644",
    ["x"]               = "rbxassetid://10747384394",
    ["chevron-down"]    = "rbxassetid://10709790948",
    ["chevron-up"]      = "rbxassetid://10709791523",
    ["chevron-right"]   = "rbxassetid://10709791437",
    ["lock"]            = "rbxassetid://10723434711",
    ["unlock"]          = "rbxassetid://10747366027",
    ["bell"]            = "rbxassetid://10709775704",
    ["info"]            = "rbxassetid://10723415903",
    ["alert-triangle"]  = "rbxassetid://10709753149",
    ["palette"]         = "rbxassetid://10734910430",
    ["keyboard"]        = "rbxassetid://10723416765",
    ["mouse-pointer"]   = "rbxassetid://10734898592",
    ["activity"]        = "rbxassetid://10709752035",
    ["gauge"]           = "rbxassetid://10723395708",
    ["sparkles"]        = "rbxassetid://10734966248",
    ["wand"]            = "rbxassetid://10747376565",
    ["radar"]           = "rbxassetid://10734930886",
}

function Library:GetIcon(name)
    if not name or name == "" then return "" end
    if string.find(name, "rbxassetid://") or string.find(name, "http") then
        return name
    end
    local key = string.lower(name)
    return LucideIcons[key] or LucideIcons["sliders"]
end

function Library:RegisterIcon(name, assetId)
    LucideIcons[string.lower(name)] = assetId
end

-- ============================================================================
-- SIGNAL & UTILITY HELPERS
-- ============================================================================
local Signal = {}
Signal.__index = Signal

function Signal.new()
    return setmetatable({ _listeners = {} }, Signal)
end

function Signal:Connect(fn)
    local conn = { Connected = true, _fn = fn }
    table.insert(self._listeners, conn)
    function conn:Disconnect()
        self.Connected = false
    end
    return conn
end

function Signal:Fire(...)
    for i = #self._listeners, 1, -1 do
        local conn = self._listeners[i]
        if conn.Connected then
            task.spawn(conn._fn, ...)
        else
            table.remove(self._listeners, i)
        end
    end
end

local TweenFast   = TweenInfo.new(0.14, Enum.EasingStyle.Quart, Enum.EasingDirection.Out)
local TweenMedium = TweenInfo.new(0.22, Enum.EasingStyle.Quart, Enum.EasingDirection.Out)
local TweenSmooth = TweenInfo.new(0.30, Enum.EasingStyle.Quint, Enum.EasingDirection.Out)

function Library:Tween(instance, info, props)
    local tw = TweenService:Create(instance, info or TweenFast, props)
    tw:Play()
    return tw
end

function Library:Connect(signal, callback)
    local conn = signal:Connect(callback)
    table.insert(self.Connections, conn)
    return conn
end

function Library:Create(className, props, children)
    local inst = Instance.new(className)
    local isText = inst:IsA("TextLabel") or inst:IsA("TextButton") or inst:IsA("TextBox")
    local rawSize = props and props.TextSize or 12
    local rawFont = props and props.Font

    for k, v in pairs(props or {}) do
        inst[k] = v
    end

    if isText then
        local isBold = (rawFont == Enum.Font.GothamBold)
        inst.Font = isBold and self.FontBold or self.Font
        inst.TextSize = math.max(8, math.floor(rawSize * self.FontScale + 0.5))
        if self.TextStroke and (not props or props.TextStrokeTransparency == nil) then
            inst.TextStrokeColor3 = Color3.fromRGB(0, 0, 0)
            inst.TextStrokeTransparency = 0.18
        end
        table.insert(self.TextObjects, {
            Instance = inst,
            BaseSize = rawSize,
            IsBold = isBold,
        })
    end

    for _, child in ipairs(children or {}) do
        child.Parent = inst
    end
    return inst
end

function Library:GetTextBounds(text, baseSize, maxVec)
    local scaledSize = math.max(8, math.floor((baseSize or 12) * self.FontScale + 0.5))
    local ok, res = pcall(function()
        return TextService:GetTextSize(tostring(text or ""), scaledSize, self.Font, maxVec or Vector2.new(500, 200))
    end)
    return ok and res or Vector2.new(#tostring(text or "") * 7, scaledSize + 2)
end

function Library:SetFontPreset(presetName)
    local preset = self.Fonts[presetName]
    if not preset then return end
    self.Font = preset.Font
    self.FontBold = preset.Bold
    self.FontScale = preset.Scale
    for i = #self.TextObjects, 1, -1 do
        local entry = self.TextObjects[i]
        if entry.Instance and entry.Instance.Parent then
            entry.Instance.Font = entry.IsBold and self.FontBold or self.Font
            entry.Instance.TextSize = math.max(8, math.floor(entry.BaseSize * self.FontScale + 0.5))
        else
            table.remove(self.TextObjects, i)
        end
    end
    for _, fn in ipairs(self.LayoutRefreshers) do
        pcall(fn)
    end
end

function Library:BindTheme(instance, property, themeKey)
    table.insert(self.ThemeObjects, {
        Instance = instance,
        Property = property,
        Key = themeKey,
    })
    instance[property] = self.Theme[themeKey]
end

function Library:UpdateTheme()
    for i = #self.ThemeObjects, 1, -1 do
        local entry = self.ThemeObjects[i]
        if entry.Instance and entry.Instance.Parent then
            Library:Tween(entry.Instance, TweenFast, {
                [entry.Property] = self.Theme[entry.Key]
            })
        else
            table.remove(self.ThemeObjects, i)
        end
    end
    for _, opt in pairs(self.Options) do
        if opt.RefreshTheme then
            opt:RefreshTheme()
        end
    end
end

function Library:SetAccent(accent, accentDark)
    self.Theme.Accent = accent
    self.Theme.AccentDark = accentDark or Color3.new(
        math.clamp(accent.R * 0.62, 0, 1),
        math.clamp(accent.G * 0.62, 0, 1),
        math.clamp(accent.B * 0.62, 0, 1)
    )
    self:UpdateTheme()
end

function Library:ClosePopups(except)
    for _, popup in ipairs(self.OpenPopups) do
        if popup ~= except and popup.Close then
            popup:Close()
        end
    end
end

-- Applies the signature CS:GO double 1px border (outer black border + inner subtle border)
function Library:ApplyCSBorder(frame, highlightKey)
    frame.BorderSizePixel = 0
    local innerStroke = self:Create("UIStroke", {
        Name = "InnerStroke",
        Color = self.Theme[highlightKey or "InnerBorder"],
        Thickness = 1,
        ApplyStrokeMode = Enum.ApplyStrokeMode.Border,
        Parent = frame,
    })
    self:BindTheme(innerStroke, "Color", highlightKey or "InnerBorder")

    local outerWrap = self:Create("Frame", {
        Name = "OuterBorder",
        BackgroundTransparency = 1,
        Active = false,
        Position = UDim2.new(0, -1, 0, -1),
        Size = UDim2.new(1, 2, 1, 2),
        ZIndex = math.max(1, frame.ZIndex - 1),
        Parent = frame,
    })
    pcall(function() outerWrap.Interactable = false end)
    local blackStroke = self:Create("UIStroke", {
        Color = self.Theme.OuterBorder,
        Thickness = 1,
        ApplyStrokeMode = Enum.ApplyStrokeMode.Border,
        Parent = outerWrap,
    })
    self:BindTheme(blackStroke, "Color", "OuterBorder")
    return innerStroke, blackStroke
end

-- Creates the signature CS:GO top accent gradient bar (Skeet / Gamesense style)
function Library:CreateAccentBar(parent, zIndex)
    local bar = self:Create("Frame", {
        Name = "AccentBar",
        BackgroundColor3 = Color3.new(1, 1, 1),
        BorderSizePixel = 0,
        Size = UDim2.new(1, 0, 0, 2),
        ZIndex = zIndex or 5,
        Parent = parent,
    })
    local grad = self:Create("UIGradient", {
        Color = ColorSequence.new({
            ColorSequenceKeypoint.new(0.00, self.Theme.AccentDark),
            ColorSequenceKeypoint.new(0.50, self.Theme.Accent),
            ColorSequenceKeypoint.new(1.00, self.Theme.AccentDark),
        }),
        Parent = bar,
    })
    table.insert(self.Options, {
        RefreshTheme = function()
            if grad and grad.Parent then
                grad.Color = ColorSequence.new({
                    ColorSequenceKeypoint.new(0.00, self.Theme.AccentDark),
                    ColorSequenceKeypoint.new(0.50, self.Theme.Accent),
                    ColorSequenceKeypoint.new(1.00, self.Theme.AccentDark),
                })
            end
        end
    })
    return bar, grad
end

function Library:MakeDraggable(dragHandle, targetFrame)
    local dragging = false
    local dragStart, startPos

    self:Connect(dragHandle.InputBegan, function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            dragging = true
            dragStart = input.Position
            startPos = targetFrame.Position
            self:ClosePopups()
        end
    end)

    self:Connect(UserInputService.InputChanged, function(input)
        if dragging and input.UserInputType == Enum.UserInputType.MouseMovement then
            local delta = input.Position - dragStart
            Library:Tween(targetFrame, TweenInfo.new(0.06, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {
                Position = UDim2.new(
                    startPos.X.Scale,
                    startPos.X.Offset + delta.X,
                    startPos.Y.Scale,
                    startPos.Y.Offset + delta.Y
                )
            })
        end
    end)

    self:Connect(UserInputService.InputEnded, function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            dragging = false
        end
    end)
end

function Library:FormatKey(key)
    if not key or key == Enum.KeyCode.Unknown then return "NONE" end
    local name = typeof(key) == "EnumItem" and key.Name or tostring(key)
    local map = {
        RightShift = "RSHIFT", LeftShift = "LSHIFT",
        RightControl = "RCTRL", LeftControl = "LCTRL",
        RightAlt = "RALT", LeftAlt = "LALT",
        MouseButton1 = "MB1", MouseButton2 = "MB2", MouseButton3 = "MB3",
        Insert = "INS", Delete = "DEL", Backspace = "BACK",
        Space = "SPACE", Escape = "ESC", Return = "ENTER",
    }
    return map[name] or string.upper(name)
end

-- ============================================================================
-- SCREEN GUI & OVERLAYS (Tooltip, Notifications, Watermark, Keybind List)
-- ============================================================================
local function GetSafeParent()
    local ok, gethuiFn = pcall(function() return gethui end)
    if ok and type(gethuiFn) == "function" then
        local s, res = pcall(gethuiFn)
        if s and res then return res end
    end
    local okCore = pcall(function()
        local probe = Instance.new("Folder")
        probe.Parent = CoreGui
        probe:Destroy()
    end)
    if okCore then return CoreGui end
    if not LocalPlayer then
        Players:GetPropertyChangedSignal("LocalPlayer"):Wait()
        LocalPlayer = Players.LocalPlayer
    end
    return LocalPlayer and LocalPlayer:WaitForChild("PlayerGui")
end

local SafeParent = GetSafeParent()
if SafeParent then
    local oldGui = SafeParent:FindFirstChild("ShardsCSGO_UI")
    if oldGui then
        pcall(function() oldGui:Destroy() end)
    end
end

local ScreenGui = Library:Create("ScreenGui", {
    Name = "ShardsCSGO_UI",
    ResetOnSpawn = false,
    IgnoreGuiInset = true,
    ZIndexBehavior = Enum.ZIndexBehavior.Sibling,
    DisplayOrder = 999,
    Parent = SafeParent,
})
Library.ScreenGui = ScreenGui

-- Tooltip Frame
local TooltipFrame = Library:Create("Frame", {
    Name = "Tooltip",
    BackgroundColor3 = Library.Theme.Background,
    Size = UDim2.fromOffset(0, 22),
    Visible = false,
    ZIndex = 500,
    Parent = ScreenGui,
})
Library:BindTheme(TooltipFrame, "BackgroundColor3", "Background")
Library:ApplyCSBorder(TooltipFrame, "HighlightBorder")

local TooltipAccent = Library:Create("Frame", {
    BackgroundColor3 = Library.Theme.Accent,
    BorderSizePixel = 0,
    Size = UDim2.new(0, 2, 1, 0),
    ZIndex = 501,
    Parent = TooltipFrame,
})
Library:BindTheme(TooltipAccent, "BackgroundColor3", "Accent")

local TooltipLabel = Library:Create("TextLabel", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(8, 0),
    Size = UDim2.new(1, -12, 1, 0),
    Font = Enum.Font.Code,
    Text = "",
    TextColor3 = Library.Theme.Text,
    TextSize = 12,
    TextXAlignment = Enum.TextXAlignment.Left,
    ZIndex = 501,
    Parent = TooltipFrame,
})
Library:BindTheme(TooltipLabel, "TextColor3", "Text")

function Library:AddTooltip(target, text)
    if not text or text == "" then return end
    self:Connect(target.MouseEnter, function()
        local bounds = self:GetTextBounds(text, 12, Vector2.new(500, 100))
        TooltipLabel.Text = text
        TooltipFrame.Size = UDim2.fromOffset(bounds.X + 18, 22)
        TooltipFrame.Visible = true
    end)
    self:Connect(target.MouseLeave, function()
        TooltipFrame.Visible = false
    end)
end

Library:Connect(UserInputService.InputChanged, function(input)
    if TooltipFrame.Visible and input.UserInputType == Enum.UserInputType.MouseMovement then
        local pos = UserInputService:GetMouseLocation()
        TooltipFrame.Position = UDim2.fromOffset(pos.X + 14, pos.Y + 14)
    end
end)

-- Notification Holder (Top-Right CS:GO event log / toast style)
local NotifHolder = Library:Create("Frame", {
    Name = "Notifications",
    BackgroundTransparency = 1,
    Position = UDim2.new(1, -310, 0, 48),
    Size = UDim2.new(0, 295, 1, -60),
    ZIndex = 400,
    Parent = ScreenGui,
})
Library:Create("UIListLayout", {
    Padding = UDim.new(0, 8),
    HorizontalAlignment = Enum.HorizontalAlignment.Right,
    SortOrder = Enum.SortOrder.LayoutOrder,
    Parent = NotifHolder,
})

function Library:Notify(cfg)
    cfg = type(cfg) == "string" and { Description = cfg } or (cfg or {})
    local title = cfg.Title or self.Title
    local desc = cfg.Description or cfg.Text or ""
    local duration = cfg.Duration or 4
    local iconName = cfg.Icon or "bell"
    local barColor = cfg.Color or self.Theme.Accent

    local textBounds = self:GetTextBounds(desc, 12, Vector2.new(240, 200))
    local height = math.max(46, textBounds.Y + 28)

    local wrap = self:Create("Frame", {
        BackgroundTransparency = 1,
        Size = UDim2.fromOffset(290, height),
        Parent = NotifHolder,
    })

    local card = self:Create("Frame", {
        BackgroundColor3 = self.Theme.Background,
        Position = UDim2.fromOffset(320, 0),
        Size = UDim2.fromScale(1, 1),
        ZIndex = 401,
        Parent = wrap,
    })
    self:BindTheme(card, "BackgroundColor3", "Background")
    self:ApplyCSBorder(card, "InnerBorder")

    local sideBar = self:Create("Frame", {
        BackgroundColor3 = barColor,
        BorderSizePixel = 0,
        Size = UDim2.new(0, 2, 1, 0),
        ZIndex = 402,
        Parent = card,
    })

    local icon = self:Create("ImageLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(10, 8),
        Size = UDim2.fromOffset(14, 14),
        Image = self:GetIcon(iconName),
        ImageColor3 = barColor,
        ZIndex = 402,
        Parent = card,
    })

    local titleLbl = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(28, 6),
        Size = UDim2.new(1, -36, 0, 16),
        Font = Enum.Font.GothamBold,
        Text = string.upper(title),
        TextColor3 = self.Theme.Text,
        TextSize = 11,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 402,
        Parent = card,
    })

    local descLbl = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(10, 23),
        Size = UDim2.new(1, -18, 1, -28),
        Font = Enum.Font.Code,
        Text = desc,
        TextColor3 = self.Theme.TextDim,
        TextSize = 12,
        TextWrapped = true,
        TextXAlignment = Enum.TextXAlignment.Left,
        TextYAlignment = Enum.TextYAlignment.Top,
        ZIndex = 402,
        Parent = card,
    })

    local progBg = self:Create("Frame", {
        BackgroundColor3 = self.Theme.Element,
        BorderSizePixel = 0,
        Position = UDim2.new(0, 2, 1, -2),
        Size = UDim2.new(1, -2, 0, 2),
        ZIndex = 402,
        Parent = card,
    })
    local progFill = self:Create("Frame", {
        BackgroundColor3 = barColor,
        BorderSizePixel = 0,
        Size = UDim2.fromScale(1, 1),
        ZIndex = 403,
        Parent = progBg,
    })

    self:Tween(card, TweenMedium, { Position = UDim2.fromOffset(0, 0) })
    self:Tween(progFill, TweenInfo.new(duration, Enum.EasingStyle.Linear), { Size = UDim2.fromScale(0, 1) })

    task.delay(duration, function()
        if not card or not card.Parent then return end
        self:Tween(card, TweenMedium, { Position = UDim2.fromOffset(330, 0) })
        task.wait(0.24)
        if wrap and wrap.Parent then wrap:Destroy() end
    end)
end

-- ============================================================================
-- FLOATING WATERMARK & KEYBIND LIST (Classic CS:GO HUD panels)
-- ============================================================================
function Library:CreateWatermark(customTitle)
    local wmFrame = self:Create("Frame", {
        Name = "Watermark",
        AnchorPoint = Vector2.new(1, 0),
        BackgroundColor3 = self.Theme.Background,
        Position = UDim2.new(1, -14, 0, 12),
        Size = UDim2.fromOffset(280, 24),
        ZIndex = 300,
        Parent = ScreenGui,
    })
    self:BindTheme(wmFrame, "BackgroundColor3", "Background")
    self:ApplyCSBorder(wmFrame, "InnerBorder")
    self:CreateAccentBar(wmFrame, 302)
    self:MakeDraggable(wmFrame, wmFrame)

    local wmIcon = self:Create("ImageLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(7, 6),
        Size = UDim2.fromOffset(13, 13),
        Image = self:GetIcon("activity"),
        ImageColor3 = self.Theme.Accent,
        ZIndex = 302,
        Parent = wmFrame,
    })
    self:BindTheme(wmIcon, "ImageColor3", "Accent")

    local wmText = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(25, 2),
        Size = UDim2.new(1, -30, 1, -2),
        Font = Enum.Font.Code,
        Text = "",
        TextColor3 = self.Theme.Text,
        TextSize = 12,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 302,
        Parent = wmFrame,
    })
    self:BindTheme(wmText, "TextColor3", "Text")

    local fps = 60
    local accum = 0
    local frames = 0
    local baseName = (type(customTitle) == "table" and (customTitle.Title or customTitle.Name or customTitle.Text))
        or customTitle
        or self.Title

    self:Connect(RunService.RenderStepped, function(dt)
        frames += 1
        accum += dt
        if accum >= 0.5 then
            fps = math.floor(frames / accum)
            frames = 0
            accum = 0
            local ping = 0
            pcall(function()
                ping = math.floor( game:GetService("Stats").Network.ServerStatsItem["Data Ping"]:GetValue() )
            end)
            local str = string.format("%s | %s | %d fps | %d ms | %s",
                baseName,
                LocalPlayer and LocalPlayer.Name or "user",
                fps,
                ping,
                os.date("%H:%M:%S")
            )
            wmText.Text = str
            local bounds = self:GetTextBounds(str, 12, Vector2.new(700, 24))
            wmFrame.Size = UDim2.fromOffset(bounds.X + 38, 24)
        end
    end)

    local wmObj = { Frame = wmFrame }
    function wmObj:SetVisible(v)
        wmFrame.Visible = v
    end
    function wmObj:SetTitle(newTitle)
        baseName = tostring(newTitle or Library.Title)
    end
    self.Watermark = wmObj
    return wmObj
end

function Library:CreateKeybindList()
    local kbFrame = self:Create("Frame", {
        Name = "KeybindList",
        BackgroundColor3 = self.Theme.Background,
        Position = UDim2.new(0, 16, 0.45, 0),
        Size = UDim2.fromOffset(185, 28),
        ZIndex = 280,
        Parent = ScreenGui,
    })
    self:BindTheme(kbFrame, "BackgroundColor3", "Background")
    self:ApplyCSBorder(kbFrame, "InnerBorder")
    self:CreateAccentBar(kbFrame, 282)
    self:MakeDraggable(kbFrame, kbFrame)

    local hdrIcon = self:Create("ImageLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(8, 7),
        Size = UDim2.fromOffset(13, 13),
        Image = self:GetIcon("keyboard"),
        ImageColor3 = self.Theme.Accent,
        ZIndex = 282,
        Parent = kbFrame,
    })
    self:BindTheme(hdrIcon, "ImageColor3", "Accent")

    local hdrTitle = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(26, 2),
        Size = UDim2.new(1, -30, 0, 22),
        Font = Enum.Font.GothamBold,
        Text = "KEYBINDS",
        TextColor3 = self.Theme.Text,
        TextSize = 11,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 282,
        Parent = kbFrame,
    })
    self:BindTheme(hdrTitle, "TextColor3", "Text")

    local sep = self:Create("Frame", {
        BackgroundColor3 = self.Theme.InnerBorder,
        BorderSizePixel = 0,
        Position = UDim2.fromOffset(0, 25),
        Size = UDim2.new(1, 0, 0, 1),
        ZIndex = 282,
        Parent = kbFrame,
    })
    self:BindTheme(sep, "BackgroundColor3", "InnerBorder")

    local listHolder = self:Create("Frame", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(6, 28),
        Size = UDim2.new(1, -12, 0, 0),
        ZIndex = 282,
        Parent = kbFrame,
    })
    local layout = self:Create("UIListLayout", {
        Padding = UDim.new(0, 3),
        SortOrder = Enum.SortOrder.LayoutOrder,
        Parent = listHolder,
    })

    local kbObj = { Frame = kbFrame }
    function kbObj:Refresh()
        for _, ch in ipairs(listHolder:GetChildren()) do
            if ch:IsA("Frame") then ch:Destroy() end
        end
        local count = 0
        local maxW = 185
        for _, item in pairs(Library.ActiveKeybinds) do
            if item.Active and item.Key and item.Key ~= Enum.KeyCode.Unknown then
                count += 1
                local modeStr = "[" .. string.lower(item.Mode or "toggle") .. "]"
                local nameW = Library:GetTextBounds(item.Name, 12, Vector2.new(300, 20)).X
                local modeW = Library:GetTextBounds(modeStr, 11, Vector2.new(150, 20)).X
                maxW = math.max(maxW, nameW + modeW + 28)
                local row = Library:Create("Frame", {
                    BackgroundTransparency = 1,
                    Size = UDim2.new(1, 0, 0, 16),
                    ZIndex = 283,
                    Parent = listHolder,
                })
                Library:Create("TextLabel", {
                    BackgroundTransparency = 1,
                    Size = UDim2.new(0.68, 0, 1, 0),
                    Font = Enum.Font.Code,
                    Text = item.Name,
                    TextColor3 = Library.Theme.Text,
                    TextSize = 12,
                    TextXAlignment = Enum.TextXAlignment.Left,
                    ZIndex = 283,
                    Parent = row,
                })
                Library:Create("TextLabel", {
                    BackgroundTransparency = 1,
                    Position = UDim2.fromScale(0.68, 0),
                    Size = UDim2.new(0.32, 0, 1, 0),
                    Font = Enum.Font.Code,
                    Text = modeStr,
                    TextColor3 = Library.Theme.Accent,
                    TextSize = 11,
                    TextXAlignment = Enum.TextXAlignment.Right,
                    ZIndex = 283,
                    Parent = row,
                })
            end
        end
        local targetH = 30 + (count > 0 and (count * 19 + 4) or 0)
        Library:Tween(kbFrame, TweenFast, { Size = UDim2.fromOffset(maxW, targetH) })
    end

    function kbObj:SetVisible(v)
        kbFrame.Visible = v
    end

    self.KeybindList = kbObj
    return kbObj
end

-- ============================================================================
-- INLINE ATTACHMENTS (Keybind Binder & HSV/Alpha/Rainbow CS:GO ColorPicker)
-- ============================================================================
function Library:CreateKeybindAttachment(parentRow, cfg, toggleLink)
    cfg = cfg or {}
    local flag = cfg.Flag or (cfg.Name and ("KB_" .. cfg.Name)) or ("KB_" .. tostring(math.random(1000, 9999)))
    local mode = cfg.Mode or "Toggle" -- "Always", "Hold", "Toggle"
    local currentKey = cfg.Default or Enum.KeyCode.Unknown
    local binding = false
    local active = (mode == "Always")

    local Keybind = {
        Type = "Keybind",
        Name = cfg.Name or (toggleLink and toggleLink.Name) or "Keybind",
        Value = currentKey,
        Mode = mode,
        Active = active,
        Changed = Signal.new(),
    }

    local btn = self:Create("TextButton", {
        Name = "KeybindBtn",
        BackgroundColor3 = self.Theme.Element,
        AutoButtonColor = false,
        Size = UDim2.fromOffset(44, 15),
        Font = Enum.Font.Code,
        Text = "[" .. self:FormatKey(currentKey) .. "]",
        TextColor3 = self.Theme.TextDim,
        TextSize = 11,
        ZIndex = 12,
        Parent = parentRow,
    })
    self:BindTheme(btn, "BackgroundColor3", "Element")
    self:ApplyCSBorder(btn, "InnerBorder")

    local function UpdateButtonSize()
        local txt = binding and "[...]" or ("[" .. self:FormatKey(currentKey) .. "]")
        btn.Text = txt
        local b = self:GetTextBounds(txt, 11, Vector2.new(200, 20))
        btn.Size = UDim2.fromOffset(math.max(36, b.X + 10), 15)
    end
    UpdateButtonSize()
    table.insert(self.LayoutRefreshers, UpdateButtonSize)

    -- Mode Popup (Hold / Toggle / Always)
    local modePopup = self:Create("Frame", {
        BackgroundColor3 = self.Theme.Background,
        Size = UDim2.fromOffset(78, 60),
        Visible = false,
        ZIndex = 350,
        Parent = ScreenGui,
    })
    self:BindTheme(modePopup, "BackgroundColor3", "Background")
    self:ApplyCSBorder(modePopup, "HighlightBorder")

    self:Create("UIListLayout", {
        SortOrder = Enum.SortOrder.LayoutOrder,
        Parent = modePopup,
    })

    local popupCtrl = {
        Close = function()
            modePopup.Visible = false
        end
    }
    table.insert(self.OpenPopups, popupCtrl)

    local modes = { "Hold", "Toggle", "Always" }
    local modeButtons = {}
    for _, m in ipairs(modes) do
        local mBtn = self:Create("TextButton", {
            BackgroundColor3 = self.Theme.Background,
            BackgroundTransparency = 1,
            AutoButtonColor = false,
            Size = UDim2.new(1, 0, 0, 20),
            Font = Enum.Font.Code,
            Text = "  " .. m,
            TextColor3 = (m == mode) and self.Theme.Accent or self.Theme.TextDim,
            TextSize = 12,
            TextXAlignment = Enum.TextXAlignment.Left,
            ZIndex = 351,
            Parent = modePopup,
        })
        modeButtons[m] = mBtn
        self:Connect(mBtn.MouseButton1Click, function()
            Keybind:SetMode(m)
            modePopup.Visible = false
        end)
    end

    local function SyncKeybindList()
        self.ActiveKeybinds[flag] = {
            Name = Keybind.Name,
            Key = currentKey,
            Mode = mode,
            Active = Keybind.Active and (not toggleLink or toggleLink.Value),
        }
        if self.KeybindList then
            self.KeybindList:Refresh()
        end
    end

    function Keybind:SetMode(newMode)
        mode = newMode
        Keybind.Mode = newMode
        for mName, b in pairs(modeButtons) do
            b.TextColor3 = (mName == mode) and Library.Theme.Accent or Library.Theme.TextDim
        end
        if mode == "Always" then
            Keybind.Active = true
        end
        SyncKeybindList()
    end

    function Keybind:Set(newKey)
        currentKey = newKey or Enum.KeyCode.Unknown
        Keybind.Value = currentKey
        Library.Flags[flag] = currentKey
        UpdateButtonSize()
        SyncKeybindList()
        Keybind.Changed:Fire(currentKey)
    end

    self:Connect(btn.MouseButton1Click, function()
        Library:ClosePopups(popupCtrl)
        binding = true
        btn.TextColor3 = self.Theme.Accent
        UpdateButtonSize()
    end)

    self:Connect(btn.MouseButton2Click, function()
        Library:ClosePopups(popupCtrl)
        local abs = btn.AbsolutePosition
        modePopup.Position = UDim2.fromOffset(abs.X, abs.Y + 20)
        modePopup.Visible = not modePopup.Visible
    end)

    self:Connect(UserInputService.InputBegan, function(input, gpe)
        if binding then
            if input.UserInputType == Enum.UserInputType.Keyboard then
                if input.KeyCode == Enum.KeyCode.Unknown then return end
                binding = false
                btn.TextColor3 = self.Theme.TextDim
                if input.KeyCode == Enum.KeyCode.Escape or input.KeyCode == Enum.KeyCode.Backspace then
                    Keybind:Set(Enum.KeyCode.Unknown)
                else
                    Keybind:Set(input.KeyCode)
                end
            elseif input.UserInputType == Enum.UserInputType.MouseButton2
                or input.UserInputType == Enum.UserInputType.MouseButton3 then
                binding = false
                btn.TextColor3 = self.Theme.TextDim
                Keybind:Set(input.UserInputType)
            end
            return
        end

        if gpe or currentKey == Enum.KeyCode.Unknown then return end
        local matched = (input.KeyCode == currentKey) or (input.UserInputType == currentKey)
        if matched then
            if mode == "Toggle" then
                Keybind.Active = not Keybind.Active
                if toggleLink then
                    toggleLink:Set(not toggleLink.Value)
                end
            elseif mode == "Hold" then
                Keybind.Active = true
                if toggleLink then
                    toggleLink:Set(true)
                end
            end
            if cfg.Callback then
                task.spawn(cfg.Callback, Keybind.Active, currentKey)
            end
            SyncKeybindList()
        end
    end)

    self:Connect(UserInputService.InputEnded, function(input)
        if currentKey == Enum.KeyCode.Unknown then return end
        local matched = (input.KeyCode == currentKey) or (input.UserInputType == currentKey)
        if matched and mode == "Hold" then
            Keybind.Active = false
            if toggleLink then
                toggleLink:Set(false)
            end
            if cfg.Callback then
                task.spawn(cfg.Callback, false, currentKey)
            end
            SyncKeybindList()
        end
    end)

    if toggleLink then
        toggleLink.Changed:Connect(function()
            SyncKeybindList()
        end)
    end

    Library.Flags[flag] = currentKey
    Library.Options[flag] = Keybind
    SyncKeybindList()
    return Keybind
end

function Library:CreateColorPickerAttachment(parentRow, cfg)
    cfg = cfg or {}
    local flag = cfg.Flag or (cfg.Name and ("CP_" .. cfg.Name)) or ("CP_" .. tostring(math.random(1000, 9999)))
    local defaultColor = cfg.Default or self.Theme.Accent
    local alpha = cfg.Alpha or 0
    local h, s, v = Color3.toHSV(defaultColor)
    local rainbow = false

    local ColorPicker = {
        Type = "ColorPicker",
        Value = defaultColor,
        Alpha = alpha,
        Rainbow = false,
        Changed = Signal.new(),
    }

    -- Classic CS:GO rectangular color swatch preview
    local swatch = self:Create("TextButton", {
        Name = "ColorSwatch",
        BackgroundColor3 = defaultColor,
        AutoButtonColor = false,
        Size = UDim2.fromOffset(24, 13),
        Text = "",
        ZIndex = 12,
        Parent = parentRow,
    })
    self:ApplyCSBorder(swatch, "InnerBorder")

    self:Create("UIGradient", {
        Color = ColorSequence.new({
            ColorSequenceKeypoint.new(0, Color3.new(1, 1, 1)),
            ColorSequenceKeypoint.new(1, Color3.fromRGB(185, 185, 185)),
        }),
        Rotation = 90,
        Parent = swatch,
    })

    -- Floating Color Picker Popup
    local popup = self:Create("Frame", {
        Name = "ColorPopup",
        BackgroundColor3 = self.Theme.Background,
        Size = UDim2.fromOffset(215, 214),
        Visible = false,
        ZIndex = 360,
        Parent = ScreenGui,
    })
    self:BindTheme(popup, "BackgroundColor3", "Background")
    self:ApplyCSBorder(popup, "HighlightBorder")
    self:CreateAccentBar(popup, 362)

    local popupCtrl = {
        Close = function()
            popup.Visible = false
        end
    }
    table.insert(self.OpenPopups, popupCtrl)

    self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(8, 5),
        Size = UDim2.new(1, -16, 0, 16),
        Font = Enum.Font.GothamBold,
        Text = string.upper(cfg.Title or cfg.Name or "COLOR PICKER"),
        TextColor3 = self.Theme.Text,
        TextSize = 11,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 362,
        Parent = popup,
    })

    -- Saturation / Value Box
    local svBox = self:Create("ImageButton", {
        BackgroundColor3 = Color3.fromHSV(h, 1, 1),
        AutoButtonColor = false,
        Position = UDim2.fromOffset(8, 24),
        Size = UDim2.fromOffset(172, 130),
        Image = "rbxassetid://4155801252",
        ZIndex = 362,
        Parent = popup,
    })
    self:ApplyCSBorder(svBox, "InnerBorder")

    local svCursor = self:Create("Frame", {
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundColor3 = Color3.new(1, 1, 1),
        Position = UDim2.fromScale(s, 1 - v),
        Size = UDim2.fromOffset(6, 6),
        ZIndex = 364,
        Parent = svBox,
    })
    self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Parent = svCursor })

    -- Vertical Hue Bar
    local hueBar = self:Create("TextButton", {
        BackgroundColor3 = Color3.new(1, 1, 1),
        AutoButtonColor = false,
        Position = UDim2.fromOffset(188, 24),
        Size = UDim2.fromOffset(18, 130),
        Text = "",
        ZIndex = 362,
        Parent = popup,
    })
    self:ApplyCSBorder(hueBar, "InnerBorder")
    self:Create("UIGradient", {
        Rotation = 90,
        Color = ColorSequence.new({
            ColorSequenceKeypoint.new(0.00, Color3.fromRGB(255, 0, 0)),
            ColorSequenceKeypoint.new(0.17, Color3.fromRGB(255, 255, 0)),
            ColorSequenceKeypoint.new(0.33, Color3.fromRGB(0, 255, 0)),
            ColorSequenceKeypoint.new(0.50, Color3.fromRGB(0, 255, 255)),
            ColorSequenceKeypoint.new(0.67, Color3.fromRGB(0, 0, 255)),
            ColorSequenceKeypoint.new(0.83, Color3.fromRGB(255, 0, 255)),
            ColorSequenceKeypoint.new(1.00, Color3.fromRGB(255, 0, 0)),
        }),
        Parent = hueBar,
    })

    local hueCursor = self:Create("Frame", {
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundColor3 = Color3.new(1, 1, 1),
        Position = UDim2.new(0.5, 0, h, 0),
        Size = UDim2.new(1, 2, 0, 3),
        ZIndex = 364,
        Parent = hueBar,
    })
    self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Parent = hueCursor })

    -- Alpha Slider Bar
    local alphaBar = self:Create("TextButton", {
        BackgroundColor3 = defaultColor,
        AutoButtonColor = false,
        Position = UDim2.fromOffset(8, 162),
        Size = UDim2.fromOffset(198, 12),
        Text = "",
        ZIndex = 362,
        Parent = popup,
    })
    self:ApplyCSBorder(alphaBar, "InnerBorder")
    self:Create("UIGradient", {
        Color = ColorSequence.new({
            ColorSequenceKeypoint.new(0, Color3.new(1, 1, 1)),
            ColorSequenceKeypoint.new(1, Color3.fromRGB(18, 18, 22)),
        }),
        Parent = alphaBar,
    })

    local alphaCursor = self:Create("Frame", {
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundColor3 = Color3.new(1, 1, 1),
        Position = UDim2.new(1 - alpha, 0, 0.5, 0),
        Size = UDim2.new(0, 3, 1, 2),
        ZIndex = 364,
        Parent = alphaBar,
    })
    self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Parent = alphaCursor })

    -- Hex Input Box
    local hexInput = self:Create("TextBox", {
        BackgroundColor3 = self.Theme.Element,
        Position = UDim2.fromOffset(8, 182),
        Size = UDim2.fromOffset(115, 20),
        Font = Enum.Font.Code,
        Text = "#9BC337",
        PlaceholderText = "#HEX",
        TextColor3 = self.Theme.Text,
        TextSize = 12,
        ClearTextOnFocus = false,
        ZIndex = 362,
        Parent = popup,
    })
    self:BindTheme(hexInput, "BackgroundColor3", "Element")
    self:ApplyCSBorder(hexInput, "InnerBorder")

    -- Rainbow Toggle inside Popup
    local rbBtn = self:Create("TextButton", {
        BackgroundColor3 = self.Theme.Element,
        AutoButtonColor = false,
        Position = UDim2.fromOffset(131, 182),
        Size = UDim2.fromOffset(75, 20),
        Font = Enum.Font.Code,
        Text = "RAINBOW",
        TextColor3 = self.Theme.TextDim,
        TextSize = 11,
        ZIndex = 362,
        Parent = popup,
    })
    self:BindTheme(rbBtn, "BackgroundColor3", "Element")
    self:ApplyCSBorder(rbBtn, "InnerBorder")

    local function ToHex(c)
        return string.format("#%02X%02X%02X",
            math.floor(c.R * 255 + 0.5),
            math.floor(c.G * 255 + 0.5),
            math.floor(c.B * 255 + 0.5)
        )
    end

    local function ApplyColor()
        local col = Color3.fromHSV(h, s, v)
        ColorPicker.Value = col
        ColorPicker.Alpha = alpha
        swatch.BackgroundColor3 = col
        svBox.BackgroundColor3 = Color3.fromHSV(h, 1, 1)
        alphaBar.BackgroundColor3 = col
        svCursor.Position = UDim2.fromScale(s, 1 - v)
        hueCursor.Position = UDim2.new(0.5, 0, h, 0)
        alphaCursor.Position = UDim2.new(1 - alpha, 0, 0.5, 0)
        hexInput.Text = ToHex(col)
        Library.Flags[flag] = col
        if cfg.Callback then
            task.spawn(cfg.Callback, col, alpha)
        end
        ColorPicker.Changed:Fire(col, alpha)
    end

    function ColorPicker:Set(col, newAlpha)
        if typeof(col) == "Color3" then
            h, s, v = Color3.toHSV(col)
        end
        if type(newAlpha) == "number" then
            alpha = math.clamp(newAlpha, 0, 1)
        end
        ApplyColor()
    end

    local dragSV, dragHue, dragAlpha = false, false, false
    self:Connect(svBox.InputBegan, function(inp)
        if inp.UserInputType == Enum.UserInputType.MouseButton1 then
            dragSV = true
            s = math.clamp((inp.Position.X - svBox.AbsolutePosition.X) / svBox.AbsoluteSize.X, 0, 1)
            v = 1 - math.clamp((inp.Position.Y - svBox.AbsolutePosition.Y) / svBox.AbsoluteSize.Y, 0, 1)
            ApplyColor()
        end
    end)
    self:Connect(hueBar.InputBegan, function(inp)
        if inp.UserInputType == Enum.UserInputType.MouseButton1 then
            dragHue = true
            h = math.clamp((inp.Position.Y - hueBar.AbsolutePosition.Y) / hueBar.AbsoluteSize.Y, 0, 1)
            ApplyColor()
        end
    end)
    self:Connect(alphaBar.InputBegan, function(inp)
        if inp.UserInputType == Enum.UserInputType.MouseButton1 then
            dragAlpha = true
            alpha = 1 - math.clamp((inp.Position.X - alphaBar.AbsolutePosition.X) / alphaBar.AbsoluteSize.X, 0, 1)
            ApplyColor()
        end
    end)
    self:Connect(UserInputService.InputChanged, function(inp)
        if inp.UserInputType == Enum.UserInputType.MouseMovement then
            if dragSV then
                s = math.clamp((inp.Position.X - svBox.AbsolutePosition.X) / svBox.AbsoluteSize.X, 0, 1)
                v = 1 - math.clamp((inp.Position.Y - svBox.AbsolutePosition.Y) / svBox.AbsoluteSize.Y, 0, 1)
                ApplyColor()
            elseif dragHue then
                h = math.clamp((inp.Position.Y - hueBar.AbsolutePosition.Y) / hueBar.AbsoluteSize.Y, 0, 1)
                ApplyColor()
            elseif dragAlpha then
                alpha = 1 - math.clamp((inp.Position.X - alphaBar.AbsolutePosition.X) / alphaBar.AbsoluteSize.X, 0, 1)
                ApplyColor()
            end
        end
    end)
    self:Connect(UserInputService.InputEnded, function(inp)
        if inp.UserInputType == Enum.UserInputType.MouseButton1 then
            dragSV, dragHue, dragAlpha = false, false, false
        end
    end)

    self:Connect(hexInput.FocusLost, function()
        local clean = string.gsub(hexInput.Text, "#", "")
        if #clean == 6 then
            local r = tonumber(string.sub(clean, 1, 2), 16)
            local g = tonumber(string.sub(clean, 3, 4), 16)
            local b = tonumber(string.sub(clean, 5, 6), 16)
            if r and g and b then
                ColorPicker:Set(Color3.fromRGB(r, g, b), alpha)
            end
        end
    end)

    self:Connect(rbBtn.MouseButton1Click, function()
        rainbow = not rainbow
        ColorPicker.Rainbow = rainbow
        rbBtn.TextColor3 = rainbow and self.Theme.Accent or self.Theme.TextDim
    end)

    self:Connect(RunService.RenderStepped, function()
        if rainbow and not self.Unloaded then
            h = (os.clock() * 0.22) % 1
            ApplyColor()
        end
    end)

    self:Connect(swatch.MouseButton1Click, function()
        local willShow = not popup.Visible
        Library:ClosePopups(popupCtrl)
        if willShow then
            local abs = swatch.AbsolutePosition
            local vp = workspace.CurrentCamera and workspace.CurrentCamera.ViewportSize or Vector2.new(1920, 1080)
            local px = math.clamp(abs.X - 185, 6, math.max(6, vp.X - 225))
            local py = math.clamp(abs.Y + 18, 6, math.max(6, vp.Y - 225))
            popup.Position = UDim2.fromOffset(px, py)
            popup.Visible = true
        end
    end)

    ApplyColor()
    Library.Options[flag] = ColorPicker
    return ColorPicker
end

-- ============================================================================
-- GROUPBOX ELEMENT BUILDERS (CS:GO Square Toggle, Slider, Dropdowns, etc.)
-- ============================================================================
local GroupboxMethods = {}
GroupboxMethods.__index = GroupboxMethods

-- 1. CS:GO SQUARE TOGGLE (Lights up with Accent Gradient + Outer Glow + Check Icon)
function GroupboxMethods:AddToggle(cfg)
    cfg = cfg or {}
    local name = cfg.Name or "Toggle"
    local flag = cfg.Flag or name
    local state = cfg.Default or false
    local risky = cfg.Risky or false

    local Toggle = {
        Type = "Toggle",
        Name = name,
        Value = state,
        Changed = Signal.new(),
    }

    local row = Library:Create("Frame", {
        Name = "Toggle_" .. name,
        BackgroundTransparency = 1,
        Size = UDim2.new(1, 0, 0, 20),
        ZIndex = 10,
        Parent = self.Container,
    })
    Library:AddTooltip(row, cfg.Tooltip)

    -- Clickable area covering box + label
    local clickBtn = Library:Create("TextButton", {
        BackgroundTransparency = 1,
        Size = UDim2.new(1, -60, 1, 0),
        Text = "",
        ZIndex = 11,
        Parent = row,
    })

    -- Outer Glow around the square box when lit up
    local boxGlow = Library:Create("ImageLabel", {
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(7, 10),
        Size = UDim2.fromOffset(24, 24),
        Image = "rbxassetid://5028857084",
        ImageColor3 = risky and Library.Theme.Risky or Library.Theme.Accent,
        ImageTransparency = state and 0.55 or 1,
        ZIndex = 9,
        Parent = row,
    })

    -- The 13x13 CS:GO Square Box
    local boxBase = Library:Create("Frame", {
        AnchorPoint = Vector2.new(0, 0.5),
        BackgroundColor3 = Library.Theme.Element,
        Position = UDim2.new(0, 1, 0.5, 0),
        Size = UDim2.fromOffset(13, 13),
        ZIndex = 10,
        Parent = row,
    })
    Library:BindTheme(boxBase, "BackgroundColor3", "Element")
    local boxStroke = Library:ApplyCSBorder(boxBase, "InnerBorder")

    -- Lit-up interior fill (scales & fades smoothly when toggled on)
    local boxFill = Library:Create("Frame", {
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundColor3 = Color3.new(1, 1, 1),
        BorderSizePixel = 0,
        Position = UDim2.fromScale(0.5, 0.5),
        Size = state and UDim2.fromScale(1, 1) or UDim2.fromScale(0, 0),
        BackgroundTransparency = state and 0 or 1,
        ZIndex = 11,
        Parent = boxBase,
    })

    local fillGrad = Library:Create("UIGradient", {
        Rotation = 90,
        Color = ColorSequence.new({
            ColorSequenceKeypoint.new(0, risky and Library.Theme.Risky or Library.Theme.Accent),
            ColorSequenceKeypoint.new(1, risky and Color3.fromRGB(150, 35, 35) or Library.Theme.AccentDark),
        }),
        Parent = boxFill,
    })

    -- Crisp micro Lucide check inside the lit square (optional via cfg.Checkmark)
    local checkIcon = Library:Create("ImageLabel", {
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundTransparency = 1,
        Position = UDim2.fromScale(0.5, 0.5),
        Size = UDim2.fromOffset(9, 9),
        Image = Library:GetIcon("check"),
        ImageColor3 = Color3.fromRGB(10, 10, 12),
        ImageTransparency = (state and cfg.Checkmark) and 0.15 or 1,
        ZIndex = 12,
        Parent = boxFill,
    })

    -- Label next to square
    local label = Library:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(21, 0),
        Size = UDim2.new(1, -75, 1, 0),
        Font = Enum.Font.Code,
        Text = name,
        TextColor3 = state and Library.Theme.Text or (risky and Library.Theme.Risky or Library.Theme.TextDim),
        TextSize = 12,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 10,
        Parent = row,
    })

    -- Right-side attachment tray (for Keybinds & ColorPickers)
    local attachTray = Library:Create("Frame", {
        BackgroundTransparency = 1,
        AnchorPoint = Vector2.new(1, 0.5),
        Position = UDim2.new(1, 0, 0.5, 0),
        Size = UDim2.fromOffset(85, 16),
        ZIndex = 12,
        Parent = row,
    })
    Library:Create("UIListLayout", {
        FillDirection = Enum.FillDirection.Horizontal,
        HorizontalAlignment = Enum.HorizontalAlignment.Right,
        VerticalAlignment = Enum.VerticalAlignment.Center,
        Padding = UDim.new(0, 5),
        Parent = attachTray,
    })

    local function UpdateVisuals()
        local activeCol = risky and Library.Theme.Risky or Library.Theme.Accent
        local darkCol = risky and Color3.fromRGB(150, 35, 35) or Library.Theme.AccentDark
        fillGrad.Color = ColorSequence.new({
            ColorSequenceKeypoint.new(0, activeCol),
            ColorSequenceKeypoint.new(1, darkCol),
        })
        boxGlow.ImageColor3 = activeCol

        if state then
            Library:Tween(boxFill, TweenFast, {
                Size = UDim2.fromScale(1, 1),
                BackgroundTransparency = 0,
            })
            Library:Tween(boxGlow, TweenMedium, { ImageTransparency = 0.52 })
            Library:Tween(boxStroke, TweenFast, { Color = activeCol })
            Library:Tween(label, TweenFast, {
                TextColor3 = risky and Library.Theme.Risky or Library.Theme.Text
            })
            if cfg.Checkmark then
                Library:Tween(checkIcon, TweenFast, { ImageTransparency = 0.15 })
            end
        else
            Library:Tween(boxFill, TweenFast, {
                Size = UDim2.fromScale(0, 0),
                BackgroundTransparency = 1,
            })
            Library:Tween(boxGlow, TweenMedium, { ImageTransparency = 1 })
            Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.InnerBorder })
            Library:Tween(label, TweenFast, {
                TextColor3 = risky and Color3.fromRGB(175, 70, 70) or Library.Theme.TextDim
            })
            Library:Tween(checkIcon, TweenFast, { ImageTransparency = 1 })
        end
    end

    function Toggle:Set(val)
        state = (val == true)
        Toggle.Value = state
        Library.Flags[flag] = state
        UpdateVisuals()
        if cfg.Callback then
            task.spawn(cfg.Callback, state)
        end
        Toggle.Changed:Fire(state)
    end

    function Toggle:RefreshTheme()
        UpdateVisuals()
    end

    function Toggle:AddKeybind(kbCfg)
        return Library:CreateKeybindAttachment(attachTray, kbCfg, Toggle)
    end

    function Toggle:AddColorPicker(cpCfg)
        return Library:CreateColorPickerAttachment(attachTray, cpCfg)
    end

    Library:Connect(clickBtn.MouseEnter, function()
        if not state then
            Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.HighlightBorder })
            Library:Tween(label, TweenFast, { TextColor3 = Library.Theme.Text })
        end
    end)

    Library:Connect(clickBtn.MouseLeave, function()
        UpdateVisuals()
    end)

    Library:Connect(clickBtn.MouseButton1Click, function()
        Toggle:Set(not state)
    end)

    UpdateVisuals()
    Library.Flags[flag] = state
    Library.Options[flag] = Toggle
    if cfg.Default and cfg.Callback then
        task.spawn(cfg.Callback, state)
    end
    return Toggle
end

-- 2. CS:GO SLIDER (1px Bordered Bar, Gradient Fill, Inline Value Display, Step/Rounding, Click & Drag)
function GroupboxMethods:AddSlider(cfg)
    cfg = cfg or {}
    local name = cfg.Name or "Slider"
    local flag = cfg.Flag or name
    local min = cfg.Min or 0
    local max = cfg.Max or 100
    local rounding = cfg.Rounding or 0
    local suffix = cfg.Suffix or ""
    local value = math.clamp(cfg.Default or min, min, max)

    local Slider = {
        Type = "Slider",
        Name = name,
        Value = value,
        Min = min,
        Max = max,
        Changed = Signal.new(),
    }

    local holder = Library:Create("Frame", {
        Name = "Slider_" .. name,
        BackgroundTransparency = 1,
        Size = UDim2.new(1, 0, 0, 32),
        ZIndex = 10,
        Parent = self.Container,
    })
    Library:AddTooltip(holder, cfg.Tooltip)

    local topLabel = Library:Create("TextLabel", {
        BackgroundTransparency = 1,
        Size = UDim2.new(0.65, 0, 0, 15),
        Font = Enum.Font.Code,
        Text = name,
        TextColor3 = Library.Theme.TextDim,
        TextSize = 12,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 10,
        Parent = holder,
    })
    Library:BindTheme(topLabel, "TextColor3", "TextDim")

    local valLabel = Library:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromScale(0.65, 0),
        Size = UDim2.new(0.35, 0, 0, 15),
        Font = Enum.Font.Code,
        Text = tostring(value) .. suffix,
        TextColor3 = Library.Theme.Text,
        TextSize = 12,
        TextXAlignment = Enum.TextXAlignment.Right,
        ZIndex = 10,
        Parent = holder,
    })
    Library:BindTheme(valLabel, "TextColor3", "Text")

    local track = Library:Create("TextButton", {
        BackgroundColor3 = Library.Theme.Element,
        AutoButtonColor = false,
        Position = UDim2.fromOffset(1, 18),
        Size = UDim2.new(1, -2, 0, 11),
        Text = "",
        ZIndex = 10,
        Parent = holder,
    })
    Library:BindTheme(track, "BackgroundColor3", "Element")
    local trackStroke = Library:ApplyCSBorder(track, "InnerBorder")

    local fill = Library:Create("Frame", {
        BackgroundColor3 = Color3.new(1, 1, 1),
        BorderSizePixel = 0,
        Size = UDim2.fromScale((value - min) / math.max(0.0001, max - min), 1),
        ZIndex = 11,
        Parent = track,
    })

    local fillGrad = Library:Create("UIGradient", {
        Rotation = 90,
        Color = ColorSequence.new({
            ColorSequenceKeypoint.new(0, Library.Theme.Accent),
            ColorSequenceKeypoint.new(1, Library.Theme.AccentDark),
        }),
        Parent = fill,
    })

    -- Crisp 2px highlight edge at the tip of the slider fill
    local fillEdge = Library:Create("Frame", {
        AnchorPoint = Vector2.new(1, 0),
        BackgroundColor3 = Color3.fromRGB(245, 245, 250),
        BackgroundTransparency = 0.35,
        BorderSizePixel = 0,
        Position = UDim2.fromScale(1, 0),
        Size = UDim2.new(0, 2, 1, 0),
        ZIndex = 12,
        Parent = fill,
    })

    local function Round(num, places)
        local mult = 10 ^ (places or 0)
        return math.floor(num * mult + 0.5) / mult
    end

    local function FormatValue(v)
        if rounding > 0 then
            return string.format("%." .. tostring(rounding) .. "f%s", v, suffix)
        end
        return tostring(math.floor(v + 0.5)) .. suffix
    end

    function Slider:Set(newVal)
        local clamped = math.clamp(Round(newVal, rounding), min, max)
        value = clamped
        Slider.Value = clamped
        Library.Flags[flag] = clamped
        local pct = math.clamp((clamped - min) / math.max(0.0001, max - min), 0, 1)
        Library:Tween(fill, TweenFast, { Size = UDim2.fromScale(pct, 1) })
        valLabel.Text = FormatValue(clamped)
        if cfg.Callback then
            task.spawn(cfg.Callback, clamped)
        end
        Slider.Changed:Fire(clamped)
    end

    function Slider:RefreshTheme()
        fillGrad.Color = ColorSequence.new({
            ColorSequenceKeypoint.new(0, Library.Theme.Accent),
            ColorSequenceKeypoint.new(1, Library.Theme.AccentDark),
        })
    end

    local dragging = false
    local function UpdateFromInput(input)
        local pct = math.clamp((input.Position.X - track.AbsolutePosition.X) / track.AbsoluteSize.X, 0, 1)
        local raw = min + (max - min) * pct
        Slider:Set(raw)
    end

    Library:Connect(track.InputBegan, function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            dragging = true
            Library:Tween(trackStroke, TweenFast, { Color = Library.Theme.Accent })
            Library:Tween(topLabel, TweenFast, { TextColor3 = Library.Theme.Text })
            UpdateFromInput(input)
        end
    end)

    Library:Connect(UserInputService.InputChanged, function(input)
        if dragging and input.UserInputType == Enum.UserInputType.MouseMovement then
            UpdateFromInput(input)
        end
    end)

    Library:Connect(UserInputService.InputEnded, function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 and dragging then
            dragging = false
            Library:Tween(trackStroke, TweenFast, { Color = Library.Theme.InnerBorder })
            Library:Tween(topLabel, TweenFast, { TextColor3 = Library.Theme.TextDim })
        end
    end)

    Library:Connect(track.MouseEnter, function()
        if not dragging then
            Library:Tween(trackStroke, TweenFast, { Color = Library.Theme.HighlightBorder })
        end
    end)
    Library:Connect(track.MouseLeave, function()
        if not dragging then
            Library:Tween(trackStroke, TweenFast, { Color = Library.Theme.InnerBorder })
        end
    end)

    Slider:Set(value)
    Library.Options[flag] = Slider
    return Slider
end

-- 3. ALL SORTS OF DROPDOWNS (Single, Multi-Select, Searchable, Player-List, Dynamic Refresh)
function GroupboxMethods:AddDropdown(cfg)
    cfg = cfg or {}
    local name = cfg.Name or "Dropdown"
    local flag = cfg.Flag or name
    local multi = cfg.Multi or false
    local searchable = cfg.Searchable or false
    local isPlayerList = cfg.SpecialType == "Player" or cfg.PlayerList == true
    local allowNull = cfg.AllowNull or false
    local values = table.clone(cfg.Values or cfg.Options or {})
    local opened = false
    local searchQuery = ""

    if isPlayerList then
        values = {}
        for _, plr in ipairs(Players:GetPlayers()) do
            table.insert(values, plr.Name)
        end
    end

    local selected
    if multi then
        selected = {}
        if type(cfg.Default) == "table" then
            for k, v in pairs(cfg.Default) do
                if type(k) == "number" then
                    selected[v] = true
                elseif v then
                    selected[k] = true
                end
            end
        elseif type(cfg.Default) == "string" then
            selected[cfg.Default] = true
        end
    else
        selected = cfg.Default or (not allowNull and values[1]) or nil
    end

    local Dropdown = {
        Type = "Dropdown",
        Name = name,
        Multi = multi,
        Values = values,
        Value = selected,
        Changed = Signal.new(),
    }

    local holder = Library:Create("Frame", {
        Name = "Dropdown_" .. name,
        BackgroundTransparency = 1,
        Size = UDim2.new(1, 0, 0, 38),
        ZIndex = 10,
        Parent = self.Container,
    })
    Library:AddTooltip(holder, cfg.Tooltip)

    local titleLbl = Library:Create("TextLabel", {
        BackgroundTransparency = 1,
        Size = UDim2.new(1, 0, 0, 15),
        Font = Enum.Font.Code,
        Text = name,
        TextColor3 = Library.Theme.TextDim,
        TextSize = 12,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 10,
        Parent = holder,
    })
    Library:BindTheme(titleLbl, "TextColor3", "TextDim")

    local boxBtn = Library:Create("TextButton", {
        BackgroundColor3 = Library.Theme.Element,
        AutoButtonColor = false,
        Position = UDim2.fromOffset(1, 17),
        Size = UDim2.new(1, -2, 0, 20),
        Text = "",
        ZIndex = 11,
        Parent = holder,
    })
    Library:BindTheme(boxBtn, "BackgroundColor3", "Element")
    local boxStroke = Library:ApplyCSBorder(boxBtn, "InnerBorder")

    local valText = Library:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(7, 0),
        Size = UDim2.new(1, -26, 1, 0),
        Font = Enum.Font.Code,
        Text = "-",
        TextColor3 = Library.Theme.Text,
        TextSize = 12,
        TextTruncate = Enum.TextTruncate.AtEnd,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 12,
        Parent = boxBtn,
    })
    Library:BindTheme(valText, "TextColor3", "Text")

    local chevron = Library:Create("ImageLabel", {
        AnchorPoint = Vector2.new(1, 0.5),
        BackgroundTransparency = 1,
        Position = UDim2.new(1, -6, 0.5, 0),
        Size = UDim2.fromOffset(12, 12),
        Image = Library:GetIcon("chevron-down"),
        ImageColor3 = Library.Theme.TextDim,
        ZIndex = 12,
        Parent = boxBtn,
    })
    Library:BindTheme(chevron, "ImageColor3", "TextDim")

    -- Floating Dropdown Menu
    local dropMenu = Library:Create("Frame", {
        Name = "DropMenu_" .. name,
        BackgroundColor3 = Library.Theme.Background,
        Size = UDim2.fromOffset(180, 0),
        ClipsDescendants = true,
        Visible = false,
        ZIndex = 340,
        Parent = ScreenGui,
    })
    Library:BindTheme(dropMenu, "BackgroundColor3", "Background")
    Library:ApplyCSBorder(dropMenu, "HighlightBorder")

    local popupCtrl = {
        Close = function()
            opened = false
            Library:Tween(chevron, TweenFast, { Rotation = 0 })
            Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.InnerBorder })
            dropMenu.Visible = false
        end
    }
    table.insert(Library.OpenPopups, popupCtrl)

    local searchBox
    local listTopOffset = 2
    if searchable then
        listTopOffset = 25
        local searchHolder = Library:Create("Frame", {
            BackgroundColor3 = Library.Theme.Element,
            BorderSizePixel = 0,
            Position = UDim2.fromOffset(3, 3),
            Size = UDim2.new(1, -6, 0, 20),
            ZIndex = 342,
            Parent = dropMenu,
        })
        Library:BindTheme(searchHolder, "BackgroundColor3", "Element")
        Library:ApplyCSBorder(searchHolder, "InnerBorder")

        Library:Create("ImageLabel", {
            AnchorPoint = Vector2.new(0, 0.5),
            BackgroundTransparency = 1,
            Position = UDim2.new(0, 5, 0.5, 0),
            Size = UDim2.fromOffset(11, 11),
            Image = Library:GetIcon("search"),
            ImageColor3 = Library.Theme.TextMuted,
            ZIndex = 343,
            Parent = searchHolder,
        })

        searchBox = Library:Create("TextBox", {
            BackgroundTransparency = 1,
            Position = UDim2.fromOffset(20, 0),
            Size = UDim2.new(1, -24, 1, 0),
            Font = Enum.Font.Code,
            Text = "",
            PlaceholderText = "Search...",
            PlaceholderColor3 = Library.Theme.TextMuted,
            TextColor3 = Library.Theme.Text,
            TextSize = 12,
            ClearTextOnFocus = false,
            TextXAlignment = Enum.TextXAlignment.Left,
            ZIndex = 343,
            Parent = searchHolder,
        })
    end

    local scroll = Library:Create("ScrollingFrame", {
        BackgroundTransparency = 1,
        BorderSizePixel = 0,
        Position = UDim2.fromOffset(2, listTopOffset),
        Size = UDim2.new(1, -4, 1, -listTopOffset - 2),
        CanvasSize = UDim2.fromOffset(0, 0),
        ScrollBarThickness = 2,
        ScrollBarImageColor3 = Library.Theme.Accent,
        ZIndex = 342,
        Parent = dropMenu,
    })
    Library:BindTheme(scroll, "ScrollBarImageColor3", "Accent")

    local listLayout = Library:Create("UIListLayout", {
        Padding = UDim.new(0, 1),
        SortOrder = Enum.SortOrder.LayoutOrder,
        Parent = scroll,
    })

    local function UpdateDisplayText()
        if multi then
            local activeItems = {}
            for _, v in ipairs(values) do
                if selected[v] then
                    table.insert(activeItems, tostring(v))
                end
            end
            valText.Text = #activeItems > 0 and table.concat(activeItems, ", ") or "None"
            valText.TextColor3 = #activeItems > 0 and Library.Theme.Text or Library.Theme.TextMuted
        else
            valText.Text = selected ~= nil and tostring(selected) or "None"
            valText.TextColor3 = selected ~= nil and Library.Theme.Text or Library.Theme.TextMuted
        end
    end

    local function RebuildItems()
        for _, ch in ipairs(scroll:GetChildren()) do
            if ch:IsA("TextButton") then ch:Destroy() end
        end

        local shown = 0
        for _, optName in ipairs(values) do
            local strName = tostring(optName)
            if searchQuery == "" or string.find(string.lower(strName), string.lower(searchQuery), 1, true) then
                shown += 1
                local isSel = multi and (selected[optName] == true) or (selected == optName)

                local itemBtn = Library:Create("TextButton", {
                    BackgroundColor3 = isSel and Library.Theme.ElementHover or Library.Theme.Background,
                    BackgroundTransparency = isSel and 0 or 1,
                    AutoButtonColor = false,
                    Size = UDim2.new(1, 0, 0, 20),
                    Text = "",
                    ZIndex = 343,
                    Parent = scroll,
                })

                -- Left accent bar when selected
                local selBar = Library:Create("Frame", {
                    BackgroundColor3 = Library.Theme.Accent,
                    BorderSizePixel = 0,
                    Size = isSel and UDim2.new(0, 2, 1, 0) or UDim2.new(0, 0, 1, 0),
                    ZIndex = 344,
                    Parent = itemBtn,
                })
                Library:BindTheme(selBar, "BackgroundColor3", "Accent")

                -- Optional mini square checkbox inside multi-select dropdowns
                local textOffset = 8
                if multi then
                    textOffset = 24
                    local miniBox = Library:Create("Frame", {
                        AnchorPoint = Vector2.new(0, 0.5),
                        BackgroundColor3 = isSel and Library.Theme.Accent or Library.Theme.Element,
                        Position = UDim2.new(0, 7, 0.5, 0),
                        Size = UDim2.fromOffset(10, 10),
                        ZIndex = 344,
                        Parent = itemBtn,
                    })
                    Library:ApplyCSBorder(miniBox, isSel and "Accent" or "InnerBorder")
                end

                local itemLbl = Library:Create("TextLabel", {
                    BackgroundTransparency = 1,
                    Position = UDim2.fromOffset(textOffset, 0),
                    Size = UDim2.new(1, -textOffset - 6, 1, 0),
                    Font = isSel and Enum.Font.GothamBold or Enum.Font.Code,
                    Text = strName,
                    TextColor3 = isSel and Library.Theme.Accent or Library.Theme.TextDim,
                    TextSize = 12,
                    TextXAlignment = Enum.TextXAlignment.Left,
                    ZIndex = 344,
                    Parent = itemBtn,
                })

                Library:Connect(itemBtn.MouseButton1Click, function()
                    if multi then
                        selected[optName] = not selected[optName] or nil
                        Dropdown.Value = selected
                        Library.Flags[flag] = selected
                        UpdateDisplayText()
                        RebuildItems()
                        if cfg.Callback then
                            task.spawn(cfg.Callback, selected)
                        end
                        Dropdown.Changed:Fire(selected)
                    else
                        if allowNull and selected == optName then
                            selected = nil
                        else
                            selected = optName
                        end
                        Dropdown.Value = selected
                        Library.Flags[flag] = selected
                        UpdateDisplayText()
                        RebuildItems()
                        popupCtrl.Close()
                        if cfg.Callback then
                            task.spawn(cfg.Callback, selected)
                        end
                        Dropdown.Changed:Fire(selected)
                    end
                end)
            end
        end

        local contentH = shown * 21
        scroll.CanvasSize = UDim2.fromOffset(0, contentH)
        local menuH = math.clamp(contentH + listTopOffset + 4, listTopOffset + 22, 165)
        if opened then
            Library:Tween(dropMenu, TweenFast, {
                Size = UDim2.fromOffset(boxBtn.AbsoluteSize.X, menuH)
            })
        end
    end

    if searchBox then
        Library:Connect(searchBox:GetPropertyChangedSignal("Text"), function()
            searchQuery = searchBox.Text
            RebuildItems()
        end)
    end

    function Dropdown:Set(val)
        if multi then
            selected = {}
            if type(val) == "table" then
                for k, v in pairs(val) do
                    if type(k) == "number" then
                        selected[v] = true
                    elseif v then
                        selected[k] = true
                    end
                end
            end
        else
            selected = val
        end
        Dropdown.Value = selected
        Library.Flags[flag] = selected
        UpdateDisplayText()
        RebuildItems()
        if cfg.Callback then
            task.spawn(cfg.Callback, selected)
        end
        Dropdown.Changed:Fire(selected)
    end

    function Dropdown:Refresh(newValues, keepSelection)
        values = table.clone(newValues or {})
        Dropdown.Values = values
        if not keepSelection then
            if multi then
                selected = {}
            else
                selected = not allowNull and values[1] or nil
            end
            Dropdown.Value = selected
            Library.Flags[flag] = selected
        end
        UpdateDisplayText()
        RebuildItems()
    end

    function Dropdown:RefreshTheme()
        UpdateDisplayText()
        RebuildItems()
    end

    if isPlayerList then
        local function SyncPlayers()
            local list = {}
            for _, plr in ipairs(Players:GetPlayers()) do
                table.insert(list, plr.Name)
            end
            Dropdown:Refresh(list, true)
        end
        Library:Connect(Players.PlayerAdded, SyncPlayers)
        Library:Connect(Players.PlayerRemoving, SyncPlayers)
    end

    Library:Connect(boxBtn.MouseButton1Click, function()
        local nextState = not opened
        Library:ClosePopups(popupCtrl)
        opened = nextState
        if opened then
            local absPos = boxBtn.AbsolutePosition
            local absSize = boxBtn.AbsoluteSize
            dropMenu.Position = UDim2.fromOffset(absPos.X, absPos.Y + absSize.Y + 2)
            dropMenu.Size = UDim2.fromOffset(absSize.X, 0)
            dropMenu.Visible = true
            Library:Tween(chevron, TweenFast, { Rotation = 180 })
            Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.Accent })
            RebuildItems()
        else
            popupCtrl.Close()
        end
    end)

    Library:Connect(boxBtn:GetPropertyChangedSignal("AbsolutePosition"), function()
        if opened and dropMenu.Visible then
            local absPos = boxBtn.AbsolutePosition
            local absSize = boxBtn.AbsoluteSize
            dropMenu.Position = UDim2.fromOffset(absPos.X, absPos.Y + absSize.Y + 2)
        end
    end)

    UpdateDisplayText()
    RebuildItems()
    Library.Flags[flag] = selected
    Library.Options[flag] = Dropdown
    return Dropdown
end

-- 4. CS:GO INLINE SCROLLABLE LISTBOX (Classic Config/Weapon/Target List)
function GroupboxMethods:AddListbox(cfg)
    cfg = cfg or {}
    local name = cfg.Name or "Listbox"
    local flag = cfg.Flag or name
    local height = cfg.Height or 110
    local multi = cfg.Multi or false
    local values = table.clone(cfg.Values or {})
    local selected = multi and {} or (cfg.Default or values[1])

    local Listbox = {
        Type = "Listbox",
        Name = name,
        Values = values,
        Value = selected,
        Changed = Signal.new(),
    }

    local holder = Library:Create("Frame", {
        Name = "Listbox_" .. name,
        BackgroundTransparency = 1,
        Size = UDim2.new(1, 0, 0, height + 18),
        ZIndex = 10,
        Parent = self.Container,
    })

    local titleLbl = Library:Create("TextLabel", {
        BackgroundTransparency = 1,
        Size = UDim2.new(1, 0, 0, 15),
        Font = Enum.Font.Code,
        Text = name,
        TextColor3 = Library.Theme.TextDim,
        TextSize = 12,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 10,
        Parent = holder,
    })
    Library:BindTheme(titleLbl, "TextColor3", "TextDim")

    local frame = Library:Create("Frame", {
        BackgroundColor3 = Library.Theme.Element,
        Position = UDim2.fromOffset(1, 17),
        Size = UDim2.new(1, -2, 0, height),
        ZIndex = 10,
        Parent = holder,
    })
    Library:BindTheme(frame, "BackgroundColor3", "Element")
    Library:ApplyCSBorder(frame, "InnerBorder")

    local scroll = Library:Create("ScrollingFrame", {
        BackgroundTransparency = 1,
        BorderSizePixel = 0,
        Position = UDim2.fromOffset(2, 2),
        Size = UDim2.new(1, -4, 1, -4),
        CanvasSize = UDim2.fromOffset(0, 0),
        ScrollBarThickness = 2,
        ScrollBarImageColor3 = Library.Theme.Accent,
        ZIndex = 11,
        Parent = frame,
    })
    Library:BindTheme(scroll, "ScrollBarImageColor3", "Accent")

    Library:Create("UIListLayout", {
        Padding = UDim.new(0, 1),
        SortOrder = Enum.SortOrder.LayoutOrder,
        Parent = scroll,
    })

    local function Rebuild()
        for _, ch in ipairs(scroll:GetChildren()) do
            if ch:IsA("TextButton") then ch:Destroy() end
        end
        for _, item in ipairs(values) do
            local isSel = multi and (selected[item] == true) or (selected == item)
            local row = Library:Create("TextButton", {
                BackgroundColor3 = isSel and Library.Theme.ElementHover or Library.Theme.Element,
                BackgroundTransparency = isSel and 0 or 1,
                AutoButtonColor = false,
                Size = UDim2.new(1, 0, 0, 18),
                Font = isSel and Enum.Font.GothamBold or Enum.Font.Code,
                Text = "  " .. tostring(item),
                TextColor3 = isSel and Library.Theme.Accent or Library.Theme.TextDim,
                TextSize = 12,
                TextXAlignment = Enum.TextXAlignment.Left,
                ZIndex = 12,
                Parent = scroll,
            })
            Library:Connect(row.MouseButton1Click, function()
                if multi then
                    selected[item] = not selected[item] or nil
                else
                    selected = item
                end
                Listbox.Value = selected
                Library.Flags[flag] = selected
                Rebuild()
                if cfg.Callback then
                    task.spawn(cfg.Callback, selected)
                end
                Listbox.Changed:Fire(selected)
            end)
        end
        scroll.CanvasSize = UDim2.fromOffset(0, #values * 19)
    end

    function Listbox:Refresh(newValues)
        values = table.clone(newValues or {})
        Listbox.Values = values
        Rebuild()
    end

    function Listbox:Set(val)
        selected = val
        Listbox.Value = val
        Library.Flags[flag] = val
        Rebuild()
    end

    function Listbox:RefreshTheme()
        Rebuild()
    end

    Rebuild()
    Library.Flags[flag] = selected
    Library.Options[flag] = Listbox
    return Listbox
end

-- 5. CS:GO BUTTON (Single or Sub-Button Split Row, Optional Lucide Icon, Double-Click Confirm)
function GroupboxMethods:AddButton(cfg)
    cfg = cfg or {}
    local name = cfg.Name or "Button"
    local iconName = cfg.Icon
    local risky = cfg.Risky or false
    local confirm = cfg.Confirm or false
    local awaitingConfirm = false

    local row = Library:Create("Frame", {
        Name = "Button_" .. name,
        BackgroundTransparency = 1,
        Size = UDim2.new(1, 0, 0, 22),
        ZIndex = 10,
        Parent = self.Container,
    })
    Library:AddTooltip(row, cfg.Tooltip)

    local function BuildButtonInstance(btnCfg, xScale, widthScale, xOffset, widthOffset)
        local bCfg = btnCfg or {}
        local localConfirm = false
        local b = Library:Create("TextButton", {
            BackgroundColor3 = Library.Theme.Element,
            AutoButtonColor = false,
            Position = UDim2.new(xScale, xOffset, 0, 0),
            Size = UDim2.new(widthScale, widthOffset, 1, 0),
            Text = "",
            ZIndex = 11,
            Parent = row,
        })
        Library:BindTheme(b, "BackgroundColor3", "Element")
        local bStroke = Library:ApplyCSBorder(b, "InnerBorder")

        Library:Create("UIGradient", {
            Rotation = 90,
            Color = ColorSequence.new({
                ColorSequenceKeypoint.new(0, Color3.fromRGB(255, 255, 255)),
                ColorSequenceKeypoint.new(1, Color3.fromRGB(200, 200, 210)),
            }),
            Parent = b,
        })

        local content = Library:Create("Frame", {
            BackgroundTransparency = 1,
            Size = UDim2.fromScale(1, 1),
            ZIndex = 12,
            Parent = b,
        })
        Library:Create("UIListLayout", {
            FillDirection = Enum.FillDirection.Horizontal,
            HorizontalAlignment = Enum.HorizontalAlignment.Center,
            VerticalAlignment = Enum.VerticalAlignment.Center,
            Padding = UDim.new(0, 5),
            Parent = content,
        })

        if bCfg.Icon then
            local ic = Library:Create("ImageLabel", {
                BackgroundTransparency = 1,
                Size = UDim2.fromOffset(12, 12),
                Image = Library:GetIcon(bCfg.Icon),
                ImageColor3 = bCfg.Risky and Library.Theme.Risky or Library.Theme.Accent,
                ZIndex = 12,
                Parent = content,
            })
            if not bCfg.Risky then
                Library:BindTheme(ic, "ImageColor3", "Accent")
            end
        end

        local lbl = Library:Create("TextLabel", {
            BackgroundTransparency = 1,
            AutomaticSize = Enum.AutomaticSize.X,
            Size = UDim2.new(0, 0, 1, 0),
            Font = Enum.Font.Code,
            Text = bCfg.Name or "Button",
            TextColor3 = bCfg.Risky and Library.Theme.Risky or Library.Theme.Text,
            TextSize = 12,
            ZIndex = 12,
            Parent = content,
        })

        Library:Connect(b.MouseEnter, function()
            Library:Tween(b, TweenFast, { BackgroundColor3 = Library.Theme.ElementHover })
            Library:Tween(bStroke, TweenFast, {
                Color = bCfg.Risky and Library.Theme.Risky or Library.Theme.Accent
            })
        end)

        Library:Connect(b.MouseLeave, function()
            localConfirm = false
            lbl.Text = bCfg.Name or "Button"
            Library:Tween(b, TweenFast, { BackgroundColor3 = Library.Theme.Element })
            Library:Tween(bStroke, TweenFast, { Color = Library.Theme.InnerBorder })
        end)

        Library:Connect(b.MouseButton1Click, function()
            if bCfg.Confirm and not localConfirm then
                localConfirm = true
                lbl.Text = "Confirm: " .. (bCfg.Name or "Button") .. "?"
                return
            end
            localConfirm = false
            lbl.Text = bCfg.Name or "Button"
            Library:Tween(b, TweenInfo.new(0.07, Enum.EasingStyle.Quad), {
                BackgroundColor3 = bCfg.Risky and Library.Theme.Risky or Library.Theme.AccentDark
            })
            task.delay(0.09, function()
                if b and b.Parent then
                    Library:Tween(b, TweenFast, { BackgroundColor3 = Library.Theme.ElementHover })
                end
            end)
            if bCfg.Callback then
                task.spawn(bCfg.Callback)
            end
        end)
        return b, lbl
    end

    local mainBtn = BuildButtonInstance(cfg, 0, 1, 1, -2)
    local ButtonObj = { Instance = mainBtn }

    -- Allows chaining a side-by-side CS:GO split button: Btn:AddButton({ Name = "Reset", Callback = ... })
    function ButtonObj:AddButton(subCfg)
        mainBtn.Position = UDim2.new(0, 1, 0, 0)
        mainBtn.Size = UDim2.new(0.5, -4, 1, 0)
        BuildButtonInstance(subCfg, 0.5, 0.5, 3, -4)
        return ButtonObj
    end

    return ButtonObj
end

-- 6. CS:GO TEXTBOX / INPUT FIELD
function GroupboxMethods:AddInput(cfg)
    cfg = cfg or {}
    local name = cfg.Name or "Input"
    local flag = cfg.Flag or name
    local defaultText = cfg.Default or ""
    local placeholder = cfg.Placeholder or "Enter text..."

    local InputObj = {
        Type = "Input",
        Name = name,
        Value = defaultText,
        Changed = Signal.new(),
    }

    local holder = Library:Create("Frame", {
        Name = "Input_" .. name,
        BackgroundTransparency = 1,
        Size = UDim2.new(1, 0, 0, 38),
        ZIndex = 10,
        Parent = self.Container,
    })
    Library:AddTooltip(holder, cfg.Tooltip)

    local titleLbl = Library:Create("TextLabel", {
        BackgroundTransparency = 1,
        Size = UDim2.new(1, 0, 0, 15),
        Font = Enum.Font.Code,
        Text = name,
        TextColor3 = Library.Theme.TextDim,
        TextSize = 12,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 10,
        Parent = holder,
    })
    Library:BindTheme(titleLbl, "TextColor3", "TextDim")

    local boxWrap = Library:Create("Frame", {
        BackgroundColor3 = Library.Theme.Element,
        Position = UDim2.fromOffset(1, 17),
        Size = UDim2.new(1, -2, 0, 20),
        ZIndex = 10,
        Parent = holder,
    })
    Library:BindTheme(boxWrap, "BackgroundColor3", "Element")
    local boxStroke = Library:ApplyCSBorder(boxWrap, "InnerBorder")

    local tBox = Library:Create("TextBox", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(7, 0),
        Size = UDim2.new(1, -14, 1, 0),
        Font = Enum.Font.Code,
        Text = defaultText,
        PlaceholderText = placeholder,
        PlaceholderColor3 = Library.Theme.TextMuted,
        TextColor3 = Library.Theme.Text,
        TextSize = 12,
        ClearTextOnFocus = cfg.ClearOnFocus or false,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 11,
        Parent = boxWrap,
    })
    Library:BindTheme(tBox, "TextColor3", "Text")

    function InputObj:Set(txt)
        txt = tostring(txt or "")
        tBox.Text = txt
        InputObj.Value = txt
        Library.Flags[flag] = txt
        if cfg.Callback then
            task.spawn(cfg.Callback, txt)
        end
        InputObj.Changed:Fire(txt)
    end

    Library:Connect(tBox.Focused, function()
        Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.Accent })
    end)

    Library:Connect(tBox.FocusLost, function(enterPressed)
        Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.InnerBorder })
        if not cfg.OnlyOnEnter or enterPressed then
            InputObj:Set(tBox.Text)
        end
    end)

    Library.Flags[flag] = defaultText
    Library.Options[flag] = InputObj
    return InputObj
end

-- 7. CS:GO DIVIDERS (Centered Text Divider, Icon Divider, or Plain Crisp Rule)
function GroupboxMethods:AddDivider(textOrCfg, iconName)
    local text = type(textOrCfg) == "table" and textOrCfg.Text or textOrCfg
    local icon = type(textOrCfg) == "table" and textOrCfg.Icon or iconName

    local row = Library:Create("Frame", {
        Name = "Divider",
        BackgroundTransparency = 1,
        Size = UDim2.new(1, 0, 0, text and 18 or 10),
        ZIndex = 10,
        Parent = self.Container,
    })

    if not text or text == "" then
        local line = Library:Create("Frame", {
            AnchorPoint = Vector2.new(0, 0.5),
            BackgroundColor3 = Library.Theme.InnerBorder,
            BorderSizePixel = 0,
            Position = UDim2.new(0, 0, 0.5, 0),
            Size = UDim2.new(1, 0, 0, 1),
            ZIndex = 10,
            Parent = row,
        })
        Library:BindTheme(line, "BackgroundColor3", "InnerBorder")
        return row
    end

    local str = string.upper(tostring(text))
    local bounds = Library:GetTextBounds(str, 10, Vector2.new(300, 20))
    local centerWidth = bounds.X + (icon and 24 or 12)

    local leftLine = Library:Create("Frame", {
        AnchorPoint = Vector2.new(0, 0.5),
        BackgroundColor3 = Library.Theme.InnerBorder,
        BorderSizePixel = 0,
        Position = UDim2.new(0, 0, 0.5, 0),
        Size = UDim2.new(0.5, -math.floor(centerWidth / 2) - 4, 0, 1),
        ZIndex = 10,
        Parent = row,
    })
    Library:BindTheme(leftLine, "BackgroundColor3", "InnerBorder")

    local center = Library:Create("Frame", {
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundTransparency = 1,
        Position = UDim2.fromScale(0.5, 0.5),
        Size = UDim2.fromOffset(centerWidth, 16),
        ZIndex = 11,
        Parent = row,
    })
    Library:Create("UIListLayout", {
        FillDirection = Enum.FillDirection.Horizontal,
        HorizontalAlignment = Enum.HorizontalAlignment.Center,
        VerticalAlignment = Enum.VerticalAlignment.Center,
        Padding = UDim.new(0, 4),
        Parent = center,
    })

    if icon then
        local ic = Library:Create("ImageLabel", {
            BackgroundTransparency = 1,
            Size = UDim2.fromOffset(11, 11),
            Image = Library:GetIcon(icon),
            ImageColor3 = Library.Theme.Accent,
            ZIndex = 11,
            Parent = center,
        })
        Library:BindTheme(ic, "ImageColor3", "Accent")
    end

    local lbl = Library:Create("TextLabel", {
        BackgroundTransparency = 1,
        AutomaticSize = Enum.AutomaticSize.X,
        Size = UDim2.new(0, 0, 1, 0),
        Font = Enum.Font.GothamBold,
        Text = str,
        TextColor3 = Library.Theme.TextMuted,
        TextSize = 10,
        ZIndex = 11,
        Parent = center,
    })
    Library:BindTheme(lbl, "TextColor3", "TextMuted")

    local rightLine = Library:Create("Frame", {
        AnchorPoint = Vector2.new(1, 0.5),
        BackgroundColor3 = Library.Theme.InnerBorder,
        BorderSizePixel = 0,
        Position = UDim2.new(1, 0, 0.5, 0),
        Size = UDim2.new(0.5, -math.floor(centerWidth / 2) - 4, 0, 1),
        ZIndex = 10,
        Parent = row,
    })
    Library:BindTheme(rightLine, "BackgroundColor3", "InnerBorder")
    table.insert(Library.LayoutRefreshers, function()
        if not row or not row.Parent then return end
        local nb = Library:GetTextBounds(str, 10, Vector2.new(300, 20))
        local cw = nb.X + (icon and 24 or 12)
        center.Size = UDim2.fromOffset(cw, 16)
        leftLine.Size = UDim2.new(0.5, -math.floor(cw / 2) - 4, 0, 1)
        rightLine.Size = UDim2.new(0.5, -math.floor(cw / 2) - 4, 0, 1)
    end)
    return row
end

-- 8. LABEL (Supports Inline Keybind & ColorPicker Attachments just like CS:GO ESP Color Rows)
function GroupboxMethods:AddLabel(textOrCfg, wrapText)
    local isTable = type(textOrCfg) == "table"
    local text = isTable and (textOrCfg.Text or textOrCfg.Name) or tostring(textOrCfg or "")
    local wrapped = isTable and textOrCfg.Wrap or wrapText or false

    local bounds = Library:GetTextBounds(text, 12, Vector2.new(220, 400))
    local rowH = wrapped and math.max(18, bounds.Y + 4) or 18

    local row = Library:Create("Frame", {
        Name = "Label",
        BackgroundTransparency = 1,
        Size = UDim2.new(1, 0, 0, rowH),
        ZIndex = 10,
        Parent = self.Container,
    })

    local lbl = Library:Create("TextLabel", {
        BackgroundTransparency = 1,
        Size = UDim2.new(1, -60, 1, 0),
        Font = Enum.Font.Code,
        Text = text,
        TextColor3 = Library.Theme.TextDim,
        TextSize = 12,
        TextWrapped = wrapped,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 10,
        Parent = row,
    })
    Library:BindTheme(lbl, "TextColor3", "TextDim")

    local attachTray = Library:Create("Frame", {
        BackgroundTransparency = 1,
        AnchorPoint = Vector2.new(1, 0.5),
        Position = UDim2.new(1, 0, 0.5, 0),
        Size = UDim2.fromOffset(80, 16),
        ZIndex = 12,
        Parent = row,
    })
    Library:Create("UIListLayout", {
        FillDirection = Enum.FillDirection.Horizontal,
        HorizontalAlignment = Enum.HorizontalAlignment.Right,
        VerticalAlignment = Enum.VerticalAlignment.Center,
        Padding = UDim.new(0, 5),
        Parent = attachTray,
    })

    local LabelObj = { Instance = lbl }
    function LabelObj:SetText(newText)
        lbl.Text = tostring(newText)
    end
    function LabelObj:AddColorPicker(cpCfg)
        return Library:CreateColorPickerAttachment(attachTray, cpCfg)
    end
    function LabelObj:AddKeybind(kbCfg)
        return Library:CreateKeybindAttachment(attachTray, kbCfg, nil)
    end
    return LabelObj
end

function GroupboxMethods:AddKeybind(cfg)
    local lbl = self:AddLabel(cfg.Name or "Keybind")
    return lbl:AddKeybind(cfg)
end

function GroupboxMethods:AddColorPicker(cfg)
    local lbl = self:AddLabel(cfg.Name or "Color")
    return lbl:AddColorPicker(cfg)
end

-- ============================================================================
-- WINDOW, TABS, SUBTABS, GROUPBOXES & TABBOXES (CS:GO SKEET/NEVERLOSE ARCHITECTURE)
-- ============================================================================
local function CreateGroupboxContainer(parentColumn, title, iconName)
    local wrapper = Library:Create("Frame", {
        Name = "Group_" .. tostring(title),
        BackgroundTransparency = 1,
        Size = UDim2.new(1, 0, 0, 60),
        ZIndex = 6,
        Parent = parentColumn,
    })

    local box = Library:Create("Frame", {
        BackgroundColor3 = Library.Theme.Groupbox,
        Position = UDim2.fromOffset(0, 7),
        Size = UDim2.new(1, 0, 1, -7),
        ZIndex = 6,
        Parent = wrapper,
    })
    Library:BindTheme(box, "BackgroundColor3", "Groupbox")
    Library:ApplyCSBorder(box, "InnerBorder")

    -- Subtle top accent bar along the groupbox header
    local topStripe = Library:Create("Frame", {
        BackgroundColor3 = Library.Theme.Accent,
        BorderSizePixel = 0,
        Size = UDim2.new(1, 0, 0, 1),
        BackgroundTransparency = 0.45,
        ZIndex = 7,
        Parent = box,
    })
    Library:BindTheme(topStripe, "BackgroundColor3", "Accent")

    -- Classic CS:GO Overlapping Title Pill on the Top Border
    local strTitle = string.upper(tostring(title or "GROUP"))
    local tBounds = Library:GetTextBounds(strTitle, 11, Vector2.new(250, 20))
    local headerW = tBounds.X + (iconName and 26 or 12)

    local headerBadge = Library:Create("Frame", {
        BackgroundColor3 = Library.Theme.Groupbox,
        BorderSizePixel = 0,
        Position = UDim2.fromOffset(10, 0),
        Size = UDim2.fromOffset(headerW, 14),
        ZIndex = 8,
        Parent = wrapper,
    })
    Library:BindTheme(headerBadge, "BackgroundColor3", "Groupbox")
    table.insert(Library.LayoutRefreshers, function()
        if not headerBadge or not headerBadge.Parent then return end
        local nb = Library:GetTextBounds(strTitle, 11, Vector2.new(250, 20))
        headerBadge.Size = UDim2.fromOffset(nb.X + (iconName and 26 or 12), 14)
    end)

    Library:Create("UIListLayout", {
        FillDirection = Enum.FillDirection.Horizontal,
        HorizontalAlignment = Enum.HorizontalAlignment.Center,
        VerticalAlignment = Enum.VerticalAlignment.Center,
        Padding = UDim.new(0, 4),
        Parent = headerBadge,
    })

    if iconName then
        local hIcon = Library:Create("ImageLabel", {
            BackgroundTransparency = 1,
            Size = UDim2.fromOffset(11, 11),
            Image = Library:GetIcon(iconName),
            ImageColor3 = Library.Theme.Accent,
            ZIndex = 9,
            Parent = headerBadge,
        })
        Library:BindTheme(hIcon, "ImageColor3", "Accent")
    end

    local hText = Library:Create("TextLabel", {
        BackgroundTransparency = 1,
        AutomaticSize = Enum.AutomaticSize.X,
        Size = UDim2.new(0, 0, 1, 0),
        Font = Enum.Font.GothamBold,
        Text = strTitle,
        TextColor3 = Library.Theme.Text,
        TextSize = 11,
        ZIndex = 9,
        Parent = headerBadge,
    })
    Library:BindTheme(hText, "TextColor3", "Text")

    local container = Library:Create("Frame", {
        Name = "Container",
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(10, 14),
        Size = UDim2.new(1, -20, 0, 0),
        ZIndex = 8,
        Parent = box,
    })

    local layout = Library:Create("UIListLayout", {
        Padding = UDim.new(0, 6),
        SortOrder = Enum.SortOrder.LayoutOrder,
        Parent = container,
    })

    Library:Connect(layout:GetPropertyChangedSignal("AbsoluteContentSize"), function()
        local contentH = layout.AbsoluteContentSize.Y
        container.Size = UDim2.new(1, -20, 0, contentH)
        wrapper.Size = UDim2.new(1, 0, 0, contentH + 30)
    end)

    return setmetatable({
        Wrapper = wrapper,
        Box = box,
        Container = container,
    }, GroupboxMethods)
end

-- Multi-Tab Groupbox (Classic CS:GO Tabbox where one groupbox switches between multiple internal subtabs)
local function CreateTabboxContainer(parentColumn, title)
    local wrapper = Library:Create("Frame", {
        Name = "Tabbox_" .. tostring(title or "Tabbox"),
        BackgroundTransparency = 1,
        Size = UDim2.new(1, 0, 0, 80),
        ZIndex = 6,
        Parent = parentColumn,
    })

    local box = Library:Create("Frame", {
        BackgroundColor3 = Library.Theme.Groupbox,
        Size = UDim2.fromScale(1, 1),
        ZIndex = 6,
        Parent = wrapper,
    })
    Library:BindTheme(box, "BackgroundColor3", "Groupbox")
    Library:ApplyCSBorder(box, "InnerBorder")

    local tabHeaderBar = Library:Create("Frame", {
        BackgroundColor3 = Library.Theme.Sidebar,
        BorderSizePixel = 0,
        Size = UDim2.new(1, 0, 0, 24),
        ZIndex = 7,
        Parent = box,
    })
    Library:BindTheme(tabHeaderBar, "BackgroundColor3", "Sidebar")

    local tabHeaderLayout = Library:Create("UIListLayout", {
        FillDirection = Enum.FillDirection.Horizontal,
        SortOrder = Enum.SortOrder.LayoutOrder,
        Parent = tabHeaderBar,
    })

    local bottomSep = Library:Create("Frame", {
        BackgroundColor3 = Library.Theme.InnerBorder,
        BorderSizePixel = 0,
        Position = UDim2.fromOffset(0, 24),
        Size = UDim2.new(1, 0, 0, 1),
        ZIndex = 8,
        Parent = box,
    })
    Library:BindTheme(bottomSep, "BackgroundColor3", "InnerBorder")

    local Tabbox = {
        Tabs = {},
        ActiveTab = nil,
    }

    local function RecalcButtonWidths()
        local count = math.max(1, #Tabbox.Tabs)
        for _, t in ipairs(Tabbox.Tabs) do
            t.Button.Size = UDim2.new(1 / count, 0, 1, 0)
        end
    end

    local function UpdateWrapperHeight()
        if Tabbox.ActiveTab then
            local h = Tabbox.ActiveTab.Layout.AbsoluteContentSize.Y
            Tabbox.ActiveTab.Container.Size = UDim2.new(1, -20, 0, h)
            wrapper.Size = UDim2.new(1, 0, 0, h + 42)
        end
    end

    function Tabbox:AddTab(subName)
        local btn = Library:Create("TextButton", {
            BackgroundColor3 = Library.Theme.Sidebar,
            AutoButtonColor = false,
            BorderSizePixel = 0,
            Size = UDim2.fromScale(1, 1),
            Font = Enum.Font.GothamBold,
            Text = string.upper(subName),
            TextColor3 = Library.Theme.TextDim,
            TextSize = 11,
            ZIndex = 8,
            Parent = tabHeaderBar,
        })

        local topLine = Library:Create("Frame", {
            BackgroundColor3 = Library.Theme.Accent,
            BorderSizePixel = 0,
            Size = UDim2.new(1, 0, 0, 2),
            BackgroundTransparency = 1,
            ZIndex = 9,
            Parent = btn,
        })
        Library:BindTheme(topLine, "BackgroundColor3", "Accent")

        local container = Library:Create("Frame", {
            BackgroundTransparency = 1,
            Position = UDim2.fromOffset(10, 32),
            Size = UDim2.new(1, -20, 0, 0),
            Visible = false,
            ZIndex = 8,
            Parent = box,
        })

        local layout = Library:Create("UIListLayout", {
            Padding = UDim.new(0, 6),
            SortOrder = Enum.SortOrder.LayoutOrder,
            Parent = container,
        })

        local subTabObj = setmetatable({
            Name = subName,
            Button = btn,
            TopLine = topLine,
            Container = container,
            Layout = layout,
        }, GroupboxMethods)

        Library:Connect(layout:GetPropertyChangedSignal("AbsoluteContentSize"), function()
            if Tabbox.ActiveTab == subTabObj then
                UpdateWrapperHeight()
            end
        end)

        local function Select()
            for _, other in ipairs(Tabbox.Tabs) do
                other.Container.Visible = false
                Library:Tween(other.Button, TweenFast, {
                    BackgroundColor3 = Library.Theme.Sidebar,
                    TextColor3 = Library.Theme.TextDim,
                })
                Library:Tween(other.TopLine, TweenFast, { BackgroundTransparency = 1 })
            end
            Tabbox.ActiveTab = subTabObj
            container.Visible = true
            Library:Tween(btn, TweenFast, {
                BackgroundColor3 = Library.Theme.Groupbox,
                TextColor3 = Library.Theme.Text,
            })
            Library:Tween(topLine, TweenFast, { BackgroundTransparency = 0 })
            UpdateWrapperHeight()
        end

        Library:Connect(btn.MouseButton1Click, Select)
        table.insert(Tabbox.Tabs, subTabObj)
        RecalcButtonWidths()

        if #Tabbox.Tabs == 1 then
            Select()
        end
        return subTabObj
    end

    return Tabbox
end

-- ============================================================================
-- MAIN WINDOW CREATION
-- ============================================================================
function Library:CreateWindow(cfg)
    cfg = cfg or {}
    self.Title = cfg.Title or cfg.Name or "shards.cc"
    local subTitle = cfg.SubTitle or cfg.Footer or "counter-strike / luau interface"
    local winSize = cfg.Size or UDim2.fromOffset(660, 520)
    self.ToggleKey = cfg.ToggleKey or Enum.KeyCode.Insert
    self.Folder = cfg.Folder or "ShardsUI"

    if cfg.Accent then
        self:SetAccent(cfg.Accent)
    end

    local main = self:Create("Frame", {
        Name = "MainWindow",
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundColor3 = self.Theme.Background,
        Position = UDim2.fromScale(0.5, 0.5),
        Size = winSize,
        ZIndex = 2,
        Parent = ScreenGui,
    })
    self:BindTheme(main, "BackgroundColor3", "Background")
    self:ApplyCSBorder(main, "HighlightBorder")

    local winScale = self:Create("UIScale", {
        Scale = 1,
        Parent = main,
    })

    -- Classic CS:GO 2px Top Gradient Accent Strip
    self:CreateAccentBar(main, 10)

    -- Top Header Bar
    local topBar = self:Create("Frame", {
        Name = "TopBar",
        BackgroundColor3 = self.Theme.Sidebar,
        BorderSizePixel = 0,
        Position = UDim2.fromOffset(0, 2),
        Size = UDim2.new(1, 0, 0, 28),
        ZIndex = 4,
        Parent = main,
    })
    self:BindTheme(topBar, "BackgroundColor3", "Sidebar")
    self:MakeDraggable(topBar, main)

    local topSep = self:Create("Frame", {
        BackgroundColor3 = self.Theme.InnerBorder,
        BorderSizePixel = 0,
        Position = UDim2.new(0, 0, 1, 0),
        Size = UDim2.new(1, 0, 0, 1),
        ZIndex = 5,
        Parent = topBar,
    })
    self:BindTheme(topSep, "BackgroundColor3", "InnerBorder")

    local brandIcon = self:Create("ImageLabel", {
        AnchorPoint = Vector2.new(0, 0.5),
        BackgroundTransparency = 1,
        Position = UDim2.new(0, 10, 0.5, 0),
        Size = UDim2.fromOffset(14, 14),
        Image = self:GetIcon(cfg.Icon or "crosshair"),
        ImageColor3 = self.Theme.Accent,
        ZIndex = 5,
        Parent = topBar,
    })
    self:BindTheme(brandIcon, "ImageColor3", "Accent")

    local titleLabel = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(30, 0),
        Size = UDim2.new(0.5, 0, 1, 0),
        Font = Enum.Font.GothamBold,
        Text = string.upper(self.Title),
        TextColor3 = self.Theme.Text,
        TextSize = 12,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 5,
        Parent = topBar,
    })
    self:BindTheme(titleLabel, "TextColor3", "Text")

    local rightBadge = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.new(0.5, 0, 0, 0),
        Size = UDim2.new(0.5, -10, 1, 0),
        Font = Enum.Font.Code,
        Text = subTitle .. "  [" .. self:FormatKey(self.ToggleKey) .. "]",
        TextColor3 = self.Theme.TextMuted,
        TextSize = 11,
        TextXAlignment = Enum.TextXAlignment.Right,
        ZIndex = 5,
        Parent = topBar,
    })
    self:BindTheme(rightBadge, "TextColor3", "TextMuted")

    -- Left Vertical CS:GO Sidebar (Lucide Icon + Label Tabs)
    local sidebar = self:Create("Frame", {
        Name = "Sidebar",
        BackgroundColor3 = self.Theme.Sidebar,
        BorderSizePixel = 0,
        Position = UDim2.fromOffset(0, 31),
        Size = UDim2.new(0, 148, 1, -53),
        ZIndex = 4,
        Parent = main,
    })
    self:BindTheme(sidebar, "BackgroundColor3", "Sidebar")

    local sideSep = self:Create("Frame", {
        BackgroundColor3 = self.Theme.InnerBorder,
        BorderSizePixel = 0,
        Position = UDim2.new(1, 0, 0, 0),
        Size = UDim2.new(0, 1, 1, 0),
        ZIndex = 5,
        Parent = sidebar,
    })
    self:BindTheme(sideSep, "BackgroundColor3", "InnerBorder")

    local tabList = self:Create("ScrollingFrame", {
        BackgroundTransparency = 1,
        BorderSizePixel = 0,
        Position = UDim2.fromOffset(6, 8),
        Size = UDim2.new(1, -12, 1, -16),
        CanvasSize = UDim2.fromOffset(0, 0),
        ScrollBarThickness = 0,
        ZIndex = 5,
        Parent = sidebar,
    })
    local tabListLayout = self:Create("UIListLayout", {
        Padding = UDim.new(0, 4),
        SortOrder = Enum.SortOrder.LayoutOrder,
        Parent = tabList,
    })
    self:Connect(tabListLayout:GetPropertyChangedSignal("AbsoluteContentSize"), function()
        tabList.CanvasSize = UDim2.fromOffset(0, tabListLayout.AbsoluteContentSize.Y + 8)
    end)

    -- Content Area (Right of Sidebar)
    local contentArea = self:Create("Frame", {
        Name = "ContentArea",
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(149, 31),
        Size = UDim2.new(1, -149, 1, -53),
        ZIndex = 4,
        Parent = main,
    })

    -- Bottom Status Bar
    local footer = self:Create("Frame", {
        Name = "Footer",
        BackgroundColor3 = self.Theme.Sidebar,
        BorderSizePixel = 0,
        Position = UDim2.new(0, 0, 1, -22),
        Size = UDim2.new(1, 0, 0, 22),
        ZIndex = 4,
        Parent = main,
    })
    self:BindTheme(footer, "BackgroundColor3", "Sidebar")

    local footSep = self:Create("Frame", {
        BackgroundColor3 = self.Theme.InnerBorder,
        BorderSizePixel = 0,
        Size = UDim2.new(1, 0, 0, 1),
        ZIndex = 5,
        Parent = footer,
    })
    self:BindTheme(footSep, "BackgroundColor3", "InnerBorder")

    local footLeft = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(10, 0),
        Size = UDim2.new(0.5, 0, 1, 0),
        Font = Enum.Font.Code,
        Text = "build: v" .. self.Version .. " | user: " .. (LocalPlayer and LocalPlayer.Name or "local"),
        TextColor3 = self.Theme.TextMuted,
        TextSize = 11,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 5,
        Parent = footer,
    })
    self:BindTheme(footLeft, "TextColor3", "TextMuted")

    local footRight = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.new(0.5, 0, 0, 0),
        Size = UDim2.new(0.5, -10, 1, 0),
        Font = Enum.Font.Code,
        Text = "status: undetected / active",
        TextColor3 = self.Theme.Accent,
        TextSize = 11,
        TextXAlignment = Enum.TextXAlignment.Right,
        ZIndex = 5,
        Parent = footer,
    })
    self:BindTheme(footRight, "TextColor3", "Accent")

    local Window = {
        Main = main,
        Tabs = {},
        ActiveTab = nil,
    }

    function Window:SetOpen(state)
        Library.Open = state
        Library:ClosePopups()
        TooltipFrame.Visible = false
        if state then
            main.Visible = true
            winScale.Scale = 0.95
            Library:Tween(winScale, TweenMedium, { Scale = 1 })
        else
            local tw = Library:Tween(winScale, TweenFast, { Scale = 0.95 })
            task.delay(0.14, function()
                if not Library.Open then
                    main.Visible = false
                end
            end)
        end
    end

    function Window:Toggle()
        Window:SetOpen(not Library.Open)
    end

    Library:Connect(UserInputService.InputBegan, function(input, gpe)
        if not gpe and input.KeyCode == self.ToggleKey then
            Window:Toggle()
        end
    end)

    -- Add a Sidebar Category Divider (e.g., "COMBAT", "VISUALS", "MISC")
    function Window:AddTabSection(secTitle)
        local lbl = Library:Create("TextLabel", {
            BackgroundTransparency = 1,
            Size = UDim2.new(1, 0, 0, 16),
            Font = Enum.Font.GothamBold,
            Text = "  " .. string.upper(tostring(secTitle)),
            TextColor3 = Library.Theme.TextMuted,
            TextSize = 10,
            TextXAlignment = Enum.TextXAlignment.Left,
            ZIndex = 6,
            Parent = tabList,
        })
        Library:BindTheme(lbl, "TextColor3", "TextMuted")
    end

    -- Add a Main Tab with Lucide Icon
    function Window:AddTab(tabCfg)
        tabCfg = type(tabCfg) == "string" and { Name = tabCfg } or (tabCfg or {})
        local tabName = tabCfg.Name or "Tab"
        local tabIcon = tabCfg.Icon or "sliders"

        local tabBtn = Library:Create("TextButton", {
            Name = "TabBtn_" .. tabName,
            BackgroundColor3 = Library.Theme.Element,
            BackgroundTransparency = 1,
            AutoButtonColor = false,
            Size = UDim2.new(1, 0, 0, 30),
            Text = "",
            ZIndex = 6,
            Parent = tabList,
        })

        local btnBorder = Library:Create("UIStroke", {
            Color = Library.Theme.InnerBorder,
            Thickness = 1,
            Transparency = 1,
            ApplyStrokeMode = Enum.ApplyStrokeMode.Border,
            Parent = tabBtn,
        })

        local activeBar = Library:Create("Frame", {
            BackgroundColor3 = Library.Theme.Accent,
            BorderSizePixel = 0,
            Position = UDim2.fromOffset(0, 0),
            Size = UDim2.new(0, 2, 0, 0),
            AnchorPoint = Vector2.new(0, 0.5),
            ZIndex = 7,
            Parent = tabBtn,
        })
        activeBar.Position = UDim2.new(0, 0, 0.5, 0)
        Library:BindTheme(activeBar, "BackgroundColor3", "Accent")

        local iconImg = Library:Create("ImageLabel", {
            AnchorPoint = Vector2.new(0, 0.5),
            BackgroundTransparency = 1,
            Position = UDim2.new(0, 10, 0.5, 0),
            Size = UDim2.fromOffset(15, 15),
            Image = Library:GetIcon(tabIcon),
            ImageColor3 = Library.Theme.TextDim,
            ZIndex = 7,
            Parent = tabBtn,
        })

        local tabLbl = Library:Create("TextLabel", {
            BackgroundTransparency = 1,
            Position = UDim2.fromOffset(32, 0),
            Size = UDim2.new(1, -36, 1, 0),
            Font = Enum.Font.Code,
            Text = tabName,
            TextColor3 = Library.Theme.TextDim,
            TextSize = 12,
            TextXAlignment = Enum.TextXAlignment.Left,
            ZIndex = 7,
            Parent = tabBtn,
        })

        -- Page Container (Two-Column CS:GO Scrollable Page)
        local page = Library:Create("ScrollingFrame", {
            Name = "Page_" .. tabName,
            BackgroundTransparency = 1,
            BorderSizePixel = 0,
            Position = UDim2.fromOffset(10, 10),
            Size = UDim2.new(1, -20, 1, -20),
            CanvasSize = UDim2.fromOffset(0, 0),
            ScrollBarThickness = 2,
            ScrollBarImageColor3 = Library.Theme.Accent,
            Visible = false,
            ZIndex = 5,
            Parent = contentArea,
        })
        Library:BindTheme(page, "ScrollBarImageColor3", "Accent")

        local leftCol = Library:Create("Frame", {
            Name = "LeftColumn",
            BackgroundTransparency = 1,
            Position = UDim2.fromOffset(2, 2),
            Size = UDim2.new(0.5, -7, 0, 0),
            ZIndex = 5,
            Parent = page,
        })
        local leftLayout = Library:Create("UIListLayout", {
            Padding = UDim.new(0, 10),
            SortOrder = Enum.SortOrder.LayoutOrder,
            Parent = leftCol,
        })

        local rightCol = Library:Create("Frame", {
            Name = "RightColumn",
            BackgroundTransparency = 1,
            Position = UDim2.new(0.5, 5, 0, 2),
            Size = UDim2.new(0.5, -7, 0, 0),
            ZIndex = 5,
            Parent = page,
        })
        local rightLayout = Library:Create("UIListLayout", {
            Padding = UDim.new(0, 10),
            SortOrder = Enum.SortOrder.LayoutOrder,
            Parent = rightCol,
        })

        local function UpdateCanvas()
            local maxH = math.max(leftLayout.AbsoluteContentSize.Y, rightLayout.AbsoluteContentSize.Y)
            page.CanvasSize = UDim2.fromOffset(0, maxH + 16)
        end
        Library:Connect(leftLayout:GetPropertyChangedSignal("AbsoluteContentSize"), UpdateCanvas)
        Library:Connect(rightLayout:GetPropertyChangedSignal("AbsoluteContentSize"), UpdateCanvas)

        local Tab = {
            Name = tabName,
            Button = tabBtn,
            Page = page,
            Left = leftCol,
            Right = rightCol,
            ActiveBar = activeBar,
            Icon = iconImg,
            Label = tabLbl,
            Border = btnBorder,
        }

        function Tab:Select()
            Library:ClosePopups()
            for _, other in ipairs(Window.Tabs) do
                other.Page.Visible = false
                Library:Tween(other.Button, TweenFast, { BackgroundTransparency = 1 })
                Library:Tween(other.Border, TweenFast, { Transparency = 1 })
                Library:Tween(other.ActiveBar, TweenFast, { Size = UDim2.new(0, 2, 0, 0) })
                Library:Tween(other.Icon, TweenFast, { ImageColor3 = Library.Theme.TextDim })
                Library:Tween(other.Label, TweenFast, { TextColor3 = Library.Theme.TextDim })
            end
            Window.ActiveTab = Tab
            page.Visible = true
            Library:Tween(tabBtn, TweenFast, {
                BackgroundColor3 = Library.Theme.Element,
                BackgroundTransparency = 0,
            })
            Library:Tween(btnBorder, TweenFast, {
                Color = Library.Theme.InnerBorder,
                Transparency = 0,
            })
            Library:Tween(activeBar, TweenMedium, { Size = UDim2.new(0, 2, 1, -6) })
            Library:Tween(iconImg, TweenFast, { ImageColor3 = Library.Theme.Accent })
            Library:Tween(tabLbl, TweenFast, { TextColor3 = Library.Theme.Text })
        end

        function Tab:AddLeftGroupbox(title, icon)
            return CreateGroupboxContainer(leftCol, title, icon)
        end

        function Tab:AddRightGroupbox(title, icon)
            return CreateGroupboxContainer(rightCol, title, icon)
        end

        function Tab:AddLeftTabbox(title)
            return CreateTabboxContainer(leftCol, title)
        end

        function Tab:AddRightTabbox(title)
            return CreateTabboxContainer(rightCol, title)
        end

        Library:Connect(tabBtn.MouseEnter, function()
            if Window.ActiveTab ~= Tab then
                Library:Tween(tabBtn, TweenFast, {
                    BackgroundColor3 = Library.Theme.Element,
                    BackgroundTransparency = 0.45,
                })
                Library:Tween(tabLbl, TweenFast, { TextColor3 = Library.Theme.Text })
            end
        end)

        Library:Connect(tabBtn.MouseLeave, function()
            if Window.ActiveTab ~= Tab then
                Library:Tween(tabBtn, TweenFast, { BackgroundTransparency = 1 })
                Library:Tween(tabLbl, TweenFast, { TextColor3 = Library.Theme.TextDim })
            end
        end)

        Library:Connect(tabBtn.MouseButton1Click, function()
            Tab:Select()
        end)

        table.insert(Window.Tabs, Tab)
        if #Window.Tabs == 1 then
            Tab:Select()
        end
        return Tab
    end

    return Window
end

-- ============================================================================
-- BUILT-IN SETTINGS, THEME & JSON CONFIG MANAGER BUILDER
-- ============================================================================
function Library:GetConfigJSON()
    local data = {}
    for flag, opt in pairs(self.Options) do
        if opt.Type == "Toggle" or opt.Type == "Slider" or opt.Type == "Input" or opt.Type == "Dropdown" or opt.Type == "Listbox" then
            data[flag] = { Type = opt.Type, Value = opt.Value }
        elseif opt.Type == "ColorPicker" then
            local c = opt.Value
            data[flag] = {
                Type = "ColorPicker",
                R = c.R, G = c.G, B = c.B,
                Alpha = opt.Alpha,
            }
        elseif opt.Type == "Keybind" then
            data[flag] = {
                Type = "Keybind",
                Key = typeof(opt.Value) == "EnumItem" and opt.Value.Name or tostring(opt.Value),
                Mode = opt.Mode,
            }
        end
    end
    return HttpService:JSONEncode(data)
end

function Library:LoadConfigJSON(jsonStr)
    local ok, decoded = pcall(function()
        return HttpService:JSONDecode(jsonStr)
    end)
    if not ok or type(decoded) ~= "table" then
        self:Notify({ Title = "Config Error", Description = "Invalid JSON config payload.", Icon = "alert-triangle", Color = self.Theme.Risky })
        return false
    end
    for flag, entry in pairs(decoded) do
        local opt = self.Options[flag]
        if opt and type(entry) == "table" then
            if entry.Type == "ColorPicker" and entry.R then
                opt:Set(Color3.new(entry.R, entry.G, entry.B), entry.Alpha or 0)
            elseif entry.Type == "Keybind" and entry.Key then
                local kc = Enum.KeyCode.Unknown
                pcall(function() kc = Enum.KeyCode[entry.Key] end)
                if kc == Enum.KeyCode.Unknown then
                    pcall(function() kc = Enum.UserInputType[entry.Key] end)
                end
                opt:Set(kc)
                if entry.Mode then opt:SetMode(entry.Mode) end
            elseif opt.Set then
                opt:Set(entry.Value)
            end
        end
    end
    self:Notify({ Title = "Config Loaded", Description = "Configuration applied successfully.", Icon = "check" })
    return true
end

function Library:BuildConfigTab(tab)
    local themeGroup = tab:AddLeftGroupbox("Menu & Theme", "palette")

    local wmToggle = themeGroup:AddToggle({
        Name = "Show Watermark",
        Flag = "UI_ShowWatermark",
        Default = true,
        Callback = function(v)
            if self.Watermark then self.Watermark:SetVisible(v) end
        end
    })

    themeGroup:AddToggle({
        Name = "Show Keybind List",
        Flag = "UI_ShowKeybinds",
        Default = true,
        Callback = function(v)
            if self.KeybindList then self.KeybindList:SetVisible(v) end
        end
    })

    local menuBindLbl = themeGroup:AddLabel("Menu Toggle Key")
    menuBindLbl:AddKeybind({
        Name = "Menu Key",
        Flag = "UI_MenuKeybind",
        Default = self.ToggleKey,
        Callback = function(_, newKey)
            if typeof(newKey) == "EnumItem" then
                self.ToggleKey = newKey
            end
        end
    }).Changed:Connect(function(newKey)
        if typeof(newKey) == "EnumItem" and newKey.EnumType == Enum.KeyCode and newKey ~= Enum.KeyCode.Unknown then
            self.ToggleKey = newKey
        end
    end)

    themeGroup:AddDivider("Accent & Presets", "sparkles")

    local accentLbl = themeGroup:AddLabel("Primary Menu Accent")
    local accentPicker = accentLbl:AddColorPicker({
        Name = "Menu Accent",
        Flag = "UI_AccentColor",
        Default = self.Theme.Accent,
        Callback = function(col)
            self:SetAccent(col)
        end
    })

    local presetNames = {}
    for k in pairs(self.Presets) do
        table.insert(presetNames, k)
    end
    table.sort(presetNames)

    themeGroup:AddDropdown({
        Name = "Cheat Theme Preset",
        Flag = "UI_ThemePreset",
        Values = presetNames,
        Default = "Skeet Lime",
        Callback = function(choice)
            local p = self.Presets[choice]
            if p then
                accentPicker:Set(p.Accent, 0)
                self:SetAccent(p.Accent, p.AccentDark)
            end
        end
    })

    themeGroup:AddDropdown({
        Name = "UI Font Style",
        Flag = "UI_FontPreset",
        Values = { "Pixelated (CS Bitmap)", "CS2 Crisp (RobotoMono)", "Skeet Mono (Code)", "Classic (Tahoma)" },
        Default = "Pixelated (CS Bitmap)",
        Callback = function(choice)
            self:SetFontPreset(choice)
            if self.KeybindList then self.KeybindList:Refresh() end
        end
    })

    themeGroup:AddButton({
        Name = "Unload UI Library",
        Icon = "trash",
        Risky = true,
        Confirm = true,
        Callback = function()
            self:Unload()
        end
    })

    -- Right Side Config Manager
    local cfgGroup = tab:AddRightGroupbox("Configuration", "folder")
    local cfgName = "legit_default"
    local selectedCfg = "legit_default"
    local savedMemoryConfigs = {
        ["legit_default"] = self:GetConfigJSON()
    }

    cfgGroup:AddInput({
        Name = "Config Name",
        Flag = "UI_ConfigName",
        Default = "legit_default",
        Placeholder = "e.g. rage_semi",
        Callback = function(txt)
            cfgName = txt ~= "" and txt or "default"
        end
    })

    local cfgList = cfgGroup:AddListbox({
        Name = "Saved Configs",
        Flag = "UI_ConfigList",
        Height = 95,
        Values = { "legit_default", "rage_hvh", "movement_bhop", "closet_main" },
        Default = "legit_default",
        Callback = function(val)
            selectedCfg = val
        end
    })

    local createBtn = cfgGroup:AddButton({
        Name = "Save Config",
        Icon = "save",
        Callback = function()
            local target = cfgName ~= "" and cfgName or selectedCfg
            local json = self:GetConfigJSON()
            savedMemoryConfigs[target] = json
            pcall(function()
                if isfolder and makefolder and writefile then
                    if not isfolder(self.Folder) then makefolder(self.Folder) end
                    writefile(self.Folder .. "/" .. target .. ".json", json)
                end
            end)
            if not table.find(cfgList.Values, target) then
                table.insert(cfgList.Values, target)
                cfgList:Refresh(cfgList.Values)
            end
            self:Notify({ Title = "Config Saved", Description = "Saved profile '" .. target .. "'.", Icon = "save" })
        end
    })

    createBtn:AddButton({
        Name = "Load Config",
        Icon = "folder",
        Callback = function()
            local raw = savedMemoryConfigs[selectedCfg]
            pcall(function()
                if isfile and readfile and isfile(self.Folder .. "/" .. selectedCfg .. ".json") then
                    raw = readfile(self.Folder .. "/" .. selectedCfg .. ".json")
                end
            end)
            if raw then
                self:LoadConfigJSON(raw)
            else
                self:Notify({ Title = "Config", Description = "Loaded preset '" .. tostring(selectedCfg) .. "'.", Icon = "check" })
            end
        end
    })

    cfgGroup:AddButton({
        Name = "Export Config to Clipboard",
        Icon = "code",
        Callback = function()
            local json = self:GetConfigJSON()
            pcall(function()
                if setclipboard then setclipboard(json) end
            end)
            self:Notify({ Title = "Clipboard", Description = "Exported JSON config string.", Icon = "code" })
        end
    })
end

function Library:Unload()
    self.Unloaded = true
    for _, conn in ipairs(self.Connections) do
        pcall(function() conn:Disconnect() end)
    end
    if ScreenGui and ScreenGui.Parent then
        ScreenGui:Destroy()
    end
end

return Library
