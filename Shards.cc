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
local GuiService = game:GetService("GuiService")

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
    SearchEntries = {},
    SearchQuery = "",
    CopiedColor = Color3.fromRGB(155, 195, 55),
    CopiedAlpha = 0,
    ActiveKeybinds = {},
    KeybindOrder = {},
    KeybindCounter = 0,
    HUDFrames = {},
    Notifications = {},
    OpenPopups = {},
    Folder = "ShardsUI",
    Font = Enum.Font.Gotham,
    FontBold = Enum.Font.GothamBold,
    FontScale = 0.95,
    TextStroke = true,
    FontOrder = {
        "Minimalist (Gotham / GothamBold)",
        "Skeet Classic (Code / Inconsolata)",
        "CS2 Console (RobotoMono)",
        "Gamesense Tahoma (Legacy)",
        "Pixelated (Arcade 0.85)",
    },
    Fonts = {
        ["Minimalist (Gotham / GothamBold)"]  = { Font = Enum.Font.Gotham,          Bold = Enum.Font.GothamBold,      Scale = 0.95 },
        ["Skeet Classic (Code / Inconsolata)"]= { Font = Enum.Font.Code,            Bold = Enum.Font.GothamBold,      Scale = 1.00 },
        ["CS2 Console (RobotoMono)"]          = { Font = Enum.Font.RobotoMono,      Bold = Enum.Font.RobotoMono,      Scale = 1.00 },
        ["Gamesense Tahoma (Legacy)"]         = { Font = Enum.Font.Legacy,          Bold = Enum.Font.Legacy,          Scale = 0.90 },
        ["Pixelated (Arcade 0.85)"]           = { Font = Enum.Font.Arcade,          Bold = Enum.Font.Arcade,          Scale = 0.85 },
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
    -- Tactical / Combat / Aimbot
    ["crosshair"]       = "rbxassetid://10709818534",
    ["target"]          = "rbxassetid://10734977012",
    ["scan"]            = "rbxassetid://10734942565",
    ["focus"]           = "rbxassetid://10723377537",
    ["locate-fixed"]    = "rbxassetid://10723434557",
    ["swords"]          = "rbxassetid://10734975692",
    ["sword"]           = "rbxassetid://10734975486",
    ["skull"]           = "rbxassetid://10734962068",
    ["flame"]           = "rbxassetid://10723376114",
    ["zap"]             = "rbxassetid://10747384394",

    -- Anti-Aim / Defense / Security
    ["shield"]          = "rbxassetid://10734951847",
    ["shield-alert"]    = "rbxassetid://10734951173",
    ["shield-check"]    = "rbxassetid://10734951367",
    ["lock"]            = "rbxassetid://10723434711",
    ["unlock"]          = "rbxassetid://10747366027",

    -- Visuals / ESP / World / Radar
    ["eye"]             = "rbxassetid://10723346959",
    ["eye-off"]         = "rbxassetid://10723346871",
    ["radar"]           = "rbxassetid://10734930886",
    ["radio"]           = "rbxassetid://10734931596",
    ["wifi"]            = "rbxassetid://10747382504",
    ["box"]             = "rbxassetid://10709782497",
    ["layers"]          = "rbxassetid://10723424505",
    ["compass"]         = "rbxassetid://10709811445",
    ["globe"]           = "rbxassetid://10723404337",
    ["sun"]             = "rbxassetid://10734974297",
    ["moon"]            = "rbxassetid://10734897102",
    ["camera"]          = "rbxassetid://10709789686",
    ["video"]           = "rbxassetid://10747374938",
    ["maximize"]        = "rbxassetid://10723426886",

    -- Players / Movement / Telemetry
    ["user"]            = "rbxassetid://10747373176",
    ["users"]           = "rbxassetid://10747373426",
    ["user-check"]      = "rbxassetid://10747371901",
    ["user-plus"]       = "rbxassetid://10747372702",
    ["footprints"]      = "rbxassetid://10723392681",
    ["wind"]            = "rbxassetid://10747382750",
    ["activity"]        = "rbxassetid://10709752035",
    ["gauge"]           = "rbxassetid://10723395708",
    ["bar-chart"]       = "rbxassetid://10709773755",

    -- System / Config / Controls
    ["sliders"]         = "rbxassetid://10734963400",
    ["settings"]        = "rbxassetid://10734950309",
    ["wrench"]          = "rbxassetid://10747383470",
    ["cpu"]             = "rbxassetid://10709818399",
    ["terminal"]        = "rbxassetid://10734982144",
    ["code"]            = "rbxassetid://10709810463",
    ["folder"]          = "rbxassetid://10723387563",
    ["file-text"]       = "rbxassetid://10723374840",
    ["save"]            = "rbxassetid://10734941499",
    ["download"]        = "rbxassetid://10723344270",
    ["upload"]          = "rbxassetid://10747366434",
    ["clipboard"]       = "rbxassetid://10709799288",
    ["copy"]            = "rbxassetid://10709812159",
    ["trash"]           = "rbxassetid://10747362393",
    ["refresh-cw"]      = "rbxassetid://10734933222",
    ["search"]          = "rbxassetid://10734943674",
    ["check"]           = "rbxassetid://10709790644",
    ["x"]               = "rbxassetid://10747383819",
    ["chevron-down"]    = "rbxassetid://10709790948",
    ["chevron-up"]      = "rbxassetid://10709791523",
    ["chevron-right"]   = "rbxassetid://10709791437",
    ["bell"]            = "rbxassetid://10709775704",
    ["info"]            = "rbxassetid://10723415903",
    ["alert-triangle"]  = "rbxassetid://10709753149",
    ["palette"]         = "rbxassetid://10734910430",
    ["keyboard"]        = "rbxassetid://10723416765",
    ["mouse-pointer"]   = "rbxassetid://10734898592",
    ["sparkles"]        = "rbxassetid://10734966248",
    ["wand"]            = "rbxassetid://10747376565",
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
        if self.FontFace then
            pcall(function()
                inst.FontFace = isBold and (self.FontFaceBold or self.FontFace) or self.FontFace
            end)
        else
            inst.Font = isBold and self.FontBold or self.Font
        end
        inst.TextSize = math.max(8, math.floor(rawSize * self.FontScale + 0.5))
        inst.TextStrokeColor3 = Color3.fromRGB(0, 0, 0)
        inst.TextStrokeTransparency = self.TextStroke and 0.18 or 1
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

function Library:GetTextBounds(text, baseSize, maxVec, isBold)
    local scaledSize = math.max(8, math.floor((baseSize or 12) * self.FontScale + 0.5))
    local targetFont = isBold and (self.FontBold or self.Font) or self.Font
    local ok, res = pcall(function()
        return TextService:GetTextSize(tostring(text or ""), scaledSize, targetFont, maxVec or Vector2.new(500, 200))
    end)
    return ok and res or Vector2.new(#tostring(text or "") * 7, scaledSize + 2)
end

function Library:PruneRegistries()
    for i = #self.TextObjects, 1, -1 do
        local entry = self.TextObjects[i]
        if not entry.Instance or not entry.Instance.Parent then
            table.remove(self.TextObjects, i)
        end
    end
    for i = #self.ThemeObjects, 1, -1 do
        local entry = self.ThemeObjects[i]
        if not entry.Instance or not entry.Instance.Parent then
            table.remove(self.ThemeObjects, i)
        end
    end
end

function Library:RefreshAllText()
    for i = #self.TextObjects, 1, -1 do
        local entry = self.TextObjects[i]
        if entry.Instance and entry.Instance.Parent then
            if self.FontFace then
                local ok = pcall(function()
                    entry.Instance.FontFace = entry.IsBold and (self.FontFaceBold or self.FontFace) or self.FontFace
                end)
                if not ok then
                    entry.Instance.Font = entry.IsBold and self.FontBold or self.Font
                end
            else
                entry.Instance.Font = entry.IsBold and self.FontBold or self.Font
            end
            entry.Instance.TextSize = math.max(8, math.floor(entry.BaseSize * self.FontScale + 0.5))
            entry.Instance.TextStrokeTransparency = self.TextStroke and 0.18 or 1
        else
            table.remove(self.TextObjects, i)
        end
    end
    for _, fn in ipairs(self.LayoutRefreshers) do
        pcall(fn)
    end
end

function Library:SetFontPreset(presetName)
    local preset = self.Fonts[presetName]
    if not preset then return end
    self.CurrentFontPreset = presetName
    self.FontScale = preset.Scale or 1.00
    if preset.Family and type(Font) == "table" and Font.new then
        local ok, ff = pcall(function()
            return Font.new(preset.Family, Enum.FontWeight.Regular, Enum.FontStyle.Normal)
        end)
        local okB, ffb = pcall(function()
            return Font.new(preset.Family, Enum.FontWeight.Bold, Enum.FontStyle.Normal)
        end)
        if ok and ff then
            self.FontFace = ff
            self.FontFaceBold = (okB and ffb) or ff
            self.Font = preset.Fallback or Enum.Font.Code
            self.FontBold = preset.Fallback or Enum.Font.Code
        else
            self.FontFace = nil
            self.FontFaceBold = nil
            self.Font = preset.Fallback or Enum.Font.Arcade
            self.FontBold = preset.Fallback or Enum.Font.Arcade
        end
    else
        self.FontFace = nil
        self.FontFaceBold = nil
        self.Font = preset.Font or Enum.Font.Code
        self.FontBold = preset.Bold or self.Font
    end
    self:RefreshAllText()
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

function Library:GetGuiInsetY()
    if self.ScreenGui and self.ScreenGui.IgnoreGuiInset then
        local ok, inset = pcall(function()
            local tl = GuiService:GetGuiInset()
            return tl.Y
        end)
        return (ok and type(inset) == "number") and inset or 0
    end
    return 0
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

    local borderFolder = self:Create("Folder", {
        Name = "CSBorderFolder",
        Parent = frame,
    })

    local outerWrap = self:Create("Frame", {
        Name = "OuterBorder",
        BackgroundTransparency = 1,
        Active = false,
        Position = UDim2.new(0, -1, 0, -1),
        Size = UDim2.new(1, 2, 1, 2),
        ZIndex = math.max(1, frame.ZIndex - 1),
        Parent = borderFolder,
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

-- ============================================================================
-- COUNTER BLOX & UNIVERSAL ROBLOX TELEMETRY ADAPTER
-- ============================================================================
function Library:GetPlayerTelemetry(plr)
    if not plr then return nil end
    local char = plr.Character or (workspace:FindFirstChild(plr.Name))
    local statusFolder = plr:FindFirstChild("Status")
    local nrpbs = plr:FindFirstChild("NRPBS")

    -- Counter Blox Status.Team ("T", "CT", "Spectator")
    local cbTeam = statusFolder and statusFolder:FindFirstChild("Team") and statusFolder.Team.Value or nil
    if cbTeam == "Spectator" then
        return { Alive = false, Spectating = true, Team = "Spectator" }
    end

    local aliveVal = statusFolder and statusFolder:FindFirstChild("Alive")
    if aliveVal and aliveVal.Value == false then
        return { Alive = false, Spectating = true, Team = cbTeam or (plr.Team and plr.Team.Name) }
    end

    if not char then
        return { Alive = false, Spectating = false }
    end

    local hum = char:FindFirstChildOfClass("Humanoid")
    local hrp = char:FindFirstChild("HumanoidRootPart") or char:FindFirstChild("Torso") or char:FindFirstChild("UpperTorso") or char.PrimaryPart
    if not hrp then
        return { Alive = false, Spectating = false }
    end

    local hp = 100
    local maxHp = 100
    if nrpbs and nrpbs:FindFirstChild("Health") then
        hp = tonumber(nrpbs.Health.Value) or 100
        local mh = nrpbs:FindFirstChild("MaxHealth")
        maxHp = (mh and tonumber(mh.Value)) or 100
    elseif hum then
        hp = tonumber(hum.Health) or 0
        maxHp = tonumber(hum.MaxHealth) or 100
    end

    if hp <= 0 then
        return { Alive = false, Spectating = false }
    end

    -- Counter Blox EquippedTool (StringValue/ObjectValue on Character) or standard Tool
    local weapName = "UNARMED"
    local eqVal = char:FindFirstChild("EquippedTool")
    if eqVal and eqVal.Value and tostring(eqVal.Value) ~= "" then
        weapName = string.upper(tostring(eqVal.Value))
    else
        local tool = char:FindFirstChildOfClass("Tool")
        if tool then
            weapName = string.upper(tool.Name)
        end
    end

    -- Counter Blox Armor / Helmet / Scoped / Flashed / Ping / LC telemetry
    local hasArmor = false
    local hasHelmet = false
    local isScoped = false
    local isFlashed = false
    if nrpbs then
        local arm = nrpbs:FindFirstChild("Armor")
        local helm = nrpbs:FindFirstChild("Helmet")
        hasArmor = arm and (tonumber(arm.Value) or 0) > 0 or false
        hasHelmet = helm and (helm.Value == true or (tonumber(helm.Value) or 0) > 0) or false
    elseif statusFolder then
        local arm = statusFolder:FindFirstChild("Armor")
        hasArmor = arm and (tonumber(arm.Value) or 0) > 0 or false
    elseif char:FindFirstChild("Armor") then
        hasArmor = true
    end

    if statusFolder then
        local sc = statusFolder:FindFirstChild("Scoped") or statusFolder:FindFirstChild("ADS")
        local fl = statusFolder:FindFirstChild("Flashed") or statusFolder:FindFirstChild("Blinded")
        isScoped = (sc and sc.Value == true) or false
        isFlashed = (fl and (fl.Value == true or (tonumber(fl.Value) or 0) > 0)) or false
    end
    if not isScoped and (string.find(weapName, "AWP") or string.find(weapName, "SCOUT") or string.find(weapName, "SSG") or string.find(weapName, "SCAR") or string.find(weapName, "G3SG1")) then
        isScoped = (char:GetAttribute("Scoped") == true)
    end

    local velMag = 0
    pcall(function()
        velMag = Vector2.new(hrp.AssemblyLinearVelocity.X, hrp.AssemblyLinearVelocity.Z).Magnitude
    end)

    local isTeammate = false
    if LocalPlayer then
        local myStatus = LocalPlayer:FindFirstChild("Status")
        local myCbTeam = myStatus and myStatus:FindFirstChild("Team") and myStatus.Team.Value or nil
        if cbTeam and myCbTeam then
            isTeammate = (cbTeam == myCbTeam)
        elseif plr.Team ~= nil and LocalPlayer.Team ~= nil then
            isTeammate = (plr.Team == LocalPlayer.Team)
        end
    end

    return {
        Alive = true,
        Character = char,
        Humanoid = hum,
        RootPart = hrp,
        Head = char:FindFirstChild("Head") or hrp,
        HP = math.clamp(math.floor(hp + 0.5), 0, math.max(1, math.floor(maxHp + 0.5))),
        MaxHP = math.max(1, math.floor(maxHp + 0.5)),
        Weapon = weapName,
        HasArmor = hasArmor,
        HasHelmet = hasHelmet,
        IsScoped = isScoped,
        IsFlashed = isFlashed,
        Speed = velMag,
        BreakingLC = (velMag > 28),
        IsTeammate = isTeammate,
        TeamName = cbTeam or (plr.Team and plr.Team.Name) or "Enemy",
    }
end

function Library:RegisterSearchable(holder, textLabel, name)
    local entry = {
        Holder = holder,
        Label = textLabel,
        Name = string.lower(tostring(name or "")),
        DefaultColor = textLabel and textLabel.TextColor3 or self.Theme.TextDim,
    }
    table.insert(self.SearchEntries, entry)
    return entry
end

function Library:ApplyGlobalSearch(query)
    local q = string.lower(string.gsub(tostring(query or ""), "^%s*(.-)%s*$", "%1"))
    self.SearchQuery = q
    local groupMatches = {}
    local tabMatches = {}

    for i = #self.SearchEntries, 1, -1 do
        local entry = self.SearchEntries[i]
        if entry.Holder and entry.Holder.Parent then
            local matched = (q == "") or (string.find(entry.Name, q, 1, true) ~= nil)
            entry.Holder.Visible = matched
            if matched and q ~= "" then
                local curr = entry.Holder.Parent
                while curr and curr ~= ScreenGui do
                    if string.find(curr.Name, "^Group_") or string.find(curr.Name, "^Tabbox_") then
                        groupMatches[curr] = (groupMatches[curr] or 0) + 1
                    elseif string.find(curr.Name, "^Page_") then
                        tabMatches[curr] = (tabMatches[curr] or 0) + 1
                    end
                    curr = curr.Parent
                end
            end
        else
            table.remove(self.SearchEntries, i)
        end
    end

    if self.ActiveWindow and self.ActiveWindow.Tabs then
        for _, tab in ipairs(self.ActiveWindow.Tabs) do
            local count = tabMatches[tab.Page] or 0
            if q == "" then
                tab.Label.Text = tab.Name
            else
                tab.Label.Text = count > 0 and string.format("%s (%d)", tab.Name, count) or tab.Name
            end
            for _, col in ipairs({ tab.Left, tab.Right }) do
                if col then
                    for _, grp in ipairs(col:GetChildren()) do
                        if grp:IsA("Frame") and (string.find(grp.Name, "^Group_") or string.find(grp.Name, "^Tabbox_")) then
                            grp.Visible = (q == "") or ((groupMatches[grp] or 0) > 0)
                            local cont = grp:FindFirstChild("Container", true)
                            if cont then
                                for _, item in ipairs(cont:GetChildren()) do
                                    if item:IsA("Frame") and item.Name == "Divider" then
                                        item.Visible = (q == "")
                                    end
                                end
                            end
                        end
                    end
                end
            end
        end
    end
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

function Library:MakeDraggable(dragHandle, targetFrame, hudId, onMoveCallback)
    local resolvedId = hudId or (targetFrame and targetFrame.Name) or ""
    if resolvedId ~= "" then
        self.HUDFrames[resolvedId] = targetFrame
    end
    local dragging = false
    local dragStart, startPos

    self:Connect(dragHandle.InputBegan, function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            if resolvedId ~= "MainWindow" and self.Flags["UI_LockHUD"] == true then
                return
            end
            dragging = true
            dragStart = input.Position
            startPos = targetFrame.Position
            self:ClosePopups()
        end
    end)

    self:Connect(UserInputService.InputChanged, function(input)
        if dragging and input.UserInputType == Enum.UserInputType.MouseMovement then
            local delta = input.Position - dragStart
            local nextPos = UDim2.new(
                startPos.X.Scale,
                startPos.X.Offset + delta.X,
                startPos.Y.Scale,
                startPos.Y.Offset + delta.Y
            )
            if onMoveCallback then
                onMoveCallback(nextPos)
            end
            Library:Tween(targetFrame, TweenInfo.new(0.06, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {
                Position = nextPos
            })
        end
    end)

    self:Connect(UserInputService.InputEnded, function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            dragging = false
        end
    end)
end

function Library:RefreshAttachmentTray(tray)
    if not tray or tray.Name ~= "AttachTray" then return end
    local totalW = 0
    local visibleCount = 0
    for _, ch in ipairs(tray:GetChildren()) do
        if ch:IsA("GuiObject") and ch.Visible then
            visibleCount += 1
            local targetW = ch:GetAttribute("TargetWidth")
            totalW += (type(targetW) == "number" and targetW or ch.Size.X.Offset)
        end
    end
    totalW += math.max(0, visibleCount - 1) * 5
    local trayW = visibleCount > 0 and math.max(28, totalW + 4) or 0
    tray.Size = UDim2.fromOffset(trayW, 16)
    local row = tray.Parent
    if row then
        local lbl = row:FindFirstChildOfClass("TextLabel")
        local clickBtn = row:FindFirstChildOfClass("TextButton")
        local rightPad = visibleCount > 0 and (trayW + 8) or 4
        if lbl then
            local leftX = lbl.Position.X.Offset
            lbl.Size = UDim2.new(1, -(leftX + rightPad), 1, 0)
        end
        if clickBtn and clickBtn.Name ~= "ColorSwatch" and clickBtn.Name ~= "KeybindBtn" then
            clickBtn.Size = UDim2.new(1, -rightPad, 1, 0)
        end
    end
end

function Library:SyncOptionValue(flag, value)
    local opt = self.Options[flag]
    if opt and opt.Value ~= value and opt.Set then
        opt:Set(value)
    end
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

Library:Connect(UserInputService.InputBegan, function(input)
    if input.UserInputType ~= Enum.UserInputType.MouseButton1 and input.UserInputType ~= Enum.UserInputType.MouseButton2 then
        return
    end
    local mPos = UserInputService:GetMouseLocation()
    local insetY = Library:GetGuiInsetY()
    local pt = Vector2.new(mPos.X, mPos.Y - insetY)
    for _, p in ipairs(Library.OpenPopups) do
        if p.IsOpen and p.IsOpen() then
            local inside = false
            if p.Frames then
                for _, f in ipairs(p.Frames) do
                    if f and f.Visible then
                        local ap, as = f.AbsolutePosition, f.AbsoluteSize
                        if pt.X >= ap.X and pt.X <= (ap.X + as.X) and pt.Y >= ap.Y and pt.Y <= (ap.Y + as.Y) then
                            inside = true
                            break
                        end
                    end
                end
            end
            if not inside and p.Close then
                p.Close()
            end
        end
    end
end)
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
    local function ClampTooltipPos(mx, my, w, h)
        local vp = workspace.CurrentCamera and workspace.CurrentCamera.ViewportSize or Vector2.new(1920, 1080)
        local px = math.clamp(mx + 14, 6, math.max(6, vp.X - w - 6))
        local py = math.clamp(my + 14, 6, math.max(6, vp.Y - h - 6))
        return UDim2.fromOffset(px, py)
    end
    self:Connect(target.MouseEnter, function()
        local bounds = self:GetTextBounds(text, 12, Vector2.new(500, 100))
        local targetW = bounds.X + 18
        TooltipLabel.Text = text
        local pos = UserInputService:GetMouseLocation()
        TooltipFrame.Position = ClampTooltipPos(pos.X, pos.Y, targetW, 22)
        TooltipFrame.Size = UDim2.fromOffset(math.max(18, math.floor(targetW * 0.85)), 22)
        TooltipFrame.Visible = true
        Library:Tween(TooltipFrame, TweenFast, {
            Size = UDim2.fromOffset(targetW, 22),
        })
    end)
    self:Connect(target.MouseLeave, function()
        TooltipFrame.Visible = false
    end)
end

Library:Connect(UserInputService.InputChanged, function(input)
    if TooltipFrame.Visible and input.UserInputType == Enum.UserInputType.MouseMovement then
        local pos = UserInputService:GetMouseLocation()
        local vp = workspace.CurrentCamera and workspace.CurrentCamera.ViewportSize or Vector2.new(1920, 1080)
        local w = TooltipFrame.AbsoluteSize.X
        local px = math.clamp(pos.X + 14, 6, math.max(6, vp.X - w - 6))
        local py = math.clamp(pos.Y + 14, 6, math.max(6, vp.Y - 28))
        Library:Tween(TooltipFrame, TweenInfo.new(0.05, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {
            Position = UDim2.fromOffset(px, py)
        })
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
-- TOP-LEFT CS:GO / GAMESENSE CONSOLE HITLOG OVERLAY
-- ============================================================================
local HitlogHolder = Library:Create("Frame", {
    Name = "HitlogOverlay",
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(14, 60),
    Size = UDim2.fromOffset(650, 260),
    ZIndex = 390,
    Parent = ScreenGui,
})
Library:Create("UIListLayout", {
    Padding = UDim.new(0, 4),
    SortOrder = Enum.SortOrder.LayoutOrder,
    Parent = HitlogHolder,
})

local function ToRichHex(col)
    return string.format("#%02X%02X%02X",
        math.clamp(math.floor(col.R * 255 + 0.5), 0, 255),
        math.clamp(math.floor(col.G * 255 + 0.5), 0, 255),
        math.clamp(math.floor(col.B * 255 + 0.5), 0, 255)
    )
end

local hitlogCounter = 0
function Library:AddHitlog(cfg)
    cfg = type(cfg) == "string" and { Text = cfg } or (cfg or {})
    hitlogCounter += 1
    local duration = cfg.Duration or 5
    local prefix = cfg.Prefix or ("[" .. (self.Title or "shards.cc") .. "]")
    local accentCol = cfg.Color or (cfg.Miss and self.Theme.Risky or self.Theme.Accent)
    local accentHex = ToRichHex(accentCol)
    local textHex = ToRichHex(self.Theme.Text)
    local dimHex = ToRichHex(self.Theme.TextDim)
    local riskyHex = ToRichHex(self.Theme.Risky)

    local richStr
    if cfg.Target then
        local target = tostring(cfg.Target)
        local hitbox = string.lower(tostring(cfg.Hitbox or "head"))
        if cfg.Miss then
            local reason = tostring(cfg.Reason or "spread")
            local hc = cfg.Hitchance and (" | hc: " .. tostring(cfg.Hitchance) .. "%") or ""
            richStr = string.format(
                '<font color="%s">%s</font> <font color="%s">Missed</font> <font color="%s">%s</font><font color="%s">\'s</font> <font color="%s">%s</font> <font color="%s">due to</font> <font color="%s">%s</font><font color="%s">%s</font>',
                riskyHex, prefix,
                textHex,
                accentHex, target,
                dimHex,
                textHex, hitbox,
                dimHex,
                riskyHex, reason,
                dimHex, hc
            )
        else
            local dmg = tonumber(cfg.Damage) or 100
            local rem = math.max(0, tonumber(cfg.Remaining) or math.max(0, 100 - dmg))
            local bt = cfg.Backtrack and (" | bt: " .. tostring(cfg.Backtrack) .. "t") or ""
            local hc = cfg.Hitchance and (" | hc: " .. tostring(cfg.Hitchance) .. "%") or ""
            richStr = string.format(
                '<font color="%s">%s</font> <font color="%s">Hurt</font> <font color="%s">%s</font> <font color="%s">in the</font> <font color="%s">%s</font> <font color="%s">for</font> <font color="%s">%d</font> <font color="%s">damage (</font><font color="%s">%d</font> <font color="%s">hp remaining%s%s)</font>',
                accentHex, prefix,
                textHex,
                accentHex, target,
                dimHex,
                accentHex, hitbox,
                dimHex,
                accentHex, dmg,
                dimHex,
                rem == 0 and riskyHex or textHex, rem,
                dimHex, hc, bt
            )
        end
    else
        richStr = string.format(
            '<font color="%s">%s</font> <font color="%s">%s</font>',
            accentHex, prefix,
            textHex, tostring(cfg.Text or cfg.Description or "event logged")
        )
    end

    local plainStr = string.gsub(richStr, "<[^>]+>", "")
    local bounds = self:GetTextBounds(plainStr, 12, Vector2.new(900, 30))
    local pillW = math.max(90, bounds.X + 22)

    local row = self:Create("Frame", {
        BackgroundTransparency = 1,
        LayoutOrder = hitlogCounter,
        Size = UDim2.fromOffset(pillW, 20),
        ZIndex = 391,
        Parent = HitlogHolder,
    })

    local bg = self:Create("Frame", {
        BackgroundColor3 = self.Theme.Background,
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(-12, 0),
        Size = UDim2.fromScale(1, 1),
        ZIndex = 391,
        Parent = row,
    })
    self:BindTheme(bg, "BackgroundColor3", "Background")
    local bgStroke, bgOuterStroke = self:ApplyCSBorder(bg, "InnerBorder")
    bgStroke.Transparency = 1
    bgOuterStroke.Transparency = 1

    local bar = self:Create("Frame", {
        BackgroundColor3 = accentCol,
        BorderSizePixel = 0,
        Position = UDim2.fromOffset(0, 0),
        Size = UDim2.new(0, 2, 1, 0),
        BackgroundTransparency = 1,
        ZIndex = 392,
        Parent = bg,
    })

    local line = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(8, 0),
        Size = UDim2.new(1, -12, 1, 0),
        Font = Enum.Font.Code,
        RichText = true,
        Text = richStr,
        TextColor3 = self.Theme.Text,
        TextSize = 12,
        TextTransparency = 1,
        TextStrokeColor3 = Color3.new(0, 0, 0),
        TextStrokeTransparency = 1,
        TextXAlignment = Enum.TextXAlignment.Left,
        TextYAlignment = Enum.TextYAlignment.Center,
        ZIndex = 392,
        Parent = bg,
    })

    self:Tween(bg, TweenFast, {
        Position = UDim2.fromOffset(0, 0),
        BackgroundTransparency = 0.12,
    })
    self:Tween(bgStroke, TweenFast, { Transparency = 0 })
    self:Tween(bgOuterStroke, TweenFast, { Transparency = 0 })
    self:Tween(bar, TweenFast, {
        BackgroundTransparency = 0,
    })
    self:Tween(line, TweenFast, {
        TextTransparency = 0,
        TextStrokeTransparency = 0.15,
    })

    if not cfg.Miss then
        local dmgVal = tonumber(cfg.Damage) or 100
        local isHead = string.lower(tostring(cfg.Hitbox or "")) == "head"
        if self.FlashHitmarker then
            pcall(function() self:FlashHitmarker(dmgVal, isHead) end)
        end
        if self.WorldController and self.WorldController.SpawnTracer then
            pcall(function() self.WorldController.SpawnTracer(cfg.Target) end)
        end
    end

    -- Cap active lines to 10 so rapid fire never overflows the screen
    local children = {}
    for _, ch in ipairs(HitlogHolder:GetChildren()) do
        if ch:IsA("Frame") then
            table.insert(children, ch)
        end
    end
    if #children > 10 then
        table.sort(children, function(a, b) return a.LayoutOrder < b.LayoutOrder end)
        for i = 1, #children - 10 do
            children[i]:Destroy()
        end
    end

    task.delay(duration, function()
        if not row or not row.Parent then return end
        self:Tween(bg, TweenMedium, {
            Position = UDim2.fromOffset(-10, 0),
            BackgroundTransparency = 1,
        })
        self:Tween(bgStroke, TweenMedium, { Transparency = 1 })
        self:Tween(bgOuterStroke, TweenMedium, { Transparency = 1 })
        self:Tween(bar, TweenMedium, {
            BackgroundTransparency = 1,
        })
        self:Tween(line, TweenMedium, {
            TextTransparency = 1,
            TextStrokeTransparency = 1,
        })
        task.wait(0.24)
        if row and row.Parent then row:Destroy() end
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
        Visible = false,
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

    local function UpdateWatermarkText()
        local ping = 0
        pcall(function()
            ping = math.floor(game:GetService("Stats").Network.ServerStatsItem["Data Ping"]:GetValue())
        end)
        local customStr = self.Flags["UI_WatermarkText"]
        local titlePart = (type(customStr) == "string" and customStr ~= "") and customStr or baseName
        local fields = self.Flags["UI_WatermarkFields"]
        local parts = { titlePart }
        if type(fields) == "table" then
            if fields["User"] ~= false then
                table.insert(parts, LocalPlayer and LocalPlayer.Name or "user")
            end
            if fields["FPS"] ~= false then
                table.insert(parts, string.format("%d fps", fps))
            end
            if fields["Ping"] ~= false then
                table.insert(parts, string.format("%d ms", ping))
            end
            if fields["Time"] ~= false then
                table.insert(parts, os.date("%H:%M:%S"))
            end
            if fields["Build"] == true then
                table.insert(parts, "v" .. tostring(self.Version))
            end
        else
            table.insert(parts, LocalPlayer and LocalPlayer.Name or "user")
            table.insert(parts, string.format("%d fps", fps))
            table.insert(parts, string.format("%d ms", ping))
            table.insert(parts, os.date("%H:%M:%S"))
        end
        local str = table.concat(parts, " | ")
        wmText.Text = str
        local bgAlpha = tonumber(self.Flags["UI_WatermarkAlpha"]) or 0
        wmFrame.BackgroundTransparency = math.clamp(bgAlpha, 0, 0.85)
        local bounds = self:GetTextBounds(str, 12, Vector2.new(800, 24))
        Library:Tween(wmFrame, TweenFast, {
            Size = UDim2.fromOffset(bounds.X + 38, 24)
        })
    end

    self:Connect(RunService.RenderStepped, function(dt)
        frames += 1
        accum += dt
        if accum >= 0.5 then
            fps = math.floor(frames / accum)
            frames = 0
            accum = 0
            if wmFrame.Visible then
                UpdateWatermarkText()
            end
        end
    end)

    local wmObj = { Frame = wmFrame, Refresh = UpdateWatermarkText }
    function wmObj:SetVisible(v)
        local state = (v == true)
        wmFrame.Visible = state
        if state then
            UpdateWatermarkText()
        end
        Library:SyncOptionValue("UI_ShowWatermark", state)
    end
    function wmObj:SetTitle(newTitle)
        baseName = tostring(newTitle or Library.Title)
        UpdateWatermarkText()
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
        Visible = false,
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
        Position = UDim2.fromOffset(8, 28),
        Size = UDim2.new(1, -16, 1, -32),
        ZIndex = 282,
        Parent = kbFrame,
    })
    local layout = self:Create("UIListLayout", {
        Padding = UDim.new(0, 3),
        SortOrder = Enum.SortOrder.LayoutOrder,
        Parent = listHolder,
    })

    local kbObj = { Frame = kbFrame, Rows = {} }
    function kbObj:Refresh()
        local activeFlags = {}
        local count = 0
        local maxW = 195

        local showAll = (Library.Flags["UI_KeybindListMode"] == "Show All Binds")
        for idx, flag in ipairs(Library.KeybindOrder) do
            local item = Library.ActiveKeybinds[flag]
            if item and (item.Active or showAll) and item.Key and item.Key ~= Enum.KeyCode.Unknown then
                activeFlags[flag] = true
                count += 1
                local modeStr = "[" .. string.lower(item.Mode or "toggle") .. "]"
                local keyStr = "[" .. Library:FormatKey(item.Key) .. "]"
                local rightStr = keyStr .. " " .. modeStr
                local nameW = Library:GetTextBounds(item.Name, 12, Vector2.new(300, 20)).X
                local rightW = Library:GetTextBounds(rightStr, 11, Vector2.new(200, 20)).X
                maxW = math.max(maxW, nameW + rightW + 34)

                local entry = kbObj.Rows[flag]
                if not entry or not entry.Row or not entry.Row.Parent then
                    local row = Library:Create("Frame", {
                        BackgroundTransparency = 1,
                        LayoutOrder = item.Order or idx,
                        Size = UDim2.new(1, 0, 0, 16),
                        ZIndex = 283,
                        Parent = listHolder,
                    })
                    local dot = Library:Create("Frame", {
                        AnchorPoint = Vector2.new(0, 0.5),
                        BackgroundColor3 = Library.Theme.Accent,
                        BorderSizePixel = 0,
                        Position = UDim2.new(0, 0, 0.5, 0),
                        Size = UDim2.fromOffset(3, 3),
                        ZIndex = 284,
                        Parent = row,
                    })
                    Library:BindTheme(dot, "BackgroundColor3", "Accent")
                    local nameLbl = Library:Create("TextLabel", {
                        BackgroundTransparency = 1,
                        Position = UDim2.fromOffset(2, 0),
                        Size = UDim2.new(0.62, -8, 1, 0),
                        Font = Enum.Font.Code,
                        Text = item.Name,
                        TextColor3 = item.Active and Library.Theme.Text or Library.Theme.TextDim,
                        TextSize = 12,
                        TextTransparency = 1,
                        TextXAlignment = Enum.TextXAlignment.Left,
                        TextYAlignment = Enum.TextYAlignment.Center,
                        ZIndex = 283,
                        Parent = row,
                    })
                    local modeLbl = Library:Create("TextLabel", {
                        BackgroundTransparency = 1,
                        Position = UDim2.new(0.62, 6, 0, 0),
                        Size = UDim2.new(0.38, 0, 1, 0),
                        Font = Enum.Font.Code,
                        Text = rightStr,
                        TextColor3 = item.Active and Library.Theme.Accent or Library.Theme.TextMuted,
                        TextSize = 11,
                        TextTransparency = 1,
                        TextXAlignment = Enum.TextXAlignment.Right,
                        TextYAlignment = Enum.TextYAlignment.Center,
                        ZIndex = 283,
                        Parent = row,
                    })
                    Library:Tween(nameLbl, TweenFast, {
                        Position = UDim2.fromOffset(8, 0),
                        TextTransparency = 0,
                    })
                    Library:Tween(modeLbl, TweenFast, {
                        Position = UDim2.fromScale(0.62, 0),
                        TextTransparency = 0,
                    })
                    kbObj.Rows[flag] = {
                        Row = row,
                        NameLbl = nameLbl,
                        ModeLbl = modeLbl,
                    }
                else
                    entry.Row.LayoutOrder = item.Order or idx
                    entry.NameLbl.Text = item.Name
                    entry.NameLbl.TextColor3 = item.Active and Library.Theme.Text or Library.Theme.TextDim
                    entry.ModeLbl.Text = rightStr
                    entry.ModeLbl.TextColor3 = item.Active and Library.Theme.Accent or Library.Theme.TextMuted
                end
            end
        end

        for flag, entry in pairs(kbObj.Rows) do
            if not activeFlags[flag] then
                if entry.Row and entry.Row.Parent then
                    entry.Row:Destroy()
                end
                kbObj.Rows[flag] = nil
            end
        end

        listHolder.Size = UDim2.new(1, -16, 0, count * 19)
        local targetH = 30 + (count > 0 and (count * 19 + 4) or 0)
        Library:Tween(kbFrame, TweenFast, { Size = UDim2.fromOffset(maxW, targetH) })
    end

    function kbObj:SetVisible(v)
        local state = (v == true)
        kbFrame.Visible = state
        Library:SyncOptionValue("UI_ShowKeybinds", state)
    end

    self.KeybindList = kbObj
    return kbObj
end

function Library:CreateSpectatorList()
    local specFrame = self:Create("Frame", {
        Name = "SpectatorList",
        BackgroundColor3 = self.Theme.Background,
        Position = UDim2.new(0, 16, 0.62, 0),
        Size = UDim2.fromOffset(185, 28),
        Visible = false,
        ZIndex = 275,
        Parent = ScreenGui,
    })
    self:BindTheme(specFrame, "BackgroundColor3", "Background")
    self:ApplyCSBorder(specFrame, "InnerBorder")
    self:CreateAccentBar(specFrame, 277)
    self:MakeDraggable(specFrame, specFrame)

    local hdrIcon = self:Create("ImageLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(8, 7),
        Size = UDim2.fromOffset(13, 13),
        Image = self:GetIcon("eye"),
        ImageColor3 = self.Theme.Accent,
        ZIndex = 277,
        Parent = specFrame,
    })
    self:BindTheme(hdrIcon, "ImageColor3", "Accent")

    local hdrTitle = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(26, 2),
        Size = UDim2.new(1, -30, 0, 22),
        Font = Enum.Font.GothamBold,
        Text = "SPECTATORS (0)",
        TextColor3 = self.Theme.Text,
        TextSize = 11,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 277,
        Parent = specFrame,
    })
    self:BindTheme(hdrTitle, "TextColor3", "Text")

    local sep = self:Create("Frame", {
        BackgroundColor3 = self.Theme.InnerBorder,
        BorderSizePixel = 0,
        Position = UDim2.fromOffset(0, 25),
        Size = UDim2.new(1, 0, 0, 1),
        ZIndex = 277,
        Parent = specFrame,
    })
    self:BindTheme(sep, "BackgroundColor3", "InnerBorder")

    local listHolder = self:Create("Frame", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(8, 28),
        Size = UDim2.new(1, -16, 0, 0),
        ZIndex = 277,
        Parent = specFrame,
    })
    self:Create("UIListLayout", {
        Padding = UDim.new(0, 3),
        SortOrder = Enum.SortOrder.LayoutOrder,
        Parent = listHolder,
    })

    local specObj = {
        Frame = specFrame,
        Rows = {},
        CustomEntries = {},
        DemoMode = false,
    }

    local demoList = {
        { Name = "s1mple",   Mode = "1st" },
        { Name = "donk666",  Mode = "1st" },
        { Name = "m0NESY",   Mode = "free" },
    }

    function specObj:Refresh(list)
        local entries = list
        if not entries then
            entries = {}
            if specObj.DemoMode then
                for _, d in ipairs(demoList) do
                    table.insert(entries, d)
                end
            end
            for _, c in ipairs(specObj.CustomEntries) do
                table.insert(entries, c)
            end
            local cam = workspace.CurrentCamera
            local myChar = LocalPlayer and LocalPlayer.Character
            for _, plr in ipairs(Players:GetPlayers()) do
                if plr ~= LocalPlayer then
                    local isWatching = false
                    local watchMode = "cam"
                    pcall(function()
                        local tel = self:GetPlayerTelemetry(plr)
                        if tel and tel.Spectating then
                            isWatching = true
                            watchMode = tel.Team == "Spectator" and "spec" or "dead"
                        elseif cam and myChar and not plr.Character then
                            local dist = plr:DistanceFromCharacter(myChar:GetPivot().Position)
                            if dist > 0 and dist < 4 then
                                isWatching = true
                            end
                        end
                    end)
                    if isWatching then
                        table.insert(entries, { Name = plr.Name, Mode = watchMode })
                    end
                end
            end
        end

        hdrTitle.Text = string.format("SPECTATORS (%d)", #entries)
        local seen = {}
        local maxW = 185

        for idx, item in ipairs(entries) do
            local sName = type(item) == "table" and tostring(item.Name or "Player") or tostring(item)
            local sMode = type(item) == "table" and ("[" .. string.lower(tostring(item.Mode or "1st")) .. "]") or "[1st]"
            local key = sName .. "_" .. tostring(idx)
            seen[key] = true

            local nw = Library:GetTextBounds(sName, 12, Vector2.new(250, 20)).X
            local mw = Library:GetTextBounds(sMode, 11, Vector2.new(120, 20)).X
            maxW = math.max(maxW, nw + mw + 36)

            local existing = specObj.Rows[key]
            if not existing or not existing.Row or not existing.Row.Parent then
                local row = Library:Create("Frame", {
                    BackgroundTransparency = 1,
                    LayoutOrder = idx,
                    Size = UDim2.new(1, 0, 0, 16),
                    ZIndex = 278,
                    Parent = listHolder,
                })
                local dot = Library:Create("Frame", {
                    AnchorPoint = Vector2.new(0, 0.5),
                    BackgroundColor3 = Library.Theme.Accent,
                    BorderSizePixel = 0,
                    Position = UDim2.new(0, 0, 0.5, 0),
                    Size = UDim2.fromOffset(3, 3),
                    ZIndex = 279,
                    Parent = row,
                })
                Library:BindTheme(dot, "BackgroundColor3", "Accent")
                local nLbl = Library:Create("TextLabel", {
                    BackgroundTransparency = 1,
                    Position = UDim2.fromOffset(8, 0),
                    Size = UDim2.new(0.68, -8, 1, 0),
                    Font = Enum.Font.Code,
                    Text = sName,
                    TextColor3 = Library.Theme.Text,
                    TextSize = 12,
                    TextXAlignment = Enum.TextXAlignment.Left,
                    ZIndex = 278,
                    Parent = row,
                })
                Library:BindTheme(nLbl, "TextColor3", "Text")
                local mLbl = Library:Create("TextLabel", {
                    BackgroundTransparency = 1,
                    Position = UDim2.fromScale(0.68, 0),
                    Size = UDim2.new(0.32, 0, 1, 0),
                    Font = Enum.Font.Code,
                    Text = sMode,
                    TextColor3 = Library.Theme.TextDim,
                    TextSize = 11,
                    TextXAlignment = Enum.TextXAlignment.Right,
                    ZIndex = 278,
                    Parent = row,
                })
                Library:BindTheme(mLbl, "TextColor3", "TextDim")
                specObj.Rows[key] = { Row = row, NameLbl = nLbl, ModeLbl = mLbl }
            else
                existing.Row.LayoutOrder = idx
                existing.NameLbl.Text = sName
                existing.ModeLbl.Text = sMode
            end
        end

        for k, entry in pairs(specObj.Rows) do
            if not seen[k] then
                if entry.Row and entry.Row.Parent then entry.Row:Destroy() end
                specObj.Rows[k] = nil
            end
        end

        local count = #entries
        listHolder.Size = UDim2.new(1, -16, 0, count * 19)
        local targetH = 30 + (count > 0 and (count * 19 + 4) or 0)
        Library:Tween(specFrame, TweenFast, { Size = UDim2.fromOffset(maxW, targetH) })
    end

    function specObj:SetDemo(state)
        specObj.DemoMode = (state == true)
        specObj:Refresh()
    end

    function specObj:SetCustom(entries)
        specObj.CustomEntries = type(entries) == "table" and entries or {}
        specObj:Refresh()
    end

    function specObj:SetVisible(v)
        local state = (v == true)
        specFrame.Visible = state
        if state then
            specObj:Refresh()
        end
        Library:SyncOptionValue("UI_ShowSpectators", state)
    end

    local specAccum = 0
    self:Connect(RunService.Heartbeat, function(dt)
        if not specFrame.Visible or self.Unloaded then return end
        specAccum += dt
        if specAccum >= 1.0 then
            specAccum = 0
            specObj:Refresh()
        end
    end)

    specObj:Refresh()
    self.SpectatorList = specObj
    return specObj
end

function Library:CreateTargetHUD()
    local hudFrame = self:Create("Frame", {
        Name = "TargetHUD",
        AnchorPoint = Vector2.new(0.5, 0),
        BackgroundColor3 = self.Theme.Background,
        Position = UDim2.new(0.5, 0, 0.72, 0),
        Size = UDim2.fromOffset(236, 66),
        Visible = false,
        ZIndex = 270,
        Parent = ScreenGui,
    })
    self:BindTheme(hudFrame, "BackgroundColor3", "Background")
    self:ApplyCSBorder(hudFrame, "HighlightBorder")
    self:CreateAccentBar(hudFrame, 272)
    self:MakeDraggable(hudFrame, hudFrame)

    -- Avatar / Silhouette Thumbnail Box
    local thumbBox = self:Create("Frame", {
        BackgroundColor3 = self.Theme.Element,
        Position = UDim2.fromOffset(8, 10),
        Size = UDim2.fromOffset(46, 46),
        ZIndex = 271,
        Parent = hudFrame,
    })
    self:BindTheme(thumbBox, "BackgroundColor3", "Element")
    self:ApplyCSBorder(thumbBox, "InnerBorder")

    local avatarImg = self:Create("ImageLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(1, 1),
        Size = UDim2.new(1, -2, 1, -2),
        Image = "rbxthumb://type=AvatarHeadShot&id=" .. tostring(LocalPlayer and LocalPlayer.UserId or 1) .. "&w=150&h=150",
        ZIndex = 272,
        Parent = thumbBox,
    })

    local targetIcon = self:Create("ImageLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(62, 10),
        Size = UDim2.fromOffset(12, 12),
        Image = self:GetIcon("locate-fixed"),
        ImageColor3 = self.Theme.Accent,
        ZIndex = 272,
        Parent = hudFrame,
    })
    self:BindTheme(targetIcon, "ImageColor3", "Accent")

    local nameLbl = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(78, 8),
        Size = UDim2.fromOffset(115, 15),
        Font = Enum.Font.GothamBold,
        Text = "s1mple [LOCKED]",
        TextColor3 = self.Theme.Text,
        TextSize = 11,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 272,
        Parent = hudFrame,
    })
    self:BindTheme(nameLbl, "TextColor3", "Text")

    local flagBadge = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.new(1, -48, 0, 8),
        Size = UDim2.fromOffset(40, 15),
        Font = Enum.Font.GothamBold,
        Text = "LC // DT",
        TextColor3 = self.Theme.Accent,
        TextSize = 9,
        TextXAlignment = Enum.TextXAlignment.Right,
        ZIndex = 272,
        Parent = hudFrame,
    })
    self:BindTheme(flagBadge, "TextColor3", "Accent")

    local subInfoLbl = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(62, 24),
        Size = UDim2.new(1, -70, 0, 14),
        Font = Enum.Font.Code,
        Text = "92 HP  |  42 FT  |  HC: 84%",
        TextColor3 = self.Theme.TextDim,
        TextSize = 11,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 272,
        Parent = hudFrame,
    })
    self:BindTheme(subInfoLbl, "TextColor3", "TextDim")

    -- Health Bar Background + Animated Damage Trail + Fill
    local hpBg = self:Create("Frame", {
        BackgroundColor3 = self.Theme.Element,
        Position = UDim2.fromOffset(62, 43),
        Size = UDim2.new(1, -70, 0, 11),
        ZIndex = 271,
        Parent = hudFrame,
    })
    self:BindTheme(hpBg, "BackgroundColor3", "Element")
    self:ApplyCSBorder(hpBg, "InnerBorder")

    local hpTrail = self:Create("Frame", {
        BackgroundColor3 = self.Theme.Risky,
        BorderSizePixel = 0,
        Size = UDim2.fromScale(0.92, 1),
        ZIndex = 272,
        Parent = hpBg,
    })

    local hpFill = self:Create("Frame", {
        BackgroundColor3 = Color3.new(1, 1, 1),
        BorderSizePixel = 0,
        Size = UDim2.fromScale(0.92, 1),
        ZIndex = 273,
        Parent = hpBg,
    })
    local hpGrad = self:Create("UIGradient", {
        Color = ColorSequence.new({
            ColorSequenceKeypoint.new(0, self.Theme.Accent),
            ColorSequenceKeypoint.new(1, self.Theme.AccentDark),
        }),
        Parent = hpFill,
    })
    table.insert(self.Options, {
        RefreshTheme = function()
            if hpGrad and hpGrad.Parent then
                hpGrad.Color = ColorSequence.new({
                    ColorSequenceKeypoint.new(0, self.Theme.Accent),
                    ColorSequenceKeypoint.new(1, self.Theme.AccentDark),
                })
            end
        end
    })

    local hpValLbl = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Size = UDim2.fromScale(1, 1),
        Font = Enum.Font.Code,
        Text = "92 / 100",
        TextColor3 = self.Theme.Text,
        TextSize = 10,
        ZIndex = 274,
        Parent = hpBg,
    })

    local hudObj = {
        Frame = hudFrame,
        CurrentHP = 92,
        MaxHP = 100,
    }

    function hudObj:SetTarget(info)
        info = info or {}
        local tName = tostring(info.Name or "s1mple")
        local hp = math.clamp(tonumber(info.HP or info.Health) or 92, 0, tonumber(info.MaxHP) or 100)
        local maxHp = math.max(1, tonumber(info.MaxHP) or 100)
        local dist = tonumber(info.Distance) or 42
        local hc = tonumber(info.Hitchance) or (tonumber(Library.Flags["Rage_Hitchance"]) or 84)
        local flags = tostring(info.Flags or (Library.Flags["Rage_DT"] and "LC // DT" or "RESOLVE"))
        local ratio = math.clamp(hp / maxHp, 0, 1)

        local userId = info.UserId
        if not userId then
            local foundPlr = Players:FindFirstChild(tName)
            if foundPlr then
                userId = foundPlr.UserId
            end
        end
        if userId then
            avatarImg.Image = "rbxthumb://type=AvatarHeadShot&id=" .. tostring(userId) .. "&w=150&h=150"
        end
        nameLbl.Text = tName .. " [LOCKED]"
        flagBadge.Text = flags
        subInfoLbl.Text = string.format("%d HP  |  %d FT  |  HC: %d%%", hp, dist, hc)
        hpValLbl.Text = string.format("%d / %d", hp, maxHp)
        Library:Tween(hpFill, TweenFast, { Size = UDim2.fromScale(ratio, 1) })
        task.delay(0.14, function()
            if hpTrail and hpTrail.Parent then
                Library:Tween(hpTrail, TweenMedium, { Size = UDim2.fromScale(ratio, 1) })
            end
        end)
        hudObj.CurrentHP = hp
        hudObj.MaxHP = maxHp
    end

    function hudObj:SetPlayer(playerName)
        local plr = playerName and Players:FindFirstChild(tostring(playerName))
        if plr then
            local char = plr.Character
            local hum = char and char:FindFirstChildOfClass("Humanoid")
            local hp = hum and math.floor(hum.Health + 0.5) or 100
            local maxHp = hum and math.floor(hum.MaxHealth + 0.5) or 100
            local dist = 0
            pcall(function()
                if LocalPlayer and LocalPlayer.Character and char then
                    dist = math.floor((LocalPlayer.Character:GetPivot().Position - char:GetPivot().Position).Magnitude)
                end
            end)
            hudObj:SetTarget({
                Name = plr.Name,
                UserId = plr.UserId,
                HP = hp,
                MaxHP = maxHp,
                Distance = dist,
            })
        else
            hudObj:SetTarget({
                Name = tostring(playerName or "s1mple"),
                HP = 92,
                MaxHP = 100,
                Distance = 42,
            })
        end
    end

    function hudObj:SetVisible(v)
        local state = (v == true)
        hudFrame.Visible = state
        Library:SyncOptionValue("UI_ShowTargetHUD", state)
        Library:SyncOptionValue("Misc_ShowTargetHUD", state)
    end

    local targetTrackAccum = 0
    self:Connect(RunService.RenderStepped, function(dt)
        if not hudFrame.Visible or self.Unloaded then return end
        targetTrackAccum += dt
        if targetTrackAccum < 0.12 then return end
        targetTrackAccum = 0
        local targetName = self.Flags["Misc_PlayerTarget"]
        local targetPlr = targetName and Players:FindFirstChild(tostring(targetName))
        if not targetPlr and (self.Flags["Rage_Enabled"] or self.Flags["Legit_Enable"]) then
            local cam = workspace.CurrentCamera
            local mPos = UserInputService:GetMouseLocation()
            local bestDist = math.huge
            for _, plr in ipairs(Players:GetPlayers()) do
                if plr ~= LocalPlayer and plr.Character then
                    local hum = plr.Character:FindFirstChildOfClass("Humanoid")
                    local hrp = plr.Character:FindFirstChild("HumanoidRootPart") or plr.Character.PrimaryPart
                    if hum and hrp and hum.Health > 0 and cam then
                        local sp, onScr = cam:WorldToViewportPoint(hrp.Position)
                        if onScr and sp.Z > 0 then
                            local d2 = (Vector2.new(sp.X, sp.Y) - mPos).Magnitude
                            if d2 < bestDist then
                                bestDist = d2
                                targetPlr = plr
                            end
                        end
                    end
                end
            end
        end
        if targetPlr and targetPlr.Character then
            local char = targetPlr.Character
            local hum = char:FindFirstChildOfClass("Humanoid")
            if hum then
                local dist = 0
                pcall(function()
                    if LocalPlayer and LocalPlayer.Character then
                        dist = math.floor((LocalPlayer.Character:GetPivot().Position - char:GetPivot().Position).Magnitude)
                    end
                end)
                local tool = char:FindFirstChildOfClass("Tool")
                local flagStr = tool and string.upper(string.sub(tool.Name, 1, 7)) or (self.Flags["Rage_DT"] and "LC // DT" or "LOCKED")
                hudObj:SetTarget({
                    Name = targetPlr.Name,
                    UserId = targetPlr.UserId,
                    HP = math.floor(hum.Health + 0.5),
                    MaxHP = math.floor(hum.MaxHealth + 0.5),
                    Distance = dist,
                    Flags = flagStr,
                })
            end
        end
    end)

    self.TargetHUD = hudObj
    return hudObj
end

-- ============================================================================
-- 1. SKEET / GAMESENSE CROSSHAIR & BOTTOM-LEFT INDICATORS
-- ============================================================================
function Library:CreateIndicators()
    local indFrame = self:Create("Frame", {
        Name = "Indicators",
        BackgroundTransparency = 1,
        Position = UDim2.new(0, 18, 0.56, 0),
        Size = UDim2.fromOffset(180, 130),
        Visible = false,
        ZIndex = 265,
        Parent = ScreenGui,
    })
    self:MakeDraggable(indFrame, indFrame, "Indicators")

    self:Create("UIListLayout", {
        Padding = UDim.new(0, 3),
        SortOrder = Enum.SortOrder.LayoutOrder,
        VerticalAlignment = Enum.VerticalAlignment.Bottom,
        Parent = indFrame,
    })

    local rows = {}
    local customItems = {}
    local indObj = {
        Frame = indFrame,
        Rows = rows,
        Custom = customItems,
    }

    local function GetOrMakeRow(id, order)
        local entry = rows[id]
        if entry and entry.Row and entry.Row.Parent then
            entry.Row.LayoutOrder = order
            return entry
        end

        local row = self:Create("Frame", {
            Name = "Ind_" .. tostring(id),
            BackgroundTransparency = 1,
            LayoutOrder = order,
            Size = UDim2.fromOffset(120, 22),
            ZIndex = 266,
            Parent = indFrame,
        })

        local bg = self:Create("Frame", {
            BackgroundColor3 = self.Theme.Background,
            BackgroundTransparency = 0.22,
            Size = UDim2.fromScale(1, 1),
            ZIndex = 266,
            Parent = row,
        })
        self:BindTheme(bg, "BackgroundColor3", "Background")
        self:ApplyCSBorder(bg, "InnerBorder")

        local leftBar = self:Create("Frame", {
            BackgroundColor3 = self.Theme.Accent,
            BorderSizePixel = 0,
            Size = UDim2.new(0, 2, 1, 0),
            ZIndex = 268,
            Parent = bg,
        })

        local grad = self:Create("UIGradient", {
            Transparency = NumberSequence.new({
                NumberSequenceKeypoint.new(0, 0.05),
                NumberSequenceKeypoint.new(1, 0.45),
            }),
            Parent = bg,
        })

        local lbl = self:Create("TextLabel", {
            BackgroundTransparency = 1,
            Position = UDim2.fromOffset(8, 0),
            Size = UDim2.new(1, -12, 1, 0),
            Font = Enum.Font.GothamBold,
            Text = tostring(id),
            TextColor3 = self.Theme.Accent,
            TextSize = 13,
            TextXAlignment = Enum.TextXAlignment.Left,
            ZIndex = 268,
            Parent = bg,
        })

        local barBg = self:Create("Frame", {
            BackgroundColor3 = self.Theme.Element,
            BorderSizePixel = 0,
            Position = UDim2.new(0, 8, 1, -4),
            Size = UDim2.new(1, -14, 0, 2),
            Visible = false,
            ZIndex = 268,
            Parent = bg,
        })

        local barFill = self:Create("Frame", {
            BackgroundColor3 = self.Theme.Accent,
            BorderSizePixel = 0,
            Size = UDim2.fromScale(1, 1),
            ZIndex = 269,
            Parent = barBg,
        })

        entry = {
            Row = row,
            Bg = bg,
            LeftBar = leftBar,
            Grad = grad,
            Label = lbl,
            BarBg = barBg,
            BarFill = barFill,
        }
        rows[id] = entry
        return entry
    end

    function indObj:SetIndicator(id, cfg)
        if not cfg or cfg.Active == false then
            customItems[id] = nil
        else
            customItems[id] = cfg
        end
        indObj:Refresh()
    end

    function indObj:Refresh()
        local activeList = {}

        if self.Flags or Library.Flags then
            local F = Library.Flags
            if F["Rage_DT"] then
                local shift = tonumber(F["Rage_ShiftTicks"]) or 14
                table.insert(activeList, {
                    Id = "DT",
                    Text = string.format("DT [%dt]", shift),
                    Color = Library.Theme.Accent,
                    Progress = math.clamp(shift / 18, 0, 1),
                })
            end
            if F["Rage_HideShots"] then
                table.insert(activeList, {
                    Id = "HS",
                    Text = "ONSHOT // HS",
                    Color = Library.Theme.Success,
                })
            end
            if F["Rage_ForceBaim"] then
                table.insert(activeList, {
                    Id = "BAIM",
                    Text = "FORCE BAIM",
                    Color = Library.Theme.Risky,
                })
            end
            if F["Rage_Autowall"] then
                local awDmg = tonumber(F["Rage_AutowallDmg"]) or tonumber(F["Rage_MinDmg"]) or 24
                table.insert(activeList, {
                    Id = "DMG",
                    Text = string.format("MIN DMG: %d", awDmg),
                    Color = Library.Theme.Text,
                })
            end
            if F["FL_Enable"] then
                local choke = tonumber(F["FL_Limit"]) or 14
                table.insert(activeList, {
                    Id = "FL",
                    Text = string.format("CHOKE: %dt", choke),
                    Color = Library.Theme.Warning,
                    Progress = math.clamp(choke / 16, 0, 1),
                })
            end
            if F["AA_Enabled"] then
                table.insert(activeList, {
                    Id = "FREESTAND",
                    Text = "AA // ACTIVE",
                    Color = Library.Theme.Accent,
                })
            end
        end

        for id, c in pairs(customItems) do
            table.insert(activeList, {
                Id = id,
                Text = c.Text or id,
                Color = c.Color or Library.Theme.Accent,
                Progress = c.Progress,
            })
        end

        if #activeList == 0 then
            table.insert(activeList, {
                Id = "SHARDS",
                Text = string.upper(Library.Title or "SHARDS.CC"),
                Color = Library.Theme.Accent,
            })
        end

        local seen = {}
        for idx, item in ipairs(activeList) do
            seen[item.Id] = true
            local entry = GetOrMakeRow(item.Id, idx)
            local col = item.Color or Library.Theme.Accent
            entry.Label.Text = item.Text
            entry.Label.TextColor3 = col
            entry.LeftBar.BackgroundColor3 = col
            local bounds = Library:GetTextBounds(item.Text, 13, Vector2.new(300, 24), true)
            local w = math.max(76, bounds.X + 20)
            local h = item.Progress ~= nil and 24 or 20
            entry.Row.Size = UDim2.fromOffset(w, h)
            if item.Progress ~= nil then
                entry.Label.Position = UDim2.fromOffset(8, -2)
                entry.BarBg.Visible = true
                entry.BarFill.BackgroundColor3 = col
                entry.BarFill.Size = UDim2.fromScale(math.clamp(item.Progress, 0, 1), 1)
            else
                entry.Label.Position = UDim2.fromOffset(8, 0)
                entry.BarBg.Visible = false
            end
        end

        for id, entry in pairs(rows) do
            if not seen[id] then
                if entry.Row and entry.Row.Parent then
                    entry.Row:Destroy()
                end
                rows[id] = nil
            end
        end
    end

    function indObj:SetVisible(v)
        local state = (v == true)
        indFrame.Visible = state
        if state then
            indObj:Refresh()
        end
        Library:SyncOptionValue("UI_ShowIndicators", state)
        Library:SyncOptionValue("Misc_ShowIndicators", state)
    end

    -- Custom CS:GO / Gamesense Crosshair & Animated Hitmarker Overlay
    local xhairRoot = self:Create("Frame", {
        Name = "CustomCrosshair",
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundTransparency = 1,
        Position = UDim2.fromScale(0.5, 0.5),
        Size = UDim2.fromOffset(100, 100),
        Visible = false,
        ZIndex = 290,
        Parent = ScreenGui,
    })

    local xhairDot = self:Create("Frame", {
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundColor3 = self.Theme.Accent,
        BorderSizePixel = 0,
        Position = UDim2.fromScale(0.5, 0.5),
        Size = UDim2.fromOffset(2, 2),
        ZIndex = 292,
        Parent = xhairRoot,
    })
    local xhairDotStroke = self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Parent = xhairDot })

    local xhairArms = {}
    for i = 1, 4 do
        local arm = self:Create("Frame", {
            AnchorPoint = Vector2.new(0.5, 0.5),
            BackgroundColor3 = self.Theme.Accent,
            BorderSizePixel = 0,
            Position = UDim2.fromScale(0.5, 0.5),
            Size = UDim2.fromOffset(8, 2),
            ZIndex = 291,
            Parent = xhairRoot,
        })
        local st = self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Parent = arm })
        xhairArms[i] = { Frame = arm, Stroke = st }
    end

    -- Hitmarker X Overlay
    local hitmarkerRoot = self:Create("Frame", {
        Name = "HitmarkerOverlay",
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundTransparency = 1,
        Position = UDim2.fromScale(0.5, 0.5),
        Size = UDim2.fromOffset(32, 32),
        Visible = false,
        ZIndex = 295,
        Parent = ScreenGui,
    })
    local hmLines = {}
    local hmAngles = { 45, -45, 45, -45 }
    local hmOffsets = {
        Vector2.new(-6, -6),
        Vector2.new(6, -6),
        Vector2.new(6, 6),
        Vector2.new(-6, 6),
    }
    for i = 1, 4 do
        local ln = self:Create("Frame", {
            AnchorPoint = Vector2.new(0.5, 0.5),
            BackgroundColor3 = Color3.new(1, 1, 1),
            BorderSizePixel = 0,
            Position = UDim2.new(0.5, hmOffsets[i].X, 0.5, hmOffsets[i].Y),
            Rotation = hmAngles[i],
            Size = UDim2.fromOffset(7, 1),
            ZIndex = 296,
            Parent = hitmarkerRoot,
        })
        self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Transparency = 0.25, Parent = ln })
        hmLines[i] = ln
    end
    local hmDmgLbl = self:Create("TextLabel", {
        AnchorPoint = Vector2.new(0.5, 1),
        BackgroundTransparency = 1,
        Position = UDim2.new(0.5, 0, 0, -4),
        Size = UDim2.fromOffset(80, 14),
        Font = Enum.Font.GothamBold,
        Text = "-100",
        TextColor3 = self.Theme.Risky,
        TextSize = 12,
        TextTransparency = 1,
        ZIndex = 297,
        Parent = hitmarkerRoot,
    })

    local HitsoundMap = {
        ["Skeet Bell"]     = "rbxassetid://4817809188",
        ["Neverlose"]      = "rbxassetid://6607204501",
        ["Rust Headshot"]  = "rbxassetid://4764109000",
        ["COD Hitmarker"]  = "rbxassetid://160432334",
        ["Bameware"]       = "rbxassetid://3124331820",
    }
    local hitSoundInst = Instance.new("Sound")
    hitSoundInst.Name = "ShardsHitsound"
    hitSoundInst.Volume = 1
    pcall(function() hitSoundInst.Parent = ScreenGui end)

    function Library:PlayHitsound()
        if not self.Flags["ESP_Hitsound"] or self.Unloaded then return end
        local choice = self.Flags["ESP_HitsoundType"] or "Skeet Bell"
        local asset = HitsoundMap[choice]
        if choice == "Custom Asset" then
            local raw = tostring(self.Flags["ESP_HitsoundCustom"] or "")
            if string.find(raw, "rbxassetid://") then
                asset = raw
            elseif tonumber(raw) then
                asset = "rbxassetid://" .. raw
            end
        end
        if asset and hitSoundInst then
            pcall(function()
                hitSoundInst.SoundId = asset
                hitSoundInst.Volume = math.clamp(tonumber(self.Flags["ESP_HitsoundVol"]) or 1, 0.1, 3)
                hitSoundInst.PlaybackSpeed = math.clamp(tonumber(self.Flags["ESP_HitsoundPitch"]) or 1, 0.5, 2)
                hitSoundInst:Play()
            end)
        end
    end

    local hmToken = 0
    function Library:FlashHitmarker(dmg, isHead)
        self:PlayHitsound()
        if self.Flags["ESP_Hitmarker"] == false then return end
        hmToken += 1
        local tok = hmToken
        local col = isHead and self.Theme.Risky or (self.Flags["ESP_CrosshairCol"] or self.Theme.Accent)
        local mPos = UserInputService:GetMouseLocation()
        local followMouse = (self.Flags["ESP_CrosshairFollow"] == true)
        hitmarkerRoot.Position = followMouse and UDim2.fromOffset(mPos.X, mPos.Y) or UDim2.fromScale(0.5, 0.5)
        hitmarkerRoot.Visible = true
        for idx, ln in ipairs(hmLines) do
            ln.BackgroundColor3 = col
            ln.BackgroundTransparency = 0
            ln.Position = UDim2.new(0.5, hmOffsets[idx].X * 1.35, 0.5, hmOffsets[idx].Y * 1.35)
            self:Tween(ln, TweenFast, {
                Position = UDim2.new(0.5, hmOffsets[idx].X, 0.5, hmOffsets[idx].Y),
            })
        end
        hmDmgLbl.Text = "-" .. tostring(dmg or 100) .. (isHead and " HS" or "")
        hmDmgLbl.TextColor3 = col
        hmDmgLbl.Position = UDim2.new(0.5, 0, 0, -2)
        hmDmgLbl.TextTransparency = 0
        self:Tween(hmDmgLbl, TweenMedium, {
            Position = UDim2.new(0.5, 0, 0, -16),
            TextTransparency = 1,
        })
        task.delay(0.28, function()
            if hmToken ~= tok then return end
            for _, ln in ipairs(hmLines) do
                self:Tween(ln, TweenFast, { BackgroundTransparency = 1 })
            end
            task.wait(0.15)
            if hmToken == tok then
                hitmarkerRoot.Visible = false
            end
        end)
    end

    local xhairSpinAngle = 0
    self:Connect(RunService.RenderStepped, function(dt)
        if self.Unloaded then return end
        local F = self.Flags
        local showXhair = (F["ESP_Crosshair"] == true)
        xhairRoot.Visible = showXhair
        if showXhair then
            local col = F["ESP_CrosshairCol"] or self.Theme.Accent
            local length = math.clamp(tonumber(F["ESP_CrosshairLen"]) or 8, 2, 30)
            local gap = math.clamp(tonumber(F["ESP_CrosshairGap"]) or 5, 0, 25)
            local thick = math.clamp(tonumber(F["ESP_CrosshairThick"]) or 2, 1, 6)
            local tStyle = (F["ESP_CrosshairT"] == true)
            local showDot = (F["ESP_CrosshairDot"] ~= false)
            local spin = (F["ESP_CrosshairSpin"] == true)
            local followMouse = (F["ESP_CrosshairFollow"] == true)

            if followMouse then
                local mPos = UserInputService:GetMouseLocation()
                xhairRoot.Position = UDim2.fromOffset(mPos.X, mPos.Y)
            else
                xhairRoot.Position = UDim2.fromScale(0.5, 0.5)
            end

            if spin then
                xhairSpinAngle = (xhairSpinAngle + dt * 140) % 360
                xhairRoot.Rotation = xhairSpinAngle
            else
                xhairRoot.Rotation = 0
            end

            xhairDot.Visible = showDot
            xhairDot.BackgroundColor3 = col
            xhairDot.Size = UDim2.fromOffset(thick, thick)
            xhairDotStroke.Transparency = 0.15

            local centerDist = gap + length * 0.5
            -- 1: Left, 2: Right, 3: Top, 4: Bottom
            xhairArms[1].Frame.Position = UDim2.new(0.5, -centerDist, 0.5, 0)
            xhairArms[1].Frame.Size = UDim2.fromOffset(length, thick)
            xhairArms[2].Frame.Position = UDim2.new(0.5, centerDist, 0.5, 0)
            xhairArms[2].Frame.Size = UDim2.fromOffset(length, thick)
            xhairArms[3].Frame.Position = UDim2.new(0.5, 0, 0.5, -centerDist)
            xhairArms[3].Frame.Size = UDim2.fromOffset(thick, length)
            xhairArms[3].Frame.Visible = not tStyle
            xhairArms[4].Frame.Position = UDim2.new(0.5, 0, 0.5, centerDist)
            xhairArms[4].Frame.Size = UDim2.fromOffset(thick, length)

            for i = 1, 4 do
                xhairArms[i].Frame.BackgroundColor3 = col
            end
        end
    end)

    indObj:Refresh()
    self.Indicators = indObj
    return indObj
end

-- ============================================================================
-- 2. DRAGGABLE 2D RADAR WINDOW (Camera-Relative Blips, FOV Cone, Cardinal Ticks)
-- ============================================================================
function Library:CreateRadar(cfg)
    cfg = cfg or {}
    local radarSize = cfg.Size or 170
    local radarRange = cfg.Range or 180 -- studs from center to edge

    local radarFrame = self:Create("Frame", {
        Name = "RadarWindow",
        BackgroundColor3 = self.Theme.Background,
        Position = UDim2.new(0, 16, 0.14, 0),
        Size = UDim2.fromOffset(radarSize, radarSize + 24),
        Visible = false,
        ZIndex = 260,
        Parent = ScreenGui,
    })
    self:BindTheme(radarFrame, "BackgroundColor3", "Background")
    self:ApplyCSBorder(radarFrame, "InnerBorder")
    self:CreateAccentBar(radarFrame, 263)
    self:MakeDraggable(radarFrame, radarFrame, "RadarWindow")

    local hdrIcon = self:Create("ImageLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(8, 6),
        Size = UDim2.fromOffset(12, 12),
        Image = self:GetIcon("radar"),
        ImageColor3 = self.Theme.Accent,
        ZIndex = 262,
        Parent = radarFrame,
    })
    self:BindTheme(hdrIcon, "ImageColor3", "Accent")

    local hdrTitle = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(25, 2),
        Size = UDim2.new(1, -30, 0, 20),
        Font = Enum.Font.GothamBold,
        Text = "2D RADAR",
        TextColor3 = self.Theme.Text,
        TextSize = 11,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 262,
        Parent = radarFrame,
    })
    self:BindTheme(hdrTitle, "TextColor3", "Text")

    local canvas = self:Create("Frame", {
        BackgroundColor3 = Color3.fromRGB(13, 13, 17),
        ClipsDescendants = true,
        Position = UDim2.fromOffset(6, 24),
        Size = UDim2.new(1, -12, 1, -30),
        ZIndex = 261,
        Parent = radarFrame,
    })
    self:ApplyCSBorder(canvas, "InnerBorder")

    -- Subtle Tactical Crosslines + Range Rings
    self:Create("Frame", {
        AnchorPoint = Vector2.new(0.5, 0),
        BackgroundColor3 = Color3.fromRGB(28, 28, 36),
        BorderSizePixel = 0,
        Position = UDim2.fromScale(0.5, 0),
        Size = UDim2.new(0, 1, 1, 0),
        ZIndex = 261,
        Parent = canvas,
    })
    self:Create("Frame", {
        AnchorPoint = Vector2.new(0, 0.5),
        BackgroundColor3 = Color3.fromRGB(28, 28, 36),
        BorderSizePixel = 0,
        Position = UDim2.fromScale(0, 0.5),
        Size = UDim2.new(1, 0, 0, 1),
        ZIndex = 261,
        Parent = canvas,
    })

    for _, rScale in ipairs({ 0.45, 0.82 }) do
        local ring = self:Create("Frame", {
            AnchorPoint = Vector2.new(0.5, 0.5),
            BackgroundTransparency = 1,
            Position = UDim2.fromScale(0.5, 0.5),
            Size = UDim2.fromScale(rScale, rScale),
            ZIndex = 261,
            Parent = canvas,
        })
        self:Create("UICorner", { CornerRadius = UDim.new(1, 0), Parent = ring })
        self:Create("UIStroke", { Color = Color3.fromRGB(26, 26, 34), Thickness = 1, Parent = ring })
    end

    -- FOV Cone Wedge Lines (Camera Look Direction Cone)
    local fovLeftArm = self:Create("Frame", {
        AnchorPoint = Vector2.new(0.5, 1),
        BackgroundColor3 = self.Theme.Accent,
        BackgroundTransparency = 0.55,
        BorderSizePixel = 0,
        Position = UDim2.fromScale(0.5, 0.5),
        Rotation = -38,
        Size = UDim2.new(0, 1, 0.48, 0),
        ZIndex = 262,
        Parent = canvas,
    })
    self:BindTheme(fovLeftArm, "BackgroundColor3", "Accent")

    local fovRightArm = self:Create("Frame", {
        AnchorPoint = Vector2.new(0.5, 1),
        BackgroundColor3 = self.Theme.Accent,
        BackgroundTransparency = 0.55,
        BorderSizePixel = 0,
        Position = UDim2.fromScale(0.5, 0.5),
        Rotation = 38,
        Size = UDim2.new(0, 1, 0.48, 0),
        ZIndex = 262,
        Parent = canvas,
    })
    self:BindTheme(fovRightArm, "BackgroundColor3", "Accent")

    -- Rotating Cardinal Labels (N, E, S, W)
    local cardinals = {}
    local cardinalDefs = {
        { Text = "N", Dir = Vector3.new(0, 0, -1), Accent = true },
        { Text = "E", Dir = Vector3.new(1, 0, 0),  Accent = false },
        { Text = "S", Dir = Vector3.new(0, 0, 1),  Accent = false },
        { Text = "W", Dir = Vector3.new(-1, 0, 0), Accent = false },
    }
    for _, cDef in ipairs(cardinalDefs) do
        local cLbl = self:Create("TextLabel", {
            AnchorPoint = Vector2.new(0.5, 0.5),
            BackgroundTransparency = 1,
            Position = UDim2.new(0.5, 0, 0, 8),
            Size = UDim2.fromOffset(14, 12),
            Font = Enum.Font.GothamBold,
            Text = cDef.Text,
            TextColor3 = cDef.Accent and self.Theme.Accent or self.Theme.TextMuted,
            TextSize = 9,
            ZIndex = 263,
            Parent = canvas,
        })
        if cDef.Accent then
            self:BindTheme(cLbl, "TextColor3", "Accent")
        else
            self:BindTheme(cLbl, "TextColor3", "TextMuted")
        end
        table.insert(cardinals, { Label = cLbl, Dir = cDef.Dir })
    end

    -- Center LocalPlayer Arrow / Dot
    local selfDot = self:Create("Frame", {
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundColor3 = Color3.fromRGB(240, 240, 245),
        BorderSizePixel = 0,
        Position = UDim2.fromScale(0.5, 0.5),
        Size = UDim2.fromOffset(5, 5),
        ZIndex = 264,
        Parent = canvas,
    })
    self:Create("UICorner", { CornerRadius = UDim.new(1, 0), Parent = selfDot })
    self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Parent = selfDot })

    local blips = {}
    local demoBlips = {
        { Name = "s1mple",  Offset = Vector2.new(-38, -52), Enemy = true },
        { Name = "donk666", Offset = Vector2.new(46, -28),  Enemy = true },
        { Name = "m0NESY",  Offset = Vector2.new(-22, 44),  Enemy = false },
    }

    local function GetOrMakeBlip(key)
        local b = blips[key]
        if b and b.Dot and b.Dot.Parent then return b end

        local dot = self:Create("Frame", {
            AnchorPoint = Vector2.new(0.5, 0.5),
            BackgroundColor3 = self.Theme.Risky,
            BorderSizePixel = 0,
            Position = UDim2.fromScale(0.5, 0.5),
            Size = UDim2.fromOffset(5, 5),
            ZIndex = 263,
            Parent = canvas,
        })
        self:Create("UICorner", { CornerRadius = UDim.new(1, 0), Parent = dot })
        self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Parent = dot })

        local tag = self:Create("TextLabel", {
            AnchorPoint = Vector2.new(0.5, 1),
            BackgroundTransparency = 1,
            Position = UDim2.new(0.5, 0, 0, -1),
            Size = UDim2.fromOffset(70, 10),
            Font = Enum.Font.Code,
            Text = tostring(key),
            TextColor3 = self.Theme.Text,
            TextSize = 9,
            ZIndex = 263,
            Parent = dot,
        })

        b = { Dot = dot, Tag = tag }
        blips[key] = b
        return b
    end

    self:Connect(RunService.RenderStepped, function()
        if not radarFrame.Visible or self.Unloaded then return end
        local cam = workspace.CurrentCamera
        local northLocked = (self.Flags["Misc_RadarMode"] == "North-Locked")
        local blipScale = math.clamp(tonumber(self.Flags["Misc_RadarBlipSize"]) or 5, 3, 10)
        local showCone = (self.Flags["Misc_RadarCone"] ~= false)
        local bgAlpha = math.clamp(tonumber(self.Flags["Misc_RadarAlpha"]) or 0.05, 0, 0.85)
        radarFrame.BackgroundTransparency = bgAlpha
        canvas.BackgroundTransparency = bgAlpha * 0.8

        local cSize = canvas.AbsoluteSize
        local halfW = math.max(20, cSize.X * 0.5 - 8)
        local halfH = math.max(20, cSize.Y * 0.5 - 8)

        local myPos = Vector3.zero
        local camLook = Vector3.new(0, 0, -1)
        local camRight = Vector3.new(1, 0, 0)
        local halfFovDeg = 35
        if cam then
            camLook = Vector3.new(cam.CFrame.LookVector.X, 0, cam.CFrame.LookVector.Z)
            if camLook.Magnitude > 0.001 then camLook = camLook.Unit else camLook = Vector3.new(0, 0, -1) end
            camRight = Vector3.new(cam.CFrame.RightVector.X, 0, cam.CFrame.RightVector.Z)
            if camRight.Magnitude > 0.001 then camRight = camRight.Unit else camRight = Vector3.new(1, 0, 0) end
            myPos = cam.CFrame.Position
            halfFovDeg = math.clamp(cam.FieldOfView * 0.5, 20, 65)
        end
        if LocalPlayer and LocalPlayer.Character then
            local ok, piv = pcall(function() return LocalPlayer.Character:GetPivot().Position end)
            if ok and piv then myPos = piv end
        end

        local refRight = northLocked and Vector3.new(1, 0, 0) or camRight
        local refLook = northLocked and Vector3.new(0, 0, -1) or camLook

        -- Update FOV Cone arms
        fovLeftArm.Visible = showCone
        fovRightArm.Visible = showCone
        if showCone then
            local yawDeg = 0
            if northLocked then
                yawDeg = math.deg(math.atan2(camLook.X, -camLook.Z))
            end
            fovLeftArm.Rotation = yawDeg - halfFovDeg
            fovRightArm.Rotation = yawDeg + halfFovDeg
        end

        -- Update Cardinal Directions around radar perimeter
        for _, cEntry in ipairs(cardinals) do
            local cx = cEntry.Dir:Dot(refRight)
            local cy = -cEntry.Dir:Dot(refLook)
            cEntry.Label.Position = UDim2.new(0.5, math.floor(cx * (halfW - 2) + 0.5), 0.5, math.floor(cy * (halfH - 2) + 0.5))
        end

        local activeKeys = {}
        local otherCount = 0
        for _, plr in ipairs(Players:GetPlayers()) do
            if plr ~= LocalPlayer then
                local tel = self:GetPlayerTelemetry(plr)
                if tel and tel.Alive and tel.RootPart then
                    local tPos = tel.RootPart.Position
                    otherCount += 1
                    activeKeys[plr.Name] = true
                    local delta = tPos - myPos
                    local rx = delta:Dot(refRight) / radarRange
                    local ry = -delta:Dot(refLook) / radarRange
                    local mag = math.sqrt(rx * rx + ry * ry)
                    if mag > 1 then
                        rx = rx / mag
                        ry = ry / mag
                    end
                    local b = GetOrMakeBlip(plr.Name)
                    local isSelected = (Library.Flags["Misc_PlayerTarget"] == plr.Name)
                    b.Dot.Size = UDim2.fromOffset(blipScale, blipScale)
                    b.Dot.Position = UDim2.new(0.5, math.floor(rx * halfW + 0.5), 0.5, math.floor(ry * halfH + 0.5))
                    b.Dot.BackgroundColor3 = isSelected and self.Theme.Accent or (tel.IsTeammate and self.Theme.Success or self.Theme.Risky)
                    b.Tag.Text = plr.Name
                end
            end
        end

        -- Show demo blips if solo or Demo Specs is enabled so the radar is never empty while testing
        if otherCount == 0 or Library.Flags["Misc_DemoSpecs"] then
            local t = os.clock() * 0.45
            for idx, d in ipairs(demoBlips) do
                local key = "Demo_" .. d.Name
                activeKeys[key] = true
                local ang = t + idx * 1.7
                local ox = d.Offset.X + math.cos(ang) * 10
                local oy = d.Offset.Y + math.sin(ang) * 10
                local worldDelta = Vector3.new(ox, 0, oy)
                local rx = math.clamp(worldDelta:Dot(refRight) / (radarRange * 0.5), -0.92, 0.92)
                local ry = math.clamp(-worldDelta:Dot(refLook) / (radarRange * 0.5), -0.92, 0.92)
                local b = GetOrMakeBlip(key)
                b.Dot.Size = UDim2.fromOffset(blipScale, blipScale)
                b.Dot.Position = UDim2.new(0.5, math.floor(rx * halfW + 0.5), 0.5, math.floor(ry * halfH + 0.5))
                b.Dot.BackgroundColor3 = d.Enemy and self.Theme.Risky or self.Theme.Accent
                b.Tag.Text = d.Name
            end
        end

        for k, b in pairs(blips) do
            if not activeKeys[k] then
                if b.Dot and b.Dot.Parent then b.Dot:Destroy() end
                blips[k] = nil
            end
        end
    end)

    local radarObj = { Frame = radarFrame }
    function radarObj:SetVisible(v)
        local state = (v == true)
        radarFrame.Visible = state
        Library:SyncOptionValue("UI_ShowRadar", state)
        Library:SyncOptionValue("Misc_ShowRadar", state)
    end
    function radarObj:SetRange(studs)
        radarRange = math.max(25, tonumber(studs) or 180)
    end
    function radarObj:SetSize(px)
        radarSize = math.clamp(tonumber(px) or 170, 120, 320)
        Library:Tween(radarFrame, TweenFast, { Size = UDim2.fromOffset(radarSize, radarSize + 24) })
    end
    self.Radar = radarObj
    return radarObj
end

-- ============================================================================
-- 4. LIVE VIEWPORT TELEMETRY & OVERLAY CONTROLLER (FOV Circle, 2D Box/Name/HP/Tracer, World FX, Health Log)
-- ============================================================================
function Library:CreateWorldController()
    if self.WorldController then return self.WorldController end

    local overlayFolder = self:Create("Folder", {
        Name = "WorldViewportOverlay",
        Parent = ScreenGui,
    })

    -- Center / Cursor FOV Circle + Optional Fill & Inner Deadzone Ring
    local fovCircle = self:Create("Frame", {
        Name = "FOVCircle",
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundColor3 = self.Theme.Accent,
        BackgroundTransparency = 1,
        Position = UDim2.fromScale(0.5, 0.5),
        Size = UDim2.fromOffset(180, 180),
        Visible = false,
        ZIndex = 50,
        Parent = overlayFolder,
    })
    self:Create("UICorner", { CornerRadius = UDim.new(1, 0), Parent = fovCircle })
    local fovOuter = self:Create("UIStroke", {
        Color = Color3.new(0, 0, 0),
        Thickness = 2,
        Transparency = 0.35,
        Parent = fovCircle,
    })
    local fovStroke = self:Create("UIStroke", {
        Color = self.Theme.Accent,
        Thickness = 1,
        Parent = fovCircle,
    })
    self:BindTheme(fovStroke, "Color", "Accent")

    local deadzoneCircle = self:Create("Frame", {
        Name = "DeadzoneCircle",
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundTransparency = 1,
        Position = UDim2.fromScale(0.5, 0.5),
        Size = UDim2.fromOffset(30, 30),
        Visible = false,
        ZIndex = 50,
        Parent = overlayFolder,
    })
    self:Create("UICorner", { CornerRadius = UDim.new(1, 0), Parent = deadzoneCircle })
    local dzStroke = self:Create("UIStroke", {
        Color = self.Theme.Risky,
        Thickness = 1,
        Transparency = 0.25,
        Parent = deadzoneCircle,
    })

    local lighting = game:GetService("Lighting")
    local origAmbient = lighting.Ambient
    local origOutdoor = lighting.OutdoorAmbient
    local origExposure = lighting.ExposureCompensation
    local origClockTime = lighting.ClockTime
    local origBrightness = lighting.Brightness
    local origFogStart = lighting.FogStart
    local origFogEnd = lighting.FogEnd
    local origFogColor = lighting.FogColor
    local origGlobalShadows = lighting.GlobalShadows
    local nightApplied = false
    local clockApplied = false
    local fogApplied = false
    local fullbrightApplied = false
    local fovApplied = false
    local origCamFov = workspace.CurrentCamera and workspace.CurrentCamera.FieldOfView or 70
    local tpApplied = false
    local origMinZoom = LocalPlayer and LocalPlayer.CameraMinZoomDistance or 0.5
    local origMaxZoom = LocalPlayer and LocalPlayer.CameraMaxZoomDistance or 128
    local speedApplied = false
    local jumpApplied = false
    local noclipApplied = false
    local aaApplied = false
    local jitterFlip = false
    local lastTrigFire = 0
    local trigAcquireTime = nil
    local lastRageFire = 0
    local vimService = nil
    pcall(function() vimService = game:GetService("VirtualInputManager") end)

    local function FireWeapon(myChar, mPos)
        if self.Open then return end
        local eqTool = myChar and myChar:FindFirstChildOfClass("Tool")
        if eqTool then
            pcall(function() eqTool:Activate() end)
        end
        local usedPress = false
        pcall(function()
            if mouse1press and mouse1release then
                usedPress = true
                mouse1press()
                task.delay(0.018, function()
                    pcall(mouse1release)
                end)
            elseif mouse1click then
                usedPress = true
                mouse1click()
            end
        end)
        if not usedPress and vimService then
            pcall(function()
                vimService:SendMouseButtonEvent(mPos.X, mPos.Y, 0, true, game, 1)
                task.delay(0.018, function()
                    pcall(function()
                        vimService:SendMouseButtonEvent(mPos.X, mPos.Y, 0, false, game, 1)
                    end)
                end)
            end)
        end
    end

    local function RaycastPierceClips(origin, dir, baseIgnore)
        local params = RaycastParams.new()
        params.FilterType = Enum.RaycastFilterType.Exclude
        params.IgnoreWater = true
        local ignore = table.clone(baseIgnore)
        params.FilterDescendantsInstances = ignore
        for _ = 1, 5 do
            local hit = workspace:Raycast(origin, dir, params)
            if not hit or not hit.Instance then
                return nil
            end
            local inst = hit.Instance
            local isPlayerPart = false
            for _, plr in ipairs(Players:GetPlayers()) do
                if plr ~= LocalPlayer and plr.Character and inst:IsDescendantOf(plr.Character) then
                    isPlayerPart = true
                    break
                end
            end
            if isPlayerPart then
                return hit
            end
            -- Pierce through invisible Counter Blox clip/trigger parts or non-cancollide barriers
            if inst.Transparency >= 0.9 or not inst.CanCollide or string.find(string.lower(inst.Name), "clip") then
                table.insert(ignore, inst)
                params.FilterDescendantsInstances = ignore
            else
                return hit
            end
        end
        return nil
    end
    local noclipParts = {}
    local tpTransParts = {}
    local disabledPostEffects = {}
    local suppressedSmokes = {}
    local suppressedGuiOverlays = {}

    local ccEffect = Instance.new("ColorCorrectionEffect")
    ccEffect.Name = "ShardsColorCorrection"
    ccEffect.Enabled = false
    pcall(function() ccEffect.Parent = lighting end)

    local beamFolder = Instance.new("Folder")
    beamFolder.Name = "ShardsBulletBeams"
    pcall(function() beamFolder.Parent = workspace end)

    local cards = {}
    local trackedHealth = {}
    local playerConns = {}

    local function SpawnTracer(targetNameOrPos)
        if not self.Flags["World_Beams"] or self.Unloaded then return end
        local cam = workspace.CurrentCamera
        if not cam then return end
        local startPos = cam.CFrame.Position - Vector3.new(0, 0.4, 0)
        if LocalPlayer and LocalPlayer.Character then
            local head = LocalPlayer.Character:FindFirstChild("Head") or LocalPlayer.Character.PrimaryPart
            if head then
                startPos = head.Position
            end
        end

        local endPos
        if typeof(targetNameOrPos) == "Vector3" then
            endPos = targetNameOrPos
        elseif type(targetNameOrPos) == "string" then
            local tPlr = Players:FindFirstChild(targetNameOrPos)
            if tPlr and tPlr.Character then
                local tPart = tPlr.Character:FindFirstChild("Head") or tPlr.Character:FindFirstChild("HumanoidRootPart") or tPlr.Character.PrimaryPart
                if tPart then
                    endPos = tPart.Position
                end
            end
        end
        if not endPos then
            endPos = startPos + cam.CFrame.LookVector * 55 + Vector3.new(math.random(-20, 20) * 0.1, math.random(-10, 10) * 0.1, 0)
        end

        local dist = (endPos - startPos).Magnitude
        if dist < 0.5 then return end
        local beamCol = self.Flags["World_BeamCol"] or self.Theme.Accent
        local dur = math.clamp(tonumber(self.Flags["World_BeamTime"]) or 1.2, 0.2, 5)
        local thick = math.clamp(tonumber(self.Flags["World_BeamThick"]) or 0.12, 0.04, 0.5)

        local part = Instance.new("Part")
        part.Name = "ShardsBeam"
        part.Anchored = true
        part.CanCollide = false
        part.CanQuery = false
        part.CanTouch = false
        part.CastShadow = false
        part.Material = Enum.Material.Neon
        part.Color = beamCol
        part.Transparency = 0.1
        part.Size = Vector3.new(thick, thick, dist)
        part.CFrame = CFrame.lookAt(startPos:Lerp(endPos, 0.5), endPos)
        part.Parent = beamFolder

        local impact = Instance.new("Part")
        impact.Name = "ShardsImpact"
        impact.Anchored = true
        impact.CanCollide = false
        impact.CanQuery = false
        impact.CanTouch = false
        impact.CastShadow = false
        impact.Material = Enum.Material.Neon
        impact.Color = beamCol
        impact.Transparency = 0.05
        impact.Size = Vector3.new(thick * 3.2, thick * 3.2, thick * 3.2)
        impact.Position = endPos
        impact.Parent = beamFolder

        self:Tween(part, TweenInfo.new(dur, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {
            Transparency = 1,
            Size = Vector3.new(0.01, 0.01, dist),
        })
        self:Tween(impact, TweenInfo.new(dur, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {
            Transparency = 1,
            Size = Vector3.new(0.02, 0.02, 0.02),
        })
        task.delay(dur + 0.05, function()
            if part and part.Parent then part:Destroy() end
            if impact and impact.Parent then impact:Destroy() end
        end)
    end

    local function MakeCornerSet(parent, zIndex)
        local corners = {}
        -- 8 segments (2 per corner: horizontal + vertical)
        local defs = {
            -- Top-Left
            { AX = 0, AY = 0, PX = 0, PY = 0, Horiz = true },
            { AX = 0, AY = 0, PX = 0, PY = 0, Horiz = false },
            -- Top-Right
            { AX = 1, AY = 0, PX = 1, PY = 0, Horiz = true },
            { AX = 1, AY = 0, PX = 1, PY = 0, Horiz = false },
            -- Bottom-Left
            { AX = 0, AY = 1, PX = 0, PY = 1, Horiz = true },
            { AX = 0, AY = 1, PX = 0, PY = 1, Horiz = false },
            -- Bottom-Right
            { AX = 1, AY = 1, PX = 1, PY = 1, Horiz = true },
            { AX = 1, AY = 1, PX = 1, PY = 1, Horiz = false },
        }
        for _, d in ipairs(defs) do
            local seg = self:Create("Frame", {
                AnchorPoint = Vector2.new(d.AX, d.AY),
                BackgroundColor3 = self.Theme.Accent,
                BorderSizePixel = 0,
                Position = UDim2.fromScale(d.PX, d.PY),
                Size = d.Horiz and UDim2.new(0.28, 0, 0, 1) or UDim2.new(0, 1, 0.25, 0),
                Visible = false,
                ZIndex = zIndex,
                Parent = parent,
            })
            self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Transparency = 0.25, Parent = seg })
            table.insert(corners, seg)
        end
        return corners
    end

    local function GetOrMakeCard(plr)
        local c = cards[plr]
        if c and c.Root and c.Root.Parent then return c end

        local root = self:Create("Frame", {
            Name = "ESP_" .. plr.Name,
            BackgroundTransparency = 1,
            Visible = false,
            ZIndex = 55,
            Parent = overlayFolder,
        })

        local boxFill = self:Create("Frame", {
            BackgroundColor3 = self.Theme.Accent,
            BackgroundTransparency = 0.82,
            BorderSizePixel = 0,
            Size = UDim2.fromScale(1, 1),
            Visible = false,
            ZIndex = 55,
            Parent = root,
        })

        local box = self:Create("Frame", {
            BackgroundTransparency = 1,
            Size = UDim2.fromScale(1, 1),
            ZIndex = 56,
            Parent = root,
        })
        self:Create("UIStroke", {
            Color = Color3.new(0, 0, 0),
            Thickness = 2,
            Transparency = 0.25,
            Parent = box,
        })
        local boxStroke = self:Create("UIStroke", {
            Color = self.Theme.Accent,
            Thickness = 1,
            Parent = box,
        })

        local cornerSegments = MakeCornerSet(root, 57)

        local headDot = self:Create("Frame", {
            AnchorPoint = Vector2.new(0.5, 0.5),
            BackgroundColor3 = self.Theme.Accent,
            BorderSizePixel = 0,
            Position = UDim2.new(0.5, 0, 0.12, 0),
            Size = UDim2.fromOffset(4, 4),
            Visible = false,
            ZIndex = 58,
            Parent = root,
        })
        self:Create("UICorner", { CornerRadius = UDim.new(1, 0), Parent = headDot })
        self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Parent = headDot })

        local nameLbl = self:Create("TextLabel", {
            AnchorPoint = Vector2.new(0.5, 1),
            BackgroundTransparency = 1,
            Position = UDim2.new(0.5, 0, 0, -2),
            Size = UDim2.fromOffset(160, 13),
            Font = Enum.Font.Code,
            Text = plr.Name,
            TextColor3 = self.Theme.Text,
            TextSize = 11,
            ZIndex = 57,
            Parent = root,
        })

        local distLbl = self:Create("TextLabel", {
            AnchorPoint = Vector2.new(0.5, 0),
            BackgroundTransparency = 1,
            Position = UDim2.new(0.5, 0, 1, 2),
            Size = UDim2.fromOffset(160, 12),
            Font = Enum.Font.Code,
            Text = "",
            TextColor3 = self.Theme.TextDim,
            TextSize = 10,
            ZIndex = 57,
            Parent = root,
        })

        local hpBg = self:Create("Frame", {
            BackgroundColor3 = Color3.fromRGB(10, 10, 12),
            BorderSizePixel = 0,
            Position = UDim2.fromOffset(-6, 0),
            Size = UDim2.new(0, 3, 1, 0),
            ZIndex = 56,
            Parent = root,
        })
        self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Parent = hpBg })
        local hpFill = self:Create("Frame", {
            AnchorPoint = Vector2.new(0, 1),
            BackgroundColor3 = self.Theme.Success,
            BorderSizePixel = 0,
            Position = UDim2.fromScale(0, 1),
            Size = UDim2.fromScale(1, 1),
            ZIndex = 57,
            Parent = hpBg,
        })
        local hpText = self:Create("TextLabel", {
            AnchorPoint = Vector2.new(1, 0.5),
            BackgroundTransparency = 1,
            Position = UDim2.new(0, -2, 0, 0),
            Size = UDim2.fromOffset(32, 10),
            Font = Enum.Font.GothamBold,
            Text = "100",
            TextColor3 = Color3.new(1, 1, 1),
            TextSize = 9,
            TextXAlignment = Enum.TextXAlignment.Right,
            Visible = false,
            ZIndex = 58,
            Parent = hpBg,
        })

        -- Right Side Live ESP Flags Stack
        local flagsCol = self:Create("Frame", {
            BackgroundTransparency = 1,
            Position = UDim2.new(1, 4, 0, 0),
            Size = UDim2.fromOffset(48, 120),
            ZIndex = 57,
            Parent = root,
        })
        self:Create("UIListLayout", {
            Padding = UDim.new(0, 1),
            SortOrder = Enum.SortOrder.LayoutOrder,
            Parent = flagsCol,
        })
        local flagDefs = {
            { Key = "Armor (HK)",   Text = "HK",    Color = Color3.fromRGB(230, 230, 235) },
            { Key = "Scoped",       Text = "ZOOM",  Color = Color3.fromRGB(85, 190, 255)  },
            { Key = "Flashed",      Text = "FLASH", Color = Color3.fromRGB(245, 205, 75)  },
            { Key = "Exploit / LC", Text = "LC",    Color = Color3.fromRGB(235, 75, 75)   },
            { Key = "Ping",         Text = "24MS",  Color = Color3.fromRGB(145, 220, 110) },
            { Key = "Distance",     Text = "0FT",   Color = Color3.fromRGB(185, 185, 195) },
        }
        local flagLabels = {}
        for idx, fd in ipairs(flagDefs) do
            local fl = self:Create("TextLabel", {
                BackgroundTransparency = 1,
                LayoutOrder = idx,
                Size = UDim2.new(1, 0, 0, 10),
                Font = Enum.Font.GothamBold,
                Text = fd.Text,
                TextColor3 = fd.Color,
                TextSize = 9,
                TextXAlignment = Enum.TextXAlignment.Left,
                Visible = false,
                ZIndex = 58,
                Parent = flagsCol,
            })
            flagLabels[fd.Key] = fl
        end

        -- 2D Snapline / Tracer Line
        local tracerLine = self:Create("Frame", {
            AnchorPoint = Vector2.new(0.5, 0.5),
            BackgroundColor3 = self.Theme.Accent,
            BorderSizePixel = 0,
            Visible = false,
            ZIndex = 52,
            Parent = overlayFolder,
        })
        self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Transparency = 0.35, Parent = tracerLine })

        -- 2D Skeleton Bone Line Pool (14 lines for R15 / R6 rigs)
        local boneLines = {}
        for bIdx = 1, 14 do
            local bl = self:Create("Frame", {
                Name = "Bone_" .. tostring(bIdx),
                AnchorPoint = Vector2.new(0.5, 0.5),
                BackgroundColor3 = Color3.fromRGB(240, 240, 245),
                BorderSizePixel = 0,
                Size = UDim2.fromOffset(10, 1),
                Visible = false,
                ZIndex = 54,
                Parent = overlayFolder,
            })
            self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Transparency = 0.35, Parent = bl })
            boneLines[bIdx] = bl
        end

        -- Off-Screen Directional Indicator Arrow
        local oofArrow = self:Create("Frame", {
            AnchorPoint = Vector2.new(0.5, 0.5),
            BackgroundColor3 = self.Theme.Risky,
            BorderSizePixel = 0,
            Size = UDim2.fromOffset(10, 10),
            Visible = false,
            ZIndex = 54,
            Parent = overlayFolder,
        })
        self:Create("UICorner", { CornerRadius = UDim.new(0, 2), Parent = oofArrow })
        self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Parent = oofArrow })
        local oofTag = self:Create("TextLabel", {
            AnchorPoint = Vector2.new(0.5, 0),
            BackgroundTransparency = 1,
            Position = UDim2.new(0.5, 0, 1, 2),
            Size = UDim2.fromOffset(65, 10),
            Font = Enum.Font.Code,
            Text = plr.Name,
            TextColor3 = self.Theme.Text,
            TextSize = 9,
            ZIndex = 55,
            Parent = oofArrow,
        })

        c = {
            Root = root,
            Box = box,
            BoxFill = boxFill,
            BoxStroke = boxStroke,
            Corners = cornerSegments,
            HeadDot = headDot,
            Name = nameLbl,
            Dist = distLbl,
            HpBg = hpBg,
            HpFill = hpFill,
            HpText = hpText,
            FlagLabels = flagLabels,
            Tracer = tracerLine,
            Bones = boneLines,
            OOFArrow = oofArrow,
            OOFTag = oofTag,
            Highlight = nil,
            GhostPart = nil,
            History = {},
        }
        cards[plr] = c
        return c
    end

    local R15BonePairs = {
        { "Head", "UpperTorso" },
        { "UpperTorso", "LowerTorso" },
        { "UpperTorso", "LeftUpperArm" },
        { "LeftUpperArm", "LeftLowerArm" },
        { "LeftLowerArm", "LeftHand" },
        { "UpperTorso", "RightUpperArm" },
        { "RightUpperArm", "RightLowerArm" },
        { "RightLowerArm", "RightHand" },
        { "LowerTorso", "LeftUpperLeg" },
        { "LeftUpperLeg", "LeftLowerLeg" },
        { "LeftLowerLeg", "LeftFoot" },
        { "LowerTorso", "RightUpperLeg" },
        { "RightUpperLeg", "RightLowerLeg" },
        { "RightLowerLeg", "RightFoot" },
    }
    local R6BonePairs = {
        { "Head", "Torso" },
        { "Torso", "Left Arm" },
        { "Torso", "Right Arm" },
        { "Torso", "Left Leg" },
        { "Torso", "Right Leg" },
    }

    -- Dropped Weapon & Planted C4 World ESP Cards (Counter Blox & Universal)
    local worldItemCards = {}
    local c4PlantTime = nil
    local function GetOrMakeWorldItemCard(obj)
        local wc = worldItemCards[obj]
        if wc and wc.Label and wc.Label.Parent then return wc end
        local lbl = self:Create("TextLabel", {
            AnchorPoint = Vector2.new(0.5, 0.5),
            BackgroundTransparency = 1,
            Size = UDim2.fromOffset(160, 14),
            Font = Enum.Font.GothamBold,
            Text = "",
            TextColor3 = self.Theme.Accent,
            TextSize = 10,
            Visible = false,
            ZIndex = 53,
            Parent = overlayFolder,
        })
        wc = { Label = lbl }
        worldItemCards[obj] = wc
        return wc
    end

    local function DisconnectPlayerConns(plr)
        local list = playerConns[plr]
        if list then
            for _, conn in ipairs(list) do
                pcall(function() conn:Disconnect() end)
            end
            playerConns[plr] = nil
        end
    end

    local function HookCharacterHealth(plr, char)
        if not char or self.Unloaded then return end
        local list = playerConns[plr]
        if not list then
            list = {}
            playerConns[plr] = list
        end
        if list.HealthConn then
            pcall(function() list.HealthConn:Disconnect() end)
        end
        if list.CBHealthConn then
            pcall(function() list.CBHealthConn:Disconnect() end)
        end

        local function OnHealthUpdated(newHp, maxHp)
            if self.Unloaded then return end
            local oldHp = trackedHealth[plr] or maxHp or 100
            trackedHealth[plr] = newHp
            local diff = math.floor((oldHp - newHp) + 0.5)
            if diff > 0 and (Library.Flags["ESP_Box"] or Library.Flags["ESP_Name"] or Library.Flags["Rage_Enabled"]) then
                local rem = math.max(0, math.floor(newHp + 0.5))
                Library:AddHitlog({
                    Target = plr.Name,
                    Hitbox = "chest",
                    Damage = diff,
                    Remaining = rem,
                    Hitchance = Library.Flags["Rage_Hitchance"] or 84,
                })
                if Library.TargetHUD then
                    Library.TargetHUD:SetTarget({
                        Name = plr.Name,
                        UserId = plr.UserId,
                        HP = rem,
                        MaxHP = math.floor((maxHp or 100) + 0.5),
                    })
                end
            end
        end

        -- Counter Blox NRPBS.Health listener + standard Humanoid fallback
        local nrpbs = plr:FindFirstChild("NRPBS")
        local cbHpVal = nrpbs and nrpbs:FindFirstChild("Health")
        if cbHpVal then
            trackedHealth[plr] = tonumber(cbHpVal.Value) or 100
            list.CBHealthConn = cbHpVal.Changed:Connect(function(val)
                local mh = nrpbs:FindFirstChild("MaxHealth")
                OnHealthUpdated(tonumber(val) or 0, (mh and tonumber(mh.Value)) or 100)
            end)
            table.insert(list, list.CBHealthConn)
        end

        local hum = char:FindFirstChildOfClass("Humanoid") or char:WaitForChild("Humanoid", 3)
        if hum and not self.Unloaded then
            if not cbHpVal then
                trackedHealth[plr] = hum.Health
            end
            list.HealthConn = hum.HealthChanged:Connect(function(newHp)
                if not cbHpVal then
                    OnHealthUpdated(newHp, hum.MaxHealth)
                end
            end)
            table.insert(list, list.HealthConn)
        end
    end

    local function TrackPlayer(plr)
        if plr == LocalPlayer then return end
        DisconnectPlayerConns(plr)
        playerConns[plr] = {}
        if plr.Character then
            task.spawn(HookCharacterHealth, plr, plr.Character)
        end
        local charConn = plr.CharacterAdded:Connect(function(ch)
            task.wait(0.2)
            HookCharacterHealth(plr, ch)
        end)
        table.insert(playerConns[plr], charConn)
    end

    for _, plr in ipairs(Players:GetPlayers()) do
        TrackPlayer(plr)
    end
    self:Connect(Players.PlayerAdded, TrackPlayer)
    self:Connect(Players.PlayerRemoving, function(plr)
        DisconnectPlayerConns(plr)
        local c = cards[plr]
        if c then
            if c.Root then c.Root:Destroy() end
            if c.Tracer then c.Tracer:Destroy() end
            if c.OOFArrow then c.OOFArrow:Destroy() end
            if c.Highlight then pcall(function() c.Highlight:Destroy() end) end
            cards[plr] = nil
        end
        trackedHealth[plr] = nil
    end)

    -- Infinite Jump Listener
    self:Connect(UserInputService.JumpRequest, function()
        if self.Unloaded or not self.Flags["Misc_InfJump"] then return end
        local myChar = LocalPlayer and LocalPlayer.Character
        local myHum = myChar and myChar:FindFirstChildOfClass("Humanoid")
        if myHum then
            myHum:ChangeState(Enum.HumanoidStateType.Jumping)
        end
    end)

    -- Noclip Stepped Loop
    self:Connect(RunService.Stepped, function()
        if self.Unloaded then return end
        local myChar = LocalPlayer and LocalPlayer.Character
        if self.Flags["Misc_Noclip"] and myChar then
            noclipApplied = true
            for _, part in ipairs(myChar:GetDescendants()) do
                if part:IsA("BasePart") and part.CanCollide then
                    noclipParts[part] = true
                    part.CanCollide = false
                end
            end
        elseif noclipApplied then
            noclipApplied = false
            for part in pairs(noclipParts) do
                if part and part.Parent then
                    pcall(function() part.CanCollide = true end)
                end
            end
            table.clear(noclipParts)
        end
    end)

    self:Connect(RunService.RenderStepped, function(dt)
        if self.Unloaded then return end
        local F = self.Flags
        local cam = workspace.CurrentCamera
        if not cam then return end
        local vp = cam.ViewportSize
        local mPos = UserInputService:GetMouseLocation()

        -- 1) FOV Circle & Deadzone Circle
        local showFov = (F["ESP_ShowFOV"] == true) or (F["Rage_Enabled"] == true) or (F["Legit_Enable"] == true)
        fovCircle.Visible = showFov
        if showFov then
            local radius = F["Legit_Enable"] and ((tonumber(F["Legit_FOV"]) or 4.5) * 12) or ((tonumber(F["Rage_FOV"]) or 90) * 1.6)
            local d = math.clamp(radius * 2, 24, 750)
            local centerMode = (F["ESP_FOVOrigin"] == "Screen Center")
            local pos = centerMode and UDim2.fromOffset(vp.X * 0.5, vp.Y * 0.5) or UDim2.fromOffset(mPos.X, mPos.Y)
            local col = F["ESP_FOVColor"] or self.Theme.Accent
            fovCircle.Position = pos
            fovCircle.Size = UDim2.fromOffset(d, d)
            fovStroke.Color = col
            fovCircle.BackgroundColor3 = col
            fovCircle.BackgroundTransparency = (F["ESP_FOVFilled"] == true) and math.clamp(1 - (tonumber(F["ESP_FOVAlpha"]) or 0.15), 0.4, 0.96) or 1

            local dzRadius = tonumber(F["Legit_Deadzone"]) or 0
            if F["Legit_Enable"] and dzRadius > 0 then
                deadzoneCircle.Visible = true
                deadzoneCircle.Position = pos
                deadzoneCircle.Size = UDim2.fromOffset(dzRadius * 16, dzRadius * 16)
            else
                deadzoneCircle.Visible = false
            end
        else
            deadzoneCircle.Visible = false
        end

        -- 2) World Nightmode, Time of Day, Fog, Fullbright, ColorCorrection & Visual Removals
        local removals = type(F["World_Removals"]) == "table" and F["World_Removals"] or {}
        if F["World_Fullbright"] then
            fullbrightApplied = true
            lighting.Ambient = Color3.new(1, 1, 1)
            lighting.OutdoorAmbient = Color3.new(1, 1, 1)
            lighting.Brightness = 2.5
            lighting.GlobalShadows = false
        elseif F["World_Night"] then
            nightApplied = true
            local tint = F["World_Color"] or Color3.fromRGB(28, 28, 38)
            lighting.Ambient = tint
            lighting.OutdoorAmbient = tint
            lighting.ExposureCompensation = tonumber(F["World_Exposure"]) or -1.2
            if fullbrightApplied then
                fullbrightApplied = false
                lighting.Brightness = origBrightness
                lighting.GlobalShadows = origGlobalShadows
            end
        else
            if nightApplied or fullbrightApplied then
                nightApplied = false
                fullbrightApplied = false
                lighting.Ambient = origAmbient
                lighting.OutdoorAmbient = origOutdoor
                lighting.ExposureCompensation = origExposure
                lighting.Brightness = origBrightness
                lighting.GlobalShadows = origGlobalShadows
            end
        end

        if F["World_Clock_Enabled"] then
            clockApplied = true
            lighting.ClockTime = math.clamp(tonumber(F["World_ClockTime"]) or 0, 0, 24)
        elseif clockApplied then
            clockApplied = false
            lighting.ClockTime = origClockTime
        end

        if removals["Fog"] then
            fogApplied = true
            lighting.FogStart = 100000
            lighting.FogEnd = 100000
        elseif F["World_Fog_Enabled"] then
            fogApplied = true
            lighting.FogStart = 15
            lighting.FogEnd = math.clamp(tonumber(F["World_FogEnd"]) or 350, 30, 2500)
            lighting.FogColor = F["World_FogCol"] or self.Theme.Accent
        elseif fogApplied then
            fogApplied = false
            lighting.FogStart = origFogStart
            lighting.FogEnd = origFogEnd
            lighting.FogColor = origFogColor
        end

        if removals["Post Processing"] then
            for _, eff in ipairs(lighting:GetChildren()) do
                if eff ~= ccEffect and eff:IsA("PostEffect") and eff.Enabled then
                    disabledPostEffects[eff] = true
                    eff.Enabled = false
                end
            end
        elseif next(disabledPostEffects) ~= nil then
            for eff in pairs(disabledPostEffects) do
                if eff and eff.Parent then
                    pcall(function() eff.Enabled = true end)
                end
            end
            table.clear(disabledPostEffects)
        end

        -- Counter Blox Smoke Grenade Removal (workspace.Ray_Ignore.Smokes / workspace.Debris)
        if removals["Smoke Grenades"] then
            local rayIgnore = workspace:FindFirstChild("Ray_Ignore")
            local smokesFolder = (rayIgnore and rayIgnore:FindFirstChild("Smokes")) or workspace:FindFirstChild("Debris")
            if smokesFolder then
                for _, d in ipairs(smokesFolder:GetDescendants()) do
                    if d:IsA("ParticleEmitter") and d.Enabled then
                        suppressedSmokes[d] = true
                        d.Enabled = false
                    elseif d:IsA("BasePart") and string.find(string.lower(d.Name), "smoke") and d.Transparency < 1 then
                        suppressedSmokes[d] = d.Transparency
                        d.Transparency = 1
                    end
                end
            end
        elseif next(suppressedSmokes) ~= nil then
            for inst, prev in pairs(suppressedSmokes) do
                if inst and inst.Parent then
                    pcall(function()
                        if inst:IsA("ParticleEmitter") then
                            inst.Enabled = true
                        elseif inst:IsA("BasePart") and type(prev) == "number" then
                            inst.Transparency = prev
                        end
                    end)
                end
            end
            table.clear(suppressedSmokes)
        end

        -- Counter Blox Flashbang & Scope Overlay Removals (PlayerGui)
        local wantNoFlash = (removals["Flashbang Effect"] == true)
        local wantNoScope = (removals["Scope Overlay"] == true)
        if (wantNoFlash or wantNoScope) and LocalPlayer then
            local pGui = LocalPlayer:FindFirstChildOfClass("PlayerGui")
            if pGui then
                for _, guiObj in ipairs(pGui:GetDescendants()) do
                    if guiObj:IsA("GuiObject") and guiObj.Visible then
                        local lName = string.lower(guiObj.Name)
                        if (wantNoFlash and (lName == "blnd" or lName == "flash" or lName == "flashbang" or lName == "whiteout"))
                            or (wantNoScope and (lName == "scope" or lName == "scopeframe" or lName == "sniperoverlay")) then
                            suppressedGuiOverlays[guiObj] = true
                            guiObj.Visible = false
                        end
                    end
                end
            end
        elseif next(suppressedGuiOverlays) ~= nil then
            for gObj in pairs(suppressedGuiOverlays) do
                if gObj and gObj.Parent then
                    pcall(function() gObj.Visible = true end)
                end
            end
            table.clear(suppressedGuiOverlays)
        end

        if ccEffect and ccEffect.Parent then
            if F["World_CC_Enabled"] then
                ccEffect.Enabled = true
                ccEffect.Saturation = math.clamp(tonumber(F["World_Saturation"]) or 0.35, -1, 2)
                ccEffect.Contrast = math.clamp(tonumber(F["World_Contrast"]) or 0.15, -1, 2)
                ccEffect.TintColor = F["World_CCTint"] or Color3.new(1, 1, 1)
            else
                ccEffect.Enabled = false
            end
        end

        -- 3) LocalPlayer Movement Controller (WalkSpeed, JumpPower, Bhop, Air Strafe, Fast Stop)
        local myChar = LocalPlayer and LocalPlayer.Character
        local myHum = myChar and myChar:FindFirstChildOfClass("Humanoid")
        local myHrp = myChar and (myChar:FindFirstChild("HumanoidRootPart") or myChar:FindFirstChild("Torso") or myChar.PrimaryPart)
        if myHum and myHrp then
            local moveDir = myHum.MoveDirection
            local onGround = (myHum.FloorMaterial ~= Enum.Material.Air)

            if F["Misc_Speed_Enabled"] then
                speedApplied = true
                local targetSpd = math.clamp(tonumber(F["Misc_Speed"]) or 28, 16, 150)
                myHum.WalkSpeed = targetSpd
                if moveDir.Magnitude > 0.05 then
                    local vel = myHrp.AssemblyLinearVelocity
                    myHrp.AssemblyLinearVelocity = Vector3.new(moveDir.X * targetSpd, vel.Y, moveDir.Z * targetSpd)
                end
            elseif speedApplied then
                speedApplied = false
                myHum.WalkSpeed = 16
            end

            if F["Misc_Jump_Enabled"] then
                jumpApplied = true
                local targetJmp = math.clamp(tonumber(F["Misc_Jump"]) or 65, 50, 200)
                myHum.UseJumpPower = true
                myHum.JumpPower = targetJmp
            elseif jumpApplied then
                jumpApplied = false
                myHum.JumpPower = 50
            end

            local spaceHeld = UserInputService:IsKeyDown(Enum.KeyCode.Space) and (UserInputService:GetFocusedTextBox() == nil)
            if F["Misc_Bhop"] and spaceHeld then
                if onGround then
                    myHum.Jump = true
                    myHum:ChangeState(Enum.HumanoidStateType.Jumping)
                elseif moveDir.Magnitude > 0.05 then
                    local boost = math.clamp(tonumber(F["Misc_BhopSpeed"]) or 24, 0, 120)
                    local vel = myHrp.AssemblyLinearVelocity
                    local targetAirSpd = math.max(Vector2.new(vel.X, vel.Z).Magnitude, 18 + boost * 0.65)
                    myHrp.AssemblyLinearVelocity = Vector3.new(
                        moveDir.X * targetAirSpd,
                        vel.Y,
                        moveDir.Z * targetAirSpd
                    )
                end
            end

            if F["Misc_Strafe"] and not onGround and moveDir.Magnitude > 0.05 then
                local vel = myHrp.AssemblyLinearVelocity
                local horizSpeed = math.max(20, Vector2.new(vel.X, vel.Z).Magnitude)
                local targetVel = moveDir.Unit * horizSpeed
                myHrp.AssemblyLinearVelocity = Vector3.new(
                    vel.X + (targetVel.X - vel.X) * math.clamp(dt * 10, 0, 1),
                    vel.Y,
                    vel.Z + (targetVel.Z - vel.Z) * math.clamp(dt * 10, 0, 1)
                )
            end

            if F["Misc_FastStop"] and onGround and moveDir.Magnitude < 0.02 then
                local vel = myHrp.AssemblyLinearVelocity
                if Vector2.new(vel.X, vel.Z).Magnitude > 2 then
                    myHrp.AssemblyLinearVelocity = Vector3.new(vel.X * 0.15, vel.Y, vel.Z * 0.15)
                end
            end

            -- Anti-Aim Character Yaw & Jitter Controller
            if F["AA_Enabled"] then
                aaApplied = true
                myHum.AutoRotate = false
                local baseYaw = math.atan2(-cam.CFrame.LookVector.X, -cam.CFrame.LookVector.Z)
                local yawMode = F["AA_YawBase"] or "Local View"
                if yawMode ~= "Local View" then
                    local bestTargetPos
                    local bestMetric = math.huge
                    for _, plr in ipairs(Players:GetPlayers()) do
                        if plr ~= LocalPlayer then
                            local tel = self:GetPlayerTelemetry(plr)
                            if tel and tel.Alive and (not F["ESP_TeamCheck"] or not tel.IsTeammate) then
                                if yawMode == "At Targets (Distance)" then
                                    local d = (tel.RootPart.Position - myHrp.Position).Magnitude
                                    if d < bestMetric then
                                        bestMetric = d
                                        bestTargetPos = tel.RootPart.Position
                                    end
                                else
                                    local sp, onScr = cam:WorldToViewportPoint(tel.RootPart.Position)
                                    if onScr and sp.Z > 0 then
                                        local d2 = (Vector2.new(sp.X, sp.Y) - mPos).Magnitude
                                        if d2 < bestMetric then
                                            bestMetric = d2
                                            bestTargetPos = tel.RootPart.Position
                                        end
                                    end
                                end
                            end
                        end
                    end
                    if bestTargetPos then
                        local dir = bestTargetPos - myHrp.Position
                        baseYaw = math.atan2(-dir.X, -dir.Z)
                    end
                end
                jitterFlip = not jitterFlip
                local yawOffsetDeg = tonumber(F["AA_YawOffset"]) or 180
                local jitterDeg = tonumber(F["AA_Jitter"]) or 0
                local finalYaw = baseYaw + math.rad(yawOffsetDeg + (jitterFlip and jitterDeg or -jitterDeg))
                myHrp.CFrame = CFrame.new(myHrp.Position) * CFrame.Angles(0, finalYaw, 0)
            elseif aaApplied then
                aaApplied = false
                myHum.AutoRotate = true
            end
        end

        -- 4) Counter Blox & Universal 2D Player Overlays, Skeleton, Backtrack Ghost, Tracers, Off-Screen Arrows & Chams
        local hasFlagESP = type(F["ESP_Flags"]) == "table" and next(F["ESP_Flags"]) ~= nil
        local anyESP = F["ESP_Box"] or F["ESP_Name"] or F["ESP_Health"] or F["ESP_Weapon"] or F["ESP_Glow"]
            or F["ESP_Tracers"] or F["ESP_OOF"] or F["ESP_HeadDot"] or F["ESP_Skeleton"] or F["ESP_Backtrack"]
            or F["Chams_Vis"] or F["Chams_XQZ"] or hasFlagESP
        local maxDist = tonumber(F["ESP_MaxDist"]) or 2000
        local teamCheck = (F["ESP_TeamCheck"] == true)
        local boxStyle = F["ESP_BoxStyle"] or "Full 2D"
        local hpBarPos = F["ESP_HealthPos"] or "Left"
        local showHpText = (F["ESP_HealthText"] == true)
        local tracerOriginMode = F["ESP_TracerOrigin"] or "Bottom"
        local activeFlags = type(F["ESP_Flags"]) == "table" and F["ESP_Flags"] or nil
        local chamsMat = F["Chams_Mat"] or "Metallic"
        local pulseWave = 0.5 + 0.5 * math.sin(os.clock() * 4.2)
        local nowClock = os.clock()
        local btWindow = math.clamp((tonumber(F["ESP_BacktrackTime"]) or 200) / 1000, 0.05, 0.45)

        for _, plr in ipairs(Players:GetPlayers()) do
            if plr ~= LocalPlayer then
                local isWhitelisted = (F["Misc_Whitelist"] == true) and (F["Misc_PlayerTarget"] == plr.Name)
                local tel = self:GetPlayerTelemetry(plr)
                local skipTeam = teamCheck and (tel and tel.IsTeammate or false)
                local char = tel and tel.Character
                local hrp = tel and tel.RootPart
                local distStuds = 0
                if hrp then
                    distStuds = (cam.CFrame.Position - hrp.Position).Magnitude
                end
                if anyESP and not isWhitelisted and not skipTeam and tel and tel.Alive and char and hrp and distStuds <= maxDist then
                    local c = GetOrMakeCard(plr)
                    local topPos, onScreenTop = cam:WorldToViewportPoint(hrp.Position + Vector3.new(0, 2.8, 0))
                    local botPos, onScreenBot = cam:WorldToViewportPoint(hrp.Position - Vector3.new(0, 3.2, 0))
                    local centerPos, onScreenCenter = cam:WorldToViewportPoint(hrp.Position)

                    if (onScreenTop or onScreenBot or onScreenCenter) and topPos.Z > 0 then
                        c.OOFArrow.Visible = false
                        local h = math.clamp(math.abs(botPos.Y - topPos.Y), 16, 600)
                        local w = math.clamp(h * 0.58, 12, 400)
                        local rx = math.floor(topPos.X - w * 0.5 + 0.5)
                        local ry = math.floor(topPos.Y + 0.5)
                        c.Root.Position = UDim2.fromOffset(rx, ry)
                        c.Root.Size = UDim2.fromOffset(math.floor(w + 0.5), math.floor(h + 0.5))
                        c.Root.Visible = true

                        local boxCol = F["ESP_BoxColor"] or self.Theme.Accent
                        local showBox = (F["ESP_Box"] == true)
                        local isCorner = (boxStyle == "Corner")
                        local isFilled = (boxStyle == "Filled 2D")
                        c.Box.Visible = showBox and not isCorner
                        c.BoxStroke.Color = boxCol
                        c.BoxFill.Visible = showBox and isFilled
                        c.BoxFill.BackgroundColor3 = boxCol
                        c.BoxFill.BackgroundTransparency = math.clamp(1 - (tonumber(F["ESP_BoxFillAlpha"]) or 0.18), 0.35, 0.95)
                        for _, seg in ipairs(c.Corners) do
                            seg.Visible = showBox and isCorner
                            seg.BackgroundColor3 = boxCol
                        end

                        c.HeadDot.Visible = (F["ESP_HeadDot"] == true)
                        c.HeadDot.BackgroundColor3 = boxCol
                        local headPart = tel.Head
                        if headPart and c.HeadDot.Visible then
                            local hp2d = cam:WorldToViewportPoint(headPart.Position)
                            c.HeadDot.Position = UDim2.fromOffset(math.floor(hp2d.X - rx + 0.5), math.floor(hp2d.Y - ry + 0.5))
                        end

                        -- 2D Skeleton / Bone Lines (R15 & R6)
                        if F["ESP_Skeleton"] == true then
                            local skelCol = F["ESP_SkeletonCol"] or Color3.fromRGB(240, 240, 245)
                            local pairsList = char:FindFirstChild("UpperTorso") and R15BonePairs or R6BonePairs
                            for bIdx, bl in ipairs(c.Bones) do
                                local pair = pairsList[bIdx]
                                local pA = pair and char:FindFirstChild(pair[1])
                                local pB = pair and char:FindFirstChild(pair[2])
                                if pA and pB and pA:IsA("BasePart") and pB:IsA("BasePart") then
                                    local vA, okA = cam:WorldToViewportPoint(pA.Position)
                                    local vB, okB = cam:WorldToViewportPoint(pB.Position)
                                    if (okA or okB) and vA.Z > 0 and vB.Z > 0 then
                                        local ptA = Vector2.new(vA.X, vA.Y)
                                        local ptB = Vector2.new(vB.X, vB.Y)
                                        local dVec = ptB - ptA
                                        local len = dVec.Magnitude
                                        local mid = (ptA + ptB) * 0.5
                                        bl.Visible = true
                                        bl.BackgroundColor3 = skelCol
                                        bl.Position = UDim2.fromOffset(mid.X, mid.Y)
                                        bl.Size = UDim2.fromOffset(len, 1)
                                        bl.Rotation = math.deg(math.atan2(dVec.Y, dVec.X))
                                    else
                                        bl.Visible = false
                                    end
                                else
                                    bl.Visible = false
                                end
                            end
                        else
                            for _, bl in ipairs(c.Bones) do
                                bl.Visible = false
                            end
                        end

                        c.Name.Visible = (F["ESP_Name"] == true)
                        c.Name.TextColor3 = F["ESP_NameColor"] or self.Theme.Text
                        c.Name.Text = string.format("%s [%d]", plr.Name, tel.HP)

                        local showHp = (F["ESP_Health"] == true)
                        c.HpBg.Visible = showHp
                        local hpRatio = math.clamp(tel.HP / tel.MaxHP, 0, 1)
                        c.HpFill.BackgroundColor3 = Color3.fromHSV(hpRatio * 0.33, 0.85, 0.92)
                        if hpBarPos == "Bottom" then
                            c.HpBg.Position = UDim2.new(0, 0, 1, 3)
                            c.HpBg.Size = UDim2.new(1, 0, 0, 3)
                            c.HpFill.AnchorPoint = Vector2.new(0, 0)
                            c.HpFill.Position = UDim2.fromScale(0, 0)
                            c.HpFill.Size = UDim2.fromScale(hpRatio, 1)
                            c.Dist.Position = UDim2.new(0.5, 0, 1, 8)
                        elseif hpBarPos == "Right" then
                            c.HpBg.Position = UDim2.new(1, 3, 0, 0)
                            c.HpBg.Size = UDim2.new(0, 3, 1, 0)
                            c.HpFill.AnchorPoint = Vector2.new(0, 1)
                            c.HpFill.Position = UDim2.fromScale(0, 1)
                            c.HpFill.Size = UDim2.fromScale(1, hpRatio)
                            c.Dist.Position = UDim2.new(0.5, 0, 1, 2)
                        else
                            c.HpBg.Position = UDim2.fromOffset(-6, 0)
                            c.HpBg.Size = UDim2.new(0, 3, 1, 0)
                            c.HpFill.AnchorPoint = Vector2.new(0, 1)
                            c.HpFill.Position = UDim2.fromScale(0, 1)
                            c.HpFill.Size = UDim2.fromScale(1, hpRatio)
                            c.Dist.Position = UDim2.new(0.5, 0, 1, 2)
                        end
                        c.HpText.Visible = showHp and showHpText
                        if c.HpText.Visible then
                            c.HpText.Text = tostring(tel.HP)
                            c.HpText.Position = UDim2.new(0, -2, 1 - hpRatio, 0)
                        end

                        c.Dist.Visible = (F["ESP_Weapon"] == true)
                        c.Dist.TextColor3 = F["ESP_WeaponColor"] or self.Theme.TextDim
                        c.Dist.Text = tel.Weapon

                        -- Live Right-Side ESP Flags (Counter Blox Armor/Helmet/Scoped/Flashed/LC/Distance/Ping aware)
                        for key, fl in pairs(c.FlagLabels) do
                            local enabled = activeFlags and (activeFlags[key] == true) or false
                            if enabled then
                                if key == "Armor (HK)" then
                                    fl.Text = tel.HasHelmet and "HK" or (tel.HasArmor and "K" or "HK")
                                elseif key == "Scoped" then
                                    fl.Text = "ZOOM"
                                    enabled = tel.IsScoped or string.find(tel.Weapon, "AWP") ~= nil or string.find(tel.Weapon, "SCOUT") ~= nil or string.find(tel.Weapon, "SSG") ~= nil
                                elseif key == "Flashed" then
                                    fl.Text = "FLASH"
                                    enabled = tel.IsFlashed
                                elseif key == "Exploit / LC" then
                                    fl.Text = "LC"
                                    enabled = tel.BreakingLC
                                elseif key == "Ping" then
                                    local pingMs = 24
                                    pcall(function()
                                        pingMs = math.floor(game:GetService("Stats").Network.ServerStatsItem["Data Ping"]:GetValue())
                                    end)
                                    fl.Text = string.format("%dMS", pingMs)
                                elseif key == "Distance" then
                                    fl.Text = string.format("%dFT", math.floor(distStuds + 0.5))
                                end
                            end
                            fl.Visible = enabled
                        end

                        -- 2D Snapline / Tracer
                        if F["ESP_Tracers"] == true then
                            local origin = Vector2.new(vp.X * 0.5, vp.Y - 4)
                            if tracerOriginMode == "Center" then
                                origin = Vector2.new(vp.X * 0.5, vp.Y * 0.5)
                            elseif tracerOriginMode == "Mouse" then
                                origin = mPos
                            end
                            local dest = Vector2.new(rx + w * 0.5, ry + h)
                            local delta = dest - origin
                            local len = delta.Magnitude
                            local mid = (origin + dest) * 0.5
                            c.Tracer.Visible = true
                            c.Tracer.BackgroundColor3 = F["ESP_TracerColor"] or boxCol
                            c.Tracer.Position = UDim2.fromOffset(mid.X, mid.Y)
                            c.Tracer.Size = UDim2.fromOffset(len, 1)
                            c.Tracer.Rotation = math.deg(math.atan2(delta.Y, delta.X))
                        else
                            c.Tracer.Visible = false
                        end
                    else
                        c.Root.Visible = false
                        c.Tracer.Visible = false
                        for _, bl in ipairs(c.Bones) do
                            bl.Visible = false
                        end

                        -- Off-Screen Arrow when player is behind or outside viewport
                        if F["ESP_OOF"] == true then
                            local rel = cam.CFrame:PointToObjectSpace(hrp.Position)
                            local ang = math.atan2(-rel.Y, rel.X)
                            local oofRadius = math.clamp(tonumber(F["ESP_OOFRadius"]) or 150, 50, 420)
                            local oofSize = math.clamp(tonumber(F["ESP_OOFSize"]) or 11, 6, 22)
                            local cx = vp.X * 0.5 + math.cos(ang) * oofRadius
                            local cy = vp.Y * 0.5 + math.sin(ang) * oofRadius
                            c.OOFArrow.Visible = true
                            c.OOFArrow.Position = UDim2.fromOffset(cx, cy)
                            c.OOFArrow.Size = UDim2.fromOffset(oofSize, oofSize)
                            c.OOFArrow.Rotation = math.deg(ang) + 45
                            c.OOFArrow.BackgroundColor3 = F["ESP_OOFColor"] or self.Theme.Risky
                            c.OOFArrow.BackgroundTransparency = 0.1 + 0.35 * pulseWave
                            c.OOFTag.Text = string.format("%s [%dft]", plr.Name, math.floor(distStuds + 0.5))
                        else
                            c.OOFArrow.Visible = false
                        end
                    end

                    -- Backtrack Lag-Compensated Ghost Trail
                    if F["ESP_Backtrack"] == true then
                        table.insert(c.History, { Time = nowClock, CF = hrp.CFrame })
                        while #c.History > 1 and (nowClock - c.History[1].Time) > btWindow do
                            table.remove(c.History, 1)
                        end
                        local oldest = c.History[1]
                        if oldest and (hrp.Position - oldest.CF.Position).Magnitude > 0.8 then
                            if not c.GhostPart or not c.GhostPart.Parent then
                                local gp = Instance.new("Part")
                                gp.Name = "ShardsBacktrackGhost"
                                gp.Anchored = true
                                gp.CanCollide = false
                                gp.CanQuery = false
                                gp.CanTouch = false
                                gp.CastShadow = false
                                gp.Material = Enum.Material.Neon
                                gp.Size = Vector3.new(2, 4.6, 1)
                                gp.Parent = beamFolder
                                c.GhostPart = gp
                            end
                            c.GhostPart.Color = F["ESP_BacktrackCol"] or self.Theme.Accent
                            c.GhostPart.Transparency = 0.68
                            c.GhostPart.CFrame = oldest.CF
                        elseif c.GhostPart then
                            c.GhostPart.Transparency = 1
                        end
                    elseif c.GhostPart then
                        c.GhostPart.Transparency = 1
                    end

                    -- Highlight Chams & Glow Stencil (Material-Aware)
                    local hasChams = F["Chams_Vis"] or F["Chams_XQZ"]
                    if hasChams or F["ESP_Glow"] then
                        if not c.Highlight or c.Highlight.Parent ~= char then
                            if c.Highlight then pcall(function() c.Highlight:Destroy() end) end
                            local hl = Instance.new("Highlight")
                            hl.Name = "ShardsChams"
                            hl.Parent = char
                            c.Highlight = hl
                        end
                        c.Highlight.Enabled = true
                        c.Highlight.DepthMode = (F["Chams_XQZ"] or F["ESP_Glow"]) and Enum.HighlightDepthMode.AlwaysOnTop or Enum.HighlightDepthMode.Occluded

                        local baseCol = F["Chams_Vis"] and (F["Chams_VisCol"] or self.Theme.Accent) or (F["Chams_XQZCol"] or self.Theme.Accent)
                        local accentCol2 = F["Chams_VisCol2"] or F["ESP_GlowColor"] or Color3.new(0, 0, 0)
                        local fillTrans = tonumber(F["Chams_FillTrans"]) or 0.42
                        local outTrans = F["ESP_Glow"] and math.clamp((self.Options["ESP_GlowColor"] and self.Options["ESP_GlowColor"].Alpha) or 0.1, 0, 0.95) or 0.35

                        if chamsMat == "Flat" then
                            fillTrans = math.min(fillTrans, 0.18)
                        elseif chamsMat == "Glass Wireframe" then
                            fillTrans = 0.86
                            outTrans = 0.05
                        elseif chamsMat == "Glow Overlay" then
                            baseCol = baseCol:Lerp(accentCol2, pulseWave * 0.45)
                            fillTrans = 0.3 + pulseWave * 0.35
                            outTrans = 0.05
                        elseif chamsMat == "Metallic" then
                            baseCol = baseCol:Lerp(Color3.new(1, 1, 1), 0.18)
                        end

                        c.Highlight.FillColor = baseCol
                        c.Highlight.OutlineColor = F["ESP_Glow"] and (F["ESP_GlowColor"] or accentCol2) or accentCol2
                        c.Highlight.FillTransparency = hasChams and math.clamp(fillTrans, 0, 1) or 1
                        c.Highlight.OutlineTransparency = (F["ESP_Glow"] or chamsMat == "Glass Wireframe" or chamsMat == "Glow Overlay") and outTrans or 1
                    elseif c.Highlight then
                        c.Highlight.Enabled = false
                    end
                else
                    local c = cards[plr]
                    if c then
                        c.Root.Visible = false
                        c.Tracer.Visible = false
                        c.OOFArrow.Visible = false
                        for _, bl in ipairs(c.Bones) do
                            bl.Visible = false
                        end
                        if c.GhostPart then c.GhostPart.Transparency = 1 end
                        if c.Highlight then c.Highlight.Enabled = false end
                    end
                end
            end
        end

        -- 5) Counter Blox Dropped Weapons & Planted C4 Bomb ESP (workspace.Debris / workspace.C4)
        local showDropped = (F["World_DroppedESP"] == true)
        local showBomb = (F["World_BombESP"] == true)
        local seenWorldItems = {}
        if showDropped or showBomb then
            local debrisFolder = workspace:FindFirstChild("Debris")
            local scanContainers = { debrisFolder, workspace }
            for _, container in ipairs(scanContainers) do
                if container then
                    for _, item in ipairs(container:GetChildren()) do
                        local isC4 = (item.Name == "C4" or string.find(string.upper(item.Name), "C4"))
                        local isWeaponDrop = (container == debrisFolder and (item:IsA("Model") or item:IsA("BasePart") or item:IsA("Tool")) and not isC4)
                            or (container == workspace and item:IsA("Tool"))
                        if (isC4 and showBomb) or (isWeaponDrop and showDropped) then
                            local pos
                            if item:IsA("BasePart") then
                                pos = item.Position
                            elseif item:IsA("Model") or item:IsA("Tool") then
                                local okP, p = pcall(function() return item:GetPivot().Position end)
                                if okP then pos = p end
                            end
                            if pos then
                                local sp, onScr = cam:WorldToViewportPoint(pos)
                                if onScr and sp.Z > 0 then
                                    seenWorldItems[item] = true
                                    local wc = GetOrMakeWorldItemCard(item)
                                    local distFt = math.floor((cam.CFrame.Position - pos).Magnitude + 0.5)
                                    if isC4 then
                                        if not c4PlantTime then c4PlantTime = nowClock end
                                        local remSec = math.max(0, 40 - (nowClock - c4PlantTime))
                                        wc.Label.Text = string.format("[C4 BOMB // %.1fs] (%dft)", remSec, distFt)
                                        wc.Label.TextColor3 = self.Theme.Risky
                                    else
                                        wc.Label.Text = string.format("[%s] %dft", string.upper(item.Name), distFt)
                                        wc.Label.TextColor3 = F["ESP_WeaponColor"] or self.Theme.Accent
                                    end
                                    wc.Label.Position = UDim2.fromOffset(sp.X, sp.Y)
                                    wc.Label.Visible = true
                                end
                            end
                        end
                    end
                end
            end
        else
            c4PlantTime = nil
        end
        for obj, wc in pairs(worldItemCards) do
            if not seenWorldItems[obj] or not obj.Parent then
                if not obj.Parent then
                    if wc.Label then wc.Label:Destroy() end
                    worldItemCards[obj] = nil
                else
                    wc.Label.Visible = false
                end
            end
        end
    end)

    -- Bind Camera Overrides (Aimbot,FOV, Thirdperson, Aspect Ratio) at Camera + 1 priority so Roblox/CB camera scripts never overwrite them
    pcall(function()
        RunService:BindToRenderStep("ShardsAspectStretch", Enum.RenderPriority.Camera.Value + 1, function(dt)
            if self.Unloaded then return end
            local F = self.Flags
            local cam = workspace.CurrentCamera
            if not cam then return end
            local mPos = UserInputService:GetMouseLocation()
            local myChar = LocalPlayer and LocalPlayer.Character
            local myHum = myChar and myChar:FindFirstChildOfClass("Humanoid")
            local myHrp = myChar and (myChar:FindFirstChild("HumanoidRootPart") or myChar:FindFirstChild("Torso") or myChar.PrimaryPart)

            -- 1) Camera FOV Override
            if F["World_FOV_Enabled"] then
                if not fovApplied then
                    origCamFov = cam.FieldOfView
                    fovApplied = true
                end
                cam.FieldOfView = math.clamp(tonumber(F["World_FOV"]) or 105, 40, 135)
            elseif fovApplied then
                fovApplied = false
                cam.FieldOfView = origCamFov
            end

            -- 2) Combat Target Acquisition, Aimbot (Rage / Legit), AutoFire & Triggerbot
            local kbRage = self.Options["Rage_Key"]
            local kbLegit = self.Options["Legit_Key"]
            local kbTrig = self.Options["Trig_Key"]
            local kbBaim = self.Options["Rage_BaimKey"]

            local rageActive = (F["Rage_Enabled"] == true) and (not kbRage or kbRage.Mode == "Always" or kbRage.Active)
            local legitActive = (F["Legit_Enable"] == true) and (not kbLegit or kbLegit.Mode == "Always" or kbLegit.Active)
            local trigActive = (F["Trig_Enable"] == true) and (not kbTrig or kbTrig.Mode == "Always" or kbTrig.Active)
            local forceBaim = (F["Rage_ForceBaim"] == true) and (not kbBaim or kbBaim.Mode ~= "Hold" or kbBaim.Active)
                or (kbBaim and kbBaim.Mode == "Hold" and kbBaim.Active)

            if rageActive or legitActive then
                local fovLimitPx = legitActive and ((tonumber(F["Legit_FOV"]) or 4.5) * 12) or ((tonumber(F["Rage_FOV"]) or 180) * 1.6)
                local dzPx = legitActive and ((tonumber(F["Legit_Deadzone"]) or 0) * 8) or 0
                local priority = F["Rage_Priority"] or "FOV Closest"
                local allowWall = rageActive and (F["Rage_Autowall"] == true)
                local hbMap = type(F["Rage_Hitboxes"]) == "table" and F["Rage_Hitboxes"] or nil

                local baseIgnore = { cam, beamFolder }
                if myChar then table.insert(baseIgnore, myChar) end
                local rayIgnore = workspace:FindFirstChild("Ray_Ignore")
                if rayIgnore then table.insert(baseIgnore, rayIgnore) end
                local debrisFolder = workspace:FindFirstChild("Debris")
                if debrisFolder then table.insert(baseIgnore, debrisFolder) end
                local clipsFolder = workspace:FindFirstChild("Clips")
                if clipsFolder then table.insert(baseIgnore, clipsFolder) end
                local mapFolder = workspace:FindFirstChild("Map")
                if mapFolder and mapFolder:FindFirstChild("Clips") then
                    table.insert(baseIgnore, mapFolder.Clips)
                end

                local function ResolveTargetPart(char, tel)
                    if forceBaim then
                        return char:FindFirstChild("LowerTorso") or char:FindFirstChild("Torso") or tel.RootPart
                    end
                    if hbMap and next(hbMap) ~= nil then
                        if hbMap["Head"] and char:FindFirstChild("Head") then return char.Head end
                        if (hbMap["Upper Chest"] or hbMap["Neck"]) and (char:FindFirstChild("UpperTorso") or char:FindFirstChild("Torso")) then
                            return char:FindFirstChild("UpperTorso") or char:FindFirstChild("Torso")
                        end
                        if hbMap["Pelvis"] and (char:FindFirstChild("LowerTorso") or tel.RootPart) then
                            return char:FindFirstChild("LowerTorso") or tel.RootPart
                        end
                        if hbMap["Arms"] and (char:FindFirstChild("RightUpperArm") or char:FindFirstChild("Right Arm")) then
                            return char:FindFirstChild("RightUpperArm") or char:FindFirstChild("Right Arm")
                        end
                        if (hbMap["Legs"] or hbMap["Feet"]) and (char:FindFirstChild("RightUpperLeg") or char:FindFirstChild("Right Leg")) then
                            return char:FindFirstChild("RightUpperLeg") or char:FindFirstChild("Right Leg")
                        end
                    end
                    return tel.Head or tel.RootPart
                end

                local bestAimPart = nil
                local bestPlr = nil
                local bestScore = math.huge
                for _, plr in ipairs(Players:GetPlayers()) do
                    if plr ~= LocalPlayer then
                        local isWhitelisted = (F["Misc_Whitelist"] == true) and (F["Misc_PlayerTarget"] == plr.Name)
                        local tel = self:GetPlayerTelemetry(plr)
                        if not isWhitelisted and tel and tel.Alive and (not F["ESP_TeamCheck"] or not tel.IsTeammate) then
                            local part = ResolveTargetPart(tel.Character, tel)
                            if part then
                                local sp, onScr = cam:WorldToViewportPoint(part.Position)
                                if (onScr or (rageActive and (tonumber(F["Rage_FOV"]) or 180) >= 150)) and sp.Z > 0 then
                                    local fovDist = (Vector2.new(sp.X, sp.Y) - mPos).Magnitude
                                    if fovDist <= fovLimitPx and fovDist >= dzPx then
                                        local visible = allowWall
                                        if not visible then
                                            local dir = part.Position - cam.CFrame.Position
                                            local hit = RaycastPierceClips(cam.CFrame.Position, dir, baseIgnore)
                                            visible = (not hit) or hit.Instance:IsDescendantOf(tel.Character)
                                        end
                                        if visible then
                                            local score = fovDist
                                            if rageActive then
                                                if priority == "Lowest HP" then
                                                    score = tel.HP
                                                elseif priority == "Shortest Distance" then
                                                    score = (part.Position - cam.CFrame.Position).Magnitude
                                                elseif priority == "Highest Threat" then
                                                    score = (part.Position - cam.CFrame.Position).Magnitude * 0.5 + tel.HP
                                                end
                                            end
                                            if score < bestScore then
                                                bestScore = score
                                                bestAimPart = part
                                                bestPlr = plr
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    end
                end

                if bestAimPart then
                    local aimPos = bestAimPart.Position
                    if legitActive and F["Legit_RCS"] then
                        aimPos = aimPos - Vector3.new(0, 0.18, 0)
                    end
                    local targetCF = CFrame.lookAt(cam.CFrame.Position, aimPos)
                    if rageActive then
                        if F["Rage_QuickStop"] and myHrp and myHum and myHum.FloorMaterial ~= Enum.Material.Air then
                            local v = myHrp.AssemblyLinearVelocity
                            myHrp.AssemblyLinearVelocity = Vector3.new(v.X * 0.12, v.Y, v.Z * 0.12)
                        end
                        cam.CFrame = (F["Rage_Silent"] == true) and targetCF or cam.CFrame:Lerp(targetCF, 0.92)
                        local fireCooldown = (F["Rage_DT"] == true) and 0.06 or 0.14
                        if F["Rage_AutoFire"] and (os.clock() - lastRageFire) >= fireCooldown then
                            lastRageFire = os.clock()
                            FireWeapon(myChar, mPos)
                            SpawnTracer(bestPlr and bestPlr.Name or aimPos)
                        end
                    else
                        local smooth = math.max(1, tonumber(F["Legit_Smooth"]) or 6.5)
                        local alpha = math.clamp((1 / smooth) * (dt * 60), 0.04, 1)
                        cam.CFrame = cam.CFrame:Lerp(targetCF, alpha)
                    end
                end
            end

            if trigActive then
                local baseIgnore = { cam, beamFolder }
                if myChar then table.insert(baseIgnore, myChar) end
                local rayIgnore = workspace:FindFirstChild("Ray_Ignore")
                if rayIgnore then table.insert(baseIgnore, rayIgnore) end
                local debrisFolder = workspace:FindFirstChild("Debris")
                if debrisFolder then table.insert(baseIgnore, debrisFolder) end
                local clipsFolder = workspace:FindFirstChild("Clips")
                if clipsFolder then table.insert(baseIgnore, clipsFolder) end
                local mapFolder = workspace:FindFirstChild("Map")
                if mapFolder and mapFolder:FindFirstChild("Clips") then
                    table.insert(baseIgnore, mapFolder.Clips)
                end

                local unitRay = cam:ViewportPointToRay(mPos.X, mPos.Y)
                local hit = RaycastPierceClips(unitRay.Origin, unitRay.Direction * 1000, baseIgnore)
                    or RaycastPierceClips(cam.CFrame.Position, cam.CFrame.LookVector * 1000, baseIgnore)
                local hitEnemyPlr = nil

                if hit and hit.Instance then
                    for _, plr in ipairs(Players:GetPlayers()) do
                        if plr ~= LocalPlayer and plr.Character and hit.Instance:IsDescendantOf(plr.Character) then
                            local isWhitelisted = (F["Misc_Whitelist"] == true) and (F["Misc_PlayerTarget"] == plr.Name)
                            local tel = self:GetPlayerTelemetry(plr)
                            if not isWhitelisted and tel and tel.Alive and (not F["ESP_TeamCheck"] or not tel.IsTeammate) then
                                hitEnemyPlr = plr
                                break
                            end
                        end
                    end
                end

                -- Fallback: check Mouse.Target in case a custom hitbox wrapper was hit
                if not hitEnemyPlr and LocalPlayer then
                    pcall(function()
                        local mTarget = LocalPlayer:GetMouse().Target
                        if mTarget then
                            for _, plr in ipairs(Players:GetPlayers()) do
                                if plr ~= LocalPlayer and plr.Character and mTarget:IsDescendantOf(plr.Character) then
                                    local isWhitelisted = (F["Misc_Whitelist"] == true) and (F["Misc_PlayerTarget"] == plr.Name)
                                    local tel = self:GetPlayerTelemetry(plr)
                                    if not isWhitelisted and tel and tel.Alive and (not F["ESP_TeamCheck"] or not tel.IsTeammate) then
                                        hitEnemyPlr = plr
                                        break
                                    end
                                end
                            end
                        end
                    end)
                end

                if hitEnemyPlr then
                    local now = os.clock()
                    if not trigAcquireTime then
                        trigAcquireTime = now
                    end
                    local delaySec = math.max(0, (tonumber(F["Trig_Delay"]) or 0) / 1000)
                    local fireInterval = (F["Rage_DT"] == true) and 0.05 or 0.08
                    if (now - trigAcquireTime) >= delaySec and (now - lastTrigFire) >= fireInterval then
                        lastTrigFire = now
                        FireWeapon(myChar, mPos)
                        SpawnTracer(hitEnemyPlr.Name)
                    end
                else
                    trigAcquireTime = nil
                end
            else
                trigAcquireTime = nil
            end

            -- 3) Thirdperson Camera Offset (works in both Classic and Counter Blox LockFirstPerson/Scriptable cameras)
            local kbTp = self.Options["World_TPKey"]
            local tpActive = (F["World_TP"] == true) and (not kbTp or kbTp.Mode ~= "Hold" or kbTp.Active)
            if tpActive then
                tpApplied = true
                local tpStuds = math.clamp((tonumber(F["World_TPDist"]) or 115) * 0.12, 4, 35)
                cam.CFrame = cam.CFrame * CFrame.new(0, 1.2, tpStuds)
                if myChar then
                    for _, p in ipairs(myChar:GetDescendants()) do
                        if p:IsA("BasePart") and p.Name ~= "HumanoidRootPart" and p.LocalTransparencyModifier > 0.5 then
                            tpTransParts[p] = true
                            p.LocalTransparencyModifier = 0
                        end
                    end
                end
            elseif tpApplied then
                tpApplied = false
                for p in pairs(tpTransParts) do
                    if p and p.Parent then
                        pcall(function() p.LocalTransparencyModifier = 0 end)
                    end
                end
                table.clear(tpTransParts)
            end

            -- 4) Normalized 4:3 Aspect Ratio Stretch (never compounds across frames)
            if F["World_Aspect_Enabled"] then
                local ratio = math.clamp(tonumber(F["World_Aspect"]) or 0.78, 0.45, 1.5)
                local cf = cam.CFrame
                local right = cf.RightVector.Unit * ratio
                local up = cf.UpVector.Unit
                local back = -cf.LookVector.Unit
                cam.CFrame = CFrame.fromMatrix(cf.Position, right, up, back)
            end
        end)
    end)

    self.WorldController = {
        Folder = overlayFolder,
        SpawnTracer = SpawnTracer,
        Cleanup = function()
            pcall(function() RunService:UnbindFromRenderStep("ShardsAspectStretch") end)
            for plr in pairs(playerConns) do
                DisconnectPlayerConns(plr)
            end
            for _, c in pairs(cards) do
                if c.Highlight then
                    pcall(function() c.Highlight:Destroy() end)
                end
                if c.GhostPart then
                    pcall(function() c.GhostPart:Destroy() end)
                end
                for _, bl in ipairs(c.Bones or {}) do
                    pcall(function() bl:Destroy() end)
                end
            end
            table.clear(cards)
            for _, wc in pairs(worldItemCards) do
                if wc.Label then pcall(function() wc.Label:Destroy() end) end
            end
            table.clear(worldItemCards)
            if beamFolder and beamFolder.Parent then
                pcall(function() beamFolder:Destroy() end)
            end
            if ccEffect and ccEffect.Parent then
                pcall(function() ccEffect:Destroy() end)
            end
            for eff in pairs(disabledPostEffects) do
                if eff and eff.Parent then
                    pcall(function() eff.Enabled = true end)
                end
            end
            table.clear(disabledPostEffects)
            for inst, prev in pairs(suppressedSmokes) do
                if inst and inst.Parent then
                    pcall(function()
                        if inst:IsA("ParticleEmitter") then
                            inst.Enabled = true
                        elseif inst:IsA("BasePart") and type(prev) == "number" then
                            inst.Transparency = prev
                        end
                    end)
                end
            end
            table.clear(suppressedSmokes)
            for gObj in pairs(suppressedGuiOverlays) do
                if gObj and gObj.Parent then
                    pcall(function() gObj.Visible = true end)
                end
            end
            table.clear(suppressedGuiOverlays)
            if aaApplied then
                aaApplied = false
                local myChar = LocalPlayer and LocalPlayer.Character
                local myHum = myChar and myChar:FindFirstChildOfClass("Humanoid")
                if myHum then myHum.AutoRotate = true end
            end
            if nightApplied or fullbrightApplied then
                nightApplied = false
                fullbrightApplied = false
                lighting.Ambient = origAmbient
                lighting.OutdoorAmbient = origOutdoor
                lighting.ExposureCompensation = origExposure
                lighting.Brightness = origBrightness
                lighting.GlobalShadows = origGlobalShadows
            end
            if clockApplied then
                clockApplied = false
                lighting.ClockTime = origClockTime
            end
            if fogApplied then
                fogApplied = false
                lighting.FogStart = origFogStart
                lighting.FogEnd = origFogEnd
                lighting.FogColor = origFogColor
            end
            if fovApplied and workspace.CurrentCamera then
                fovApplied = false
                workspace.CurrentCamera.FieldOfView = origCamFov
            end
            if tpApplied and LocalPlayer then
                tpApplied = false
                LocalPlayer.CameraMinZoomDistance = origMinZoom
                LocalPlayer.CameraMaxZoomDistance = origMaxZoom
            end
            if noclipApplied then
                noclipApplied = false
                for part in pairs(noclipParts) do
                    if part and part.Parent then
                        pcall(function() part.CanCollide = true end)
                    end
                end
                table.clear(noclipParts)
            end
            local myChar = LocalPlayer and LocalPlayer.Character
            local myHum = myChar and myChar:FindFirstChildOfClass("Humanoid")
            if myHum then
                if speedApplied then myHum.WalkSpeed = 16 end
                if jumpApplied then myHum.JumpPower = 50 end
            end
        end,
    }
    return self.WorldController
end

-- ============================================================================
-- INLINE ATTACHMENTS (Keybind Binder & HSV/Alpha/Rainbow CS:GO ColorPicker)
-- ============================================================================
function Library:CreateKeybindAttachment(parentRow, cfg, toggleLink)
    cfg = cfg or {}
    self.KeybindCounter += 1
    local kbOrder = self.KeybindCounter
    local rawFlag = cfg.Flag
        or (toggleLink and toggleLink.Name and ("KB_" .. toggleLink.Name .. "_" .. tostring(kbOrder)))
        or (cfg.Name and ("KB_" .. cfg.Name .. "_" .. tostring(kbOrder)))
        or ("KB_" .. tostring(kbOrder))
    local flag = rawFlag
    if self.ActiveKeybinds[flag] ~= nil then
        flag = rawFlag .. "_" .. tostring(kbOrder)
    end
    if not table.find(self.KeybindOrder, flag) then
        table.insert(self.KeybindOrder, flag)
    end
    local mode = cfg.Mode or "Toggle" -- "Always", "Hold", "Toggle"
    local currentKey = cfg.Default or Enum.KeyCode.Unknown
    local binding = false
    local active = (mode == "Always") or (toggleLink ~= nil and toggleLink.Value == true and mode == "Toggle")

    local Keybind = {
        Type = "Keybind",
        Name = cfg.Name or (toggleLink and toggleLink.Name) or ("Keybind " .. tostring(kbOrder)),
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
    local btnStroke = self:ApplyCSBorder(btn, "InnerBorder")

    self:Connect(btn.MouseEnter, function()
        if not binding then
            Library:Tween(btnStroke, TweenFast, { Color = self.Theme.HighlightBorder })
            Library:Tween(btn, TweenFast, {
                BackgroundColor3 = self.Theme.ElementHover,
                TextColor3 = self.Theme.Text,
            })
        end
    end)
    self:Connect(btn.MouseLeave, function()
        if not binding then
            Library:Tween(btnStroke, TweenFast, { Color = self.Theme.InnerBorder })
            Library:Tween(btn, TweenFast, {
                BackgroundColor3 = self.Theme.Element,
                TextColor3 = self.Theme.TextDim,
            })
        end
    end)

    local function UpdateButtonSize()
        local txt = binding and "[...]" or ("[" .. self:FormatKey(currentKey) .. "]")
        btn.Text = txt
        local b = self:GetTextBounds(txt, 11, Vector2.new(200, 20))
        local targetW = math.max(36, b.X + 10)
        btn:SetAttribute("TargetWidth", targetW)
        Library:Tween(btn, TweenFast, {
            Size = UDim2.fromOffset(targetW, 15),
        })
        Library:RefreshAttachmentTray(parentRow)
    end
    UpdateButtonSize()
    table.insert(self.LayoutRefreshers, UpdateButtonSize)

    -- Mode Popup (Hold / Toggle / Always)
    local modePopup = self:Create("Frame", {
        BackgroundColor3 = self.Theme.Background,
        ClipsDescendants = true,
        Size = UDim2.fromOffset(78, 62),
        Visible = false,
        ZIndex = 350,
        Parent = ScreenGui,
    })
    self:BindTheme(modePopup, "BackgroundColor3", "Background")
    self:ApplyCSBorder(modePopup, "HighlightBorder")

    local modePopupOpen = false
    local modeToken = 0

    local modeList = self:Create("Frame", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(1, 1),
        Size = UDim2.new(1, -2, 1, -2),
        ZIndex = 351,
        Parent = modePopup,
    })

    self:Create("UIListLayout", {
        SortOrder = Enum.SortOrder.LayoutOrder,
        Parent = modeList,
    })

    local popupCtrl = {
        Frames = { modePopup, btn },
        IsOpen = function()
            return modePopupOpen and modePopup.Visible
        end,
        Close = function()
            if not modePopupOpen and not modePopup.Visible then return end
            modePopupOpen = false
            modeToken += 1
            local tok = modeToken
            Library:Tween(modePopup, TweenFast, { Size = UDim2.fromOffset(78, 0) })
            task.delay(0.14, function()
                if modeToken == tok and not modePopupOpen then
                    modePopup.Visible = false
                end
            end)
        end
    }
    table.insert(self.OpenPopups, popupCtrl)

    local modes = { "Hold", "Toggle", "Always" }
    local modeButtons = {}
    for _, m in ipairs(modes) do
        local mBtn = self:Create("TextButton", {
            BackgroundColor3 = self.Theme.ElementHover,
            BackgroundTransparency = 1,
            AutoButtonColor = false,
            Size = UDim2.new(1, 0, 0, 20),
            Font = Enum.Font.Code,
            Text = "  " .. m,
            TextColor3 = (m == mode) and self.Theme.Accent or self.Theme.TextDim,
            TextSize = 12,
            TextXAlignment = Enum.TextXAlignment.Left,
            ZIndex = 352,
            Parent = modeList,
        })
        modeButtons[m] = mBtn
        self:Connect(mBtn.MouseEnter, function()
            Library:Tween(mBtn, TweenFast, { BackgroundTransparency = 0.35 })
            if m ~= mode then
                Library:Tween(mBtn, TweenFast, { TextColor3 = self.Theme.Text })
            end
        end)
        self:Connect(mBtn.MouseLeave, function()
            Library:Tween(mBtn, TweenFast, { BackgroundTransparency = 1 })
            if m ~= mode then
                Library:Tween(mBtn, TweenFast, { TextColor3 = self.Theme.TextDim })
            end
        end)
        self:Connect(mBtn.InputBegan, function(input)
            if input.UserInputType == Enum.UserInputType.MouseButton1 then
                Keybind:SetMode(m)
                popupCtrl.Close()
            end
        end)
    end

    local function SyncKeybindList()
        local isShown = false
        if currentKey and currentKey ~= Enum.KeyCode.Unknown then
            if mode == "Always" then
                isShown = (toggleLink == nil) or (toggleLink.Value == true)
            elseif mode == "Hold" then
                isShown = (toggleLink == nil or toggleLink.Value == true) and (Keybind.Active == true)
            elseif toggleLink then
                isShown = (toggleLink.Value == true)
            else
                isShown = (Keybind.Active == true)
            end
        end
        self.ActiveKeybinds[flag] = {
            Name = Keybind.Name,
            Key = currentKey,
            Mode = mode,
            Order = kbOrder,
            Active = isShown,
        }
        Library:RefreshAttachmentTray(parentRow)
        if self.KeybindList then
            self.KeybindList:Refresh()
        end
        if self.Indicators then
            self.Indicators:Refresh()
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
        elseif mode == "Hold" then
            Keybind.Active = false
        elseif mode == "Toggle" and toggleLink then
            Keybind.Active = (toggleLink.Value == true)
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

    local function UpdateModePopupPos()
        local abs = btn.AbsolutePosition
        local insetY = self:GetGuiInsetY()
        local vp = workspace.CurrentCamera and workspace.CurrentCamera.ViewportSize or Vector2.new(1920, 1080)
        local px = math.clamp(abs.X, 6, math.max(6, vp.X - 84))
        local py = math.clamp(abs.Y + insetY + 18, 6, math.max(6, vp.Y - 66))
        modePopup.Position = UDim2.fromOffset(px, py)
    end

    self:Connect(btn.InputBegan, function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            Library:ClosePopups(popupCtrl)
            popupCtrl.Close()
            binding = true
            Library:Tween(btnStroke, TweenFast, { Color = self.Theme.Accent })
            Library:Tween(btn, TweenFast, { TextColor3 = self.Theme.Accent })
            UpdateButtonSize()
        elseif input.UserInputType == Enum.UserInputType.MouseButton2 then
            binding = false
            Library:Tween(btnStroke, TweenFast, { Color = self.Theme.InnerBorder })
            Library:Tween(btn, TweenFast, { TextColor3 = self.Theme.TextDim })
            UpdateButtonSize()
            local willShow = not modePopupOpen
            Library:ClosePopups(popupCtrl)
            if willShow then
                modePopupOpen = true
                modeToken += 1
                UpdateModePopupPos()
                modePopup.Size = UDim2.fromOffset(78, 0)
                modePopup.Visible = true
                Library:Tween(modePopup, TweenFast, { Size = UDim2.fromOffset(78, 62) })
            else
                popupCtrl.Close()
            end
        end
    end)

    self:Connect(btn:GetPropertyChangedSignal("AbsolutePosition"), function()
        if modePopup.Visible then
            UpdateModePopupPos()
        end
    end)

    self:Connect(UserInputService.InputBegan, function(input, gpe)
        if binding then
            if input.UserInputType == Enum.UserInputType.Keyboard then
                if input.KeyCode == Enum.KeyCode.Unknown then return end
                binding = false
                self._justBoundTick = os.clock()
                Library:Tween(btnStroke, TweenFast, { Color = self.Theme.InnerBorder })
                Library:Tween(btn, TweenFast, { TextColor3 = self.Theme.TextDim })
                if input.KeyCode == Enum.KeyCode.Escape or input.KeyCode == Enum.KeyCode.Backspace then
                    Keybind:Set(Enum.KeyCode.Unknown)
                else
                    Keybind:Set(input.KeyCode)
                end
            elseif input.UserInputType == Enum.UserInputType.MouseButton2
                or input.UserInputType == Enum.UserInputType.MouseButton3 then
                binding = false
                self._justBoundTick = os.clock()
                Library:Tween(btnStroke, TweenFast, { Color = self.Theme.InnerBorder })
                Library:Tween(btn, TweenFast, { TextColor3 = self.Theme.TextDim })
                Keybind:Set(input.UserInputType)
            end
            return
        end

        if currentKey == Enum.KeyCode.Unknown then return end
        if UserInputService:GetFocusedTextBox() ~= nil then return end
        if gpe and input.UserInputType == Enum.UserInputType.Keyboard then return end
        if self.Open and self.Flags["UI_MenuPauseBinds"] == true and flag ~= "UI_MenuKeybind" then return end
        local matched = (input.KeyCode == currentKey) or (input.UserInputType == currentKey)
        if matched then
            if mode == "Toggle" then
                Keybind.Active = not Keybind.Active
                if toggleLink then
                    toggleLink:Set(not toggleLink.Value)
                end
            elseif mode == "Hold" then
                Keybind.Active = true
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
            if cfg.Callback then
                task.spawn(cfg.Callback, false, currentKey)
            end
            SyncKeybindList()
        end
    end)

    if toggleLink then
        toggleLink.Changed:Connect(function(newState)
            if mode == "Toggle" then
                Keybind.Active = (newState == true)
            end
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
    local swatchStroke = self:ApplyCSBorder(swatch, "InnerBorder")

    self:Connect(swatch.MouseEnter, function()
        Library:Tween(swatchStroke, TweenFast, { Color = self.Theme.Accent })
    end)
    self:Connect(swatch.MouseLeave, function()
        Library:Tween(swatchStroke, TweenFast, { Color = self.Theme.InnerBorder })
    end)

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
        ClipsDescendants = true,
        Size = UDim2.fromOffset(215, 238),
        Visible = false,
        ZIndex = 360,
        Parent = ScreenGui,
    })
    self:BindTheme(popup, "BackgroundColor3", "Background")
    self:ApplyCSBorder(popup, "HighlightBorder")
    self:CreateAccentBar(popup, 362)

    local popupOpen = false
    local popupToken = 0

    local popupCtrl = {
        Frames = { popup, swatch },
        IsOpen = function()
            return popupOpen and popup.Visible
        end,
        Close = function()
            if not popupOpen and not popup.Visible then return end
            popupOpen = false
            popupToken += 1
            local tok = popupToken
            Library:Tween(popup, TweenFast, { Size = UDim2.fromOffset(215, 0) })
            task.delay(0.14, function()
                if popupToken == tok and not popupOpen then
                    popup.Visible = false
                end
            end)
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

    -- Alpha Slider Bar (Left = Transparent/Dark, Right = Opaque/Full Color so Alpha=0 starts on the right)
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
            ColorSequenceKeypoint.new(0, Color3.fromRGB(18, 18, 22)),
            ColorSequenceKeypoint.new(1, Color3.new(1, 1, 1)),
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

    -- Copy & Paste Color Buttons inside Popup
    local copyBtn = self:Create("TextButton", {
        BackgroundColor3 = self.Theme.Element,
        AutoButtonColor = false,
        Position = UDim2.fromOffset(8, 208),
        Size = UDim2.fromOffset(96, 20),
        Font = Enum.Font.Code,
        Text = "COPY COLOR",
        TextColor3 = self.Theme.TextDim,
        TextSize = 11,
        ZIndex = 362,
        Parent = popup,
    })
    self:BindTheme(copyBtn, "BackgroundColor3", "Element")
    local copyStroke = self:ApplyCSBorder(copyBtn, "InnerBorder")

    local pasteBtn = self:Create("TextButton", {
        BackgroundColor3 = self.Theme.Element,
        AutoButtonColor = false,
        Position = UDim2.fromOffset(110, 208),
        Size = UDim2.fromOffset(96, 20),
        Font = Enum.Font.Code,
        Text = "PASTE COLOR",
        TextColor3 = self.Theme.TextDim,
        TextSize = 11,
        ZIndex = 362,
        Parent = popup,
    })
    self:BindTheme(pasteBtn, "BackgroundColor3", "Element")
    local pasteStroke = self:ApplyCSBorder(pasteBtn, "InnerBorder")

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
        local cInfo = TweenInfo.new(0.06, Enum.EasingStyle.Quad, Enum.EasingDirection.Out)
        Library:Tween(svCursor, cInfo, { Position = UDim2.fromScale(s, 1 - v) })
        Library:Tween(hueCursor, cInfo, { Position = UDim2.new(0.5, 0, h, 0) })
        Library:Tween(alphaCursor, cInfo, { Position = UDim2.new(1 - alpha, 0, 0.5, 0) })
        hexInput.Text = ToHex(col)
        Library.Flags[flag] = col
        if Library.ESPPreview then
            Library.ESPPreview:Refresh()
        end
        if cfg.Callback then
            task.spawn(cfg.Callback, col, alpha)
        end
        ColorPicker.Changed:Fire(col, alpha)
    end

    function ColorPicker:Set(col, newAlpha, newRainbow)
        if typeof(col) == "Color3" then
            h, s, v = Color3.toHSV(col)
        end
        if type(newAlpha) == "number" then
            alpha = math.clamp(newAlpha, 0, 1)
        end
        if type(newRainbow) == "boolean" then
            rainbow = newRainbow
            ColorPicker.Rainbow = rainbow
            rbBtn.TextColor3 = rainbow and Library.Theme.Accent or Library.Theme.TextDim
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
            if rainbow then
                rainbow = false
                ColorPicker.Rainbow = false
                rbBtn.TextColor3 = self.Theme.TextDim
            end
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
                ColorPicker:Set(Color3.fromRGB(r, g, b), alpha, false)
            end
        end
    end)

    self:Connect(rbBtn.MouseButton1Click, function()
        rainbow = not rainbow
        ColorPicker.Rainbow = rainbow
        rbBtn.TextColor3 = rainbow and self.Theme.Accent or self.Theme.TextDim
    end)

    self:Connect(copyBtn.MouseEnter, function()
        Library:Tween(copyStroke, TweenFast, { Color = self.Theme.Accent })
        Library:Tween(copyBtn, TweenFast, { TextColor3 = self.Theme.Text })
    end)
    self:Connect(copyBtn.MouseLeave, function()
        Library:Tween(copyStroke, TweenFast, { Color = self.Theme.InnerBorder })
        Library:Tween(copyBtn, TweenFast, { TextColor3 = self.Theme.TextDim })
    end)
    self:Connect(copyBtn.MouseButton1Click, function()
        Library.CopiedColor = ColorPicker.Value
        Library.CopiedAlpha = ColorPicker.Alpha
        pcall(function()
            if setclipboard then
                setclipboard(ToHex(ColorPicker.Value))
            end
        end)
        copyBtn.Text = "COPIED!"
        copyBtn.TextColor3 = self.Theme.Accent
        task.delay(0.75, function()
            if copyBtn and copyBtn.Parent then
                copyBtn.Text = "COPY COLOR"
                copyBtn.TextColor3 = self.Theme.TextDim
            end
        end)
    end)

    self:Connect(pasteBtn.MouseEnter, function()
        Library:Tween(pasteStroke, TweenFast, { Color = self.Theme.Accent })
        Library:Tween(pasteBtn, TweenFast, { TextColor3 = self.Theme.Text })
    end)
    self:Connect(pasteBtn.MouseLeave, function()
        Library:Tween(pasteStroke, TweenFast, { Color = self.Theme.InnerBorder })
        Library:Tween(pasteBtn, TweenFast, { TextColor3 = self.Theme.TextDim })
    end)
    self:Connect(pasteBtn.MouseButton1Click, function()
        if Library.CopiedColor then
            rainbow = false
            ColorPicker.Rainbow = false
            rbBtn.TextColor3 = self.Theme.TextDim
            ColorPicker:Set(Library.CopiedColor, Library.CopiedAlpha or 0)
            pasteBtn.Text = "PASTED!"
            pasteBtn.TextColor3 = self.Theme.Accent
            task.delay(0.75, function()
                if pasteBtn and pasteBtn.Parent then
                    pasteBtn.Text = "PASTE COLOR"
                    pasteBtn.TextColor3 = self.Theme.TextDim
                end
            end)
        end
    end)

    self:Connect(RunService.RenderStepped, function()
        if rainbow and not self.Unloaded then
            h = (os.clock() * 0.22) % 1
            ApplyColor()
        end
    end)

    local function CalcPopupCoords()
        local abs = swatch.AbsolutePosition
        local insetY = self:GetGuiInsetY()
        local vp = workspace.CurrentCamera and workspace.CurrentCamera.ViewportSize or Vector2.new(1920, 1080)
        local px = math.clamp(abs.X - 185, 6, math.max(6, vp.X - 225))
        local py = math.clamp(abs.Y + insetY + 18, 6, math.max(6, vp.Y - 246))
        return px, py
    end

    self:Connect(swatch:GetPropertyChangedSignal("AbsolutePosition"), function()
        if popup.Visible then
            local px, py = CalcPopupCoords()
            popup.Position = UDim2.fromOffset(px, py)
        end
    end)

    self:Connect(swatch.InputBegan, function(input)
        if input.UserInputType ~= Enum.UserInputType.MouseButton1 then return end
        local willShow = not popupOpen
        Library:ClosePopups(popupCtrl)
        if willShow then
            popupOpen = true
            popupToken += 1
            local px, py = CalcPopupCoords()
            popup.Position = UDim2.fromOffset(px, py - 4)
            popup.Size = UDim2.fromOffset(215, 0)
            popup.Visible = true
            Library:Tween(popup, TweenFast, {
                Position = UDim2.fromOffset(px, py),
                Size = UDim2.fromOffset(215, 238),
            })
        else
            popupCtrl.Close()
        end
    end)

    ApplyColor()
    Library:RefreshAttachmentTray(parentRow)
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

    -- Clickable area covering box + label (expands to full width when no attachments exist)
    local clickBtn = Library:Create("TextButton", {
        BackgroundTransparency = 1,
        Size = UDim2.new(1, -4, 1, 0),
        Text = "",
        ZIndex = 11,
        Parent = row,
    })

    -- Outer Glow around the square box when lit up
    local boxGlow = Library:Create("ImageLabel", {
        AnchorPoint = Vector2.new(0, 0),
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(-4, -2),
        Size = UDim2.fromOffset(24, 24),
        Image = "rbxassetid://5028857084",
        ImageColor3 = risky and Library.Theme.Risky or Library.Theme.Accent,
        ImageTransparency = state and 0.55 or 1,
        ZIndex = 9,
        Parent = row,
    })

    -- The 14x14 CS:GO Square Box (top-left anchored at integer offsets to prevent fractional column subpixel rounding)
    local boxBase = Library:Create("Frame", {
        AnchorPoint = Vector2.new(0, 0),
        BackgroundColor3 = Library.Theme.Element,
        Position = UDim2.fromOffset(1, 3),
        Size = UDim2.fromOffset(14, 14),
        ZIndex = 10,
        Parent = row,
    })
    Library:BindTheme(boxBase, "BackgroundColor3", "Element")
    local boxStroke = Library:ApplyCSBorder(boxBase, "InnerBorder")

    -- Lit-up interior fill (locked to 100% of boxBase with 0,0 anchor so it never shifts off-center)
    local boxFill = Library:Create("Frame", {
        AnchorPoint = Vector2.new(0, 0),
        BackgroundColor3 = Color3.new(1, 1, 1),
        BorderSizePixel = 0,
        Position = UDim2.fromOffset(0, 0),
        Size = UDim2.fromScale(1, 1),
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
        Size = UDim2.fromOffset(10, 10),
        Image = Library:GetIcon("check"),
        ImageColor3 = Color3.fromRGB(10, 10, 12),
        ImageTransparency = (state and cfg.Checkmark) and 0.15 or 1,
        ZIndex = 12,
        Parent = boxFill,
    })

    -- Label next to square
    local label = Library:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(22, 0),
        Size = UDim2.new(1, -26, 1, 0),
        Font = Enum.Font.Code,
        Text = name,
        TextColor3 = state and Library.Theme.Text or (risky and Library.Theme.Risky or Library.Theme.TextDim),
        TextSize = 12,
        TextXAlignment = Enum.TextXAlignment.Left,
        TextYAlignment = Enum.TextYAlignment.Center,
        ZIndex = 10,
        Parent = row,
    })
    Library:RegisterSearchable(row, label, name)

    -- Right-side attachment tray (for Keybinds & ColorPickers)
    local attachTray = Library:Create("Frame", {
        Name = "AttachTray",
        BackgroundTransparency = 1,
        AnchorPoint = Vector2.new(1, 0.5),
        Position = UDim2.new(1, 0, 0.5, 0),
        Size = UDim2.fromOffset(0, 16),
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
        if Toggle.SubSection then
            Toggle.SubSection:UpdateState(state)
        end
        if Library.ESPPreview then
            Library.ESPPreview:Refresh()
        end
        if Library.Indicators then
            Library.Indicators:Refresh()
        end
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

    local parentGroupbox = self
    function Toggle:AddSubSection()
        if Toggle.SubSection then
            return Toggle.SubSection
        end
        local depWrapper = Library:Create("Frame", {
            Name = "Dep_" .. name,
            BackgroundTransparency = 1,
            ClipsDescendants = true,
            Size = UDim2.new(1, 0, 0, 0),
            Visible = state,
            ZIndex = 10,
            Parent = parentGroupbox.Container,
        })
        local depBar = Library:Create("Frame", {
            BackgroundColor3 = Library.Theme.Accent,
            BorderSizePixel = 0,
            Position = UDim2.fromOffset(5, 2),
            Size = UDim2.new(0, 1, 1, -4),
            BackgroundTransparency = 0.35,
            ZIndex = 11,
            Parent = depWrapper,
        })
        Library:BindTheme(depBar, "BackgroundColor3", "Accent")

        local depContainer = Library:Create("Frame", {
            Name = "DepContainer",
            BackgroundTransparency = 1,
            Position = UDim2.fromOffset(14, 0),
            Size = UDim2.new(1, -14, 0, 0),
            ZIndex = 11,
            Parent = depWrapper,
        })
        local depLayout = Library:Create("UIListLayout", {
            Padding = UDim.new(0, 6),
            SortOrder = Enum.SortOrder.LayoutOrder,
            Parent = depContainer,
        })
        local subToken = 0
        local function SyncSubHeight(animate)
            local h = depLayout.AbsoluteContentSize.Y
            depContainer.Size = UDim2.new(1, -14, 0, h)
            subToken += 1
            local tok = subToken
            if state then
                depWrapper.Visible = true
                if animate then
                    depWrapper.ClipsDescendants = true
                    Library:Tween(depWrapper, TweenFast, { Size = UDim2.new(1, 0, 0, h) })
                    task.delay(0.15, function()
                        if subToken == tok and state then
                            depWrapper.ClipsDescendants = false
                        end
                    end)
                else
                    depWrapper.Size = UDim2.new(1, 0, 0, h)
                    depWrapper.ClipsDescendants = false
                end
            else
                depWrapper.ClipsDescendants = true
                if animate then
                    Library:Tween(depWrapper, TweenFast, { Size = UDim2.new(1, 0, 0, 0) })
                    task.delay(0.14, function()
                        if subToken == tok and not state then
                            depWrapper.Visible = false
                        end
                    end)
                else
                    depWrapper.Size = UDim2.new(1, 0, 0, 0)
                    depWrapper.Visible = false
                end
            end
        end
        Library:Connect(depLayout:GetPropertyChangedSignal("AbsoluteContentSize"), function()
            SyncSubHeight(false)
        end)

        local subObj = setmetatable({
            Wrapper = depWrapper,
            Container = depContainer,
            UpdateState = function(_, newState)
                state = newState
                SyncSubHeight(true)
            end,
        }, GroupboxMethods)
        Toggle.SubSection = subObj
        SyncSubHeight(false)
        return subObj
    end
    Toggle.AddDepbox = Toggle.AddSubSection
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
    Library:RegisterSearchable(holder, topLabel, name)

    local valLabel = Library:Create("TextBox", {
        BackgroundTransparency = 1,
        Position = UDim2.fromScale(0.65, 0),
        Size = UDim2.new(0.35, 0, 0, 15),
        Font = Enum.Font.Code,
        Text = tostring(value) .. suffix,
        TextColor3 = Library.Theme.Text,
        TextSize = 12,
        ClearTextOnFocus = false,
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
        if Library.ESPPreview then
            Library.ESPPreview:Refresh()
        end
        if Library.Indicators then
            Library.Indicators:Refresh()
        end
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
        elseif input.UserInputType == Enum.UserInputType.MouseButton2 then
            if cfg.Default ~= nil then
                Slider:Set(cfg.Default)
            end
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

    Library:Connect(valLabel.FocusLost, function()
        local rawNum = tonumber(string.match(valLabel.Text, "[-+]?%d*%.?%d+"))
        if rawNum then
            Slider:Set(rawNum)
        else
            valLabel.Text = FormatValue(value)
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
    Library:RegisterSearchable(holder, titleLbl, name)

    -- Single Unified Dropdown Box (expands downward as 1 whole bordered frame instead of 2 separate boxes)
    local dropBox = Library:Create("Frame", {
        Name = "DropBox",
        BackgroundColor3 = Library.Theme.Element,
        Position = UDim2.fromOffset(1, 17),
        Size = UDim2.new(1, -2, 0, 20),
        ZIndex = 15,
        Parent = holder,
    })
    Library:BindTheme(dropBox, "BackgroundColor3", "Element")
    local boxStroke = Library:ApplyCSBorder(dropBox, "InnerBorder")

    -- Top clickable header area inside dropBox
    local boxBtn = Library:Create("TextButton", {
        BackgroundTransparency = 1,
        AutoButtonColor = false,
        Size = UDim2.new(1, 0, 0, 20),
        Text = "",
        ZIndex = 17,
        Parent = dropBox,
    })

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
        ZIndex = 18,
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
        ZIndex = 18,
        Parent = boxBtn,
    })
    Library:BindTheme(chevron, "ImageColor3", "TextDim")

    -- Interior expandable list area inside the same dropBox (no second border)
    local dropMenu = Library:Create("Frame", {
        Name = "DropMenu_" .. name,
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(0, 20),
        Size = UDim2.new(1, 0, 1, -20),
        ClipsDescendants = true,
        Visible = false,
        ZIndex = 16,
        Parent = dropBox,
    })

    -- Subtle 1px inner divider inside the unified box between the header text and options
    local seamDivider = Library:Create("Frame", {
        BackgroundColor3 = Library.Theme.InnerBorder,
        BorderSizePixel = 0,
        Position = UDim2.fromOffset(6, 0),
        Size = UDim2.new(1, -12, 0, 1),
        BackgroundTransparency = 0.25,
        ZIndex = 17,
        Parent = dropMenu,
    })
    Library:BindTheme(seamDivider, "BackgroundColor3", "InnerBorder")

    local closeToken = 0

    local popupCtrl = {
        Frames = { dropBox },
        IsOpen = function()
            return opened and dropMenu.Visible
        end,
        Close = function()
            if not opened and not dropMenu.Visible then return end
            opened = false
            closeToken += 1
            local token = closeToken
            Library:Tween(chevron, TweenFast, { Rotation = 0 })
            Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.InnerBorder })
            Library:Tween(dropBox, TweenFast, { Size = UDim2.new(1, -2, 0, 20) })
            Library:Tween(holder, TweenFast, { Size = UDim2.new(1, 0, 0, 38) })
            task.delay(0.14, function()
                if closeToken == token and not opened then
                    dropMenu.Visible = false
                    holder.ZIndex = 10
                end
            end)
        end
    }
    table.insert(Library.OpenPopups, popupCtrl)

    local searchBox
    local listTopOffset = 4
    if searchable then
        listTopOffset = 27
        local searchHolder = Library:Create("Frame", {
            BackgroundColor3 = Library.Theme.Background,
            BorderSizePixel = 0,
            Position = UDim2.fromOffset(4, 4),
            Size = UDim2.new(1, -8, 0, 20),
            ZIndex = 22,
            Parent = dropMenu,
        })
        Library:BindTheme(searchHolder, "BackgroundColor3", "Background")
        Library:ApplyCSBorder(searchHolder, "InnerBorder")

        Library:Create("ImageLabel", {
            AnchorPoint = Vector2.new(0, 0.5),
            BackgroundTransparency = 1,
            Position = UDim2.new(0, 5, 0.5, 0),
            Size = UDim2.fromOffset(11, 11),
            Image = Library:GetIcon("search"),
            ImageColor3 = Library.Theme.TextMuted,
            ZIndex = 23,
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
            ZIndex = 23,
            Parent = searchHolder,
        })
    end

    local scroll = Library:Create("ScrollingFrame", {
        BackgroundTransparency = 1,
        BorderSizePixel = 0,
        Position = UDim2.fromOffset(2, listTopOffset),
        Size = UDim2.new(1, -4, 1, -listTopOffset - 3),
        CanvasSize = UDim2.fromOffset(0, 0),
        ScrollBarThickness = 2,
        ScrollBarImageColor3 = Library.Theme.Accent,
        ZIndex = 22,
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
        Library:PruneRegistries()

        local shown = 0
        for _, optName in ipairs(values) do
            local strName = tostring(optName)
            if searchQuery == "" or string.find(string.lower(strName), string.lower(searchQuery), 1, true) then
                shown += 1
                local isSel = multi and (selected[optName] == true) or (selected == optName)

                local itemBtn = Library:Create("TextButton", {
                    BackgroundColor3 = isSel and Library.Theme.ElementHover or Library.Theme.Element,
                    BackgroundTransparency = isSel and 0 or 1,
                    AutoButtonColor = false,
                    Size = UDim2.new(1, 0, 0, 20),
                    Text = "",
                    ZIndex = 23,
                    Parent = scroll,
                })

                -- Left accent bar when selected
                local selBar = Library:Create("Frame", {
                    BackgroundColor3 = Library.Theme.Accent,
                    BorderSizePixel = 0,
                    Size = isSel and UDim2.new(0, 2, 1, 0) or UDim2.new(0, 0, 1, 0),
                    ZIndex = 24,
                    Parent = itemBtn,
                })
                Library:BindTheme(selBar, "BackgroundColor3", "Accent")

                -- Optional mini square checkbox inside multi-select dropdowns
                local textOffset = 8
                if multi then
                    textOffset = 24
                    local miniBox = Library:Create("Frame", {
                        AnchorPoint = Vector2.new(0, 0.5),
                        BackgroundColor3 = isSel and Library.Theme.Accent or Library.Theme.Background,
                        Position = UDim2.new(0, 7, 0.5, 0),
                        Size = UDim2.fromOffset(10, 10),
                        ZIndex = 24,
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
                    ZIndex = 24,
                    Parent = itemBtn,
                })

                Library:Connect(itemBtn.MouseEnter, function()
                    if not isSel then
                        Library:Tween(itemBtn, TweenFast, {
                            BackgroundColor3 = Library.Theme.ElementHover,
                            BackgroundTransparency = 0.35,
                        })
                        Library:Tween(itemLbl, TweenFast, { TextColor3 = Library.Theme.Text })
                    end
                end)

                Library:Connect(itemBtn.MouseLeave, function()
                    if not isSel then
                        Library:Tween(itemBtn, TweenFast, { BackgroundTransparency = 1 })
                        Library:Tween(itemLbl, TweenFast, { TextColor3 = Library.Theme.TextDim })
                    end
                end)

                Library:Connect(itemBtn.MouseButton1Click, function()
                    if multi then
                        selected[optName] = not selected[optName] or nil
                        Dropdown.Value = selected
                        Library.Flags[flag] = selected
                        UpdateDisplayText()
                        RebuildItems()
                        if Library.ESPPreview then
                            Library.ESPPreview:Refresh()
                        end
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
                        if Library.ESPPreview then
                            Library.ESPPreview:Refresh()
                        end
                        if cfg.Callback then
                            task.spawn(cfg.Callback, selected)
                        end
                        Dropdown.Changed:Fire(selected)
                    end
                end)
            end
        end

        if shown == 0 then
            Library:Create("TextButton", {
                BackgroundTransparency = 1,
                AutoButtonColor = false,
                Size = UDim2.new(1, 0, 0, 20),
                Font = Enum.Font.Code,
                Text = "  No options found",
                TextColor3 = Library.Theme.TextMuted,
                TextSize = 11,
                TextXAlignment = Enum.TextXAlignment.Left,
                ZIndex = 23,
                Parent = scroll,
            })
            shown = 1
        end

        local contentH = shown * 21
        scroll.CanvasSize = UDim2.fromOffset(0, contentH)
        local menuH = math.clamp(math.max(20, contentH) + listTopOffset + 4, listTopOffset + 22, 145)
        if opened then
            Library:Tween(dropBox, TweenFast, {
                Size = UDim2.new(1, -2, 0, 20 + menuH)
            })
            Library:Tween(holder, TweenFast, {
                Size = UDim2.new(1, 0, 0, 38 + menuH)
            })
        end
        return menuH
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
        if Library.ESPPreview then
            Library.ESPPreview:Refresh()
        end
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

    local function ToggleDropdown()
        local nextState = not opened
        Library:ClosePopups(popupCtrl)
        opened = nextState
        if opened then
            closeToken += 1
            holder.ZIndex = 35
            dropMenu.Visible = true
            Library:Tween(chevron, TweenFast, { Rotation = 180 })
            Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.Accent })
            RebuildItems()
        else
            popupCtrl.Close()
        end
    end

    Library:Connect(boxBtn.InputBegan, function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            ToggleDropdown()
        end
    end)

    Library:Connect(boxBtn.MouseEnter, function()
        if not opened then
            Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.HighlightBorder })
        end
    end)

    Library:Connect(boxBtn.MouseLeave, function()
        if not opened then
            Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.InnerBorder })
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
    Library:RegisterSearchable(holder, titleLbl, name)

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
        Library:PruneRegistries()
        for _, item in ipairs(values) do
            local isSel = multi and (selected[item] == true) or (selected == item)
            local row = Library:Create("TextButton", {
                BackgroundColor3 = isSel and Library.Theme.ElementHover or Library.Theme.Element,
                BackgroundTransparency = isSel and 0 or 1,
                AutoButtonColor = false,
                Size = UDim2.new(1, 0, 0, 18),
                Text = "",
                ZIndex = 12,
                Parent = scroll,
            })
            local selBar = Library:Create("Frame", {
                BackgroundColor3 = Library.Theme.Accent,
                BorderSizePixel = 0,
                Size = isSel and UDim2.new(0, 2, 1, 0) or UDim2.new(0, 0, 1, 0),
                ZIndex = 13,
                Parent = row,
            })
            Library:BindTheme(selBar, "BackgroundColor3", "Accent")
            local rowLbl = Library:Create("TextLabel", {
                BackgroundTransparency = 1,
                Position = UDim2.fromOffset(isSel and 8 or 6, 0),
                Size = UDim2.new(1, -12, 1, 0),
                Font = isSel and Enum.Font.GothamBold or Enum.Font.Code,
                Text = tostring(item),
                TextColor3 = isSel and Library.Theme.Accent or Library.Theme.TextDim,
                TextSize = 12,
                TextXAlignment = Enum.TextXAlignment.Left,
                ZIndex = 13,
                Parent = row,
            })
            Library:Connect(row.MouseEnter, function()
                if not isSel then
                    Library:Tween(row, TweenFast, {
                        BackgroundColor3 = Library.Theme.ElementHover,
                        BackgroundTransparency = 0.4,
                    })
                    Library:Tween(rowLbl, TweenFast, {
                        Position = UDim2.fromOffset(9, 0),
                        TextColor3 = Library.Theme.Text,
                    })
                end
            end)
            Library:Connect(row.MouseLeave, function()
                if not isSel then
                    Library:Tween(row, TweenFast, { BackgroundTransparency = 1 })
                    Library:Tween(rowLbl, TweenFast, {
                        Position = UDim2.fromOffset(6, 0),
                        TextColor3 = Library.Theme.TextDim,
                    })
                end
            end)
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
        if not multi and selected ~= nil and not table.find(values, selected) then
            selected = values[1]
            Listbox.Value = selected
            Library.Flags[flag] = selected
        end
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
    Library:RegisterSearchable(row, nil, name)

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
    Library:RegisterSearchable(holder, titleLbl, name)

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
        Library:Tween(titleLbl, TweenFast, { TextColor3 = Library.Theme.Text })
        Library:Tween(boxWrap, TweenFast, { BackgroundColor3 = Library.Theme.ElementHover })
    end)

    Library:Connect(tBox.FocusLost, function(enterPressed)
        Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.InnerBorder })
        Library:Tween(titleLbl, TweenFast, { TextColor3 = Library.Theme.TextDim })
        Library:Tween(boxWrap, TweenFast, { BackgroundColor3 = Library.Theme.Element })
        if not cfg.OnlyOnEnter or enterPressed then
            InputObj:Set(tBox.Text)
        end
    end)

    Library:Connect(boxWrap.MouseEnter, function()
        if not tBox:IsFocused() then
            Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.HighlightBorder })
        end
    end)
    Library:Connect(boxWrap.MouseLeave, function()
        if not tBox:IsFocused() then
            Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.InnerBorder })
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
    local bounds = Library:GetTextBounds(str, 10, Vector2.new(300, 20), true)
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
        local nb = Library:GetTextBounds(str, 10, Vector2.new(300, 20), true)
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
    Library:RegisterSearchable(row, lbl, text)

    local attachTray = Library:Create("Frame", {
        Name = "AttachTray",
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
    local boxStroke = Library:ApplyCSBorder(box, "InnerBorder")

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

    Library:Connect(box.MouseEnter, function()
        Library:Tween(topStripe, TweenFast, { BackgroundTransparency = 0.08 })
        Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.HighlightBorder })
    end)
    Library:Connect(box.MouseLeave, function()
        Library:Tween(topStripe, TweenFast, { BackgroundTransparency = 0.45 })
        Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.InnerBorder })
    end)
    -- Classic CS:GO Overlapping Title Pill on the Top Border
    local strTitle = string.upper(tostring(title or "GROUP"))
    local tBounds = Library:GetTextBounds(strTitle, 11, Vector2.new(250, 20), true)
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
        local nb = Library:GetTextBounds(strTitle, 11, Vector2.new(250, 20), true)
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
        Position = UDim2.fromOffset(0, 7),
        Size = UDim2.new(1, 0, 1, -7),
        ZIndex = 6,
        Parent = wrapper,
    })
    Library:BindTheme(box, "BackgroundColor3", "Groupbox")
    local boxStroke = Library:ApplyCSBorder(box, "InnerBorder")

    Library:Connect(box.MouseEnter, function()
        Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.HighlightBorder })
    end)
    Library:Connect(box.MouseLeave, function()
        Library:Tween(boxStroke, TweenFast, { Color = Library.Theme.InnerBorder })
    end)

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
            wrapper.Size = UDim2.new(1, 0, 0, h + 49)
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
            AnchorPoint = Vector2.new(0.5, 0),
            BackgroundColor3 = Library.Theme.Accent,
            BorderSizePixel = 0,
            Position = UDim2.fromScale(0.5, 0),
            Size = UDim2.new(0, 0, 0, 1),
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
                Library:Tween(other.TopLine, TweenFast, {
                    Size = UDim2.new(0, 0, 0, 1),
                    BackgroundTransparency = 1,
                })
            end
            Tabbox.ActiveTab = subTabObj
            container.Visible = true
            container.Position = UDim2.fromOffset(10, 36)
            Library:Tween(container, TweenFast, { Position = UDim2.fromOffset(10, 32) })
            Library:Tween(btn, TweenFast, {
                BackgroundColor3 = Library.Theme.Groupbox,
                TextColor3 = Library.Theme.Text,
            })
            Library:Tween(topLine, TweenMedium, {
                Size = UDim2.new(1, 0, 0, 1),
                BackgroundTransparency = 0,
            })
            UpdateWrapperHeight()
        end

        Library:Connect(btn.MouseEnter, function()
            if Tabbox.ActiveTab ~= subTabObj then
                Library:Tween(btn, TweenFast, {
                    BackgroundColor3 = Library.Theme.Element,
                    TextColor3 = Library.Theme.Text,
                })
            end
        end)
        Library:Connect(btn.MouseLeave, function()
            if Tabbox.ActiveTab ~= subTabObj then
                Library:Tween(btn, TweenFast, {
                    BackgroundColor3 = Library.Theme.Sidebar,
                    TextColor3 = Library.Theme.TextDim,
                })
            end
        end)
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

    local openToken = 0
    local savedWinPos = main.Position
    local resizeStartTopLeft = Vector2.zero

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
    self:MakeDraggable(topBar, main, "MainWindow", function(nextPos)
        savedWinPos = nextPos
    end)

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
        Size = UDim2.new(0, 130, 1, 0),
        Font = Enum.Font.GothamBold,
        Text = string.upper(self.Title),
        TextColor3 = self.Theme.Text,
        TextSize = 12,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 5,
        Parent = topBar,
    })
    self:BindTheme(titleLabel, "TextColor3", "Text")

    -- Global Top-Bar Search Box
    local globalSearchWrap = self:Create("Frame", {
        AnchorPoint = Vector2.new(0, 0.5),
        BackgroundColor3 = self.Theme.Element,
        Position = UDim2.new(0, 149, 0.5, 0),
        Size = UDim2.fromOffset(180, 18),
        ZIndex = 6,
        Parent = topBar,
    })
    self:BindTheme(globalSearchWrap, "BackgroundColor3", "Element")
    local globalSearchStroke = self:ApplyCSBorder(globalSearchWrap, "InnerBorder")

    local globalSearchIcon = self:Create("ImageLabel", {
        AnchorPoint = Vector2.new(0, 0.5),
        BackgroundTransparency = 1,
        Position = UDim2.new(0, 5, 0.5, 0),
        Size = UDim2.fromOffset(11, 11),
        Image = self:GetIcon("search"),
        ImageColor3 = self.Theme.TextMuted,
        ZIndex = 7,
        Parent = globalSearchWrap,
    })
    self:BindTheme(globalSearchIcon, "ImageColor3", "TextMuted")

    local globalSearchBox = self:Create("TextBox", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(20, 0),
        Size = UDim2.new(1, -24, 1, 0),
        Font = Enum.Font.Code,
        Text = "",
        PlaceholderText = "Search controls...",
        PlaceholderColor3 = self.Theme.TextMuted,
        TextColor3 = self.Theme.Text,
        TextSize = 11,
        ClearTextOnFocus = false,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 7,
        Parent = globalSearchWrap,
    })
    self:BindTheme(globalSearchBox, "TextColor3", "Text")

    self:Connect(globalSearchBox.Focused, function()
        Library:Tween(globalSearchStroke, TweenFast, { Color = self.Theme.Accent })
        Library:Tween(globalSearchIcon, TweenFast, { ImageColor3 = self.Theme.Accent })
    end)
    self:Connect(globalSearchBox.FocusLost, function()
        Library:Tween(globalSearchStroke, TweenFast, { Color = self.Theme.InnerBorder })
        Library:Tween(globalSearchIcon, TweenFast, { ImageColor3 = self.Theme.TextMuted })
    end)
    self:Connect(globalSearchBox:GetPropertyChangedSignal("Text"), function()
        self:ApplyGlobalSearch(globalSearchBox.Text)
    end)

    local rightBadge = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(335, 0),
        Size = UDim2.new(1, -345, 1, 0),
        Font = Enum.Font.Code,
        Text = subTitle .. "  [" .. self:FormatKey(self.ToggleKey) .. "]",
        TextColor3 = self.Theme.TextMuted,
        TextSize = 11,
        TextXAlignment = Enum.TextXAlignment.Right,
        ZIndex = 5,
        Parent = topBar,
    })
    self:BindTheme(rightBadge, "TextColor3", "TextMuted")
    self.UpdateToggleKeyBadge = function()
        if rightBadge and rightBadge.Parent then
            rightBadge.Text = subTitle .. "  [" .. self:FormatKey(self.ToggleKey) .. "]"
        end
    end

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
        Size = UDim2.new(0.5, -24, 1, 0),
        Font = Enum.Font.Code,
        Text = "status: undetected / active",
        TextColor3 = self.Theme.Accent,
        TextSize = 11,
        TextXAlignment = Enum.TextXAlignment.Right,
        ZIndex = 5,
        Parent = footer,
    })
    self:BindTheme(footRight, "TextColor3", "Accent")

    -- Bottom-Right Diagonal CS:GO Window Resize Grip
    local resizeGrip = self:Create("TextButton", {
        Name = "ResizeGrip",
        AnchorPoint = Vector2.new(1, 1),
        BackgroundTransparency = 1,
        AutoButtonColor = false,
        Position = UDim2.new(1, -2, 1, -2),
        Size = UDim2.fromOffset(16, 16),
        Text = "",
        ZIndex = 12,
        Parent = main,
    })
    local gripDots = {}
    local dotCoords = {
        { 11, 11 },
        { 7, 11 }, { 11, 7 },
        { 3, 11 }, { 7, 7 }, { 11, 3 },
    }
    for _, dc in ipairs(dotCoords) do
        local d = self:Create("Frame", {
            BackgroundColor3 = self.Theme.TextMuted,
            BorderSizePixel = 0,
            Position = UDim2.fromOffset(dc[1], dc[2]),
            Size = UDim2.fromOffset(2, 2),
            ZIndex = 13,
            Parent = resizeGrip,
        })
        self:BindTheme(d, "BackgroundColor3", "TextMuted")
        table.insert(gripDots, d)
    end

    local resizing = false
    local resizeStartMouse = Vector2.zero
    local resizeStartSize = Vector2.zero
    local resizeStartPos = UDim2.new()

    self:Connect(resizeGrip.MouseEnter, function()
        for _, d in ipairs(gripDots) do
            Library:Tween(d, TweenFast, { BackgroundColor3 = self.Theme.Accent })
        end
    end)
    self:Connect(resizeGrip.MouseLeave, function()
        if not resizing then
            for _, d in ipairs(gripDots) do
                Library:Tween(d, TweenFast, { BackgroundColor3 = self.Theme.TextMuted })
            end
        end
    end)
    self:Connect(resizeGrip.InputBegan, function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            resizing = true
            resizeStartMouse = Vector2.new(input.Position.X, input.Position.Y)
            resizeStartSize = main.AbsoluteSize
            resizeStartTopLeft = main.AbsolutePosition
            resizeStartPos = main.Position
            self:ClosePopups()
        end
    end)
    self:Connect(UserInputService.InputChanged, function(input)
        if resizing and input.UserInputType == Enum.UserInputType.MouseMovement then
            local delta = Vector2.new(input.Position.X, input.Position.Y) - resizeStartMouse
            local newW = math.floor((math.clamp(resizeStartSize.X + delta.X, 580, 1100) + 1) / 2) * 2
            local newH = math.floor((math.clamp(resizeStartSize.Y + delta.Y, 440, 850) + 1) / 2) * 2
            local dw = newW - resizeStartSize.X
            local dh = newH - resizeStartSize.Y
            local nextPos = UDim2.new(
                resizeStartPos.X.Scale,
                math.floor(resizeStartPos.X.Offset + dw * 0.5 + 0.5),
                resizeStartPos.Y.Scale,
                math.floor(resizeStartPos.Y.Offset + dh * 0.5 + 0.5)
            )
            main.Size = UDim2.fromOffset(newW, newH)
            main.Position = nextPos
            local curTL = main.AbsolutePosition
            local errX = math.floor((resizeStartTopLeft.X - curTL.X) + 0.5)
            local errY = math.floor((resizeStartTopLeft.Y - curTL.Y) + 0.5)
            if errX ~= 0 or errY ~= 0 then
                nextPos = UDim2.new(nextPos.X.Scale, nextPos.X.Offset + errX, nextPos.Y.Scale, nextPos.Y.Offset + errY)
                main.Position = nextPos
            end
            savedWinPos = nextPos
            if Window.ESPPreview and Window.ESPPreview.SyncPosition then
                Window.ESPPreview:SyncPosition(false)
            end
        end
    end)
    self:Connect(UserInputService.InputEnded, function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 and resizing then
            resizing = false
            for _, d in ipairs(gripDots) do
                Library:Tween(d, TweenFast, { BackgroundColor3 = self.Theme.TextMuted })
            end
        end
    end)

    local Window = {
        Main = main,
        Tabs = {},
        ActiveTab = nil,
        TabChanged = Signal.new(),
    }

    function Window:SetOpen(state)
        Library.Open = state
        Library:ClosePopups()
        TooltipFrame.Visible = false
        if Window.ESPPreview then
            Window.ESPPreview:SyncVisibility()
        end
        openToken += 1
        local token = openToken
        if state then
            main.Visible = true
            main.Position = UDim2.new(savedWinPos.X.Scale, savedWinPos.X.Offset, savedWinPos.Y.Scale, savedWinPos.Y.Offset + 8)
            Library:Tween(main, TweenMedium, { Position = savedWinPos })
            Library:RefreshAllText()
        else
            Library:Tween(main, TweenFast, {
                Position = UDim2.new(savedWinPos.X.Scale, savedWinPos.X.Offset, savedWinPos.Y.Scale, savedWinPos.Y.Offset + 6)
            })
            task.delay(0.14, function()
                if openToken == token and not Library.Open then
                    main.Visible = false
                    main.Position = savedWinPos
                end
            end)
        end
    end

    function Window:Toggle()
        Window:SetOpen(not Library.Open)
    end

    Library:Connect(UserInputService.InputBegan, function(input, gpe)
        if not gpe and input.KeyCode == self.ToggleKey then
            if self._justBoundTick and (os.clock() - self._justBoundTick) < 0.08 then
                return
            end
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
                Library:Tween(other.Icon, TweenFast, {
                    Position = UDim2.new(0, 10, 0.5, 0),
                    ImageColor3 = Library.Theme.TextDim,
                })
                Library:Tween(other.Label, TweenFast, {
                    Position = UDim2.fromOffset(32, 0),
                    TextColor3 = Library.Theme.TextDim,
                })
            end
            Window.ActiveTab = Tab
            page.Visible = true
            UpdateCanvas()
            page.Position = UDim2.fromOffset(10, 16)
            Library:Tween(page, TweenMedium, { Position = UDim2.fromOffset(10, 10) })
            Library:Tween(tabBtn, TweenFast, {
                BackgroundColor3 = Library.Theme.Element,
                BackgroundTransparency = 0,
            })
            Library:Tween(btnBorder, TweenFast, {
                Color = Library.Theme.InnerBorder,
                Transparency = 0,
            })
            Library:Tween(activeBar, TweenMedium, { Size = UDim2.new(0, 2, 1, -6) })
            Library:Tween(iconImg, TweenFast, {
                Position = UDim2.new(0, 11, 0.5, 0),
                ImageColor3 = Library.Theme.Accent,
            })
            Library:Tween(tabLbl, TweenFast, {
                Position = UDim2.fromOffset(34, 0),
                TextColor3 = Library.Theme.Text,
            })
            Window.TabChanged:Fire(Tab)
            if Window.ESPPreview then
                Window.ESPPreview:SyncVisibility()
            end
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
                Library:Tween(iconImg, TweenFast, {
                    Position = UDim2.new(0, 11, 0.5, 0),
                    ImageColor3 = Library.Theme.Text,
                })
                Library:Tween(tabLbl, TweenFast, {
                    Position = UDim2.fromOffset(34, 0),
                    TextColor3 = Library.Theme.Text,
                })
            end
        end)

        Library:Connect(tabBtn.MouseLeave, function()
            if Window.ActiveTab ~= Tab then
                Library:Tween(tabBtn, TweenFast, { BackgroundTransparency = 1 })
                Library:Tween(iconImg, TweenFast, {
                    Position = UDim2.new(0, 10, 0.5, 0),
                    ImageColor3 = Library.Theme.TextDim,
                })
                Library:Tween(tabLbl, TweenFast, {
                    Position = UDim2.fromOffset(32, 0),
                    TextColor3 = Library.Theme.TextDim,
                })
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

    self.ActiveWindow = Window
    return Window
end

-- ============================================================================
-- INTERACTIVE CS2 2D ESP PREVIEW PANEL (Synced with Players ESP Toggles/Colors)
-- ============================================================================
function Library:CreateESPPreview(window, targetTab)
    if not window or not window.Main then return end
    local main = window.Main
    local targetTabName = type(targetTab) == "table" and targetTab.Name or tostring(targetTab or "Players ESP")
    local previewEnabled = false

    local panel = self:Create("Frame", {
        Name = "ESPPreviewPanel",
        BackgroundColor3 = self.Theme.Background,
        Position = UDim2.fromOffset(0, 0),
        Size = UDim2.fromOffset(205, 315),
        Visible = false,
        ZIndex = 20,
        Parent = ScreenGui,
    })
    self:BindTheme(panel, "BackgroundColor3", "Background")
    self:ApplyCSBorder(panel, "HighlightBorder")
    self:CreateAccentBar(panel, 25)

    local header = self:Create("Frame", {
        BackgroundColor3 = self.Theme.Sidebar,
        BorderSizePixel = 0,
        Position = UDim2.fromOffset(0, 2),
        Size = UDim2.new(1, 0, 0, 24),
        ZIndex = 21,
        Parent = panel,
    })
    self:BindTheme(header, "BackgroundColor3", "Sidebar")

    local hSep = self:Create("Frame", {
        BackgroundColor3 = self.Theme.InnerBorder,
        BorderSizePixel = 0,
        Position = UDim2.new(0, 0, 1, 0),
        Size = UDim2.new(1, 0, 0, 1),
        ZIndex = 22,
        Parent = header,
    })
    self:BindTheme(hSep, "BackgroundColor3", "InnerBorder")

    local hIcon = self:Create("ImageLabel", {
        AnchorPoint = Vector2.new(0, 0.5),
        BackgroundTransparency = 1,
        Position = UDim2.new(0, 8, 0.5, 0),
        Size = UDim2.fromOffset(12, 12),
        Image = self:GetIcon("eye"),
        ImageColor3 = self.Theme.Accent,
        ZIndex = 22,
        Parent = header,
    })
    self:BindTheme(hIcon, "ImageColor3", "Accent")

    local hTitle = self:Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(25, 0),
        Size = UDim2.new(1, -30, 1, 0),
        Font = Enum.Font.GothamBold,
        Text = "ESP PREVIEW // CS2",
        TextColor3 = self.Theme.Text,
        TextSize = 11,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 22,
        Parent = header,
    })
    self:BindTheme(hTitle, "TextColor3", "Text")

    -- Interior Dark Viewport Box
    local stage = self:Create("Frame", {
        BackgroundColor3 = Color3.fromRGB(14, 14, 19),
        Position = UDim2.fromOffset(10, 34),
        Size = UDim2.new(1, -20, 1, -44),
        ZIndex = 21,
        Parent = panel,
    })
    self:ApplyCSBorder(stage, "InnerBorder")

    -- Subtle Tactical Grid Lines inside Stage
    for gx = 1, 3 do
        self:Create("Frame", {
            BackgroundColor3 = Color3.fromRGB(24, 24, 32),
            BorderSizePixel = 0,
            Position = UDim2.new(gx / 4, 0, 0, 0),
            Size = UDim2.new(0, 1, 1, 0),
            ZIndex = 21,
            Parent = stage,
        })
    end
    for gy = 1, 4 do
        self:Create("Frame", {
            BackgroundColor3 = Color3.fromRGB(24, 24, 32),
            BorderSizePixel = 0,
            Position = UDim2.new(0, 0, gy / 5, 0),
            Size = UDim2.new(1, 0, 0, 1),
            ZIndex = 21,
            Parent = stage,
        })
    end

    -- Enemy Glow Stencil Aura behind Dummy
    local glowAura = self:Create("ImageLabel", {
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundTransparency = 1,
        Position = UDim2.fromScale(0.5, 0.49),
        Size = UDim2.fromOffset(145, 225),
        Image = "rbxassetid://5028857084",
        ImageColor3 = Color3.fromRGB(225, 55, 110),
        ImageTransparency = 0.4,
        ZIndex = 22,
        Parent = stage,
    })

    -- 2D Interactive Dummy Character Silhouette (Click body parts to toggle Target Hitboxes)
    local dummyRoot = self:Create("Frame", {
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundTransparency = 1,
        Position = UDim2.fromScale(0.5, 0.48),
        Size = UDim2.fromOffset(92, 182),
        ZIndex = 24,
        Parent = stage,
    })

    local bodyParts = {}
    local hitboxHovered = nil
    local PreviewObj = { Frame = panel }

    local hintLbl = self:Create("TextLabel", {
        AnchorPoint = Vector2.new(0.5, 1),
        BackgroundTransparency = 1,
        Position = UDim2.new(0.5, 0, 1, -3),
        Size = UDim2.new(1, -8, 0, 12),
        Font = Enum.Font.Code,
        Text = "CLICK BODY TO TOGGLE HITBOX",
        TextColor3 = self.Theme.TextMuted,
        TextSize = 9,
        ZIndex = 28,
        Parent = stage,
    })
    self:BindTheme(hintLbl, "TextColor3", "TextMuted")

    local function ToggleHitboxZone(zoneName)
        local hbOpt = self.Options["Rage_Hitboxes"]
        local current = self.Flags["Rage_Hitboxes"]
        local nextMap = {}
        if type(current) == "table" then
            for k, v in pairs(current) do
                if type(k) == "number" then
                    nextMap[v] = true
                elseif v then
                    nextMap[k] = true
                end
            end
        else
            nextMap["Head"] = true
            nextMap["Upper Chest"] = true
            nextMap["Pelvis"] = true
        end
        local nextState = not nextMap[zoneName]
        nextMap[zoneName] = nextState or nil
        if hbOpt and hbOpt.Set then
            hbOpt:Set(nextMap)
        else
            self.Flags["Rage_Hitboxes"] = nextMap
            PreviewObj:Refresh()
        end
        hintLbl.Text = string.upper(zoneName) .. (nextState and " [ENABLED]" or " [DISABLED]")
    end

    local function MakePart(px, py, pw, ph, zoneName, shortTag)
        local p = self:Create("TextButton", {
            AutoButtonColor = false,
            BackgroundColor3 = Color3.fromRGB(155, 195, 55),
            BackgroundTransparency = 0.2,
            BorderSizePixel = 0,
            Position = UDim2.fromOffset(px, py),
            Size = UDim2.fromOffset(pw, ph),
            Text = "",
            ZIndex = 24,
            Parent = dummyRoot,
        })
        local st = self:Create("UIStroke", {
            Color = Color3.fromRGB(10, 10, 14),
            Thickness = 1,
            Parent = p,
        })
        local node = self:Create("Frame", {
            AnchorPoint = Vector2.new(0.5, 0.5),
            BackgroundColor3 = self.Theme.Accent,
            BorderSizePixel = 0,
            Position = UDim2.fromScale(0.5, 0.5),
            Size = UDim2.fromOffset(4, 4),
            ZIndex = 26,
            Parent = p,
        })
        self:Create("UIStroke", {
            Color = Color3.new(0, 0, 0),
            Thickness = 1,
            Parent = node,
        })
        local tagLbl = self:Create("TextLabel", {
            AnchorPoint = Vector2.new(0.5, 0.5),
            BackgroundTransparency = 1,
            Position = UDim2.fromScale(0.5, 0.5),
            Size = UDim2.fromScale(1, 1),
            Font = Enum.Font.GothamBold,
            Text = shortTag or "",
            TextColor3 = Color3.fromRGB(245, 245, 250),
            TextTransparency = 0.25,
            TextSize = 8,
            ZIndex = 26,
            Parent = p,
        })
        local partEntry = {
            Frame = p,
            Stroke = st,
            Node = node,
            Tag = tagLbl,
            Zone = zoneName,
        }
        table.insert(bodyParts, partEntry)

        self:Connect(p.MouseEnter, function()
            hitboxHovered = zoneName
            hintLbl.Text = "HITBOX: " .. string.upper(zoneName)
            PreviewObj:Refresh()
        end)
        self:Connect(p.MouseLeave, function()
            if hitboxHovered == zoneName then
                hitboxHovered = nil
            end
            hintLbl.Text = "CLICK BODY TO TOGGLE HITBOX"
            PreviewObj:Refresh()
        end)
        self:Connect(p.MouseButton1Click, function()
            ToggleHitboxZone(zoneName)
        end)
        return p
    end

    -- Interactive Hitbox Zones: Head, Neck, Upper Chest, Pelvis, Arms, Legs, Feet
    MakePart(33, 3, 26, 23, "Head", "HD")
    MakePart(38, 27, 16, 7, "Neck", "")
    MakePart(24, 35, 44, 37, "Upper Chest", "CH")
    MakePart(24, 74, 44, 27, "Pelvis", "PL")
    MakePart(8, 36, 13, 60, "Arms", "LA")
    MakePart(71, 36, 13, 60, "Arms", "RA")
    MakePart(25, 103, 19, 57, "Legs", "LL")
    MakePart(48, 103, 19, 57, "Legs", "RL")
    MakePart(23, 162, 21, 14, "Feet", "FT")
    MakePart(48, 162, 21, 14, "Feet", "FT")

    -- 2D Bounding Box around Dummy (Supports Full 2D, Corner, and Filled 2D)
    local espBoxFill = self:Create("Frame", {
        BackgroundColor3 = Color3.fromRGB(155, 195, 55),
        BackgroundTransparency = 0.82,
        BorderSizePixel = 0,
        Size = UDim2.fromScale(1, 1),
        Visible = false,
        ZIndex = 26,
        Parent = dummyRoot,
    })

    local espBox = self:Create("Frame", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(0, 0),
        Size = UDim2.fromScale(1, 1),
        ZIndex = 27,
        Parent = dummyRoot,
    })
    self:Create("UIStroke", {
        Color = Color3.fromRGB(0, 0, 0),
        Thickness = 3,
        Transparency = 0.25,
        ApplyStrokeMode = Enum.ApplyStrokeMode.Border,
        Parent = espBox,
    })
    local espBoxStroke = self:Create("UIStroke", {
        Color = Color3.fromRGB(155, 195, 55),
        Thickness = 1,
        ApplyStrokeMode = Enum.ApplyStrokeMode.Border,
        Parent = espBox,
    })

    local previewCorners = {}
    local pCornerDefs = {
        { AX = 0, AY = 0, PX = 0, PY = 0, Horiz = true },
        { AX = 0, AY = 0, PX = 0, PY = 0, Horiz = false },
        { AX = 1, AY = 0, PX = 1, PY = 0, Horiz = true },
        { AX = 1, AY = 0, PX = 1, PY = 0, Horiz = false },
        { AX = 0, AY = 1, PX = 0, PY = 1, Horiz = true },
        { AX = 0, AY = 1, PX = 0, PY = 1, Horiz = false },
        { AX = 1, AY = 1, PX = 1, PY = 1, Horiz = true },
        { AX = 1, AY = 1, PX = 1, PY = 1, Horiz = false },
    }
    for _, d in ipairs(pCornerDefs) do
        local seg = self:Create("Frame", {
            AnchorPoint = Vector2.new(d.AX, d.AY),
            BackgroundColor3 = Color3.fromRGB(155, 195, 55),
            BorderSizePixel = 0,
            Position = UDim2.fromScale(d.PX, d.PY),
            Size = d.Horiz and UDim2.new(0.28, 0, 0, 1) or UDim2.new(0, 1, 0.24, 0),
            Visible = false,
            ZIndex = 28,
            Parent = dummyRoot,
        })
        self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Transparency = 0.25, Parent = seg })
        table.insert(previewCorners, seg)
    end

    local previewHeadDot = self:Create("Frame", {
        AnchorPoint = Vector2.new(0.5, 0.5),
        BackgroundColor3 = Color3.fromRGB(155, 195, 55),
        BorderSizePixel = 0,
        Position = UDim2.fromOffset(46, 14),
        Size = UDim2.fromOffset(5, 5),
        Visible = false,
        ZIndex = 29,
        Parent = dummyRoot,
    })
    self:Create("UICorner", { CornerRadius = UDim.new(1, 0), Parent = previewHeadDot })
    self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Parent = previewHeadDot })

    -- Preview Skeleton Bone Lines inside Dummy
    local previewBones = {}
    local pBoneCoords = {
        { 46, 14, 46, 31 }, -- Head -> Neck
        { 46, 31, 46, 85 }, -- Neck -> Pelvis
        { 46, 40, 14, 42 }, -- Left Shoulder
        { 14, 42, 14, 90 }, -- Left Arm
        { 46, 40, 78, 42 }, -- Right Shoulder
        { 78, 42, 78, 90 }, -- Right Arm
        { 46, 85, 34, 106 }, -- Left Hip
        { 34, 106, 34, 166 }, -- Left Leg
        { 46, 85, 58, 106 }, -- Right Hip
        { 58, 106, 58, 166 }, -- Right Leg
    }
    for _, bc in ipairs(pBoneCoords) do
        local pA = Vector2.new(bc[1], bc[2])
        local pB = Vector2.new(bc[3], bc[4])
        local dVec = pB - pA
        local mid = (pA + pB) * 0.5
        local bl = self:Create("Frame", {
            AnchorPoint = Vector2.new(0.5, 0.5),
            BackgroundColor3 = Color3.fromRGB(240, 240, 245),
            BorderSizePixel = 0,
            Position = UDim2.fromOffset(mid.X, mid.Y),
            Rotation = math.deg(math.atan2(dVec.Y, dVec.X)),
            Size = UDim2.fromOffset(dVec.Magnitude, 1),
            Visible = false,
            ZIndex = 28,
            Parent = dummyRoot,
        })
        self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Transparency = 0.35, Parent = bl })
        table.insert(previewBones, bl)
    end

    local previewTracer = self:Create("Frame", {
        AnchorPoint = Vector2.new(0.5, 0),
        BackgroundColor3 = Color3.fromRGB(155, 195, 55),
        BorderSizePixel = 0,
        Position = UDim2.new(0.5, 0, 1, 0),
        Size = UDim2.fromOffset(1, 34),
        Visible = false,
        ZIndex = 27,
        Parent = dummyRoot,
    })
    self:Create("UIStroke", { Color = Color3.new(0, 0, 0), Thickness = 1, Transparency = 0.25, Parent = previewTracer })

    -- Top Player Name Label
    local espName = self:Create("TextLabel", {
        AnchorPoint = Vector2.new(0.5, 1),
        BackgroundTransparency = 1,
        Position = UDim2.new(0.5, 0, 0, -4),
        Size = UDim2.fromOffset(140, 14),
        Font = Enum.Font.Code,
        Text = "s1mple [92 HP]",
        TextColor3 = Color3.fromRGB(235, 235, 240),
        TextSize = 11,
        ZIndex = 28,
        Parent = dummyRoot,
    })

    -- Health Bar (Supports Left, Right, Bottom + HP Value Text)
    local hpBarBg = self:Create("Frame", {
        BackgroundColor3 = Color3.fromRGB(10, 10, 12),
        BorderSizePixel = 0,
        Position = UDim2.fromOffset(-7, 0),
        Size = UDim2.new(0, 4, 1, 0),
        ZIndex = 27,
        Parent = dummyRoot,
    })
    local hpBarFill = self:Create("Frame", {
        AnchorPoint = Vector2.new(0, 1),
        BackgroundColor3 = Color3.fromRGB(115, 225, 85),
        BorderSizePixel = 0,
        Position = UDim2.fromScale(0, 1),
        Size = UDim2.new(1, 0, 0.92, 0),
        ZIndex = 28,
        Parent = hpBarBg,
    })
    self:Create("UIStroke", {
        Color = Color3.new(0, 0, 0),
        Thickness = 1,
        Parent = hpBarBg,
    })
    local hpValPreview = self:Create("TextLabel", {
        AnchorPoint = Vector2.new(1, 0.5),
        BackgroundTransparency = 1,
        Position = UDim2.new(0, -2, 0.08, 0),
        Size = UDim2.fromOffset(26, 10),
        Font = Enum.Font.GothamBold,
        Text = "92",
        TextColor3 = Color3.new(1, 1, 1),
        TextSize = 9,
        TextXAlignment = Enum.TextXAlignment.Right,
        Visible = false,
        ZIndex = 29,
        Parent = hpBarBg,
    })

    -- Bottom Weapon Text Label
    local espWeapon = self:Create("TextLabel", {
        AnchorPoint = Vector2.new(0.5, 0),
        BackgroundTransparency = 1,
        Position = UDim2.new(0.5, 0, 1, 4),
        Size = UDim2.fromOffset(140, 14),
        Font = Enum.Font.Code,
        Text = "SSG-08 SCOUT",
        TextColor3 = Color3.fromRGB(170, 200, 255),
        TextSize = 10,
        ZIndex = 28,
        Parent = dummyRoot,
    })

    -- Right Side ESP Flags Stack (HK, SCOPED, LC, etc.)
    local flagsHolder = self:Create("Frame", {
        BackgroundTransparency = 1,
        Position = UDim2.new(1, 4, 0, 0),
        Size = UDim2.fromOffset(48, 150),
        ZIndex = 28,
        Parent = dummyRoot,
    })
    self:Create("UIListLayout", {
        Padding = UDim.new(0, 1),
        SortOrder = Enum.SortOrder.LayoutOrder,
        Parent = flagsHolder,
    })

    local flagOrder = {
        { Key = "Armor (HK)",   Text = "HK",     Color = Color3.fromRGB(230, 230, 235) },
        { Key = "Scoped",       Text = "ZOOM",   Color = Color3.fromRGB(85, 190, 255)  },
        { Key = "Flashed",      Text = "FLASH",  Color = Color3.fromRGB(245, 205, 75)  },
        { Key = "Exploit / LC", Text = "LC",     Color = Color3.fromRGB(235, 75, 75)   },
        { Key = "Ping",         Text = "24MS",   Color = Color3.fromRGB(145, 220, 110) },
        { Key = "Distance",     Text = "42FT",   Color = Color3.fromRGB(185, 185, 195) },
    }
    local flagLabels = {}
    for idx, fInfo in ipairs(flagOrder) do
        local fl = self:Create("TextLabel", {
            BackgroundTransparency = 1,
            LayoutOrder = idx,
            Size = UDim2.new(1, 0, 0, 11),
            Font = Enum.Font.GothamBold,
            Text = fInfo.Text,
            TextColor3 = fInfo.Color,
            TextSize = 9,
            TextXAlignment = Enum.TextXAlignment.Left,
            ZIndex = 29,
            Parent = flagsHolder,
        })
        flagLabels[fInfo.Key] = fl
    end

    local function SyncPosition(animate)
        if not main or not main.Parent then return end
        local absPos = main.AbsolutePosition
        local absSize = main.AbsoluteSize
        local insetY = self:GetGuiInsetY()
        local vp = workspace.CurrentCamera and workspace.CurrentCamera.ViewportSize or Vector2.new(1920, 1080)
        local px = math.floor(absPos.X + absSize.X + 10 + 0.5)
        if px + 210 > vp.X then
            px = math.max(6, math.floor(absPos.X - 215 + 0.5))
        end
        local py = math.floor(absPos.Y + insetY + 0.5)
        if animate then
            panel.Position = UDim2.fromOffset(px - 6, py)
            Library:Tween(panel, TweenMedium, { Position = UDim2.fromOffset(px, py) })
        else
            panel.Position = UDim2.fromOffset(px, py)
        end
    end

    function PreviewObj:SyncPosition(animate)
        SyncPosition(animate)
    end

    function PreviewObj:Refresh()
        local F = Library.Flags
        local showBox = (F["ESP_Box"] == true)
        local boxStyle = F["ESP_BoxStyle"] or "Full 2D"
        local boxCol = F["ESP_BoxColor"] or Library.Theme.Accent
        local isCorner = (boxStyle == "Corner")
        local isFilled = (boxStyle == "Filled 2D")

        espBox.Visible = showBox and not isCorner
        espBoxStroke.Color = boxCol
        espBoxFill.Visible = showBox and isFilled
        espBoxFill.BackgroundColor3 = boxCol
        espBoxFill.BackgroundTransparency = math.clamp(1 - (tonumber(F["ESP_BoxFillAlpha"]) or 0.18), 0.35, 0.95)
        for _, seg in ipairs(previewCorners) do
            seg.Visible = showBox and isCorner
            seg.BackgroundColor3 = boxCol
        end

        previewHeadDot.Visible = (F["ESP_HeadDot"] == true)
        previewHeadDot.BackgroundColor3 = boxCol

        local showSkel = (F["ESP_Skeleton"] == true)
        local skelCol = F["ESP_SkeletonCol"] or Color3.fromRGB(240, 240, 245)
        for _, bl in ipairs(previewBones) do
            bl.Visible = showSkel
            bl.BackgroundColor3 = skelCol
        end

        previewTracer.Visible = (F["ESP_Tracers"] == true)
        previewTracer.BackgroundColor3 = F["ESP_TracerColor"] or boxCol

        local showName = F["ESP_Name"]
        espName.Visible = (showName == true)
        espName.TextColor3 = F["ESP_NameColor"] or Color3.fromRGB(235, 235, 240)

        local showHp = (F["ESP_Health"] == true)
        local hpPos = F["ESP_HealthPos"] or "Left"
        hpBarBg.Visible = showHp
        hpValPreview.Visible = showHp and (F["ESP_HealthText"] == true)
        if hpPos == "Bottom" then
            hpBarBg.Position = UDim2.new(0, 0, 1, 3)
            hpBarBg.Size = UDim2.new(1, 0, 0, 4)
            hpBarFill.AnchorPoint = Vector2.new(0, 0)
            hpBarFill.Position = UDim2.fromScale(0, 0)
            hpBarFill.Size = UDim2.new(0.92, 0, 1, 0)
            espWeapon.Position = UDim2.new(0.5, 0, 1, 10)
        elseif hpPos == "Right" then
            hpBarBg.Position = UDim2.new(1, 4, 0, 0)
            hpBarBg.Size = UDim2.new(0, 4, 1, 0)
            hpBarFill.AnchorPoint = Vector2.new(0, 1)
            hpBarFill.Position = UDim2.fromScale(0, 1)
            hpBarFill.Size = UDim2.new(1, 0, 0.92, 0)
            espWeapon.Position = UDim2.new(0.5, 0, 1, 4)
        else
            hpBarBg.Position = UDim2.fromOffset(-7, 0)
            hpBarBg.Size = UDim2.new(0, 4, 1, 0)
            hpBarFill.AnchorPoint = Vector2.new(0, 1)
            hpBarFill.Position = UDim2.fromScale(0, 1)
            hpBarFill.Size = UDim2.new(1, 0, 0.92, 0)
            espWeapon.Position = UDim2.new(0.5, 0, 1, 4)
        end

        local showWeap = F["ESP_Weapon"]
        espWeapon.Visible = (showWeap == true)
        espWeapon.TextColor3 = F["ESP_WeaponColor"] or Color3.fromRGB(170, 200, 255)

        local showGlow = F["ESP_Glow"]
        glowAura.Visible = (showGlow == true)
        glowAura.ImageColor3 = F["ESP_GlowColor"] or Color3.fromRGB(225, 55, 110)

        local visChams = F["Chams_Vis"]
        local xqzChams = F["Chams_XQZ"]
        local visCol = F["Chams_VisCol"] or Library.Theme.Accent
        local visCol2 = F["Chams_VisCol2"] or Color3.fromRGB(225, 55, 110)
        local xqzCol = F["Chams_XQZCol"] or Color3.fromRGB(60, 145, 245)
        local chamsMat = F["Chams_Mat"] or "Metallic"
        local fillTrans = tonumber(F["Chams_FillTrans"]) or 0.25
        local activeHitboxes = F["Rage_Hitboxes"]

        for _, bp in ipairs(bodyParts) do
            local hbSelected = false
            if type(activeHitboxes) == "table" then
                hbSelected = (activeHitboxes[bp.Zone] == true)
            end
            local isHover = (hitboxHovered == bp.Zone)
            local pCol = Color3.fromRGB(52, 54, 66)
            local pTrans = hbSelected and 0.22 or 0.62
            local strokeCol = isHover and Color3.new(1, 1, 1) or (hbSelected and Library.Theme.Accent or Color3.fromRGB(16, 16, 22))

            if visChams or xqzChams then
                pCol = visChams and visCol or xqzCol
                if chamsMat == "Flat" then
                    pTrans = hbSelected and 0.08 or math.clamp(fillTrans, 0.1, 0.7)
                elseif chamsMat == "Glass Wireframe" then
                    pTrans = 0.85
                    strokeCol = visChams and visCol or xqzCol
                elseif chamsMat == "Glow Overlay" then
                    pCol = pCol:Lerp(visCol2, 0.38)
                    pTrans = hbSelected and 0.18 or 0.45
                    strokeCol = visCol2
                elseif chamsMat == "Metallic" then
                    pCol = pCol:Lerp(Color3.new(1, 1, 1), 0.18)
                    pTrans = hbSelected and 0.14 or math.clamp(fillTrans + 0.15, 0.15, 0.75)
                else
                    pTrans = hbSelected and 0.14 or math.clamp(fillTrans + 0.2, 0.15, 0.75)
                end
            end

            Library:Tween(bp.Frame, TweenFast, {
                BackgroundColor3 = pCol,
                BackgroundTransparency = pTrans,
            })
            Library:Tween(bp.Stroke, TweenFast, {
                Color = strokeCol,
                Thickness = 1,
            })
            if bp.Node then
                bp.Node.Visible = hbSelected or isHover
                bp.Node.BackgroundColor3 = isHover and Color3.new(1, 1, 1) or Library.Theme.Accent
            end
            if bp.Tag then
                bp.Tag.Visible = hbSelected or isHover
            end
        end

        local activeFlags = F["ESP_Flags"]
        for key, lbl in pairs(flagLabels) do
            if type(activeFlags) == "table" then
                lbl.Visible = (activeFlags[key] == true)
            else
                lbl.Visible = false
            end
        end
    end

    function PreviewObj:SyncVisibility()
        local isOnTab = window.ActiveTab and (window.ActiveTab.Name == targetTabName)
        local wasVisible = panel.Visible
        panel.Visible = (Library.Open and previewEnabled and isOnTab) and true or false
        if panel.Visible then
            SyncPosition(not wasVisible)
            PreviewObj:Refresh()
        end
    end

    function PreviewObj:SetEnabled(state)
        previewEnabled = (state == true)
        PreviewObj:SyncVisibility()
    end

    self:Connect(main:GetPropertyChangedSignal("AbsolutePosition"), function()
        if panel.Visible then SyncPosition() end
    end)
    self:Connect(main:GetPropertyChangedSignal("AbsoluteSize"), function()
        if panel.Visible then SyncPosition() end
    end)

    window.ESPPreview = PreviewObj
    self.ESPPreview = PreviewObj
    PreviewObj:SyncVisibility()
    return PreviewObj
end

-- ============================================================================
-- BUILT-IN SETTINGS, THEME & JSON CONFIG MANAGER BUILDER
-- ============================================================================
function Library:GetConfigJSON(overrides)
    local data = {}
    for flag, opt in pairs(self.Options) do
        if flag ~= "UI_ConfigName" and flag ~= "UI_ConfigList" and flag ~= "UI_RawConfigJSON" then
            if opt.Type == "Toggle" or opt.Type == "Slider" or opt.Type == "Input" or opt.Type == "Dropdown" or opt.Type == "Listbox" then
                data[flag] = { Type = opt.Type, Value = opt.Value }
            elseif opt.Type == "ColorPicker" then
                local c = opt.Value
                data[flag] = {
                    Type = "ColorPicker",
                    R = c.R, G = c.G, B = c.B,
                    Alpha = opt.Alpha,
                    Rainbow = opt.Rainbow == true,
                }
            elseif opt.Type == "Keybind" then
                data[flag] = {
                    Type = "Keybind",
                    Key = typeof(opt.Value) == "EnumItem" and opt.Value.Name or tostring(opt.Value),
                    Mode = opt.Mode,
                }
            end
        end
    end
    if type(overrides) == "table" then
        for k, v in pairs(overrides) do
            if data[k] and data[k].Type then
                data[k].Value = v
            end
        end
    end
    local hudPositions = {}
    for hudId, frame in pairs(self.HUDFrames) do
        if frame and frame.Parent then
            local p = frame.Position
            hudPositions[hudId] = {
                XS = p.X.Scale, XO = p.X.Offset,
                YS = p.Y.Scale, YO = p.Y.Offset,
            }
        end
    end
    data["__HUDPositions"] = {
        Type = "HUDPositions",
        Positions = hudPositions,
    }
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
    if type(decoded["__HUDPositions"]) == "table" and type(decoded["__HUDPositions"].Positions) == "table" then
        for hudId, pos in pairs(decoded["__HUDPositions"].Positions) do
            if hudId ~= "MainWindow" then
                local frame = self.HUDFrames[hudId]
                if frame and frame.Parent and type(pos) == "table" then
                    frame.Position = UDim2.new(pos.XS or 0, pos.XO or 0, pos.YS or 0, pos.YO or 0)
                end
            end
        end
    end
    for flag, entry in pairs(decoded) do
        if flag ~= "UI_ConfigName" and flag ~= "UI_ConfigList" and flag ~= "UI_RawConfigJSON" then
            local opt = self.Options[flag]
            if opt and type(entry) == "table" then
                if entry.Type == "ColorPicker" and entry.R then
                    opt:Set(Color3.new(entry.R, entry.G, entry.B), entry.Alpha or 0, entry.Rainbow == true)
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
    end
    self:Notify({ Title = "Config Loaded", Description = "Configuration applied successfully.", Icon = "check" })
    return true
end

function Library:BuildConfigTab(tab)
    local themeGroup = tab:AddLeftGroupbox("Menu & Theme", "palette")

    local wmToggle = themeGroup:AddToggle({
        Name = "Show Watermark",
        Flag = "UI_ShowWatermark",
        Default = false,
        Callback = function(v)
            if self.Watermark then self.Watermark:SetVisible(v) end
        end
    })
    local wmSub = wmToggle:AddSubSection()
    wmSub:AddInput({
        Name = "Custom Watermark Title",
        Flag = "UI_WatermarkText",
        Default = "shards.cc | counter-strike",
        Placeholder = "shards.cc | cs2",
        Callback = function(txt)
            if self.Watermark and self.Watermark.Refresh then
                self.Watermark.Refresh()
            end
        end
    })
    wmSub:AddDropdown({
        Name = "Watermark Telemetry Fields",
        Flag = "UI_WatermarkFields",
        Multi = true,
        Values = { "User", "FPS", "Ping", "Time", "Build" },
        Default = { "User", "FPS", "Ping", "Time" },
        Callback = function()
            if self.Watermark and self.Watermark.Refresh then
                self.Watermark.Refresh()
            end
        end
    })
    wmSub:AddSlider({
        Name = "Watermark Background Transparency",
        Flag = "UI_WatermarkAlpha",
        Min = 0,
        Max = 0.8,
        Rounding = 2,
        Default = 0,
        Callback = function()
            if self.Watermark and self.Watermark.Refresh then
                self.Watermark.Refresh()
            end
        end
    })

    local kbToggle = themeGroup:AddToggle({
        Name = "Show Keybind List",
        Flag = "UI_ShowKeybinds",
        Default = false,
        Callback = function(v)
            if self.KeybindList then self.KeybindList:SetVisible(v) end
        end
    })
    local kbSub = kbToggle:AddSubSection()
    kbSub:AddDropdown({
        Name = "Keybind List Mode",
        Flag = "UI_KeybindListMode",
        Values = { "Active Binds Only", "Show All Binds" },
        Default = "Active Binds Only",
        Callback = function()
            if self.KeybindList then self.KeybindList:Refresh() end
        end
    })

    themeGroup:AddToggle({
        Name = "Lock HUD Panel Positions",
        Flag = "UI_LockHUD",
        Default = false,
    })

    themeGroup:AddToggle({
        Name = "Pause Keybinds While Menu Open",
        Flag = "UI_MenuPauseBinds",
        Default = false,
    })

    themeGroup:AddToggle({
        Name = "Show Spectator List",
        Flag = "UI_ShowSpectators",
        Default = false,
        Callback = function(v)
            if self.SpectatorList then self.SpectatorList:SetVisible(v) end
        end
    })

    themeGroup:AddToggle({
        Name = "Show Target HUD",
        Flag = "UI_ShowTargetHUD",
        Default = false,
        Callback = function(v)
            if self.TargetHUD then self.TargetHUD:SetVisible(v) end
        end
    })

    themeGroup:AddToggle({
        Name = "Show Crosshair Indicators",
        Flag = "UI_ShowIndicators",
        Default = false,
        Callback = function(v)
            if self.Indicators then self.Indicators:SetVisible(v) end
        end
    })

    themeGroup:AddToggle({
        Name = "Show 2D Radar Window",
        Flag = "UI_ShowRadar",
        Default = false,
        Callback = function(v)
            if self.Radar then self.Radar:SetVisible(v) end
        end
    })

    local menuBindLbl = themeGroup:AddLabel("Menu Toggle Key")
    menuBindLbl:AddKeybind({
        Name = "Menu Key",
        Flag = "UI_MenuKeybind",
        Default = self.ToggleKey,
        Callback = function(_, newKey)
            if typeof(newKey) == "EnumItem" and newKey.EnumType == Enum.KeyCode and newKey ~= Enum.KeyCode.Unknown then
                self.ToggleKey = newKey
                if self.UpdateToggleKeyBadge then self.UpdateToggleKeyBadge() end
            end
        end
    }).Changed:Connect(function(newKey)
        if typeof(newKey) == "EnumItem" and newKey.EnumType == Enum.KeyCode and newKey ~= Enum.KeyCode.Unknown then
            self.ToggleKey = newKey
            if self.UpdateToggleKeyBadge then self.UpdateToggleKeyBadge() end
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

    local fontScaleSlider
    themeGroup:AddDropdown({
        Name = "UI Font Style",
        Flag = "UI_FontPreset",
        Searchable = true,
        Values = self.FontOrder,
        Default = "Minimalist (Gotham / GothamBold)",
        Callback = function(choice)
            self:SetFontPreset(choice)
            if fontScaleSlider then
                fontScaleSlider:Set(self.FontScale)
            end
            if self.KeybindList then self.KeybindList:Refresh() end
        end
    })

    fontScaleSlider = themeGroup:AddSlider({
        Name = "Font Scale Fine-Tune",
        Flag = "UI_FontScale",
        Min = 0.65,
        Max = 1.25,
        Default = self.FontScale,
        Rounding = 2,
        Suffix = "x",
        Callback = function(sc)
            self.FontScale = sc
            self:RefreshAllText()
            if self.KeybindList then self.KeybindList:Refresh() end
        end
    })

    themeGroup:AddToggle({
        Name = "1px Black Text Outline",
        Flag = "UI_TextStroke",
        Default = self.TextStroke,
        Callback = function(state)
            self.TextStroke = state
            self:RefreshAllText()
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
    local cfgGroup = tab:AddRightGroupbox("Configuration", "file-text")
    local cfgName = "legit_default"
    local selectedCfg = "legit_default"
    local autoloadCfg = "none"
    local cfgNames = { "legit_default", "rage_hvh", "movement_bhop", "closet_main" }
    local savedMemoryConfigs = {
        ["legit_default"] = self:GetConfigJSON(),
        ["rage_hvh"] = self:GetConfigJSON({
            Rage_Enabled = true, Rage_Silent = true, Rage_AutoFire = true, Rage_Autowall = true,
            Rage_Hitchance = 78, Rage_MinDmg = 55, Rage_DT = true, AA_Enabled = true, FL_Enable = true,
            ESP_Box = true, ESP_Name = true, ESP_Health = true, Chams_Vis = true, Chams_XQZ = true,
            UI_ShowWatermark = true, UI_ShowKeybinds = true, UI_ShowIndicators = true, Misc_ShowIndicators = true,
        }),
        ["movement_bhop"] = self:GetConfigJSON({
            Misc_Bhop = true, Misc_Strafe = true, Misc_FastStop = true,
            UI_ShowWatermark = true, UI_ShowKeybinds = true,
        }),
        ["closet_main"] = self:GetConfigJSON({
            Legit_Enable = true, Legit_Smooth = 8.5, Legit_FOV = 3.5, Legit_RCS = true,
            ESP_Box = true, ESP_Name = true, ESP_Health = true,
        }),
    }

    pcall(function()
        if isfolder and makefolder then
            if not isfolder(self.Folder) then makefolder(self.Folder) end
        end
        if listfiles and isfolder and isfolder(self.Folder) then
            for _, filePath in ipairs(listfiles(self.Folder)) do
                local fn = tostring(filePath):match("([^/\\]+)%.json$")
                if fn and not table.find(cfgNames, fn) then
                    table.insert(cfgNames, fn)
                end
            end
        end
        if isfile and readfile and isfile(self.Folder .. "/autoload.txt") then
            local rawAuto = readfile(self.Folder .. "/autoload.txt"):gsub("%s+", "")
            if rawAuto ~= "" then
                autoloadCfg = rawAuto
            end
        end
    end)

    local autoStatusLbl = cfgGroup:AddLabel("Autoload Profile: " .. autoloadCfg)

    local cfgNameInput = cfgGroup:AddInput({
        Name = "Config Name",
        Flag = "UI_ConfigName",
        Default = "legit_default",
        Placeholder = "e.g. rage_semi",
        Callback = function(txt)
            local trimmed = tostring(txt or ""):gsub("^%s*(.-)%s*$", "%1")
            cfgName = trimmed
        end
    })

    local cfgList = cfgGroup:AddListbox({
        Name = "Saved Configs",
        Flag = "UI_ConfigList",
        Height = 95,
        Values = cfgNames,
        Default = "legit_default",
        Callback = function(val)
            selectedCfg = val
            cfgName = val
            if cfgNameInput then
                cfgNameInput:Set(val)
            end
        end
    })

    local createBtn = cfgGroup:AddButton({
        Name = "Save Config",
        Icon = "save",
        Callback = function()
            local target = (cfgName and cfgName ~= "") and cfgName or selectedCfg or "legit_default"
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
            selectedCfg = target
            cfgList:Set(target)
            self:Notify({ Title = "Config Saved", Description = "Saved profile '" .. target .. "'.", Icon = "save" })
        end
    })

    createBtn:AddButton({
        Name = "Load Config",
        Icon = "download",
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

    local autoBtn = cfgGroup:AddButton({
        Name = "Set Autoload",
        Icon = "shield-check",
        Callback = function()
            local target = selectedCfg or (cfgName ~= "" and cfgName) or "legit_default"
            autoloadCfg = target
            autoStatusLbl:SetText("Autoload Profile: " .. autoloadCfg)
            pcall(function()
                if isfolder and makefolder and writefile then
                    if not isfolder(self.Folder) then makefolder(self.Folder) end
                    writefile(self.Folder .. "/autoload.txt", target)
                    if not isfile or not isfile(self.Folder .. "/" .. target .. ".json") then
                        writefile(self.Folder .. "/" .. target .. ".json", savedMemoryConfigs[target] or self:GetConfigJSON())
                    end
                end
            end)
            self:Notify({ Title = "Autoload Set", Description = "Profile '" .. target .. "' will auto-load on launch.", Icon = "shield-check" })
        end
    })

    autoBtn:AddButton({
        Name = "Clear Autoload",
        Icon = "x",
        Callback = function()
            autoloadCfg = "none"
            autoStatusLbl:SetText("Autoload Profile: none")
            pcall(function()
                if delfile and isfile and isfile(self.Folder .. "/autoload.txt") then
                    delfile(self.Folder .. "/autoload.txt")
                elseif writefile then
                    writefile(self.Folder .. "/autoload.txt", "")
                end
            end)
            self:Notify({ Title = "Autoload Cleared", Description = "Disabled automatic config loading.", Icon = "info" })
        end
    })

    local deleteBtn = cfgGroup:AddButton({
        Name = "Delete Config",
        Icon = "trash",
        Risky = true,
        Confirm = true,
        Callback = function()
            local target = selectedCfg
            if not target then return end
            savedMemoryConfigs[target] = nil
            pcall(function()
                if delfile and isfile and isfile(self.Folder .. "/" .. target .. ".json") then
                    delfile(self.Folder .. "/" .. target .. ".json")
                end
            end)
            local idx = table.find(cfgList.Values, target)
            if idx and #cfgList.Values > 1 then
                table.remove(cfgList.Values, idx)
                selectedCfg = cfgList.Values[1]
                cfgName = selectedCfg
                cfgList:Refresh(cfgList.Values)
                cfgList:Set(selectedCfg)
                if cfgNameInput then
                    cfgNameInput:Set(selectedCfg)
                end
            end
            self:Notify({ Title = "Config Deleted", Description = "Removed profile '" .. tostring(target) .. "'.", Icon = "trash", Color = self.Theme.Risky })
        end
    })

    local rawJsonInput = cfgGroup:AddInput({
        Name = "Clipboard / Raw JSON Payload",
        Flag = "UI_RawConfigJSON",
        Default = "",
        Placeholder = "Paste JSON string or use Import...",
    })

    deleteBtn:AddButton({
        Name = "Export JSON",
        Icon = "clipboard",
        Callback = function()
            local json = self:GetConfigJSON()
            if rawJsonInput then
                rawJsonInput:Set(json)
            end
            pcall(function()
                if setclipboard then setclipboard(json) end
            end)
            self:Notify({ Title = "Clipboard", Description = "Exported JSON config string.", Icon = "clipboard" })
        end
    })

    cfgGroup:AddButton({
        Name = "Import JSON (Input / Clipboard)",
        Icon = "upload",
        Callback = function()
            local payload = rawJsonInput and rawJsonInput.Value or ""
            if not payload or payload == "" then
                pcall(function()
                    if getclipboard then
                        payload = getclipboard()
                    end
                end)
            end
            if payload and payload ~= "" then
                self:LoadConfigJSON(payload)
            else
                self:Notify({ Title = "Import Error", Description = "Paste a JSON string into the box first.", Icon = "alert-triangle", Color = self.Theme.Risky })
            end
        end
    })

    -- Automatically execute autoload profile if present on disk
    if autoloadCfg ~= "none" then
        task.defer(function()
            local raw = savedMemoryConfigs[autoloadCfg]
            pcall(function()
                if isfile and readfile and isfile(self.Folder .. "/" .. autoloadCfg .. ".json") then
                    raw = readfile(self.Folder .. "/" .. autoloadCfg .. ".json")
                end
            end)
            if raw then
                self:LoadConfigJSON(raw)
            end
        end)
    end
end

function Library:Unload()
    self.Unloaded = true
    if self.WorldController and self.WorldController.Cleanup then
        pcall(self.WorldController.Cleanup)
    end
    for _, conn in ipairs(self.Connections) do
        pcall(function() conn:Disconnect() end)
    end
    table.clear(self.Connections)
    if ScreenGui and ScreenGui.Parent then
        ScreenGui:Destroy()
    end
end

pcall(function()
    if getgenv then
        getgenv().ShardsLibrary = Library
    end
    _G.ShardsLibrary = Library
end)

return Library
