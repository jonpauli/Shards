--!nocheck
-- SHARDS.CC // Arsenal Ultimate 
-- Uses high-perf math, custom FOV projection, and standard methods.
-- Mounts Shards UI internally.

local Players = game:GetService("Players")
local RunService = game:GetService("RunService")
local UserInputService = game:GetService("UserInputService")
local Workspace = game:GetService("Workspace")

local LocalPlayer = Players.LocalPlayer
local Camera = Workspace.CurrentCamera

-- Mount Shards UI
local Library = loadstring(game:HttpGet("https://raw.githubusercontent.com/jonpauli/Shards/refs/heads/main/ShardsUI.lua"))()

local Window = Library:CreateWindow({
    Title = "shards.cc | arsenal ultimate",
    SubTitle = "optimized hitscan & esp",
    Size = UDim2.fromOffset(620, 480),
    ToggleKey = Enum.KeyCode.Insert,
    Accent = Color3.fromRGB(155, 195, 55)
})

-- ============================================================================
-- UI CONFIG
-- ============================================================================
local CombatTab = Window:AddTab({ Name = "Combat", Icon = "crosshair" })

local AimGroup = CombatTab:AddLeftGroupbox("Aimbot", "target")
AimGroup:AddToggle({ Name = "Enabled", Flag = "Aim_Enabled", Default = false })
    :AddKeybind({ Default = Enum.KeyCode.MouseButton2, Mode = "Hold", Flag = "Aim_Bind" })

AimGroup:AddToggle({ Name = "Team Check", Flag = "Aim_TeamCheck", Default = true })
AimGroup:AddToggle({ Name = "Visibility Check", Flag = "Aim_VisCheck", Default = true })

AimGroup:AddSlider({ Name = "Field of View", Flag = "Aim_FOV", Min = 0, Max = 600, Default = 80, Rounding = 0, Suffix = "px" })
AimGroup:AddSlider({ Name = "Smoothing", Flag = "Aim_Smooth", Min = 1, Max = 25, Default = 5, Rounding = 1 })
AimGroup:AddDropdown({ Name = "Target Bone", Flag = "Aim_Hitbox", List = {"Head", "UpperTorso", "HumanoidRootPart"}, Default = "Head" })

local FOVGroup = CombatTab:AddRightGroupbox("Field of View", "scan")
FOVGroup:AddToggle({ Name = "Show FOV", Flag = "Aim_ShowFOV", Default = true }):AddColorPicker({ Default = Color3.fromRGB(155, 195, 55), Flag = "Aim_FOVColor" })
FOVGroup:AddSlider({ Name = "FOV Thickness", Flag = "Aim_FOVThick", Min = 1, Max = 5, Default = 1, Rounding = 1 })
FOVGroup:AddSlider({ Name = "FOV Transparency", Flag = "Aim_FOVTrans", Min = 0, Max = 1, Default = 0.8, Rounding = 2 })

local VisualsTab = Window:AddTab({ Name = "Visuals", Icon = "eye" })
local ESPGroup = VisualsTab:AddLeftGroupbox("ESP Main", "box")
ESPGroup:AddToggle({ Name = "Enabled", Flag = "ESP_Enabled", Default = false })
ESPGroup:AddToggle({ Name = "Show Boxes", Flag = "ESP_Boxes", Default = true }):AddColorPicker({ Default = Color3.fromRGB(255, 255, 255), Flag = "ESP_BoxColor" })
ESPGroup:AddToggle({ Name = "Show Names", Flag = "ESP_Names", Default = true }):AddColorPicker({ Default = Color3.fromRGB(255, 255, 255), Flag = "ESP_NameColor" })
ESPGroup:AddToggle({ Name = "Show Health", Flag = "ESP_Health", Default = true })
ESPGroup:AddToggle({ Name = "Show Distance", Flag = "ESP_Dist", Default = false })

local ESPLayoutGroup = VisualsTab:AddRightGroupbox("ESP Settings", "layers")
ESPLayoutGroup:AddDropdown({ Name = "Box Type", Flag = "ESP_BoxType", List = {"Bounding", "Corners"}, Default = "Bounding" })

local SettingsTab = Window:AddTab({ Name = "Settings", Icon = "settings" })
Library:BuildConfigTab(SettingsTab)

-- ============================================================================
-- DRAWING CLASSES & CACHE
-- ============================================================================
local FOVCircle = Drawing.new("Circle")
FOVCircle.Thickness = 1
FOVCircle.NumSides = 120
FOVCircle.Filled = false
FOVCircle.Visible = false

local ESP_Cache = {}

local function CreateESP(player)
    local dr = {
        Box = Drawing.new("Square"),
        BoxOutline = Drawing.new("Square"),
        Name = Drawing.new("Text"),
        HealthBar = Drawing.new("Square"),
        HealthBarBg = Drawing.new("Square"),
        Distance = Drawing.new("Text"),
        -- For Corners
        TL = Drawing.new("Line"), TR = Drawing.new("Line"),
        BL = Drawing.new("Line"), BR = Drawing.new("Line"),
        LT = Drawing.new("Line"), LB = Drawing.new("Line"),
        RT = Drawing.new("Line"), RB = Drawing.new("Line")
    }

    dr.Box.Thickness = 1
    dr.Box.ZIndex = 10
    dr.Box.Filled = false

    dr.BoxOutline.Thickness = 3
    dr.BoxOutline.Color = Color3.new(0, 0, 0)
    dr.BoxOutline.ZIndex = 9
    dr.BoxOutline.Filled = false

    -- Lines configuration for corner box
    for _, k in ipairs({"TL","TR","BL","BR","LT","LB","RT","RB"}) do
        dr[k].Thickness = 1
        dr[k].ZIndex = 10
    end

    dr.Name.Size = 13
    dr.Name.Center = true
    dr.Name.Outline = true
    dr.Name.Font = 2
    dr.Name.ZIndex = 11

    dr.Distance.Size = 12
    dr.Distance.Center = true
    dr.Distance.Outline = true
    dr.Distance.Font = 2
    dr.Distance.ZIndex = 11

    dr.HealthBar.Filled = true
    dr.HealthBar.ZIndex = 11
    dr.HealthBarBg.Filled = true
    dr.HealthBarBg.Color = Color3.new(0, 0, 0)
    dr.HealthBarBg.ZIndex = 10

    ESP_Cache[player] = dr
end

local function RemoveESP(player)
    if ESP_Cache[player] then
        for _, dr in pairs(ESP_Cache[player]) do dr:Remove() end
        ESP_Cache[player] = nil
    end
end

-- ============================================================================
-- MATH & UTILS
-- ============================================================================
local function GetChar(p) return p.Character end
local function GetHum(p) 
    local c = GetChar(p)
    return c and c:FindFirstChildOfClass("Humanoid")
end

local function IsAlive(p)
    local h = GetHum(p)
    return h and h.Health > 0 and GetChar(p):FindFirstChild("Head")
end

local function GetClosest()
    local target, dist = nil, math.huge
    local mouse = UserInputService:GetMouseLocation()
    local fovRadius = Library.Flags.Aim_FOV

    for _, p in pairs(Players:GetPlayers()) do
        if p == LocalPlayer or not IsAlive(p) then continue end
        if Library.Flags.Aim_TeamCheck and p.Team == LocalPlayer.Team then continue end

        local bone = GetChar(p):FindFirstChild(Library.Flags.Aim_Hitbox)
        if not bone then continue end

        local pos, onScreen = Camera:WorldToViewportPoint(bone.Position)
        if not onScreen then continue end

        if Library.Flags.Aim_VisCheck then
            -- High perf raycast equivalent using native obscure check
            local cast = Camera:GetPartsObscuringTarget({Camera.CFrame.Position, bone.Position}, {LocalPlayer.Character, Camera})
            if #cast > 0 then continue end
        end

        local mag = (Vector2.new(pos.X, pos.Y) - mouse).Magnitude
        if mag < fovRadius and mag < dist then
            target = p
            dist = mag
        end
    end
    return target
end

-- ============================================================================
-- MAIN LOOP
-- ============================================================================
RunService.RenderStepped:Connect(function()
    local mouse = UserInputService:GetMouseLocation()
    
    -- FOV Update
    FOVCircle.Position = mouse
    FOVCircle.Radius = Library.Flags.Aim_FOV
    FOVCircle.Color = Library.Flags.Aim_FOVColor
    FOVCircle.Thickness = Library.Flags.Aim_FOVThick
    FOVCircle.Transparency = Library.Flags.Aim_FOVTrans
    FOVCircle.Visible = Library.Flags.Aim_ShowFOV and Library.Flags.Aim_Enabled

    -- Aimbot Logic
    if Library.Flags.Aim_Enabled and Library.Options.Aim_Bind.Active then
        local target = GetClosest()
        if target then
            local bone = GetChar(target)[Library.Flags.Aim_Hitbox]
            local targetCF = CFrame.new(Camera.CFrame.Position, bone.Position)
            Camera.CFrame = Camera.CFrame:Lerp(targetCF, 1 / Library.Flags.Aim_Smooth)
        end
    end

    -- ESP Logic
    for p, dr in pairs(ESP_Cache) do
        local alive = IsAlive(p)
        local visible = Library.Flags.ESP_Enabled and alive and (not Library.Flags.Aim_TeamCheck or p.Team ~= LocalPlayer.Team)

        if visible then
            local char = GetChar(p)
            local root = char.HumanoidRootPart
            local head = char.Head
            
            local rootPos, onScreen = Camera:WorldToViewportPoint(root.Position)
            if onScreen then
                local headPos = Camera:WorldToViewportPoint(head.Position + Vector3.new(0, 0.8, 0))
                local footPos = Camera:WorldToViewportPoint(root.Position - Vector3.new(0, 3.2, 0))
                
                local h = math.abs(headPos.Y - footPos.Y)
                local w = h * 0.55
                local size = Vector2.new(w, h)
                local pos = Vector2.new(rootPos.X - w/2, headPos.Y)

                local isCorner = Library.Flags.ESP_BoxType == "Corners"
                local boxVis = Library.Flags.ESP_Boxes

                -- Reset standard box
                dr.Box.Visible = boxVis and not isCorner
                dr.BoxOutline.Visible = boxVis and not isCorner
                
                -- Reset corner lines
                for _, k in ipairs({"TL","TR","BL","BR","LT","LB","RT","RB"}) do
                    dr[k].Visible = boxVis and isCorner
                end

                if boxVis then
                    if not isCorner then
                        dr.Box.Size = size
                        dr.Box.Position = pos
                        dr.Box.Color = Library.Flags.ESP_BoxColor
                        
                        dr.BoxOutline.Size = size
                        dr.BoxOutline.Position = pos
                    else
                        local c = Library.Flags.ESP_BoxColor
                        local length = w / 3
                        
                        -- Top Left
                        dr.TL.From = pos
                        dr.TL.To = pos + Vector2.new(length, 0)
                        dr.LT.From = pos
                        dr.LT.To = pos + Vector2.new(0, length)
                        
                        -- Top Right
                        dr.TR.From = pos + Vector2.new(w, 0)
                        dr.TR.To = pos + Vector2.new(w - length, 0)
                        dr.RT.From = pos + Vector2.new(w, 0)
                        dr.RT.To = pos + Vector2.new(w, length)
                        
                        -- Bottom Left
                        dr.BL.From = pos + Vector2.new(0, h)
                        dr.BL.To = pos + Vector2.new(length, h)
                        dr.LB.From = pos + Vector2.new(0, h)
                        dr.LB.To = pos + Vector2.new(0, h - length)
                        
                        -- Bottom Right
                        dr.BR.From = pos + Vector2.new(w, h)
                        dr.BR.To = pos + Vector2.new(w - length, h)
                        dr.RB.From = pos + Vector2.new(w, h)
                        dr.RB.To = pos + Vector2.new(w, h - length)
                        
                        for _, k in ipairs({"TL","TR","BL","BR","LT","LB","RT","RB"}) do
                            dr[k].Color = c
                        end
                    end
                end

                -- Name
                dr.Name.Visible = Library.Flags.ESP_Names
                dr.Name.Text = p.Name
                dr.Name.Position = Vector2.new(rootPos.X, pos.Y - 15)
                dr.Name.Color = Library.Flags.ESP_NameColor

                -- Health
                if Library.Flags.ESP_Health then
                    local hum = GetHum(p)
                    local pct = math.clamp(hum.Health / hum.MaxHealth, 0, 1)
                    local barH = h * pct
                    
                    dr.HealthBarBg.Visible = true
                    dr.HealthBarBg.Size = Vector2.new(4, h + 2)
                    dr.HealthBarBg.Position = Vector2.new(pos.X - 6, pos.Y - 1)

                    dr.HealthBar.Visible = true
                    dr.HealthBar.Size = Vector2.new(2, barH)
                    dr.HealthBar.Position = Vector2.new(pos.X - 5, pos.Y + (h - barH))
                    dr.HealthBar.Color = Color3.fromHSV(pct * 0.35, 0.9, 1)
                else
                    dr.HealthBar.Visible = false
                    dr.HealthBarBg.Visible = false
                end

                -- Distance
                if Library.Flags.ESP_Dist then
                    local d = math.floor((root.Position - Camera.CFrame.Position).Magnitude)
                    dr.Distance.Visible = true
                    dr.Distance.Text = "[" .. d .. "m]"
                    dr.Distance.Position = Vector2.new(rootPos.X, pos.Y + h + 2)
                    dr.Distance.Color = Library.Flags.ESP_NameColor
                else
                    dr.Distance.Visible = false
                end
            else
                for _, v in pairs(dr) do v.Visible = false end
            end
        else
            for _, v in pairs(dr) do v.Visible = false end
        end
    end
end)

-- Lifecycle
for _, p in pairs(Players:GetPlayers()) do if p ~= LocalPlayer then CreateESP(p) end end
Players.PlayerAdded:Connect(function(p) CreateESP(p) end)
Players.PlayerRemoving:Connect(RemoveESP)

Library:Notify({ Title = "Loaded", Description = "Arsenal Ultimate ready. Run up the score.", Duration = 4 })
