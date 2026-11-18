--!nocheck
-- SHARDS.CC // Multi-Game Loader (Matching ShardsUI CS:GO Aesthetic)

local TweenService = game:GetService("TweenService")
local UserInputService = game:GetService("UserInputService")
local MarketplaceService = game:GetService("MarketplaceService")
local HttpService = game:GetService("HttpService")
local Players = game:GetService("Players")
local CoreGui = game:GetService("CoreGui")

local LocalPlayer = Players.LocalPlayer

-- Clean up any existing loader instance
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
    return LocalPlayer and LocalPlayer:WaitForChild("PlayerGui")
end

local SafeParent = GetSafeParent()
if SafeParent then
    local old = SafeParent:FindFirstChild("ShardsCSGO_Loader")
    if old then
        pcall(function() old:Destroy() end)
    end
end

-- ============================================================================
-- THEME & PRESETS (Matches ShardsUI.lua 1:1)
-- ============================================================================
local Theme = {
    Background      = Color3.fromRGB(11, 11, 14),
    Sidebar         = Color3.fromRGB(14, 14, 18),
    Groupbox        = Color3.fromRGB(16, 16, 21),
    Element         = Color3.fromRGB(22, 22, 29),
    ElementHover    = Color3.fromRGB(28, 28, 37),
    OuterBorder     = Color3.fromRGB(5, 5, 7),
    InnerBorder     = Color3.fromRGB(38, 38, 48),
    HighlightBorder = Color3.fromRGB(58, 58, 72),
    Accent          = Color3.fromRGB(155, 195, 55),
    AccentDark      = Color3.fromRGB(98, 128, 28),
    Text            = Color3.fromRGB(228, 228, 235),
    TextDim         = Color3.fromRGB(142, 142, 156),
    TextMuted       = Color3.fromRGB(92, 92, 106),
    Risky           = Color3.fromRGB(235, 75, 75),
    Success         = Color3.fromRGB(110, 215, 95),
    Warning         = Color3.fromRGB(240, 180, 65),
}

local Presets = {
    ["Skeet Lime"] = { Accent = Color3.fromRGB(155, 195, 55),  AccentDark = Color3.fromRGB(98, 128, 28)  },
    ["Neverlose"]  = { Accent = Color3.fromRGB(55, 165, 245),  AccentDark = Color3.fromRGB(25, 105, 175) },
    ["Fatality"]   = { Accent = Color3.fromRGB(225, 45, 95),   AccentDark = Color3.fromRGB(145, 20, 58)  },
    ["Onetap"]     = { Accent = Color3.fromRGB(235, 155, 45),  AccentDark = Color3.fromRGB(165, 98, 18)  },
    ["Primordial"] = { Accent = Color3.fromRGB(185, 115, 245), AccentDark = Color3.fromRGB(115, 62, 168) },
    ["Aimware"]    = { Accent = Color3.fromRGB(220, 50, 50),   AccentDark = Color3.fromRGB(140, 25, 25)  },
}
local PresetOrder = { "Skeet Lime", "Neverlose", "Fatality", "Onetap", "Primordial", "Aimware" }

local Icons = {
    ["crosshair"] = "rbxassetid://10709818534",
    ["scan"]      = "rbxassetid://10734942565",
    ["cpu"]       = "rbxassetid://10709818399",
    ["sliders"]   = "rbxassetid://10734963400",
    ["check"]     = "rbxassetid://10709790644",
    ["x"]         = "rbxassetid://10747383819",
    ["search"]    = "rbxassetid://10734943674",
    ["play"]      = "rbxassetid://10747384394",
    ["palette"]   = "rbxassetid://10734910430",
    ["globe"]     = "rbxassetid://10723404337",
}

-- ============================================================================
-- MULTI-GAME REGISTRY (Exact PlaceIds & Universe GameIds)
-- ============================================================================
local GamesRegistry = {
    {
        Id = "bloxstrike",
        Name = "BloxStrike",
        SubTitle = "5v5 competitive // defusal & zombies",
        Version = "v2.4.0",
        Status = "ONLINE",
        PlaceIds = { 114234929420007 },
        GameIds = { 7633926880 },
        Keywords = { "bloxstrike" },
        LocalFile = "Example.lua",
        RemoteUrl = "https://raw.githubusercontent.com/jonpauli/Shards/refs/heads/main/Example.lua",
        Features = {
            "PlaceId: 114234929420007",
            "5v5 Tactical & Zombies Support",
            "2D ESP, Chams & Crosshair HUD",
            "Theme & JSON Config Manager",
        },
    },
    {
        Id = "apoc2",
        Name = "Apocalypse Rising 2",
        SubTitle = "dualpoint // open-world survival",
        Version = "v2.4.0",
        Status = "ONLINE",
        PlaceIds = { 863266079 },
        GameIds = { 358276974 },
        Keywords = { "apocalypse rising 2" },
        LocalFile = "Apoc2.lua",
        RemoteUrl = "https://raw.githubusercontent.com/jonpauli/Shards/refs/heads/main/Apoc2.lua",
        Features = {
            "PlaceId: 863266079",
            "Long-Range Player & Loot Visuals",
            "2D Radar & Off-Screen Arrows",
            "World Lighting & Fullbright",
        },
    },
    {
        Id = "coldwar",
        Name = "Cold War",
        SubTitle = "grip studios // 80-player combat",
        Version = "v2.4.0",
        Status = "ONLINE",
        PlaceIds = { 13687899540 },
        GameIds = { 4750561026 },
        Keywords = { "cold war" },
        LocalFile = "Example.lua",
        RemoteUrl = "https://raw.githubusercontent.com/jonpauli/Shards/refs/heads/main/Example.lua",
        Features = {
            "PlaceId: 13687899540",
            "80-Player R6 Combat Visuals",
            "Team-Aware Box, Skeleton & Chams",
            "Custom Crosshair & Telemetry HUD",
        },
    },
    {
        Id = "phantomforces",
        Name = "Phantom Forces",
        SubTitle = "stylis studios // tactical fps",
        Version = "v2.4.0",
        Status = "ONLINE",
        PlaceIds = { 292439477 },
        GameIds = { 113491250 },
        Keywords = { "phantom forces" },
        LocalFile = "Example.lua",
        RemoteUrl = "https://raw.githubusercontent.com/jonpauli/Shards/refs/heads/main/Example.lua",
        Features = {
            "PlaceId: 292439477",
            "Long-Range ESP & 2D Radar",
            "Custom Lighting & Nightmode",
            "Crosshair & Hitmarker Audio",
        },
    },
}

-- ============================================================================
-- PERSISTENT LOADER SETTINGS
-- ============================================================================
local LoaderFolder = "ShardsCSGO"
local LoaderConfigFile = LoaderFolder .. "/loader.json"
local LoaderSettings = {
    Theme = "Skeet Lime",
    AutoLoad = false,
    LastGameId = nil,
}

local function SaveLoaderSettings()
    pcall(function()
        if isfolder and makefolder and writefile then
            if not isfolder(LoaderFolder) then
                makefolder(LoaderFolder)
            end
            writefile(LoaderConfigFile, HttpService:JSONEncode(LoaderSettings))
        end
    end)
end

local function LoadLoaderSettings()
    pcall(function()
        if isfile and readfile and isfile(LoaderConfigFile) then
            local data = HttpService:JSONDecode(readfile(LoaderConfigFile))
            if type(data) == "table" then
                if type(data.Theme) == "string" and Presets[data.Theme] then
                    LoaderSettings.Theme = data.Theme
                end
                if type(data.AutoLoad) == "boolean" then
                    LoaderSettings.AutoLoad = data.AutoLoad
                end
                if type(data.LastGameId) == "string" then
                    LoaderSettings.LastGameId = data.LastGameId
                end
            end
        end
    end)
end

LoadLoaderSettings()
if Presets[LoaderSettings.Theme] then
    Theme.Accent = Presets[LoaderSettings.Theme].Accent
    Theme.AccentDark = Presets[LoaderSettings.Theme].AccentDark
end

-- ============================================================================
-- UI HELPER FUNCTIONS
-- ============================================================================
local TweenFast = TweenInfo.new(0.14, Enum.EasingStyle.Quart, Enum.EasingDirection.Out)
local TweenSmooth = TweenInfo.new(0.26, Enum.EasingStyle.Quint, Enum.EasingDirection.Out)

local AccentObjects = {}
local GradientObjects = {}
local Connections = {}

local function Connect(sig, fn)
    local c = sig:Connect(fn)
    table.insert(Connections, c)
    return c
end

local function Tween(inst, info, props)
    local tw = TweenService:Create(inst, info or TweenFast, props)
    tw:Play()
    return tw
end

local function Create(className, props, children)
    local inst = Instance.new(className)
    for k, v in pairs(props or {}) do
        inst[k] = v
    end
    if inst:IsA("TextLabel") or inst:IsA("TextButton") or inst:IsA("TextBox") then
        inst.TextStrokeColor3 = Color3.new(0, 0, 0)
        inst.TextStrokeTransparency = 0.18
    end
    for _, ch in ipairs(children or {}) do
        ch.Parent = inst
    end
    return inst
end

local function BindAccent(inst, prop)
    table.insert(AccentObjects, { Instance = inst, Property = prop })
    inst[prop] = Theme.Accent
end

local function ApplyCSBorder(frame, borderCol)
    frame.BorderSizePixel = 0
    local innerStroke = Create("UIStroke", {
        Color = borderCol or Theme.InnerBorder,
        Thickness = 1,
        ApplyStrokeMode = Enum.ApplyStrokeMode.Border,
        Parent = frame,
    })
    local outerWrap = Create("Frame", {
        BackgroundTransparency = 1,
        Position = UDim2.new(0, -1, 0, -1),
        Size = UDim2.new(1, 2, 1, 2),
        ZIndex = math.max(1, frame.ZIndex - 1),
        Parent = frame,
    })
    Create("UIStroke", {
        Color = Theme.OuterBorder,
        Thickness = 1,
        ApplyStrokeMode = Enum.ApplyStrokeMode.Border,
        Parent = outerWrap,
    })
    return innerStroke
end

local function CreateAccentBar(parent, zIndex)
    local bar = Create("Frame", {
        BackgroundColor3 = Color3.new(1, 1, 1),
        BorderSizePixel = 0,
        Size = UDim2.new(1, 0, 0, 2),
        ZIndex = zIndex or 10,
        Parent = parent,
    })
    local grad = Create("UIGradient", {
        Color = ColorSequence.new({
            ColorSequenceKeypoint.new(0.0, Theme.AccentDark),
            ColorSequenceKeypoint.new(0.5, Theme.Accent),
            ColorSequenceKeypoint.new(1.0, Theme.AccentDark),
        }),
        Parent = bar,
    })
    table.insert(GradientObjects, grad)
    return bar
end

local function RefreshAccentTheme()
    for _, item in ipairs(AccentObjects) do
        if item.Instance and item.Instance.Parent then
            Tween(item.Instance, TweenFast, { [item.Property] = Theme.Accent })
        end
    end
    for _, grad in ipairs(GradientObjects) do
        if grad and grad.Parent then
            grad.Color = ColorSequence.new({
                ColorSequenceKeypoint.new(0.0, Theme.AccentDark),
                ColorSequenceKeypoint.new(0.5, Theme.Accent),
                ColorSequenceKeypoint.new(1.0, Theme.AccentDark),
            })
        end
    end
end

-- ============================================================================
-- AUTO-DETECT CURRENT GAME
-- ============================================================================
local currentPlaceId = game.PlaceId
local currentGameId = game.GameId
local currentPlaceName = ""
pcall(function()
    local info = MarketplaceService:GetProductInfo(currentPlaceId)
    if info and info.Name then
        currentPlaceName = tostring(info.Name)
    end
end)

local detectedGame = nil
local lowerPlaceName = string.lower(currentPlaceName)

for _, g in ipairs(GamesRegistry) do
    if table.find(g.PlaceIds, currentPlaceId) or table.find(g.GameIds, currentGameId) then
        detectedGame = g
        break
    end
    if lowerPlaceName ~= "" then
        for _, kw in ipairs(g.Keywords) do
            if string.find(lowerPlaceName, string.lower(kw), 1, true) then
                detectedGame = g
                break
            end
        end
    end
    if detectedGame then break end
end

local selectedGame = detectedGame or GamesRegistry[1]

-- ============================================================================
-- BUILD LOADER INTERFACE
-- ============================================================================
local ScreenGui = Create("ScreenGui", {
    Name = "ShardsCSGO_Loader",
    ResetOnSpawn = false,
    IgnoreGuiInset = true,
    ZIndexBehavior = Enum.ZIndexBehavior.Sibling,
    DisplayOrder = 1000,
    Parent = SafeParent,
})

local MainFrame = Create("Frame", {
    Name = "LoaderWindow",
    AnchorPoint = Vector2.new(0.5, 0.5),
    BackgroundColor3 = Theme.Background,
    Position = UDim2.fromScale(0.5, 0.5),
    Size = UDim2.fromOffset(520, 360),
    ZIndex = 10,
    Parent = ScreenGui,
})
ApplyCSBorder(MainFrame, Theme.InnerBorder)
CreateAccentBar(MainFrame, 15)

-- Header Bar
local Header = Create("Frame", {
    BackgroundColor3 = Theme.Sidebar,
    BorderSizePixel = 0,
    Position = UDim2.fromOffset(0, 2),
    Size = UDim2.new(1, 0, 0, 32),
    ZIndex = 11,
    Parent = MainFrame,
})

Create("Frame", {
    BackgroundColor3 = Theme.InnerBorder,
    BorderSizePixel = 0,
    Position = UDim2.new(0, 0, 1, -1),
    Size = UDim2.new(1, 0, 0, 1),
    ZIndex = 12,
    Parent = Header,
})

local HeaderIcon = Create("ImageLabel", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(10, 8),
    Size = UDim2.fromOffset(15, 15),
    Image = Icons["crosshair"],
    ZIndex = 12,
    Parent = Header,
})
BindAccent(HeaderIcon, "ImageColor3")

Create("TextLabel", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(31, 0),
    Size = UDim2.fromOffset(90, 32),
    Font = Enum.Font.GothamBold,
    Text = "SHARDS.CC",
    TextColor3 = Theme.Text,
    TextSize = 13,
    TextXAlignment = Enum.TextXAlignment.Left,
    ZIndex = 12,
    Parent = Header,
})

Create("TextLabel", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(112, 0),
    Size = UDim2.fromOffset(260, 32),
    Font = Enum.Font.Code,
    Text = "// multi-game loader v2.4",
    TextColor3 = Theme.TextMuted,
    TextSize = 12,
    TextXAlignment = Enum.TextXAlignment.Left,
    ZIndex = 12,
    Parent = Header,
})

local CloseBtn = Create("TextButton", {
    AnchorPoint = Vector2.new(1, 0.5),
    BackgroundColor3 = Theme.Element,
    AutoButtonColor = false,
    Position = UDim2.new(1, -8, 0.5, 0),
    Size = UDim2.fromOffset(20, 20),
    Text = "",
    ZIndex = 13,
    Parent = Header,
})
local closeStroke = ApplyCSBorder(CloseBtn, Theme.InnerBorder)
local closeIcon = Create("ImageLabel", {
    AnchorPoint = Vector2.new(0.5, 0.5),
    BackgroundTransparency = 1,
    Position = UDim2.fromScale(0.5, 0.5),
    Size = UDim2.fromOffset(11, 11),
    Image = Icons["x"],
    ImageColor3 = Theme.TextDim,
    ZIndex = 14,
    Parent = CloseBtn,
})

-- Make Window Draggable
do
    local dragging = false
    local dragStart, startPos
    Connect(Header.InputBegan, function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            dragging = true
            dragStart = input.Position
            startPos = MainFrame.Position
        end
    end)
    Connect(UserInputService.InputChanged, function(input)
        if dragging and input.UserInputType == Enum.UserInputType.MouseMovement then
            local delta = input.Position - dragStart
            Tween(MainFrame, TweenInfo.new(0.06, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {
                Position = UDim2.new(startPos.X.Scale, startPos.X.Offset + delta.X, startPos.Y.Scale, startPos.Y.Offset + delta.Y)
            })
        end
    end)
    Connect(UserInputService.InputEnded, function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            dragging = false
        end
    end)
end

-- ============================================================================
-- LEFT PANEL: GAME SELECTOR LIST
-- ============================================================================
local LeftPanel = Create("Frame", {
    BackgroundColor3 = Theme.Groupbox,
    Position = UDim2.fromOffset(10, 44),
    Size = UDim2.fromOffset(215, 272),
    ZIndex = 11,
    Parent = MainFrame,
})
ApplyCSBorder(LeftPanel, Theme.InnerBorder)

local LeftHeader = Create("Frame", {
    BackgroundColor3 = Theme.Sidebar,
    BorderSizePixel = 0,
    Size = UDim2.new(1, 0, 0, 24),
    ZIndex = 12,
    Parent = LeftPanel,
})
Create("Frame", {
    BackgroundColor3 = Theme.InnerBorder,
    BorderSizePixel = 0,
    Position = UDim2.new(0, 0, 1, -1),
    Size = UDim2.new(1, 0, 0, 1),
    ZIndex = 13,
    Parent = LeftHeader,
})
local leftHdrIcon = Create("ImageLabel", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(8, 6),
    Size = UDim2.fromOffset(12, 12),
    Image = Icons["globe"],
    ZIndex = 13,
    Parent = LeftHeader,
})
BindAccent(leftHdrIcon, "ImageColor3")
Create("TextLabel", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(24, 0),
    Size = UDim2.new(1, -28, 1, 0),
    Font = Enum.Font.GothamBold,
    Text = "SUPPORTED GAMES",
    TextColor3 = Theme.Text,
    TextSize = 11,
    TextXAlignment = Enum.TextXAlignment.Left,
    ZIndex = 13,
    Parent = LeftHeader,
})

-- Search Filter Box
local SearchWrap = Create("Frame", {
    BackgroundColor3 = Theme.Background,
    Position = UDim2.fromOffset(7, 30),
    Size = UDim2.new(1, -14, 0, 22),
    ZIndex = 12,
    Parent = LeftPanel,
})
ApplyCSBorder(SearchWrap, Theme.InnerBorder)
Create("ImageLabel", {
    AnchorPoint = Vector2.new(0, 0.5),
    BackgroundTransparency = 1,
    Position = UDim2.new(0, 6, 0.5, 0),
    Size = UDim2.fromOffset(11, 11),
    Image = Icons["search"],
    ImageColor3 = Theme.TextMuted,
    ZIndex = 13,
    Parent = SearchWrap,
})
local SearchBox = Create("TextBox", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(22, 0),
    Size = UDim2.new(1, -26, 1, 0),
    Font = Enum.Font.Code,
    PlaceholderText = "filter games...",
    PlaceholderColor3 = Theme.TextMuted,
    Text = "",
    TextColor3 = Theme.Text,
    TextSize = 11,
    TextXAlignment = Enum.TextXAlignment.Left,
    ClearTextOnFocus = false,
    ZIndex = 13,
    Parent = SearchWrap,
})

local GameScroll = Create("ScrollingFrame", {
    BackgroundTransparency = 1,
    BorderSizePixel = 0,
    Position = UDim2.fromOffset(7, 58),
    Size = UDim2.new(1, -14, 1, -65),
    CanvasSize = UDim2.fromOffset(0, 0),
    AutomaticCanvasSize = Enum.AutomaticSize.Y,
    ScrollBarThickness = 2,
    ScrollBarImageColor3 = Theme.Accent,
    ZIndex = 12,
    Parent = LeftPanel,
})
BindAccent(GameScroll, "ScrollBarImageColor3")
Create("UIListLayout", {
    Padding = UDim.new(0, 5),
    SortOrder = Enum.SortOrder.LayoutOrder,
    Parent = GameScroll,
})

-- ============================================================================
-- RIGHT PANEL: SELECTED GAME TELEMETRY & OPTIONS
-- ============================================================================
local RightPanel = Create("Frame", {
    BackgroundColor3 = Theme.Groupbox,
    Position = UDim2.fromOffset(235, 44),
    Size = UDim2.fromOffset(275, 272),
    ZIndex = 11,
    Parent = MainFrame,
})
ApplyCSBorder(RightPanel, Theme.InnerBorder)

local RightHeader = Create("Frame", {
    BackgroundColor3 = Theme.Sidebar,
    BorderSizePixel = 0,
    Size = UDim2.new(1, 0, 0, 24),
    ZIndex = 12,
    Parent = RightPanel,
})
Create("Frame", {
    BackgroundColor3 = Theme.InnerBorder,
    BorderSizePixel = 0,
    Position = UDim2.new(0, 0, 1, -1),
    Size = UDim2.new(1, 0, 0, 1),
    ZIndex = 13,
    Parent = RightHeader,
})
local rightHdrIcon = Create("ImageLabel", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(8, 6),
    Size = UDim2.fromOffset(12, 12),
    Image = Icons["cpu"],
    ZIndex = 13,
    Parent = RightHeader,
})
BindAccent(rightHdrIcon, "ImageColor3")
local RightHdrTitle = Create("TextLabel", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(24, 0),
    Size = UDim2.new(1, -80, 1, 0),
    Font = Enum.Font.GothamBold,
    Text = "MODULE TELEMETRY",
    TextColor3 = Theme.Text,
    TextSize = 11,
    TextXAlignment = Enum.TextXAlignment.Left,
    ZIndex = 13,
    Parent = RightHeader,
})
local StatusBadge = Create("TextLabel", {
    AnchorPoint = Vector2.new(1, 0.5),
    BackgroundTransparency = 1,
    Position = UDim2.new(1, -8, 0.5, 0),
    Size = UDim2.fromOffset(75, 16),
    Font = Enum.Font.GothamBold,
    Text = "[ONLINE]",
    TextColor3 = Theme.Success,
    TextSize = 10,
    TextXAlignment = Enum.TextXAlignment.Right,
    ZIndex = 13,
    Parent = RightHeader,
})

-- Selected Game Title & Subtitle Card
local InfoCard = Create("Frame", {
    BackgroundColor3 = Theme.Element,
    Position = UDim2.fromOffset(8, 32),
    Size = UDim2.new(1, -16, 0, 56),
    ZIndex = 12,
    Parent = RightPanel,
})
ApplyCSBorder(InfoCard, Theme.InnerBorder)

local GameTitleLbl = Create("TextLabel", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(8, 5),
    Size = UDim2.new(1, -60, 0, 16),
    Font = Enum.Font.GothamBold,
    Text = selectedGame.Name,
    TextColor3 = Theme.Text,
    TextSize = 13,
    TextXAlignment = Enum.TextXAlignment.Left,
    ZIndex = 13,
    Parent = InfoCard,
})

local VersionBadge = Create("TextLabel", {
    AnchorPoint = Vector2.new(1, 0),
    BackgroundTransparency = 1,
    Position = UDim2.new(1, -8, 0, 6),
    Size = UDim2.fromOffset(50, 14),
    Font = Enum.Font.Code,
    Text = selectedGame.Version,
    TextSize = 11,
    TextXAlignment = Enum.TextXAlignment.Right,
    ZIndex = 13,
    Parent = InfoCard,
})
BindAccent(VersionBadge, "TextColor3")

local GameSubLbl = Create("TextLabel", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(8, 21),
    Size = UDim2.new(1, -16, 0, 14),
    Font = Enum.Font.Code,
    Text = selectedGame.SubTitle,
    TextColor3 = Theme.TextDim,
    TextSize = 11,
    TextXAlignment = Enum.TextXAlignment.Left,
    ZIndex = 13,
    Parent = InfoCard,
})

local execName = "Standard"
pcall(function()
    if identifyexecutor then
        local n = identifyexecutor()
        if n then execName = tostring(n) end
    end
end)

local PlaceInfoLbl = Create("TextLabel", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(8, 37),
    Size = UDim2.new(1, -16, 0, 14),
    Font = Enum.Font.Code,
    Text = string.format("PlaceId: %d  |  Env: %s", currentPlaceId, execName),
    TextColor3 = Theme.TextMuted,
    TextSize = 10,
    TextXAlignment = Enum.TextXAlignment.Left,
    ZIndex = 13,
    Parent = InfoCard,
})

-- Feature List Box
local FeatBox = Create("Frame", {
    BackgroundColor3 = Theme.Background,
    Position = UDim2.fromOffset(8, 95),
    Size = UDim2.new(1, -16, 0, 78),
    ZIndex = 12,
    Parent = RightPanel,
})
ApplyCSBorder(FeatBox, Theme.InnerBorder)

local FeatLayout = Create("UIListLayout", {
    Padding = UDim.new(0, 3),
    SortOrder = Enum.SortOrder.LayoutOrder,
    Parent = FeatBox,
})
Create("UIPadding", {
    PaddingLeft = UDim.new(0, 8),
    PaddingTop = UDim.new(0, 6),
    Parent = FeatBox,
})

local featLabels = {}
for i = 1, 4 do
    local fl = Create("TextLabel", {
        BackgroundTransparency = 1,
        LayoutOrder = i,
        Size = UDim2.new(1, -12, 0, 14),
        Font = Enum.Font.Code,
        Text = "",
        TextColor3 = Theme.TextDim,
        TextSize = 11,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 13,
        Parent = FeatBox,
    })
    featLabels[i] = fl
end

-- Theme Preset Swatch Row
local ThemeLabel = Create("TextLabel", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(9, 180),
    Size = UDim2.fromOffset(95, 18),
    Font = Enum.Font.Code,
    Text = "Accent Theme:",
    TextColor3 = Theme.TextDim,
    TextSize = 11,
    TextXAlignment = Enum.TextXAlignment.Left,
    ZIndex = 12,
    Parent = RightPanel,
})

local SwatchTray = Create("Frame", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(105, 180),
    Size = UDim2.new(1, -113, 0, 18),
    ZIndex = 12,
    Parent = RightPanel,
})
Create("UIListLayout", {
    FillDirection = Enum.FillDirection.Horizontal,
    HorizontalAlignment = Enum.HorizontalAlignment.Right,
    Padding = UDim.new(0, 5),
    Parent = SwatchTray,
})

local swatchStrokes = {}
for _, pName in ipairs(PresetOrder) do
    local pData = Presets[pName]
    local sw = Create("TextButton", {
        BackgroundColor3 = pData.Accent,
        AutoButtonColor = false,
        Size = UDim2.fromOffset(18, 16),
        Text = "",
        ZIndex = 13,
        Parent = SwatchTray,
    })
    local st = ApplyCSBorder(sw, (LoaderSettings.Theme == pName) and Color3.new(1, 1, 1) or Theme.InnerBorder)
    swatchStrokes[pName] = st
    Connect(sw.MouseButton1Click, function()
        LoaderSettings.Theme = pName
        Theme.Accent = pData.Accent
        Theme.AccentDark = pData.AccentDark
        for k, s in pairs(swatchStrokes) do
            Tween(s, TweenFast, { Color = (k == pName) and Color3.new(1, 1, 1) or Theme.InnerBorder })
        end
        RefreshAccentTheme()
        SaveLoaderSettings()
    end)
end

-- Auto-Load Toggle Row
local AutoRow = Create("Frame", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(9, 204),
    Size = UDim2.new(1, -18, 0, 18),
    ZIndex = 12,
    Parent = RightPanel,
})
local AutoBox = Create("Frame", {
    AnchorPoint = Vector2.new(0, 0.5),
    BackgroundColor3 = LoaderSettings.AutoLoad and Theme.Accent or Theme.Element,
    Position = UDim2.new(0, 0, 0.5, 0),
    Size = UDim2.fromOffset(12, 12),
    ZIndex = 13,
    Parent = AutoRow,
})
local autoStroke = ApplyCSBorder(AutoBox, LoaderSettings.AutoLoad and Theme.Accent or Theme.InnerBorder)
local AutoLbl = Create("TextLabel", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(18, 0),
    Size = UDim2.new(1, -18, 1, 0),
    Font = Enum.Font.Code,
    Text = "Auto-load detected game on launch",
    TextColor3 = LoaderSettings.AutoLoad and Theme.Text or Theme.TextDim,
    TextSize = 11,
    TextXAlignment = Enum.TextXAlignment.Left,
    ZIndex = 13,
    Parent = AutoRow,
})
local AutoBtn = Create("TextButton", {
    BackgroundTransparency = 1,
    Size = UDim2.fromScale(1, 1),
    Text = "",
    ZIndex = 14,
    Parent = AutoRow,
})
Connect(AutoBtn.MouseButton1Click, function()
    LoaderSettings.AutoLoad = not LoaderSettings.AutoLoad
    Tween(AutoBox, TweenFast, { BackgroundColor3 = LoaderSettings.AutoLoad and Theme.Accent or Theme.Element })
    Tween(autoStroke, TweenFast, { Color = LoaderSettings.AutoLoad and Theme.Accent or Theme.InnerBorder })
    Tween(AutoLbl, TweenFast, { TextColor3 = LoaderSettings.AutoLoad and Theme.Text or Theme.TextDim })
    SaveLoaderSettings()
end)

-- Action Buttons (LOAD MODULE + UNIVERSAL)
local LoadBtn = Create("TextButton", {
    BackgroundColor3 = Color3.new(1, 1, 1),
    AutoButtonColor = false,
    Position = UDim2.fromOffset(8, 232),
    Size = UDim2.new(1, -16, 0, 30),
    Font = Enum.Font.GothamBold,
    Text = "LOAD SELECTED MODULE",
    TextColor3 = Color3.fromRGB(10, 10, 12),
    TextSize = 12,
    ZIndex = 13,
    Parent = RightPanel,
})
LoadBtn.TextStrokeTransparency = 0.75
ApplyCSBorder(LoadBtn, Theme.HighlightBorder)
local loadGrad = Create("UIGradient", {
    Rotation = 90,
    Color = ColorSequence.new({
        ColorSequenceKeypoint.new(0, Theme.Accent),
        ColorSequenceKeypoint.new(1, Theme.AccentDark),
    }),
    Parent = LoadBtn,
})
table.insert(GradientObjects, loadGrad)

-- Bottom Console Status & Progress Bar
local BottomBar = Create("Frame", {
    BackgroundColor3 = Theme.Groupbox,
    Position = UDim2.fromOffset(10, 322),
    Size = UDim2.new(1, -20, 0, 28),
    ZIndex = 11,
    Parent = MainFrame,
})
ApplyCSBorder(BottomBar, Theme.InnerBorder)

local StatusText = Create("TextLabel", {
    BackgroundTransparency = 1,
    Position = UDim2.fromOffset(8, 0),
    Size = UDim2.new(1, -16, 1, -3),
    Font = Enum.Font.Code,
    Text = detectedGame
        and string.format("[shards.cc] auto-detected '%s' (%d)", detectedGame.Name, currentPlaceId)
        or string.format("[shards.cc] ready // place %d (universal profile)", currentPlaceId),
    TextColor3 = Theme.TextDim,
    TextSize = 11,
    TextXAlignment = Enum.TextXAlignment.Left,
    ZIndex = 12,
    Parent = BottomBar,
})

local ProgTrack = Create("Frame", {
    BackgroundColor3 = Theme.Element,
    BorderSizePixel = 0,
    Position = UDim2.new(0, 0, 1, -2),
    Size = UDim2.new(1, 0, 0, 2),
    ZIndex = 12,
    Parent = BottomBar,
})
local ProgFill = Create("Frame", {
    BackgroundColor3 = Theme.Accent,
    BorderSizePixel = 0,
    Size = UDim2.fromScale(0, 1),
    ZIndex = 13,
    Parent = ProgTrack,
})
BindAccent(ProgFill, "BackgroundColor3")

-- ============================================================================
-- POPULATE GAME CARDS & SELECTION LOGIC
-- ============================================================================
local gameCards = {}
local isLoading = false

local function GetStatusColor(status)
    if status == "ONLINE" then return Theme.Success end
    if status == "BETA" then return Theme.Warning end
    return Theme.Risky
end

local function SelectGame(gameEntry)
    selectedGame = gameEntry
    GameTitleLbl.Text = gameEntry.Name
    VersionBadge.Text = gameEntry.Version
    GameSubLbl.Text = gameEntry.SubTitle
    StatusBadge.Text = "[" .. tostring(gameEntry.Status) .. "]"
    StatusBadge.TextColor3 = GetStatusColor(gameEntry.Status)

    for i = 1, 4 do
        local fText = gameEntry.Features and gameEntry.Features[i]
        featLabels[i].Text = fText and ("+  " .. fText) or ""
    end

    for _, card in ipairs(gameCards) do
        local isSel = (card.Game == gameEntry)
        Tween(card.Frame, TweenFast, {
            BackgroundColor3 = isSel and Theme.ElementHover or Theme.Element
        })
        Tween(card.Stroke, TweenFast, {
            Color = isSel and Theme.Accent or Theme.InnerBorder
        })
        Tween(card.Bar, TweenFast, {
            BackgroundTransparency = isSel and 0 or 1
        })
        Tween(card.Title, TweenFast, {
            TextColor3 = isSel and Theme.Text or Theme.TextDim
        })
    end
end

for idx, g in ipairs(GamesRegistry) do
    local isDet = (detectedGame == g)
    local row = Create("TextButton", {
        Name = "Game_" .. g.Id,
        BackgroundColor3 = Theme.Element,
        AutoButtonColor = false,
        LayoutOrder = isDet and 0 or idx,
        Size = UDim2.new(1, -4, 0, 38),
        Text = "",
        ZIndex = 13,
        Parent = GameScroll,
    })
    local stroke = ApplyCSBorder(row, Theme.InnerBorder)

    local leftBar = Create("Frame", {
        BackgroundColor3 = Theme.Accent,
        BackgroundTransparency = 1,
        BorderSizePixel = 0,
        Size = UDim2.new(0, 2, 1, 0),
        ZIndex = 14,
        Parent = row,
    })
    BindAccent(leftBar, "BackgroundColor3")

    local dot = Create("Frame", {
        AnchorPoint = Vector2.new(0, 0.5),
        BackgroundColor3 = GetStatusColor(g.Status),
        BorderSizePixel = 0,
        Position = UDim2.new(0, 9, 0, 12),
        Size = UDim2.fromOffset(5, 5),
        ZIndex = 14,
        Parent = row,
    })
    Create("UICorner", { CornerRadius = UDim.new(1, 0), Parent = dot })

    local title = Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(19, 3),
        Size = UDim2.new(1, -24, 0, 16),
        Font = Enum.Font.GothamBold,
        Text = g.Name,
        TextColor3 = Theme.TextDim,
        TextSize = 11,
        TextTruncate = Enum.TextTruncate.AtEnd,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 14,
        Parent = row,
    })

    local sub = Create("TextLabel", {
        BackgroundTransparency = 1,
        Position = UDim2.fromOffset(9, 19),
        Size = UDim2.new(1, -14, 0, 14),
        Font = Enum.Font.Code,
        Text = isDet and "[DETECTED] " .. g.SubTitle or g.SubTitle,
        TextColor3 = isDet and Theme.Accent or Theme.TextMuted,
        TextSize = 10,
        TextTruncate = Enum.TextTruncate.AtEnd,
        TextXAlignment = Enum.TextXAlignment.Left,
        ZIndex = 14,
        Parent = row,
    })
    if isDet then
        BindAccent(sub, "TextColor3")
    end

    table.insert(gameCards, {
        Game = g,
        Frame = row,
        Stroke = stroke,
        Bar = leftBar,
        Title = title,
    })

    Connect(row.MouseEnter, function()
        if selectedGame ~= g then
            Tween(row, TweenFast, { BackgroundColor3 = Theme.ElementHover })
            Tween(title, TweenFast, { TextColor3 = Theme.Text })
        end
    end)
    Connect(row.MouseLeave, function()
        if selectedGame ~= g then
            Tween(row, TweenFast, { BackgroundColor3 = Theme.Element })
            Tween(title, TweenFast, { TextColor3 = Theme.TextDim })
        end
    end)
    Connect(row.MouseButton1Click, function()
        SelectGame(g)
        StatusText.Text = string.format("[shards.cc] selected profile '%s' (%s)", g.Name, g.Version)
    end)
end

Connect(SearchBox:GetPropertyChangedSignal("Text"), function()
    local q = string.lower(SearchBox.Text or "")
    for _, c in ipairs(gameCards) do
        local match = (q == "")
            or (string.find(string.lower(c.Game.Name), q, 1, true) ~= nil)
            or (string.find(string.lower(c.Game.SubTitle), q, 1, true) ~= nil)
        c.Frame.Visible = match
    end
end)

SelectGame(selectedGame)

-- ============================================================================
-- CLEANUP & MODULE EXECUTION HANDOFF
-- ============================================================================
local function DestroyLoader()
    for _, c in ipairs(Connections) do
        pcall(function() c:Disconnect() end)
    end
    table.clear(Connections)
    if ScreenGui and ScreenGui.Parent then
        ScreenGui:Destroy()
    end
end

Connect(CloseBtn.MouseEnter, function()
    Tween(closeStroke, TweenFast, { Color = Theme.Risky })
    Tween(closeIcon, TweenFast, { ImageColor3 = Theme.Risky })
end)
Connect(CloseBtn.MouseLeave, function()
    Tween(closeStroke, TweenFast, { Color = Theme.InnerBorder })
    Tween(closeIcon, TweenFast, { ImageColor3 = Theme.TextDim })
end)
Connect(CloseBtn.MouseButton1Click, function()
    DestroyLoader()
end)

local function ExecuteSelectedGame(gameEntry)
    if isLoading then return end
    isLoading = true
    LoaderSettings.LastGameId = gameEntry.Id
    SaveLoaderSettings()

    StatusText.TextColor3 = Theme.Text
    StatusText.Text = string.format("[1/3] preparing '%s' environment...", gameEntry.Name)
    Tween(ProgFill, TweenInfo.new(0.22, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {
        Size = UDim2.fromScale(0.38, 1)
    })
    task.wait(0.22)

    StatusText.Text = string.format("[2/3] fetching '%s' module...", gameEntry.LocalFile or gameEntry.Id)
    Tween(ProgFill, TweenInfo.new(0.25, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {
        Size = UDim2.fromScale(0.78, 1)
    })

    local sourceCode = nil
    if isfile and readfile and gameEntry.LocalFile and isfile(gameEntry.LocalFile) then
        pcall(function()
            sourceCode = readfile(gameEntry.LocalFile)
        end)
    end
    if (not sourceCode or sourceCode == "") and gameEntry.RemoteUrl then
        pcall(function()
            sourceCode = game:HttpGet(gameEntry.RemoteUrl)
        end)
    end

    if not sourceCode or sourceCode == "" then
        StatusText.TextColor3 = Theme.Risky
        StatusText.Text = "[error] failed to fetch module script!"
        Tween(ProgFill, TweenFast, { Size = UDim2.fromScale(0, 1) })
        isLoading = false
        return
    end

    local compiledFn, compileErr = loadstring(sourceCode)
    if not compiledFn then
        StatusText.TextColor3 = Theme.Risky
        StatusText.Text = "[error] syntax error: " .. tostring(compileErr)
        Tween(ProgFill, TweenFast, { Size = UDim2.fromScale(0, 1) })
        isLoading = false
        return
    end

    -- Expose loader context so the loaded script can read the selected game & theme preset
    local ctx = {
        GameId = gameEntry.Id,
        GameName = gameEntry.Name,
        Theme = LoaderSettings.Theme,
        Accent = Theme.Accent,
        AccentDark = Theme.AccentDark,
        PlaceId = currentPlaceId,
    }
    if getgenv then
        getgenv().ShardsLoaderContext = ctx
    end
    _G.ShardsLoaderContext = ctx

    StatusText.TextColor3 = Theme.Success
    StatusText.Text = string.format("[3/3] launching %s...", gameEntry.Name)
    Tween(ProgFill, TweenInfo.new(0.18, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {
        Size = UDim2.fromScale(1, 1)
    })
    task.wait(0.20)

    DestroyLoader()
    task.spawn(function()
        compiledFn()
        -- Sync selected theme preset into ShardsLibrary if loaded
        local lib = (getgenv and getgenv().ShardsLibrary) or _G.ShardsLibrary
        if lib and lib.SetAccent then
            pcall(function()
                lib:SetAccent(Theme.Accent, Theme.AccentDark)
                lib:SyncOptionValue("UI_ThemePreset", LoaderSettings.Theme)
            end)
        end
    end)
end

Connect(LoadBtn.MouseButton1Click, function()
    ExecuteSelectedGame(selectedGame)
end)

-- Auto-load if enabled and a matching game was detected
if LoaderSettings.AutoLoad and detectedGame then
    task.delay(0.35, function()
        if ScreenGui and ScreenGui.Parent and not isLoading then
            ExecuteSelectedGame(detectedGame)
        end
    end)
end
