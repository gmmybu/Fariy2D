require 'gl.define'
require 'gl.engine'
require 'gl.matrix'
require 'tower.evil'
require 'tower.prop'
require 'tower.hero'
require 'tower.port'
require 'tower.scene'
require 'tower.terrain'
require 'tower.talk'
require 'tower.npc'
require 'tower.frames'

local P = Properties
local M = Monsters
local T = Terrains
--楼层
local F = {
  {
    {{4}, {4}, {4}, {3}, {1}, {3}, {4}, {4}, {4}},
    {{4}, {3}, {3}, {3}, {1}, {3}, {3}, {3}, {4}},
    
    {{4}, {3}, {1, P['黄钥匙']}, {3}, {1}, {3}, {1, P['红钥匙']}, {3}, {4}},
    
    {{4}, {3}, {1}, {1}, {1}, {1}, {1}, {3}, {4}},
    
    {{1}, {3}, {1, P['黄钥匙']}, {3}, {1}, {3}, {1, P['蓝钥匙']}, {3}, {1}},
    
    {{3}, {3}, {3}, {3}, {1, M['绿色史莱姆']}, {3}, {3}, {3}, {3}},
    
    {{2}, {2}, {2}, {3}, {1}, {3}, {2}, {2}, {2}},
    {{2}, {2}, {2}, {3}, {1}, {3}, {2}, {2}, {2}},
    {{2}, {2}, {2}, {3}, {1}, {3}, {2}, {2}, {2}}
  },
  
  {
    {{1}, {1, P['蓝宝石']}, {2}, {2}, {1}, {1, P['蓝钥匙']}, {2}, {2}, {2}},
    {{2}, {1, M['绿色史莱姆']}, {1, M['红色史莱姆']}, {1, M['绿色史莱姆']}, {1}, {2}, {1, P['红宝石']}, {1, P['蓝钥匙']}, {1, P['蓝宝石']}},
    {{2}, {1}, {2}, {2}, {1}, {2}, {1, P['红药水']}, {1, M['小蝙蝠']}, {1, P['红药水']}},
    {{2}, {1, M['小蝙蝠']}, {1}, {1, P['蓝药水']}, {1}, {2}, {1, M['绿色史莱姆']}, {1}, {1, M['绿色史莱姆']}},
    {{2}, {1}, {2}, {2}, {1, YellowDoor()}, {2}, {2}, {1}, {2}},
    {{2}, {1}, {2}, {2}, {1, M['绿色史莱姆']}, {1}, {1}, {1}, {1}},
    {{2}, {1, M['绿色史莱姆']}, {1}, {1, P['红药水']}, {1, M['红色史莱姆']}, {2}, {2}, {1, M['骷髅怪']}, {2}},
    {{2}, {1}, {2}, {2}, {1, M['绿色史莱姆']}, {2}, {1}, {1, P['黄药水']}, {1, NPC(Frames['老人'])}},
    {{1, NPC(Frames['老人'])}, {1, M['绿色史莱姆']}, {1}, {1}, {1}, {2}, {1, P['蓝宝石']}, {1}, {1, NPC(Frames['老人'])}}
  },
  
  {
  
  },
}

local renderer = loadTextRenderer('黑体', 18)

--添加传送点
table.insert(F[1][9][5], UpLadder(F[2], 4, 8, 0))
table.insert(F[2][9][5], DownLadder(F[1], 4, 8, 0))
table.insert(F[2][1][1], UpLadder(F[3], 0, 0, 0))

--添加对话
F[2][9][1][2].toEnter = function()
  local layer = TalkLayer(15, 36, 450, 126, 28, renderer, {0.8,0.9,1,1})
  layer.talk = function()
    layer.showSentence({'老人:', '这座塔有30层，但据说不止30层'})
  end
  layer.begin()
end

F[2][9][9][2].toEnter = function()
  local layer = TalkLayer(15, 36, 450, 126, 28, renderer, {0.8,0.9,1,1})
  layer.talk = function()
    layer.showSentence({'老人:', '铁件在3楼，铁盾在4楼，金剑在5楼，金', '盾在6楼，尽快拿到提升你的力量'})
  end
  layer.begin()
end

F[2][8][9][2].toEnter = function()
  local layer = TalkLayer(15, 36, 450, 126, 28, renderer, {0.8,0.9,1,1})
  layer.talk = function()
    layer.showSentence({'老人:', '你知道什么叫章吗？那是一种非常强大的', '东西，但是我们从来没有见识过他的威力......'})
    layer.showSentence({'老人:', '你可以展示给我看嘛？'})
  end
  layer.begin()
end


local GameLayer = Class()
function GameLayer:__init__()
  self.hero = Hero()
  
  self.frame = 0
  self.timer = Timer(self.onTimer, 0.4)
  
  self.alpha = 1
  self.transition = Animation(self, 'alpha')
  self.transition.onStart = self.onStart
  self.transition.onStop  = self.onStop
  self.renderTarget = createRenderTexture(288, 288)
  
  self.transport(F[1], 4, 0, 0)
end

function GameLayer:onStart()
  disableKeyboard()
  self.hero.stop()
end

function GameLayer:onStop()
  enableKeyboard()
  self.render = self.oldrender
  self.oldfloor = nil
end

function GameLayer:onTimer()
  self.frame = (self.frame + 1) % 4
end

local renderer = loadTextRenderer('黑体', 18)
function GameLayer:onEnter()
  self.timer.setTimer()
end

function GameLayer:onLeave()
  self.hero.onLeave()
  
  self.timer.killTimer()
end

local loadImage = loadImage
local drawImage = drawImage
local color = {1, 1, 1, 1}
local function drawSprite(sprite, x, y)
  local image = loadImage(sprite.image)
  local frame = sprite.frame
  drawImage(image, x, y, 32, 32, frame[1], frame[2], 32, 32, color)
end

local outerlb = Sprites['边框左下']
local outerb  = Sprites['边框下']
local outerrb = Sprites['边框右下']

local outerl = Sprites['边框左']
local outerr = Sprites['边框右']
local outert = Sprites['边框上']

function GameLayer:renderBorder()
  drawSprite(outerlb, 0, 0)
  for i = 1, 9 do
    drawSprite(outerb, i * 32, 0)
  end
  drawSprite(outerrb, 320, 0)
  
  for i = 1, 9 do
    drawSprite(outerl, 0, i * 32)
    drawSprite(outerr, 320, i * 32)
  end
  
  for i = 0, 10 do
    drawSprite(outert, i * 32, 320)
  end
end

local background = loadImage('window\\background.png')
function GameLayer:renderTransition()
  drawImageScaleGrid(background, 0, 0, 480, 480, 16, 16, 16, 16, color)
  
  local matrix1 = Matrix()
  matrix1.translate(64, 64)
  pushMatrix(matrix1, true)
  self.renderBorder()
  
  local matrix2 = Matrix()
  matrix2.translate(32, 32)
  pushMatrix(matrix2, true)
  
  for i = 0, 8 do
    for j = 0, 8 do
      local x, y = j * 32, i * 32
      local grid = self.oldfloor[i + 1][j + 1]
      
      drawSprite(T[grid[1]][1], j * 32, i * 32)
      
      if grid[2] and grid[2].render then
        grid[2].render(x, y, self.frame)
      end
    end
  end
  popMatrix()
  
  popMatrix()
  
  self.renderTarget:beginRender()
  local matrix3 = Matrix()
  matrix3.translate(-144, -144)
  matrix3.scale(1, -1)
  matrix3.translate(144, 144)
  pushMatrix(matrix3, true)
    
  for i = 0, 8 do
    for j = 0, 8 do
      local x, y = j * 32, i * 32
      local grid = self.floor[i + 1][j + 1]
      
      drawSprite(T[grid[1]][1], j * 32, i * 32)
      
      if grid[2] and grid[2].render then
        grid[2].render(x, y, self.frame)
      end
    end
  end
  self.hero.render()
  
  popMatrix()
  
  self.renderTarget:endRender()
  
  drawImage(self.renderTarget, 96, 96, 288, 288, 0, 0, 288, 288, {1, 1, 1, self.alpha})
end

function GameLayer:render()
  drawImageScaleGrid(background, 0, 0, 480, 480, 16, 16, 16, 16, color)
  
  local matrix1 = Matrix()
  matrix1.translate(64, 64)
  pushMatrix(matrix1, true)
  self.renderBorder()

  local matrix2 = Matrix()
  matrix2.translate(32, 32)
  pushMatrix(matrix2, true)
  
  for i = 0, 8 do
    for j = 0, 8 do
      local x, y = j * 32, i * 32
      local grid = self.floor[i + 1][j + 1]
      
      drawSprite(T[grid[1]][1], j * 32, i * 32)
      
      if grid[2] and grid[2].render then
        grid[2].render(x, y, self.frame)
      end
    end
  end
  self.hero.render()
  
  popMatrix()
  
  popMatrix()
end

function GameLayer:handle_keyboard_event(what, how)
  if how == Win32.WM_KEYDOWN then
    if what == Win32.VK_DOWN      then
      self.hero.move(0)
    elseif what == Win32.VK_LEFT  then
      self.hero.move(1)
    elseif what == Win32.VK_UP    then
      self.hero.move(2)
    elseif what == Win32.VK_RIGHT then
      self.hero.move(3)
    end
  else
    if what == Win32.VK_DOWN      then
      self.hero.stop(0)
    elseif what == Win32.VK_LEFT  then
      self.hero.stop(1)
    elseif what == Win32.VK_UP    then
      self.hero.stop(2)
    elseif what == Win32.VK_RIGHT then
      self.hero.stop(3)
    end
  end
end

function GameLayer:transport(level, gridx, gridy, face)
  self.oldfloor = self.floor
  self.floor = level
  self.hero.place(gridx, gridy, face)
  if self.oldfloor then
    self.alpha = 0
    self.oldrender = self.render
    self.render = self.renderTransition
    self.transition.start(1, 0.75)
  end
end

function GameLayer:getGrid(gridx, gridy)
  if gridx >= 0 and gridx <= 8 and gridy >= 0 and gridy <= 8 then
    return self.floor[gridy + 1][gridx + 1]
  end
end

Game = Scene()

local layer = GameLayer()
Game.addLayer(layer)

function Game.getGrid(x, y)
  return layer.getGrid(x, y)
end

function Game.delGrid(x, y)
  local grid = layer.getGrid(x, y)
  if grid then
    grid[2] = nil
  end
end

function Game.transport(level, gridx, gridy, face)
  layer.transport(level, gridx, gridy, face)
end