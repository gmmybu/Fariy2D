require 'ds.class'
require 'gl.engine'
require 'tower.sprite'

local Sprites = Sprites

local loadImage = loadImage
local drawImage = drawImage
local function render(sprite)
  return function(x, y)
    local image = loadImage(sprite.image)
    local frame = sprite.frame
    drawImage(image, x, y, 32, 32,
      frame[1], frame[2], 32, 32, {1, 1, 1, 1})
  end
end

Properties = {
  ['黄钥匙'] = {
    render = render(Sprites['黄钥匙']),
    toEnter = function(player)
      local x, y = player.nextGrid()
      Game.delGrid(x, y)
    
      local count = player.yellowKey or 0
      player.yellowKey = count + 1
      
      return true
    end
  },
  
  ['蓝钥匙'] = {
    render = render(Sprites['蓝钥匙']),
    toEnter = function(player)
      local x, y = player.nextGrid()
      Game.delGrid(x, y)
      
      local count = player.blueKey or 0
      player.blueKey = count + 1
      
      return true
    end
  },
  
  ['红钥匙'] = {
    render = render(Sprites['红钥匙']),
    toEnter = function(player)
      local x, y = player.nextGrid()
      Game.delGrid(x, y)
      
      local count = player.redKey or 0
      player.redKey = count + 1
      
      return true  
    end
  },

  ['绿钥匙'] = {
    render = render(Sprites['绿钥匙']),
    toEnter = function(player)
      local x, y = player.nextGrid()
      Game.delGrid(x, y)
      
      local count = player.greenKey or 0
      player.greenKey = count + 1
      
      return true
    end
  },
  
  ['红宝石'] = {
    render = render(Sprites['红宝石']),
    toEnter = function(player)
      local x, y = player.nextGrid()
      Game.delGrid(x, y)
      
      local count = player.attack or 0
      player.attack = count + 3
      
      return true
    end
  },
  
  ['蓝宝石'] = {
    render = render(Sprites['蓝宝石']),
    toEnter = function(player)
      local x, y = player.nextGrid()
      Game.delGrid(x, y)
      
      local count = player.defence or 0
      player.defence = count + 3
      
      return true
    end
  },
  
  ['红药水'] = {
    render = render(Sprites['红药水']),
    toEnter = function(player)
      local x, y = player.nextGrid()
      Game.delGrid(x, y)
      
      local count = player.health or 0
      player.health = count + 100
      
      return true
    end
  },
  
  ['蓝药水'] = {
    render = render(Sprites['蓝药水']),
    toEnter = function(player)
      local x, y = player.nextGrid()
      Game.delGrid(x, y)
      
      local count = player.health or 0
      player.health = count + 200
      
      return true
    end
  },
  
  ['黄药水'] = {
    render = render(Sprites['黄药水']),
    toEnter = function(player)
      local x, y = player.nextGrid()
      Game.delGrid(x, y)
      
      local count = player.health or 0
      player.health = count + 500
      
      return true
    end
  },
  
  ['绿药水'] = {
    render = render(Sprites['绿药水']),
    toEnter = function(player)
      local x, y = player.nextGrid()
      Game.delGrid(x, y)
      
      local count = player.health or 0
      player.health = count + 1000
      
      return true
    end
  },
  
  ['武器'] = {
    render = render(Sprites['武器']),
    toEnter = function(player)
      local x, y = player.nextGrid()
      Game.delGrid(x, y)
      
      local count = player.attack or 0
      player.attack = count + 10
      
      return true
    end
  },
  
  ['护甲'] = {
    render = render(Sprites['护甲']),
    toEnter = function(player)
      local x, y = player.nextGrid()
      Game.delGrid(x, y)
      
      local count = player.defence or 0
      player.defense = count + 10
      
      return true
    end
  },
  
  ['盾牌'] = {
    render = render(Sprites['盾牌']),
    toEnter = function(player)
      local x, y = player.nextGrid()
      Game.delGrid(x, y)
      
      local count = player.defence or 0
      player.defence = count + 10
      
      return true
    end
  },
}

local Door = Class()
function Door:__init__(texture, frames, field)
  self.texture = texture
  self.frames  = frames
  
  self.timer = Timer(function()
    self.frame = self.frame + 1
    if self.frame == 5 then
      self.timer.killTimer()
      enableKeyboard()
      Game.delGrid(self.x, self.y)
    end
  end, 0.1)
  
  self.field = field
  self.frame = 1
end

function Door:render(x, y)
  local image, frame
  if self.opening then
    image = loadImage(self.frames.image)
    frame = self.frames.frame[self.frame]
  else
    image = loadImage(self.texture.image)
    frame = self.texture.frame
  end
  if frame then
    drawImage(image, x, y, 32, 32,
      frame[1], frame[2], 32, 32, {1, 1, 1, 1})
  end
end

function Door:toEnter(player, x, y)
  local gridx, gridy = player.nextGrid()
  self.x = gridx
  self.y = gridy
  
  local count = player[self.field]
  if count and count >= 1 then
    player[self.field] = count - 1
    self.opening = true
    disableKeyboard()
    self.timer.setTimer()
  end
end

YellowDoor = Class(Door)
function YellowDoor:__init__()
  Door.__init__(self, Sprites['黄门'], Frames['黄门'], 'yellowKey')
end

RedDoor = Class(Door)
function RedDoor:__init__()
  Door.__init__(self, Sprites['红门'], Frames['红门'], 'redKey')
end
