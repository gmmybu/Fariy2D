require 'gl.engine'
require 'gl.matrix'
require 'gl.define'
require 'tower.scene'

local drawImage = drawImage
local window = loadImage('window\\window.png')
local border = loadImage('window\\border.png')
local frames = {{160, 64}, {176, 64}, {160, 80}, {176, 80}}

TalkLayer = Class()

function TalkLayer:__init__(left, bottom, width, height, lineheight, renderer, color)
  self.left   = left
  self.bottom = bottom
  self.width  = width
  self.height = height
  self.lineheight = lineheight
  self.renderer = renderer
  self.color = color
  
  self.frame = 0
  self.timer = Timer(function()
    self.frame = (self.frame + 1) % 4
  end, 0.2)
end

function TalkLayer:onEnter()
  self.timer.setTimer()
end

function TalkLayer:onLeave()
  self.timer.killTimer()
  self.thread = nil
end

function TalkLayer:showSentence(texts, selections)
  self.texts = texts
  self.select = 1
  self.selections = selections
  
  self.alpha = 0
  local transition = Animation(self, 'alpha')
  transition.onStart = disableKeyboard
  transition.onStop  = enableKeyboard
  
  transition.start(1, 0.2)
  local answer = coroutine.yield()
  
  --[[self.alpha = 1
  local transition = Animation(self, 'alpha')
  transition.onStart = disableKeyboard
  transition.onStop  = function()
    enableKeyboard()
    coroutine.resume(self.thread)
    if coroutine.status(self.thread) == 'dead' then
      local scene = getCurrentScene()
      scene.delLayer(self)
    end
  end
  
  transition.start(0, 0.2)
  coroutine.yield()]]--
  
  return answer
end

function TalkLayer:begin()
  if self.talk then
    local scene = getCurrentScene()
    scene.addLayer(self)
    self.thread = coroutine.create(self.talk)
    coroutine.resume(self.thread)
    if coroutine.status(self.thread) == 'dead' then
      scene.delLayer(self)
    end
  end
end

function TalkLayer:render()
  local matrix = Matrix()
  matrix.translate(self.left, self.bottom)
  pushMatrix(matrix, true)
  
  drawImage(window, 0, 0, self.width, self.height, 0, 0, 128, 128, {1, 1, 1, .75})
  drawImageScaleGrid(border, -1, -1, self.width + 2, self.height + 2, 4, 4, 4, 4, {0.9, 0.9, 0.9, 1})
  
  local left = 14
  local bottom = self.height - self.lineheight
  local color = {self.color[1], self.color[2], self.color[3], self.color[4] * self.alpha}
  for i, v in ipairs(self.texts) do
    drawString(self.renderer, v, left, bottom, color)
    bottom = bottom - self.lineheight
  end
  
  if self.selections then
    for i, v in ipairs(self.selections) do
      if i == self.select then
        drawString(self.renderer, v, left, bottom, {1, 0, 0, self.alpha})
      else
        drawString(self.renderer, v, left, bottom, {0, 0, 1, self.alpha})
      end
      bottom = bottom - self.lineheight
    end
  end
  
  if getKeyboardEnabled() then
    local frame = frames[self.frame + 1]
    drawImage(window, self.left + self.width / 2 - 8, 0, 16, 16, frame[1], frame[2], 16, 16, {1, 1, 1, 1})
  end
  
  popMatrix()
end

function TalkLayer:handle_keyboard_event(what, how)
  if how == Win32.WM_KEYDOWN then
    if what == Win32.VK_RETURN then
      local answer
      if self.selections then
        answer = self.selections[self.select]
      end
      coroutine.resume(self.thread, answer)
      if coroutine.status(self.thread) == 'dead' then
        local scene = getCurrentScene()
        scene.delLayer(self)
      end
    elseif what == Win32.VK_UP then
      if self.select ~= 1 then
        self.select = self.select - 1
      end
    elseif what == Win32.VK_DOWN then
      if self.select ~= #self.selections then
        self.select = self.select + 1
      end
    end
  end
  return true
end

function TalkLayer:talk()
  self.showSentence({'商店:', '人类啊!', '如果你愿意给我20个金币的话', '我可以增加你的能力'})
  self.showSentence({}, {'2攻击($20)', '3防御($20)', '400血($20)', '离开'})
  
  self.showSentence({'firstline', 'second line', 'third line'})
  local choice = self.showSentence({'what\'s your choice'}, {'A', 'B'})
  if choice == 'A' then
    self.showSentence({'answer is A', 'second line', 'third line'})
  else
    self.showSentence({'answer is B', 'second line', 'third line'})
  end
  self.showSentence({'firstline', 'second line', 'third line'})
  self.showSentence({'firstline', 'second line', 'third line'})
  self.showSentence({'firstline', 'second line', 'third line'})
  self.showSentence({'firstline', 'second line', 'third line'})
  self.showSentence({'firstline', 'second line', 'third line'})
  self.showSentence({'firstline', 'second line', 'third line'})
  self.showSentence({'firstline', 'second line', 'third line'})
  self.showSentence({'firstline', 'second line', 'third line'})
end