require 'ds.class'

Scene = Class()
function Scene:__init__()
  self.layers = Queue()
  self.status = 0
end

function Scene:render()
  local layers = Queue()
  for layer in self.layers.riterator() do
    layers.push_front(layer)
    if layer.fullscreen then
      break
    end
  end
  
  for layer in layers.iterator() do
    layer.render()
  end
end

function Scene:handle_mouse_event(x, y, how, press)
  for layer in self.layers.riterator() do
    if layer.handle_mouse_event and
       layer.handle_mouse_event(x, y, how, press)
    then
      break
    end
  end
end

function Scene:handle_keyboard_event(what, how)
  for layer in self.layers.riterator() do
    if layer.handle_keyboard_event and
       layer.handle_keyboard_event(what, how)
    then
      break
    end
  end
end

function Scene:addLayer(layer)
  self.layers.push_back(layer)
  if self.status == 1 and layer.onEnter then
    layer.onEnter()
  end
end

function Scene:delLayer(layer)
  self.layers.remove(layer)
  if self.status == 1 and layer.onLeave then
    layer.onLeave()
  end
end

function Scene:triggerEvent(event)
  if event == 'enter' then
    if self.onEnter then
      self.onEnter()
    end
  elseif event == 'enter_finish' then
    if self.onEnterFinish then
      self.onEnterFinish()
    end
  elseif event == 'leave_start' then
    if self.onLeaveStart then
      self.onLeaveStart()
    end
  elseif event == 'leave' then
    if self.onLeave then
      self.onLeave()
    end
  end
end

function Scene:enter()
  self.status = 1
  self.triggerEvent('enter')
  self.triggerEvent('enter_finish')
end

function Scene:leave()
  self.status = 0
  self.triggerEvent('leave_start')
  self.triggerEvent('leave')
end

function Scene:onEnter()
  for layer in self.layers.iterator() do
    if layer.onEnter then
      layer.onEnter()
    end
  end
end

function Scene:onEnterFinish()

end

function Scene:onLeaveStart()

end

function Scene:onLeave()
  for layer in self.layers.iterator() do
    if layer.onLeave then
      layer.onLeave()
    end
  end
end

local current
function setCurrentScene(scene)
  if not scene.transition and current and current.leave  then
    current.leave()
  end
  
  current = scene  
  if scene.enter then
    scene.enter()
  end
end

function getCurrentScene()
  return current
end


require 'gl.engine'

function render()
  if current and current.render then
    current.render()
  end
end

local keyboardDisabled

function disableKeyboard()
  keyboardDisabled = true
end

function enableKeyboard()
  keyboardDisabled = false
end

function getKeyboardEnabled()
  return not keyboardDisabled
end

function handle_keyboard_event(what, how)
  if not keyboardDisabled and  current and current.handle_keyboard_event then
    current.handle_keyboard_event(what, how)
  end
end

local mouseDisabled

function disableMouse()
  mouseDisabled = true
end

function enableMouse()
  mouseDisabled = false
end

function getMouseEnabled()
  return not mouseDisabled
end

function handle_mouse_event(x, y, how, press)
  if not mouseDiabled and  current and current.handle_mouse_event then
    current.handle_mouse_event(x, y, how, press)
  end
end

local callbacks = {}
local incomings = {}
local outgoings = {}
local runupdate
function update(elapse)
  runupdate = true
  for callback, _ in pairs(callbacks) do
    if not outgoings[callback] then
      callback(elapse)
    end
  end
  runupdate = false
  
  for callback, _ in pairs(outgoings) do
    callbacks[callback] = nil
  end
  outgoings = {}
  
  for callback, _ in pairs(incomings) do
    callbacks[callback] = true
  end
  incomings = {}
end

function schedule(callback)
  if not runupdate then
    callbacks[callback] = true
  else
    outgoings[callback] = nil
    incomings[callback] = true
  end
end

function unschedule(callback)
  if not runupdate then
    callbacks[callback] = nil
  else
    incomings[callback] = nil
    outgoings[callback] = true
  end
end

Timer = Class()
function Timer:__init__(func, interval)
  self.func = func
  self.enabled = false
  self.interval = interval
end

function Timer:update(elapse)
  self.time = self.time + elapse
  while self.time > self.interval and self.enabled do
    self.time = self.time - self.interval
    self.func()
  end
end

function Timer:setInterval(interval)
  self.interval = interval
end

function Timer:setTimer()
  self.time = 0
  self.enabled = true
  schedule(self.update)
end

function Timer:killTimer()
  self.enabled = false
  unschedule(self.update)
end

Animation = Class()
function Animation:__init__(obj, key)
  self.obj = obj
  self.key = key
end

function Animation:step(elapse)
  self.time = self.time + elapse
  local prog = self.time / self.period
  if prog > 1 then
    prog = 1
  end
  self.update(prog)
end

function Animation:update(prog)
  self.obj[self.key] = self.from + (self.dest - self.from) * prog
  if prog == 1 then
    self.stop()
  end
end

function Animation:start(dest, period)
  self.from = self.obj[self.key]
  self.dest = dest
  self.time = 0
  self.period = period
  self.running = true
  schedule(self.step)
  if self.onStart then
    self.onStart()
  end
end

function Animation:stop()
  if self.running then
    self.running = false
    unschedule(self.step)
    if self.onStop then
      self.onStop()
    end
  end
end

function scheduleonce(func)
  local callback
  callback = function()
    func()
    unschedule(callback)
  end
  schedule(callback)
end
