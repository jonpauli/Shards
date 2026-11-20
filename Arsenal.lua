-- // Shards: Arsenal
local Players = game:GetService("Players")
local RunService = game:GetService("RunService")
local UserInputService = game:GetService("UserInputService")
local Workspace = game:GetService("Workspace")
local Lighting = game:GetService("Lighting")
local Camera = Workspace.CurrentCamera
local LocalPlayer = Players.LocalPlayer
local Mouse = LocalPlayer:GetMouse()

-- // Config
local Config = {
    Aimbot = { Enabled = false, Silent = false, WallCheck = true, Smoothing = 0.4, FOV = 200 },
    Triggerbot = { Enabled = false, Delay = 0 },
    GunMods = { NoRecoil = false, NoSpread = false, InfAmmo = false, Auto = false },
    ESP = { Highlight = false, HighlightColor = Color3.fromRGB(155, 195, 55), Chams = false, ChamsColor = Color3.fromRGB(255, 65, 85) },
    TeamCheck = true,
    HitboxExpand = false,
    Movement = { Bhop = false, InfJump = false },
    World = { FOV = 90, TimeOfDay = 14 }
}

-- // UI Initialization
local ShardsUI = loadstring(game:HttpGet("https://raw.githubusercontent.com/jonpauli/Shards/refs/heads/main/ShardsUI.lua"))()
ShardsUI:SetFontPreset("Minimalist (Gotham / GothamBold)")
ShardsUI.Theme.Accent = Color3.fromRGB(155, 195, 55) -- Skeet Lime

local Window = ShardsUI:CreateWindow({
    Name = "SHARDS.CC | Arsenal",
    Size = UDim2.fromOffset(560, 420)
})

local CombatTab = Window:CreateTab({ Name = "Combat", Icon = "swords" })
local VisualsTab = Window:CreateTab({ Name = "Visuals", Icon = "eye" })
local MovementTab = Window:CreateTab({ Name = "Movement", Icon = "footprints" })
local SettingsTab = Window:CreateTab({ Name = "Settings", Icon = "settings" })

-- =========================================================================
-- // COMBAT TAB
-- =========================================================================
local AimbotGroup = CombatTab:CreateGroupbox({ Name = "Aimbot", Side = "Left" })

AimbotGroup:CreateToggle({
    Name = "Enable Aimbot",
    Flag = "Aimbot_Enable",
    Default = false,
    Callback = function(v) Config.Aimbot.Enabled = v end
}):CreateKeybind({ Default = Enum.UserInputType.MouseButton2, Mode = "Hold" })

AimbotGroup:CreateToggle({
    Name = "Silent Aim",
    Flag = "Aimbot_Silent",
    Default = false,
    Callback = function(v) Config.Aimbot.Silent = v end
})

AimbotGroup:CreateToggle({
    Name = "Visibility Check",
    Flag = "Aimbot_WallCheck",
    Default = true,
    Callback = function(v) Config.Aimbot.WallCheck = v end
})

AimbotGroup:CreateSlider({
    Name = "Aimbot FOV",
    Flag = "Aimbot_FOV",
    Min = 10, Max = 800, Default = 200,
    Suffix = "px",
    Callback = function(v) Config.Aimbot.FOV = v end
})

AimbotGroup:CreateSlider({
    Name = "Smoothing",
    Flag = "Aimbot_Smooth",
    Min = 0.05, Max = 1.0, Default = 0.4,
    Suffix = "s",
    Callback = function(v) Config.Aimbot.Smoothing = v end
})

local TriggerGroup = CombatTab:CreateGroupbox({ Name = "Triggerbot", Side = "Left" })
TriggerGroup:CreateToggle({
    Name = "Enable Triggerbot",
    Flag = "Triggerbot_Enable",
    Default = false,
    Callback = function(v) Config.Triggerbot.Enabled = v end
}):CreateKeybind({ Default = Enum.KeyCode.V, Mode = "Toggle" })

TriggerGroup:CreateSlider({
    Name = "Reaction Delay",
    Flag = "Triggerbot_Delay",
    Min = 0, Max = 500, Default = 0,
    Suffix = "ms",
    Callback = function(v) Config.Triggerbot.Delay = v end
})

local GunGroup = CombatTab:CreateGroupbox({ Name = "Gun Mods (Risky)", Side = "Right" })
GunGroup:CreateToggle({ Name = "No Recoil", Flag = "Gun_NoRecoil", Default = false, Callback = function(v) Config.GunMods.NoRecoil = v end })
GunGroup:CreateToggle({ Name = "No Spread", Flag = "Gun_NoSpread", Default = false, Callback = function(v) Config.GunMods.NoSpread = v end })
GunGroup:CreateToggle({ Name = "Infinite Ammo", Flag = "Gun_InfAmmo", Default = false, Callback = function(v) Config.GunMods.InfAmmo = v end })
GunGroup:CreateToggle({ Name = "Force Automatic", Flag = "Gun_Auto", Default = false, Callback = function(v) Config.GunMods.Auto = v end })

local MiscCombat = CombatTab:CreateGroupbox({ Name = "Misc", Side = "Right" })
MiscCombat:CreateToggle({
    Name = "Team Check",
    Flag = "Misc_TeamCheck",
    Default = true,
    Callback = function(v) Config.TeamCheck = v end
})

MiscCombat:CreateToggle({
    Name = "Expand Hitboxes (HRP)",
    Flag = "Misc_Hitbox",
    Default = false,
    Callback = function(v) Config.HitboxExpand = v end
})

-- =========================================================================
-- // VISUALS TAB
-- =========================================================================
local ESPGroup = VisualsTab:CreateGroupbox({ Name = "Player ESP", Side = "Left" })

ESPGroup:CreateToggle({
    Name = "Highlight ESP",
    Flag = "ESP_Highlight",
    Default = false,
    Callback = function(v) Config.ESP.Highlight = v end
}):CreateColorPicker({
    Flag = "ESP_HighlightColor",
    Default = Config.ESP.HighlightColor,
    Callback = function(v) Config.ESP.HighlightColor = v end
})

ESPGroup:CreateToggle({
    Name = "Material Chams",
    Flag = "ESP_Chams",
    Default = false,
    Callback = function(v) Config.ESP.Chams = v end
}):CreateColorPicker({
    Flag = "ESP_ChamsColor",
    Default = Config.ESP.ChamsColor,
    Callback = function(v) Config.ESP.ChamsColor = v end
})

local WorldGroup = VisualsTab:CreateGroupbox({ Name = "World", Side = "Right" })

WorldGroup:CreateSlider({
    Name = "Field of View",
    Flag = "World_FOV",
    Min = 60, Max = 120, Default = 90,
    Callback = function(v) Config.World.FOV = v end
})

WorldGroup:CreateSlider({
    Name = "Time of Day",
    Flag = "World_Time",
    Min = 0, Max = 24, Default = 14,
    Suffix = "h",
    Callback = function(v) Config.World.TimeOfDay = v end
})

-- =========================================================================
-- // MOVEMENT TAB
-- =========================================================================
local MoveGroup = MovementTab:CreateGroupbox({ Name = "Local Player", Side = "Left" })

MoveGroup:CreateToggle({
    Name = "Bunny Hop",
    Flag = "Move_Bhop",
    Default = false,
    Callback = function(v) Config.Movement.Bhop = v end
})

MoveGroup:CreateToggle({
    Name = "Infinite Jump",
    Flag = "Move_InfJump",
    Default = false,
    Callback = function(v) Config.Movement.InfJump = v end
})

-- =========================================================================
-- // SETTINGS TAB
-- =========================================================================
local ThemeGroup = SettingsTab:CreateGroupbox({ Name = "Menu Options", Side = "Left" })
ThemeGroup:CreateToggle({
    Name = "Show Watermark",
    Flag = "UI_Watermark",
    Default = true,
    Callback = function(v) if ShardsUI.SetWatermarkVisible then ShardsUI:SetWatermarkVisible(v) end end
})

ThemeGroup:CreateToggle({
    Name = "Show Keybind List",
    Flag = "UI_KeybindList",
    Default = true,
    Callback = function(v) if ShardsUI.SetKeybindListVisible then ShardsUI:SetKeybindListVisible(v) end end
})

ThemeGroup:CreateButton({ Name = "Unload Script", Callback = function() ShardsUI:Unload() end })

-- =========================================================================
-- // LOGIC LOOPS
-- =========================================================================
local function IsVisible(targetPart)
    if not Config.Aimbot.WallCheck then return true end
    local origin = Camera.CFrame.Position
    local dir = (targetPart.Position - origin).Unit * (targetPart.Position - origin).Magnitude
    local raycastParams = RaycastParams.new()
    raycastParams.FilterDescendantsInstances = {LocalPlayer.Character, Camera}
    raycastParams.FilterType = Enum.RaycastFilterType.Exclude
    raycastParams.IgnoreWater = true
    local result = Workspace:Raycast(origin, dir, raycastParams)
    if result and result.Instance and result.Instance:IsDescendantOf(targetPart.Parent) then
        return true
    elseif not result then
        return true
    end
    return false
end

local function GetClosestPlayer()
    local closest = nil
    local maxDist = Config.Aimbot.FOV
    local mousePos = UserInputService:GetMouseLocation()

    for _, v in pairs(Players:GetPlayers()) do
        if v ~= LocalPlayer then
            if Config.TeamCheck and v.Team == LocalPlayer.Team then continue end
            
            local char = v.Character
            if char and char:FindFirstChild("Head") and char:FindFirstChild("Humanoid") and char.Humanoid.Health > 0 then
                local pos, onScreen = Camera:WorldToViewportPoint(char.Head.Position)
                if onScreen then
                    local dist = (Vector2.new(pos.X, pos.Y) - mousePos).Magnitude
                    if dist < maxDist then
                        if IsVisible(char.Head) then
                            maxDist = dist
                            closest = char.Head
                        end
                    end
                end
            end
        end
    end
    return closest
end

-- Infinite Jump Hook
UserInputService.JumpRequest:Connect(function()
    if Config.Movement.InfJump then
        local char = LocalPlayer.Character
        if char and char:FindFirstChildOfClass("Humanoid") then
            char:FindFirstChildOfClass("Humanoid"):ChangeState(Enum.HumanoidStateType.Jumping)
        end
    end
end)

RunService.RenderStepped:Connect(function()
    -- World Modifications
    Camera.FieldOfView = Config.World.FOV
    Lighting.ClockTime = Config.World.TimeOfDay

    -- Bunny Hop
    if Config.Movement.Bhop and UserInputService:IsKeyDown(Enum.KeyCode.Space) then
        local char = LocalPlayer.Character
        if char and char:FindFirstChildOfClass("Humanoid") then
            local hum = char:FindFirstChildOfClass("Humanoid")
            if hum.FloorMaterial ~= Enum.Material.Air then
                hum:ChangeState(Enum.HumanoidStateType.Jumping)
            end
        end
    end

    -- Triggerbot
    local triggerActive = ShardsUI.Options["Triggerbot_Enable_Keybind"] and ShardsUI.Options["Triggerbot_Enable_Keybind"].Active
    if Config.Triggerbot.Enabled and triggerActive then
        local target = Mouse.Target
        if target and target.Parent and target.Parent:FindFirstChild("Humanoid") then
            local p = Players:GetPlayerFromCharacter(target.Parent)
            if p and p ~= LocalPlayer then
                if not Config.TeamCheck or p.Team ~= LocalPlayer.Team then
                    task.wait(Config.Triggerbot.Delay / 1000)
                    mouse1click()
                end
            end
        end
    end

    -- ESP & Hitboxes
    for _, v in pairs(Players:GetPlayers()) do
        if v ~= LocalPlayer then
            local char = v.Character
            if char and char:FindFirstChild("HumanoidRootPart") then
                local isEnemy = true
                if Config.TeamCheck and v.Team == LocalPlayer.Team then isEnemy = false end
                
                -- Highlight ESP
                local hl = char:FindFirstChild("ShardsESP_HL")
                if Config.ESP.Highlight and isEnemy then
                    if not hl then
                        hl = Instance.new("Highlight")
                        hl.Name = "ShardsESP_HL"
                        hl.Parent = char
                        hl.FillTransparency = 0.5
                        hl.OutlineTransparency = 0.2
                    end
                    hl.FillColor = Config.ESP.HighlightColor
                    hl.OutlineColor = Color3.fromRGB(255, 255, 255)
                else
                    if hl then hl:Destroy() end
                end

                -- Chams ESP
                for _, part in ipairs(char:GetChildren()) do
                    if part:IsA("BasePart") then
                        if Config.ESP.Chams and isEnemy then
                            part.Material = Enum.Material.ForceField
                            part.Color = Config.ESP.ChamsColor
                        else
                            part.Material = Enum.Material.Plastic
                        end
                    end
                end

                -- Hitbox Expansion
                local hrp = char.HumanoidRootPart
                if Config.HitboxExpand and isEnemy then
                    hrp.Size = Vector3.new(10, 10, 10)
                    hrp.Transparency = 0.8
                    hrp.BrickColor = BrickColor.new("Bright purple")
                    hrp.CanCollide = false
                else
                    hrp.Size = Vector3.new(2, 2, 1)
                    hrp.Transparency = 1
                end
            end
        end
    end

    -- Aimbot Lock (Camera)
    local aimActive = ShardsUI.Options["Aimbot_Enable_Keybind"] and ShardsUI.Options["Aimbot_Enable_Keybind"].Active
    if Config.Aimbot.Enabled and aimActive and not Config.Aimbot.Silent then
        local target = GetClosestPlayer()
        if target then
            Camera.CFrame = Camera.CFrame:Lerp(CFrame.new(Camera.CFrame.Position, target.Position), Config.Aimbot.Smoothing)
        end
    end
end)

if ShardsUI.SetWatermark then ShardsUI:SetWatermark("shards.cc | Arsenal") end
print("[Shards] Premium Arsenal script loaded.")
