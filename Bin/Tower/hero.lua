require 'ds.class'
require 'gl.engine'
require 'tower.terrain'
require 'tower.frames'
require 'tower.scene'

--下0, 左1, 上2，右3, 停4

local T = Terrains

Hero = Class()
function Hero:__init__()
  --战斗属性
  self.level = 1
  self.health = 1000
  self.attack = 10
  self.defence = 10
  self.resistance = 0
  
  self.agility = 3
  self.strength = 0
  
  self.experience = 31
  self.gold = 71
  
  --物品
  self.redKey = 0
  self.buleKey = 0
  self.greenKey = 0
  self.yellowKey = 0
  
  --移动
  self.face = 0  --当前朝向
  self.gridx = 0
  self.grixy = 0
  
  self.head = 4  --移动朝向
  self.step = 0  --移动步数
  self.frame = 0 --移动帧数
  
  self.timer = Timer(function()
    self.frame = (self.frame + 1) % 4
  end, 0.08)
  
  self.transition = Animation(self, 'step')
  self.transition.onStart = function()
    self.timer.setTimer()
  end
  
  self.transition.onStop = function()
    local item = self.grid[2]
    self.gridx, self.gridy = self.nextGrid()
    self.step = 0
    
    if item and item.onEnter then
      item.onEnter(self)
    end
    
    if self.head ~= 4 then
      self.face = self.head
      local x, y = self.nextGrid()
      local grid = Game.getGrid(x, y)
      if grid and T[grid[1]][2] then
        local item = grid[2]
        if not item or item.toEnter(self) then
          self.grid = grid
          self.transition.start(32, 0.16)
          return
        end
      end
    end
    self.head = 4
    self.frame = 0
    self.timer.killTimer()
  end
end

function Hero:place(x, y, face)
  self.face = face
  self.head = 4
  self.gridx = x
  self.gridy = y
  self.frame = 0
  self.step = 0
  self.transition.stop()
end

function Hero:nextGrid()
  local x = self.gridx
  local y = self.gridy
  local f = self.face
  if f == 0 then
    y = y - 1
  elseif f == 1 then
    x = x - 1
  elseif f == 2 then
    y = y + 1
  elseif f == 3 then
    x = x + 1
  end
  return x, y
end

function Hero:move(face)
  self.head = face
  if not self.timer.enabled then
    self.face = face
    local x, y = self.nextGrid()
    local grid = Game.getGrid(x, y)
    if grid and T[grid[1]][2] then
      local item = grid[2]
      if not item or item.toEnter(self) then
        self.step = 0
        self.grid = grid
        self.transition.start(32, 0.16)
        return
      end
    end
    self.head = 4
  end
end

function Hero:stop(face)
  if not face or self.head == face then
    self.head = 4
  end
end

function Hero:location()
  if not self.transition.running then
    return self.gridx * 32, self.gridy * 32
  else
    local face = self.face
    if face == 1 then
      return self.gridx * 32 - self.step, self.gridy * 32
    elseif face == 0 then
      return self.gridx * 32, self.gridy * 32 - self.step
    elseif face == 3 then
      return self.gridx * 32 + self.step, self.gridy * 32
    elseif face == 2 then
      return self.gridx * 32, self.gridy * 32 + self.step
    end
  end
end

function Hero:render()
  local face = self.face
  local frame = Frames['英雄下']
  if face == 1 then
    frame = Frames['英雄左']
  elseif face == 2 then
    frame = Frames['英雄上']
  elseif face == 3 then
    frame = Frames['英雄右']
  end
  
  local x, y = self.location()
  local image = loadImage(frame.image)
  local texuv = frame.frame[self.frame + 1]
  drawImage(image, x, y, 32, 32, texuv[1], texuv[2], 32, 32, {1, 1, 1, 1})
end

function Hero:onLeave()
  self.stop()
  self.transition.stop()
end

function Hero:doAttack(monster)
  if self.attack > monster.defence then
    return self.attack - monster.defence
  end
  return 0
end
