--!nocheck
-- SHARDS.CC // Loader UI
-- Matches the Skeet / Gamesense aesthetic from the main Shards UI

local TweenService = game:GetService("TweenService")
local CoreGui = game:GetService("CoreGui")
local Players = game:GetService("Players")

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
    return Players.LocalPlayer and Players.LocalPlayer:WaitForChild("PlayerGui")
end

local Theme = {
    Background       = Color3.fromRGB(11, 11, 14),
    OuterBorder      = Color3.fromRGB(5, 5, 7),
    InnerBorder      = Color3.fromRGB(38, 38, 48),
    Accent           = Color3.fromRGB(155, 195, 55), -- Skeet Lime
    AccentDark       = Color3.fromRGB(98, 128, 28),
    Text             = Color3.fromRGB(228, 228, 235),
    TextDim          = Color3.fromRGB(142, 142, 156),
    Element          = Color3.fromRGB(22, 22, 29),
}

local function ApplyCSBorder(frame)
    frame.BorderSizePixel = 0
    local innerStroke = Instance.new("UIStroke")
    innerStroke.Name = "InnerStroke"
    innerStroke.Color = Theme.InnerBorder
    innerStroke.Thickness = 1
    innerStroke.ApplyStrokeMode = Enum.ApplyStrokeMode.Border
    innerStroke.Parent = frame

    local outerWrap = Instance.new("Frame")
    outerWrap.Name = "OuterBorder"
    outerWrap.BackgroundTransparency = 1
    outerWrap.Position = UDim2.new(0, -1, 0, -1)
    outerWrap.Size = UDim2.new(1, 2, 1, 2)
    outerWrap.ZIndex = math.max(1, frame.ZIndex - 1)
    outerWrap.Parent = frame

    local blackStroke = Instance.new("UIStroke")
    blackStroke.Name = "BlackStroke"
    blackStroke.Color = Theme.OuterBorder
    blackStroke.Thickness = 1
    blackStroke.ApplyStrokeMode = Enum.ApplyStrokeMode.Border
    blackStroke.Parent = outerWrap
end

local LoaderGui = Instance.new("ScreenGui")
LoaderGui.Name = "ShardsLoader"
LoaderGui.ResetOnSpawn = false
LoaderGui.IgnoreGuiInset = true
LoaderGui.ZIndexBehavior = Enum.ZIndexBehavior.Sibling
LoaderGui.DisplayOrder = 10000
LoaderGui.Parent = GetSafeParent()

local MainFrame = Instance.new("Frame")
MainFrame.Name = "MainFrame"
MainFrame.BackgroundColor3 = Theme.Background
MainFrame.Size = UDim2.fromOffset(300, 95)
MainFrame.Position = UDim2.fromScale(0.5, 0.5)
MainFrame.AnchorPoint = Vector2.new(0.5, 0.5)
MainFrame.ZIndex = 10
MainFrame.Parent = LoaderGui

ApplyCSBorder(MainFrame)

local AccentBar = Instance.new("Frame")
AccentBar.Name = "AccentBar"
AccentBar.BackgroundColor3 = Color3.new(1, 1, 1)
AccentBar.BorderSizePixel = 0
AccentBar.Size = UDim2.new(1, 0, 0, 2)
AccentBar.ZIndex = 11
AccentBar.Parent = MainFrame

local AccentGrad = Instance.new("UIGradient")
AccentGrad.Color = ColorSequence.new({
    ColorSequenceKeypoint.new(0.00, Theme.AccentDark),
    ColorSequenceKeypoint.new(0.50, Theme.Accent),
    ColorSequenceKeypoint.new(1.00, Theme.AccentDark),
})
AccentGrad.Parent = AccentBar

local TitleLabel = Instance.new("TextLabel")
TitleLabel.Name = "Title"
TitleLabel.BackgroundTransparency = 1
TitleLabel.Position = UDim2.fromOffset(14, 12)
TitleLabel.Size = UDim2.new(1, -28, 0, 16)
TitleLabel.Font = Enum.Font.GothamBold
TitleLabel.Text = "SHARDS.CC"
TitleLabel.TextColor3 = Theme.Text
TitleLabel.TextSize = 12
TitleLabel.TextXAlignment = Enum.TextXAlignment.Left
TitleLabel.ZIndex = 11
TitleLabel.Parent = MainFrame

local StatusLabel = Instance.new("TextLabel")
StatusLabel.Name = "Status"
StatusLabel.BackgroundTransparency = 1
StatusLabel.Position = UDim2.fromOffset(14, 38)
StatusLabel.Size = UDim2.new(1, -28, 0, 14)
StatusLabel.Font = Enum.Font.Code
StatusLabel.Text = "Waiting..."
StatusLabel.TextColor3 = Theme.TextDim
StatusLabel.TextSize = 12
StatusLabel.TextXAlignment = Enum.TextXAlignment.Left
StatusLabel.ZIndex = 11
StatusLabel.Parent = MainFrame

local ProgBg = Instance.new("Frame")
ProgBg.Name = "ProgressBg"
ProgBg.BackgroundColor3 = Theme.Element
ProgBg.Position = UDim2.fromOffset(14, 60)
ProgBg.Size = UDim2.new(1, -28, 0, 8)
ProgBg.ZIndex = 11
ProgBg.Parent = MainFrame
ApplyCSBorder(ProgBg)

local ProgFill = Instance.new("Frame")
ProgFill.Name = "ProgressFill"
ProgFill.BackgroundColor3 = Color3.new(1, 1, 1)
ProgFill.BorderSizePixel = 0
ProgFill.Size = UDim2.fromScale(0, 1)
ProgFill.ZIndex = 12
ProgFill.Parent = ProgBg

local FillGrad = Instance.new("UIGradient")
FillGrad.Color = ColorSequence.new({
    ColorSequenceKeypoint.new(0.00, Theme.AccentDark),
    ColorSequenceKeypoint.new(1.00, Theme.Accent),
})
FillGrad.Parent = ProgFill

-- Loader Logic
local function UpdateLoader(text, percent, duration)
    StatusLabel.Text = text
    local tw = TweenService:Create(ProgFill, TweenInfo.new(duration, Enum.EasingStyle.Quart, Enum.EasingDirection.Out), {
        Size = UDim2.fromScale(percent, 1)
    })
    tw:Play()
    task.wait(duration)
end

local function FadeOutAndDestroy()
    local twInfo = TweenInfo.new(0.35, Enum.EasingStyle.Quad, Enum.EasingDirection.Out)
    for _, obj in ipairs(MainFrame:GetDescendants()) do
        if obj:IsA("TextLabel") then
            TweenService:Create(obj, twInfo, { TextTransparency = 1 }):Play()
        elseif obj:IsA("Frame") and obj.BackgroundTransparency == 0 then
            TweenService:Create(obj, twInfo, { BackgroundTransparency = 1 }):Play()
        elseif obj:IsA("UIStroke") then
            TweenService:Create(obj, twInfo, { Transparency = 1 }):Play()
        end
    end
    local mainTw = TweenService:Create(MainFrame, twInfo, { BackgroundTransparency = 1 })
    mainTw:Play()
    mainTw.Completed:Wait()
    LoaderGui:Destroy()
end

task.spawn(function()
    -- Example loading sequence
    UpdateLoader("Authenticating session...", 0.25, 0.7)
    UpdateLoader("Fetching configs...", 0.45, 0.6)
    UpdateLoader("Loading UI elements...", 0.75, 0.8)
    UpdateLoader("Injecting shards.cc...", 1.0, 0.5)
    
    StatusLabel.Text = "Ready!"
    StatusLabel.TextColor3 = Theme.Accent
    task.wait(0.5)
    
    FadeOutAndDestroy()
    
    -- Load Arsenal Ultimate script
    local ok, err = pcall(function()
        loadstring(game:HttpGet("https://raw.githubusercontent.com/jonpauli/Shards/refs/heads/main/Arsenal.lua"))()
    end)
    if not ok then
        warn("Failed to load Arsenal script: " .. tostring(err))
    end
end)
