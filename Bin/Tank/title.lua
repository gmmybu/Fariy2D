require 'gl.define'
require 'tank.scene'

local state = {opening = 1, ongoing = 2, leaving = 3}

TitleLayer = Class()

function TitleLayer:__init__()
  self.offsetY = -480

  self.openingAni = Animation(self, 'offsetY')
  self.openingAni.onStop = self.onOpeningStop

  self.selectIndex = 1
  self.selectFrame = 0
  self.selectTimer = Timer(self.onTimer, 0.1)

  self.keyboardHandlers = {self.handle_keyboard_event_opening,
                           self.handle_keyboard_event_ongoing}

  self.scissorY = 0
  self.leavingAni = Animation(self, 'scissorY')
  self.leavingAni.onStop = self.onLeavingStop
end

function TitleLayer:onOpeningStop()
  self.state = state.ongoing

  self.offsetY = 0
  self.selectTimer.setTimer()
end

function TitleLayer:onLeavingStop()
  --场景转换
  Game.delLayer(self)

  local worldLayer = WorldLayer()
  worldLayer.load(1)

  Game.addLayer(worldLayer)
end

function TitleLayer:onTimer()
  self.selectFrame = (self.selectFrame + 1) % 2
end

function TitleLayer:onEnter()
  self.state = state.opening
  self.openingAni.start(0, 3.2)
end

local VK_S = 83
local VK_W = 87
local VK_ENTER = 13

function TitleLayer:handle_keyboard_event_opening(what, how)
  if how == Win32.WM_KEYDOWN and what == VK_ENTER then
    self.openingAni.stop()
  end  
end

function TitleLayer:handle_keyboard_event_ongoing(what, how)
  if how == Win32.WM_KEYDOWN then
    if what == VK_ENTER then
      self.selectTimer.killTimer()
      self.state = state.leaving
      self.leavingAni.start(208, 0.8)
    elseif what == VK_W then
      self.selectIndex = self.selectIndex == 1 and 3 or self.selectIndex - 1
    elseif what == VK_S then
      self.selectIndex = self.selectIndex == 3 and 1 or self.selectIndex + 1
    end    
  end
end

function TitleLayer:handle_keyboard_event(what, how)
  local handler = self.keyboardHandlers[self.state]
  if handler then handler(what, how) end
end

local bkgnd = loadImage('tank\\char.png')
local white = {1.0, 1.0, 1.0, 1.0}
local black = {0.0, 0.0, 0.0, 1.0}

local loadImage = loadImage
local drawImage = drawImage

local function drawSprite(sprite, x, y, f)
  local image = loadImage(sprite.image)
  local frame = sprite.frame
  drawImage(image, x, y, 32, 32, frame[f][1], frame[f][2], 32, 32, white)
end

local font24 = loadTextRenderer('微软雅黑', 24)
local sprite = {image = 'tank\\tank.png', frame = {{36 * 32, 0}, {54 * 32, 0}}}

function TitleLayer:render()
  local matrix = Matrix()
  matrix.translate(0, self.offsetY)
  pushMatrix(matrix, true)

  drawImage(bkgnd, 52, 175, 372, 156, 0, 0, 372, 156, white)

  drawString(font24, '1 PLAYER',     192, 130, white)
  drawString(font24, '2 PLAYERS',    192, 100, white)
  drawString(font24, 'CONSTRUCTION', 192,  70, white)

  if self.state == state.ongoing then
    local tankY = (3 - self.selectIndex) * 30 + 66
    drawSprite(sprite, 150, tankY, self.selectFrame + 1)
  end

  popMatrix()

  if self.state == state.leaving then
    drawRectangle(0, 0, 480, self.scissorY, black)
    drawRectangle(0, 416 - self.scissorY, 480, 416, black)
  end
end

