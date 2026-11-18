--!nocheck
-- ============================================================================
-- SHARDS.CC // APOCALYPSE RISING 2 
-- ============================================================================

local CoreGui = game:GetService("CoreGui")
local Players = game:GetService("Players")
local RunService = game:GetService("RunService")
local UserInputService = game:GetService("UserInputService")
local Workspace = game:GetService("Workspace")
local Lighting = game:GetService("Lighting")
local HttpService = game:GetService("HttpService")

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
-- HUD & CACHE
-- ============================================================================
local HUD = { Watermark = nil }

local GameDirs = {
    Zombies = Workspace:FindFirstChild("Zombies") and Workspace.Zombies:FindFirstChild("Mobs") or Workspace,
    Loot = Workspace:FindFirstChild("Loot") or Workspace,
    Vehicles = Workspace:FindFirstChild("Vehicles") and Workspace.Vehicles:FindFirstChild("Spawned") or Workspace
}

local Cache = {
    Connections = {},
    Players = {},
    Zombies = {},
    Loot = {},
    Vehicles = {},
    Target = nil,
    OriginalStats = {} -- For reverting gun mods / hitboxes
}

local function InitHUD()
    local wm = Drawing.new("Text")
    wm.Text = "shards.cc | apocalypse rising 2 | " .. Library.Version
    wm.Size = 14
    wm.Font = 2
    wm.Color = Color3.new(1, 1, 1)
    wm.Outline = true
    wm.Position = Vector2.new(15, 15)
    wm.Visible = true
    HUD.Watermark = wm
end

-- ============================================================================
-- UI BUILDER
-- ============================================================================
local Window = Library:CreateWindow({ Title = "shards.cc // apocalypse rising 2", Center = true, AutoShow = true })

local Tabs = {
    Combat = Window:AddTab({ Title = "Combat", Icon = "crosshair" }),
    Visuals = Window:AddTab({ Title = "Visuals", Icon = "scan" }),
    Movement = Window:AddTab({ Title = "Movement", Icon = "activity" }),
    World = Window:AddTab({ Title = "World", Icon = "globe" }),
    Settings = Window:AddTab({ Title = "Settings", Icon = "sliders" }),
}

local AimbotGrp = Tabs.Combat:AddLeftGroupbox("Aimbot", "target")
AimbotGrp:AddToggle({ Flag = "Aimbot_Enabled", Name = "Enable Aimbot" }):AddKeybind({ Flag = "Aimbot_Key", Mode = "Hold", Key = "MouseButton2" })
AimbotGrp:AddToggle({ Flag = "SilentAim_Enabled", Name = "Silent Aim (Safe)" })
AimbotGrp:AddDropdown({ Flag = "Aimbot_Hitbox", Name = "Hitbox", Options = {"Head", "Torso", "Random"}, Default = "Head" })
AimbotGrp:AddSlider({ Flag = "Aimbot_FOV", Name = "FOV Radius", Min = 10, Max = 800, Default = 150 })
AimbotGrp:AddSlider({ Flag = "Aimbot_Smoothness", Name = "Smoothness", Min = 1, Max = 10, Default = 1 })

local GunModsGrp = Tabs.Combat:AddRightGroupbox("Gun Mods", "cpu")
GunModsGrp:AddToggle({ Flag = "Gun_ModsEnabled", Name = "Enable Gun Mods (Apply)" })
GunModsGrp:AddToggle({ Flag = "Gun_NoRecoil", Name = "No Recoil" })
GunModsGrp:AddToggle({ Flag = "Gun_NoSpread", Name = "No Spread" })
GunModsGrp:AddToggle({ Flag = "Gun_InstaEquip", Name = "Instant Equip" })
GunModsGrp:AddToggle({ Flag = "Gun_HitboxExp", Name = "Hitbox Expander" })

local PlayerEspGrp = Tabs.Visuals:AddLeftGroupbox("Player ESP", "users")
PlayerEspGrp:AddToggle({ Flag = "ESP_Enabled", Name = "Enable ESP" }):AddKeybind({ Flag = "ESP_Key", Mode = "Toggle", Key = "None" })
PlayerEspGrp:AddToggle({ Flag = "ESP_Boxes", Name = "Bounding Boxes" }):AddColorPicker({ Flag = "ESP_BoxCol", Color = Color3.fromRGB(255, 255, 255) })
PlayerEspGrp:AddToggle({ Flag = "ESP_Names", Name = "Names" }):AddColorPicker({ Flag = "ESP_NameCol", Color = Color3.fromRGB(255, 255, 255) })
PlayerEspGrp:AddToggle({ Flag = "ESP_Distance", Name = "Distance" })
PlayerEspGrp:AddToggle({ Flag = "ESP_Health", Name = "Health Bar" })
PlayerEspGrp:AddSlider({ Flag = "ESP_MaxDist", Name = "Max Distance", Min = 100, Max = 5000, Default = 1500, Suffix = "st" })

local WorldEspGrp = Tabs.Visuals:AddRightGroupbox("World ESP", "globe")
WorldEspGrp:AddToggle({ Flag = "ESP_Zombies", Name = "Zombies" }):AddColorPicker({ Flag = "ESP_ZomCol", Color = Color3.fromRGB(255, 50, 50) })
WorldEspGrp:AddToggle({ Flag = "ESP_Loot", Name = "Loot" }):AddColorPicker({ Flag = "ESP_LootCol", Color = Color3.fromRGB(50, 255, 50) })
WorldEspGrp:AddToggle({ Flag = "ESP_Vehicles", Name = "Vehicles" }):AddColorPicker({ Flag = "ESP_VehCol", Color = Color3.fromRGB(255, 200, 50) })
WorldEspGrp:AddSlider({ Flag = "ESP_WorldMaxDist", Name = "World Max Dist", Min = 50, Max = 1000, Default = 300, Suffix = "st" })

local MoveGrp = Tabs.Movement:AddLeftGroupbox("Movement", "activity")
MoveGrp:AddToggle({ Flag = "Move_Speed", Name = "CFrame Speedhack" }):AddKeybind({ Flag = "Move_SpeedKey", Mode = "Hold", Key = "None" })
MoveGrp:AddSlider({ Flag = "Move_SpeedAmt", Name = "Speed Amount", Min = 0.1, Max = 3, Default = 0.5, Decimals = 1 })
MoveGrp:AddToggle({ Flag = "Move_Noclip", Name = "Safe Noclip" }):AddKeybind({ Flag = "Move_NoclipKey", Mode = "Toggle", Key = "None" })
MoveGrp:AddToggle({ Flag = "Move_InfStam", Name = "Infinite Stamina" })

local EnvGrp = Tabs.World:AddLeftGroupbox("Environment", "sun")
EnvGrp:AddToggle({ Flag = "Env_Fullbright", Name = "Fullbright" })
EnvGrp:AddToggle({ Flag = "Env_NoFog", Name = "No Fog" })
EnvGrp:AddToggle({ Flag = "Env_Time", Name = "Custom Time" })
EnvGrp:AddSlider({ Flag = "Env_TimeVal", Name = "Time of Day", Min = 0, Max = 24, Default = 12 })

local UtilGrp = Tabs.World:AddRightGroupbox("Utility", "tool")
UtilGrp:AddToggle({ Flag = "Util_FastInteract", Name = "Fast Interact" })

Library:BuildConfigTab(Tabs.Settings)

-- ============================================================================
-- UTILITIES & ESP RENDERING
-- ============================================================================
local function GetClosestTarget()
    local closestDist = Flags.Aimbot_FOV
    local target = nil
    for _, player in ipairs(Players:GetPlayers()) do
        if player ~= LocalPlayer and player.Character then
            local root = player.Character:FindFirstChild("HumanoidRootPart")
            if root then
                local pos, onScreen = Camera:WorldToViewportPoint(root.Position)
                if onScreen then
                    local mouse = UserInputService:GetMouseLocation()
                    local dist = (Vector2.new(pos.X, pos.Y) - mouse).Magnitude
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

-- Player ESP Cache
local function CreateESP(player)
    Cache.Players[player] = {
        Box = Drawing.new("Square"),
        Name = Drawing.new("Text"),
        HealthBar = Drawing.new("Square"),
        HealthOutline = Drawing.new("Square"),
        Distance = Drawing.new("Text"),
    }
    local esp = Cache.Players[player]
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

for _, p in ipairs(Players:GetPlayers()) do if p ~= LocalPlayer then CreateESP(p) end end
Players.PlayerAdded:Connect(function(p) CreateESP(p) end)
Players.PlayerRemoving:Connect(function(p)
    if Cache.Players[p] then for _, draw in pairs(Cache.Players[p]) do draw:Remove() end Cache.Players[p] = nil end
end)

-- World ESP generic creation
local function GetWorldESP(inst, cacheTable)
    if not cacheTable[inst] then
        local txt = Drawing.new("Text")
        txt.Size = 13
        txt.Center = true
        txt.Outline = true
        txt.Visible = false
        cacheTable[inst] = txt
    end
    return cacheTable[inst]
end

local function ClearDeadWorldESP(cacheTable)
    for inst, draw in pairs(cacheTable) do
        if not inst or not inst.Parent then
            draw:Remove()
            cacheTable[inst] = nil
        end
    end
end

-- ============================================================================
-- MAIN LOOP
-- ============================================================================
Cache.Connections.RenderStepped = RunService.RenderStepped:Connect(function(deltaTime)
    
    Cache.Target = GetClosestTarget()
    if HUD.Watermark then HUD.Watermark.Text = string.format("shards.cc | apocalypse rising 2 | %d FPS", math.floor(1 / deltaTime)) end
    
    -- 1. Player ESP & Hitbox Expander
    for player, esp in pairs(Cache.Players) do
        local char = player.Character
        local root = char and char:FindFirstChild("HumanoidRootPart")
        local hum = char and char:FindFirstChildOfClass("Humanoid")
        local isAlive = char and root and hum and hum.Health > 0
        local onScreen = false
        local pos = Vector3.zero
        
        if isAlive and Flags.ESP_Enabled then
            pos, onScreen = Camera:WorldToViewportPoint(root.Position)
            local dist = (Camera.CFrame.Position - root.Position).Magnitude
            if dist > Flags.ESP_MaxDist then onScreen = false end
            
            -- Hitbox Expander logic
            local head = char:FindFirstChild("Head")
            if head then
                if Flags.Gun_HitboxExp then
                    if not Cache.OriginalStats[player] then Cache.OriginalStats[player] = head.Size end
                    head.Size = Vector3.new(4, 4, 4)
                    head.Transparency = 0.6
                else
                    if Cache.OriginalStats[player] then
                        head.Size = Cache.OriginalStats[player]
                        head.Transparency = 0
                    end
                end
            end
        end
        
        if isAlive and onScreen then
            local tl, _ = Camera:WorldToViewportPoint(root.Position + Vector3.new(0, 3, 0))
            local br, _ = Camera:WorldToViewportPoint(root.Position - Vector3.new(0, 3.5, 0))
            local height = math.abs(tl.Y - br.Y)
            local width = height * 0.6
            local x = pos.X - (width / 2)
            local y = tl.Y
            
            if Flags.ESP_Boxes then
                esp.Box.Visible = true
                esp.Box.Size = Vector2.new(width, height)
                esp.Box.Position = Vector2.new(x, y)
                esp.Box.Color = Flags.ESP_BoxCol
            else esp.Box.Visible = false end
            
            if Flags.ESP_Names then
                esp.Name.Visible = true
                esp.Name.Text = player.Name
                esp.Name.Position = Vector2.new(pos.X, y - 15)
                esp.Name.Color = Flags.ESP_NameCol
            else esp.Name.Visible = false end
            
            if Flags.ESP_Distance then
                esp.Distance.Visible = true
                esp.Distance.Text = string.format("[%d st]", (Camera.CFrame.Position - root.Position).Magnitude)
                esp.Distance.Position = Vector2.new(pos.X, br.Y + 2)
                esp.Distance.Color = Flags.ESP_NameCol
            else esp.Distance.Visible = false end
            
            if Flags.ESP_Health then
                esp.HealthOutline.Visible = true
                esp.HealthBar.Visible = true
                local hpPct = hum.Health / hum.MaxHealth
                esp.HealthOutline.Size = Vector2.new(3, height)
                esp.HealthOutline.Position = Vector2.new(x - 5, y)
                esp.HealthBar.Size = Vector2.new(1, height * hpPct)
                esp.HealthBar.Position = Vector2.new(x - 4, y + (height - esp.HealthBar.Size.Y))
                esp.HealthBar.Color = Color3.fromHSV(hpPct * 0.3, 1, 1)
            else
                esp.HealthOutline.Visible = false
                esp.HealthBar.Visible = false
            end
        else
            for _, draw in pairs(esp) do draw.Visible = false end
        end
    end
    
    -- 2. World ESP (Zombies, Loot, Vehicles)
    ClearDeadWorldESP(Cache.Zombies)
    ClearDeadWorldESP(Cache.Loot)
    ClearDeadWorldESP(Cache.Vehicles)

    if Flags.ESP_Enabled then
        local camPos = Camera.CFrame.Position
        
        -- Zombies
        if Flags.ESP_Zombies and GameDirs.Zombies then
            for _, z in ipairs(GameDirs.Zombies:GetChildren()) do
                local root = z:FindFirstChild("HumanoidRootPart")
                if root then
                    local dist = (camPos - root.Position).Magnitude
                    local d = GetWorldESP(z, Cache.Zombies)
                    if dist <= Flags.ESP_WorldMaxDist then
                        local pos, vis = Camera:WorldToViewportPoint(root.Position)
                        if vis then
                            d.Visible = true
                            d.Text = string.format("Zombie [%dm]", dist)
                            d.Position = Vector2.new(pos.X, pos.Y)
                            d.Color = Flags.ESP_ZomCol
                        else d.Visible = false end
                    else d.Visible = false end
                end
            end
        else for _, d in pairs(Cache.Zombies) do d.Visible = false end end

        -- Vehicles
        if Flags.ESP_Vehicles and GameDirs.Vehicles then
            for _, v in ipairs(GameDirs.Vehicles:GetChildren()) do
                local root = v.PrimaryPart or v:FindFirstChildWhichIsA("BasePart")
                if root then
                    local dist = (camPos - root.Position).Magnitude
                    local d = GetWorldESP(v, Cache.Vehicles)
                    if dist <= Flags.ESP_WorldMaxDist then
                        local pos, vis = Camera:WorldToViewportPoint(root.Position)
                        if vis then
                            d.Visible = true
                            d.Text = string.format("%s [%dm]", v.Name, dist)
                            d.Position = Vector2.new(pos.X, pos.Y)
                            d.Color = Flags.ESP_VehCol
                        else d.Visible = false end
                    else d.Visible = false end
                end
            end
        else for _, d in pairs(Cache.Vehicles) do d.Visible = false end end
    else
        for _, d in pairs(Cache.Zombies) do d.Visible = false end
        for _, d in pairs(Cache.Loot) do d.Visible = false end
        for _, d in pairs(Cache.Vehicles) do d.Visible = false end
    end
    
    -- 3. Camera Aimbot
    if Flags.Aimbot_Enabled and Flags.Aimbot_Key and Cache.Target then
        local tPart = Cache.Target.Character:FindFirstChild(Flags.Aimbot_Hitbox or "Head")
        if tPart then
            local newCFrame = CFrame.new(Camera.CFrame.Position, tPart.Position)
            if Flags.Aimbot_Smoothness > 1 then
                Camera.CFrame = Camera.CFrame:Lerp(newCFrame, 1 / Flags.Aimbot_Smoothness)
            else
                Camera.CFrame = newCFrame
            end
        end
    end
    
    -- 4. Speedhack & Noclip
    local myChar = LocalPlayer.Character
    local myRoot = myChar and myChar:FindFirstChild("HumanoidRootPart")
    local myHum = myChar and myChar:FindFirstChildOfClass("Humanoid")
    
    if myRoot and myHum then
        if Flags.Move_Speed and Flags.Move_SpeedKey and myHum.MoveDirection.Magnitude > 0 then
            myRoot.CFrame = myRoot.CFrame + (myHum.MoveDirection * Flags.Move_SpeedAmt)
        end
        if Flags.Move_Noclip and Flags.Move_NoclipKey then
            for _, part in ipairs(myChar:GetChildren()) do
                if part:IsA("BasePart") then part.CanCollide = false end
            end
        end
    end
    
    -- 5. Environment
    if Flags.Env_Fullbright then
        Lighting.Ambient = Color3.new(1, 1, 1)
        Lighting.ColorShift_Bottom = Color3.new(1, 1, 1)
        Lighting.ColorShift_Top = Color3.new(1, 1, 1)
    end
    if Flags.Env_NoFog then Lighting.FogEnd = 100000 end
    if Flags.Env_Time then Lighting.ClockTime = Flags.Env_TimeVal end
end)

-- ============================================================================
-- PERIODIC THREADS (Gun Mods, Fast Interact, Stamina)
-- ============================================================================
task.spawn(function()
    while task.wait(1) do
        -- Fast Interact
        if Flags.Util_FastInteract then
            for _, prompt in ipairs(Workspace:GetDescendants()) do
                if prompt:IsA("ProximityPrompt") then
                    prompt.HoldDuration = 0
                end
            end
        end
        
        -- Gun Mods (Scanning GC for Weapon Config Tables)
        if Flags.Gun_ModsEnabled then
            for _, v in pairs(getgc(true)) do
                if type(v) == "table" and rawget(v, "RecoilMin") and rawget(v, "Spread") then
                    if Flags.Gun_NoRecoil then
                        rawset(v, "RecoilMin", 0)
                        rawset(v, "RecoilMax", 0)
                        rawset(v, "CameraKickMin", Vector3.zero)
                        rawset(v, "CameraKickMax", Vector3.zero)
                    end
                    if Flags.Gun_NoSpread then
                        rawset(v, "Spread", 0)
                        rawset(v, "WalkSpread", 0)
                    end
                    if Flags.Gun_InstaEquip then
                        rawset(v, "EquipTime", 0)
                    end
                end
            end
        end
        
        -- Infinite Stamina (AR2 uses specific controllers, generic spoof via LocalScript GC if exposed)
        if Flags.Move_InfStam then
            for _, v in pairs(getgc(true)) do
                if type(v) == "table" and rawget(v, "Stamina") and rawget(v, "MaxStamina") then
                    rawset(v, "Stamina", rawget(v, "MaxStamina"))
                end
            end
        end
    end
end)

-- ============================================================================
-- HOOKS
-- ============================================================================
local oldNamecall
oldNamecall = hookmetamethod(game, "__namecall", function(self, ...)
    local method = getnamecallmethod()
    local args = {...}
    
    if not checkcaller() then
        if (method == "FindPartOnRayWithIgnoreList" or method == "Raycast") and Flags.SilentAim_Enabled then
            if Cache.Target and Cache.Target.Character then
                local tPart = Cache.Target.Character:FindFirstChild(Flags.Aimbot_Hitbox or "Head")
                if tPart then
                    if method == "Raycast" then
                        args[2] = (tPart.Position - args[1]).Unit * 5000
                        return oldNamecall(self, unpack(args))
                    end
                end
            end
        end
    end
    
    return oldNamecall(self, ...)
end)

-- ============================================================================
-- CLEANUP
-- ============================================================================
InitHUD()

Library.OnUnload = function()
    for _, conn in pairs(Cache.Connections) do conn:Disconnect() end
    for _, esp in pairs(Cache.Players) do for _, draw in pairs(esp) do draw:Remove() end end
    for _, draw in pairs(Cache.Zombies) do draw:Remove() end
    for _, draw in pairs(Cache.Loot) do draw:Remove() end
    for _, draw in pairs(Cache.Vehicles) do draw:Remove() end
    if HUD.Watermark then HUD.Watermark:Remove() end
    
    -- Revert hitbox expansions
    for player, size in pairs(Cache.OriginalStats) do
        if player and player.Character and player.Character:FindFirstChild("Head") then
            player.Character.Head.Size = size
            player.Character.Head.Transparency = 0
        end
    end
end

Library:Notify({ Title = "shards.cc", Text = "Apoc 2 Loaded fully. Toggle menu with Insert.", Duration = 4 })