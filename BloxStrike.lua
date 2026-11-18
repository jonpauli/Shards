--!nocheck
-- ============================================================================
-- SHARDS.CC // BLOXSTRIKE (5v5 DEFUSAL & ZOMBIES)
-- ============================================================================

local Players = game:GetService("Players")
local RunService = game:GetService("RunService")
local UserInputService = game:GetService("UserInputService")
local Workspace = game:GetService("Workspace")
local Lighting = game:GetService("Lighting")
local CoreGui = game:GetService("CoreGui")

local LocalPlayer = Players.LocalPlayer
local Camera = Workspace.CurrentCamera

-- ============================================================================
-- LOAD UI FRAMEWORK
-- ============================================================================
local Library
pcall(function()
    if getgenv and getgenv().ShardsLibrary then Library = getgenv().ShardsLibrary
    elseif _G.ShardsLibrary then Library = _G.ShardsLibrary end
end)

if not Library then
    local suc, res = pcall(function()
        if isfile and isfile("ShardsUI.lua") then return loadstring(readfile("ShardsUI.lua"))() end
        return loadstring(game:HttpGet("https://raw.githubusercontent.com/jonpauli/Shards/main/ShardsUI.lua"))()
    end)
    if suc and res then Library = res else warn("[shards.cc] Failed to load UI") return end
end

local Flags = Library.Flags

-- ============================================================================
-- CACHE & HUD
-- ============================================================================
local Cache = {
    Connections = {},
    ESP = {},
    Chams = {},
    Target = nil,
    OriginalSizes = {}
}

local HUD = {
    Watermark = Drawing.new("Text"),
    FOVCircle = Drawing.new("Circle")
}

HUD.Watermark.Text = "shards.cc | bloxstrike | " .. Library.Version
HUD.Watermark.Size = 14
HUD.Watermark.Font = 2
HUD.Watermark.Color = Color3.new(1, 1, 1)
HUD.Watermark.Outline = true
HUD.Watermark.Position = Vector2.new(15, 15)
HUD.Watermark.Visible = true

HUD.FOVCircle.Thickness = 1
HUD.FOVCircle.NumSides = 64
HUD.FOVCircle.Filled = false
HUD.FOVCircle.Transparency = 1
HUD.FOVCircle.Color = Color3.new(1, 1, 1)
HUD.FOVCircle.Visible = false

-- ============================================================================
-- UI BUILDER
-- ============================================================================
local Window = Library:CreateWindow({ Title = "shards.cc // bloxstrike", Center = true, AutoShow = true })

local Tabs = {
    Combat = Window:AddTab({ Name = "Combat", Icon = "crosshair" }),
    Rage = Window:AddTab({ Name = "Anti-Aim", Icon = "shield" }),
    Visuals = Window:AddTab({ Name = "Visuals", Icon = "scan" }),
    Misc = Window:AddTab({ Name = "Misc", Icon = "activity" }),
    Settings = Window:AddTab({ Name = "Settings", Icon = "sliders" }),
}

-- [ COMBAT ]
local AimGrp = Tabs.Combat:AddLeftGroupbox("Aimbot", "target")
AimGrp:AddToggle({ Flag = "Aim_Enabled", Name = "Enable Aimbot" }):AddKeybind({ Flag = "Aim_Key", Mode = "Hold", Key = "MouseButton2" })
AimGrp:AddToggle({ Flag = "Aim_TeamCheck", Name = "Team Check", Default = true })
AimGrp:AddToggle({ Flag = "Aim_VisCheck", Name = "Visible Check", Default = true })
AimGrp:AddDropdown({ Flag = "Aim_Hitbox", Name = "Target Hitbox", Values = {"Head", "Torso", "HumanoidRootPart"}, Default = "Head" })
AimGrp:AddSlider({ Flag = "Aim_FOV", Name = "FOV Radius", Min = 10, Max = 800, Default = 120 })
AimGrp:AddToggle({ Flag = "Aim_DrawFOV", Name = "Draw FOV Circle" }):AddColorPicker({ Flag = "Aim_FOVCol", Default = Color3.fromRGB(255, 255, 255) })
AimGrp:AddSlider({ Flag = "Aim_Smooth", Name = "Smoothness", Min = 1, Max = 20, Default = 3 })

local SilentGrp = Tabs.Combat:AddRightGroupbox("Silent Aim", "crosshair")
SilentGrp:AddToggle({ Flag = "Aim_Silent", Name = "Enable Silent Aim" })
SilentGrp:AddToggle({ Flag = "Silent_TeamCheck", Name = "Team Check", Default = true })
SilentGrp:AddToggle({ Flag = "Silent_VisCheck", Name = "Visible Check (Wallbang)", Default = true })
SilentGrp:AddDropdown({ Flag = "Silent_Hitbox", Name = "Target Hitbox", Values = {"Head", "Torso", "Random"}, Default = "Head" })
SilentGrp:AddSlider({ Flag = "Silent_FOV", Name = "Silent FOV Radius", Min = 10, Max = 800, Default = 150 })

local TrigGrp = Tabs.Combat:AddLeftGroupbox("Triggerbot", "zap")
TrigGrp:AddToggle({ Flag = "Trig_Enabled", Name = "Enable Triggerbot" }):AddKeybind({ Flag = "Trig_Key", Mode = "Hold", Key = "None" })
TrigGrp:AddSlider({ Flag = "Trig_Delay", Name = "Shot Delay (ms)", Min = 0, Max = 500, Default = 25 })

-- [ RAGE ]
local AAGrp = Tabs.Rage:AddLeftGroupbox("Anti-Aim", "shield")
AAGrp:AddToggle({ Flag = "AA_Enabled", Name = "Enable Anti-Aim" })
AAGrp:AddDropdown({ Flag = "AA_Mode", Name = "Yaw Base", Values = {"Spin", "Jitter", "Backwards"}, Default = "Spin" })
AAGrp:AddSlider({ Flag = "AA_Speed", Name = "Spin Speed", Min = 1, Max = 50, Default = 20 })

-- [ VISUALS ]
local EspGrp = Tabs.Visuals:AddLeftGroupbox("Player ESP", "users")
EspGrp:AddToggle({ Flag = "ESP_Enabled", Name = "Enable ESP" })
EspGrp:AddToggle({ Flag = "ESP_TeamCheck", Name = "Team Check", Default = true })
EspGrp:AddToggle({ Flag = "ESP_Boxes", Name = "Bounding Box" }):AddColorPicker({ Flag = "ESP_BoxCol", Default = Color3.fromRGB(255, 255, 255) })
EspGrp:AddToggle({ Flag = "ESP_Names", Name = "Player Name" }):AddColorPicker({ Flag = "ESP_NameCol", Default = Color3.fromRGB(255, 255, 255) })
EspGrp:AddToggle({ Flag = "ESP_Health", Name = "Health Bar" })
EspGrp:AddToggle({ Flag = "ESP_Distance", Name = "Distance" })

local ChamGrp = Tabs.Visuals:AddRightGroupbox("Chams & World", "eye")
ChamGrp:AddToggle({ Flag = "Cham_Enabled", Name = "Player Chams (Highlight)" })
ChamGrp:AddColorPicker({ Flag = "Cham_FillCol", Name = "Fill Color", Default = Color3.fromRGB(155, 195, 55) })
ChamGrp:AddColorPicker({ Flag = "Cham_OutCol", Name = "Outline Color", Default = Color3.fromRGB(0, 0, 0) })
ChamGrp:AddToggle({ Flag = "Vis_NoFlash", Name = "No Flash / Smoke" })
ChamGrp:AddToggle({ Flag = "Vis_Fullbright", Name = "Fullbright" })
ChamGrp:AddToggle({ Flag = "Vis_FOV", Name = "Custom Camera FOV" })
ChamGrp:AddSlider({ Flag = "Vis_FOVVal", Name = "Camera FOV", Min = 50, Max = 120, Default = 90 })

-- [ MISC ]
local MoveGrp = Tabs.Misc:AddLeftGroupbox("Movement", "fast-forward")
MoveGrp:AddToggle({ Flag = "Move_Bhop", Name = "Bunnyhop" })
MoveGrp:AddToggle({ Flag = "Move_Speed", Name = "CFrame Speed" }):AddKeybind({ Flag = "Move_SpeedKey", Mode = "Hold", Key = "None" })
MoveGrp:AddSlider({ Flag = "Move_SpeedVal", Name = "Speed Multiplier", Min = 0.1, Max = 2, Default = 0.4, Rounding = 1 })

Library:BuildConfigTab(Tabs.Settings)

-- ============================================================================
-- HELPER FUNCTIONS
-- ============================================================================
local function IsTeammate(player)
    if not Flags.Aim_TeamCheck and not Flags.ESP_TeamCheck then return false end
    if player.Team and LocalPlayer.Team and player.Team == LocalPlayer.Team then
        return true
    end
    return false
end

local function IsVisible(part, ignoreList)
    local origin = Camera.CFrame.Position
    local dir = (part.Position - origin)
    local params = RaycastParams.new()
    params.FilterType = Enum.RaycastFilterType.Exclude
    params.FilterDescendantsInstances = ignoreList or {LocalPlayer.Character, Camera}
    
    local result = Workspace:Raycast(origin, dir, params)
    if result and result.Instance then
        return result.Instance:IsDescendantOf(part.Parent)
    end
    return true
end

local function GetClosestTarget()
    local closestDist = Flags.Aim_FOV or 120
    local target = nil
    local mousePos = UserInputService:GetMouseLocation()

    for _, player in ipairs(Players:GetPlayers()) do
        if player ~= LocalPlayer and player.Character then
            if Flags.Aim_TeamCheck and player.Team == LocalPlayer.Team and player.Team ~= nil then
                continue
            end
            local hum = player.Character:FindFirstChildOfClass("Humanoid")
            local part = player.Character:FindFirstChild(Flags.Aim_Hitbox or "Head") or player.Character:FindFirstChild("HumanoidRootPart")
            
            if hum and hum.Health > 0 and part then
                if Flags.Aim_VisCheck and not IsVisible(part) then
                    continue
                end
                local pos, onScreen = Camera:WorldToViewportPoint(part.Position)
                if onScreen then
                    local dist = (Vector2.new(pos.X, pos.Y) - mousePos).Magnitude
                    if dist < closestDist then
                        closestDist = dist
                        target = player
                    end
                end
            end
        end
    end
    return target
end

-- ============================================================================
-- ESP & CHAMS CACHE
-- ============================================================================
local function CreateESP(player)
    Cache.ESP[player] = {
        Box = Drawing.new("Square"),
        Name = Drawing.new("Text"),
        HealthBar = Drawing.new("Square"),
        HealthOutline = Drawing.new("Square"),
        Distance = Drawing.new("Text"),
    }
    local esp = Cache.ESP[player]
    esp.Box.Thickness = 1
    esp.Box.Filled = false
    esp.Name.Size = 13
    esp.Name.Center = true
    esp.Name.Outline = true
    esp.Distance.Size = 12
    esp.Distance.Center = true
    esp.Distance.Outline = true
    esp.HealthBar.Filled = true
    esp.HealthOutline.Filled = false
    esp.HealthOutline.Thickness = 1
    esp.HealthOutline.Color = Color3.new(0, 0, 0)
end

local function RemoveESP(player)
    if Cache.ESP[player] then
        for _, d in pairs(Cache.ESP[player]) do d:Remove() end
        Cache.ESP[player] = nil
    end
    if Cache.Chams[player] then
        Cache.Chams[player]:Destroy()
        Cache.Chams[player] = nil
    end
end

for _, p in ipairs(Players:GetPlayers()) do if p ~= LocalPlayer then CreateESP(p) end end
Players.PlayerAdded:Connect(CreateESP)
Players.PlayerRemoving:Connect(RemoveESP)

-- ============================================================================
-- MAIN LOOP
-- ============================================================================
local spinAngle = 0
local lastTrigShot = 0

Cache.Connections.RenderStepped = RunService.RenderStepped:Connect(function(dt)
    HUD.Watermark.Text = string.format("shards.cc | bloxstrike | %d FPS", math.floor(1 / dt))
    
    -- FOV Circle
    HUD.FOVCircle.Visible = Flags.Aim_DrawFOV or false
    HUD.FOVCircle.Radius = Flags.Aim_FOV or 120
    HUD.FOVCircle.Position = UserInputService:GetMouseLocation()
    HUD.FOVCircle.Color = Flags.Aim_FOVCol or Color3.new(1, 1, 1)

    Cache.Target = GetClosestTarget()

    -- 1. ESP & Hitbox Loop
    for player, esp in pairs(Cache.ESP) do
        local char = player.Character
        local root = char and char:FindFirstChild("HumanoidRootPart")
        local hum = char and char:FindFirstChildOfClass("Humanoid")
        local isAlive = char and root and hum and hum.Health > 0 and char:GetAttribute("Dead") ~= true
        local onScreen = false
        local pos = Vector3.zero

        if isAlive then
            local pTeam = player.Team
            if not pTeam then
                local teamAttr = player:GetAttribute("Team")
                if teamAttr then pTeam = teamAttr end
            end
            local myTeam = LocalPlayer.Team
            if not myTeam then
                local myTeamAttr = LocalPlayer:GetAttribute("Team")
                if myTeamAttr then myTeam = myTeamAttr end
            end
            local isTeam = (Flags.ESP_TeamCheck and pTeam == myTeam and pTeam ~= nil)
            if isTeam then isAlive = false end

            -- Chams
            if Flags.Cham_Enabled then
                local hl = Cache.Chams[player]
                if not hl or hl.Parent ~= char then
                    if hl then hl:Destroy() end
                    hl = Instance.new("Highlight")
                    hl.Parent = char
                    Cache.Chams[player] = hl
                end
                hl.Enabled = true
                hl.FillColor = Flags.Cham_FillCol or Color3.fromRGB(155, 195, 55)
                hl.OutlineColor = Flags.Cham_OutCol or Color3.new(0, 0, 0)
                hl.FillTransparency = 0.4
                hl.OutlineTransparency = 0
            elseif Cache.Chams[player] then
                Cache.Chams[player].Enabled = false
            end

            if Flags.ESP_Enabled then
                pos, onScreen = Camera:WorldToViewportPoint(root.Position)
            end
        else
            if Cache.Chams[player] then
                Cache.Chams[player].Enabled = false
            end
        end

        if isAlive and onScreen and Flags.ESP_Enabled then
            local tl, _ = Camera:WorldToViewportPoint(root.Position + Vector3.new(0, 3, 0))
            local br, _ = Camera:WorldToViewportPoint(root.Position - Vector3.new(0, 3.5, 0))
            local h = math.abs(tl.Y - br.Y)
            local w = h * 0.6
            local x = pos.X - (w / 2)
            local y = tl.Y

            esp.Box.Visible = Flags.ESP_Boxes or false
            if esp.Box.Visible then
                esp.Box.Size = Vector2.new(w, h)
                esp.Box.Position = Vector2.new(x, y)
                esp.Box.Color = Flags.ESP_BoxCol or Color3.new(1, 1, 1)
            end

            esp.Name.Visible = Flags.ESP_Names or false
            if esp.Name.Visible then
                esp.Name.Text = player.Name
                esp.Name.Position = Vector2.new(pos.X, y - 15)
                esp.Name.Color = Flags.ESP_NameCol or Color3.new(1, 1, 1)
            end

            esp.Distance.Visible = Flags.ESP_Distance or false
            if esp.Distance.Visible then
                esp.Distance.Text = string.format("[%dm]", (Camera.CFrame.Position - root.Position).Magnitude)
                esp.Distance.Position = Vector2.new(pos.X, br.Y + 2)
                esp.Distance.Color = Color3.new(1, 1, 1)
            end

            esp.HealthOutline.Visible = Flags.ESP_Health or false
            esp.HealthBar.Visible = Flags.ESP_Health or false
            if esp.HealthBar.Visible then
                local pct = math.clamp(hum.Health / hum.MaxHealth, 0, 1)
                esp.HealthOutline.Size = Vector2.new(3, h)
                esp.HealthOutline.Position = Vector2.new(x - 5, y)
                esp.HealthBar.Size = Vector2.new(1, h * pct)
                esp.HealthBar.Position = Vector2.new(x - 4, y + (h - esp.HealthBar.Size.Y))
                esp.HealthBar.Color = Color3.fromHSV(pct * 0.3, 1, 1)
            end
        else
            for _, d in pairs(esp) do d.Visible = false end
        end
    end

    -- 2. Camera Aimbot
    if Flags.Aim_Enabled and Flags.Aim_Key and Cache.Target and Cache.Target.Character then
        local tPart = silentTarget
        if tPart then
            local goal = CFrame.new(Camera.CFrame.Position, tPart.Position)
            local smooth = Flags.Aim_Smooth or 1
            Camera.CFrame = (smooth > 1) and Camera.CFrame:Lerp(goal, 1 / smooth) or goal
        end
    end

    -- 3. Triggerbot
    if Flags.Trig_Enabled and Flags.Trig_Key and mouse1click then
        local mouse = LocalPlayer:GetMouse()
        local targetPart = mouse.Target
        if targetPart and targetPart.Parent then
            local hitPlayer = Players:GetPlayerFromCharacter(targetPart.Parent) or Players:GetPlayerFromCharacter(targetPart.Parent.Parent)
            if hitPlayer and hitPlayer ~= LocalPlayer and not IsTeammate(hitPlayer) then
                local hum = hitPlayer.Character:FindFirstChildOfClass("Humanoid")
                if hum and hum.Health > 0 then
                    local now = tick()
                    if (now - lastTrigShot) * 1000 >= (Flags.Trig_Delay or 25) then
                        lastTrigShot = now
                        mouse1click()
                    end
                end
            end
        end
    end

    -- 4. Movement & Anti-Aim
    local myChar = LocalPlayer.Character
    local myRoot = myChar and myChar:FindFirstChild("HumanoidRootPart")
    local myHum = myChar and myChar:FindFirstChildOfClass("Humanoid")

    if myRoot and myHum and myHum.Health > 0 then
        if Flags.Move_Bhop and UserInputService:IsKeyDown(Enum.KeyCode.Space) then
            if myHum.FloorMaterial ~= Enum.Material.Air then
                myHum:ChangeState(Enum.HumanoidStateType.Jumping)
            end
        end

        if Flags.Move_Speed and Flags.Move_SpeedKey and myHum.MoveDirection.Magnitude > 0 then
            myRoot.CFrame = myRoot.CFrame + (myHum.MoveDirection * (Flags.Move_SpeedVal or 0.4))
        end

        if Flags.AA_Enabled then
            local mode = Flags.AA_Mode or "Spin"
            if mode == "Spin" then
                spinAngle = (spinAngle + (Flags.AA_Speed or 20)) % 360
                myRoot.CFrame = CFrame.new(myRoot.Position) * CFrame.Angles(0, math.rad(spinAngle), 0)
            elseif mode == "Jitter" then
                local jit = (math.random(1, 2) == 1) and 90 or -90
                myRoot.CFrame = CFrame.new(myRoot.Position) * CFrame.Angles(0, math.rad(jit), 0)
            elseif mode == "Backwards" then
                local look = Camera.CFrame.LookVector
                myRoot.CFrame = CFrame.new(myRoot.Position, myRoot.Position - Vector3.new(look.X, 0, look.Z))
            end
        end
    end

    -- 5. World & Visuals
    if Flags.Vis_FOV then Camera.FieldOfView = Flags.Vis_FOVVal or 90 end
    if Flags.Vis_Fullbright then
        Lighting.Ambient = Color3.new(1, 1, 1)
        Lighting.OutdoorAmbient = Color3.new(1, 1, 1)
    end
    if Flags.Vis_NoFlash then
        local pg = LocalPlayer:FindFirstChild("PlayerGui")
        if pg then
            for _, v in ipairs(pg:GetChildren()) do
                if string.find(string.lower(v.Name), "flash") or string.find(string.lower(v.Name), "blind") then
                    v.Enabled = false
                end
            end
        end
    end
end)



-- ============================================================================
-- SILENT AIM HOOK
-- ============================================================================
local oldNamecall
if hookmetamethod then
    oldNamecall = hookmetamethod(game, "__namecall", function(self, ...)
        local method = getnamecallmethod()
        local args = {...}

        if not checkcaller() and Flags.Aim_Silent and silentTarget then
            local tPart = silentTarget
            if tPart then
                if method == "Raycast" then
                    args[2] = (tPart.Position - args[1]).Unit * 1000
                    return oldNamecall(self, unpack(args))
                elseif method == "FindPartOnRayWithIgnoreList" or method == "FindPartOnRay" then
                    local origRay = args[1]
                    if typeof(origRay) == "Ray" then
                        args[1] = Ray.new(origRay.Origin, (tPart.Position - origRay.Origin).Unit * 1000)
                        return oldNamecall(self, unpack(args))
                    end
                end
            end
        end

        return oldNamecall(self, ...)
    end)
end

-- ============================================================================
-- CLEANUP
-- ============================================================================
Library.OnUnload = function()
    for _, conn in pairs(Cache.Connections) do conn:Disconnect() end
    for p, _ in pairs(Cache.ESP) do RemoveESP(p) end
    HUD.Watermark:Remove()
    HUD.FOVCircle:Remove()
end

Library:Notify({ Title = "shards.cc", Description = "BloxStrike module loaded.", Icon = "check" })
