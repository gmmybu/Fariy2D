require 'gl.engine'
require 'game.snake'

glViewport(800, 600)

local snake = Snake(20, 15)

local game = {}

function game.update(elapse)
  snake.update(elapse)
end

function game.render()
  snake.render()
end

function game.handle_mouse_event(x, y, how, pressed)

end

local VK_LEFT = 0x25
local VK_UP   = 0x26
local VK_RIGHT = 0x27
local VK_DOWN  = 0x28

function game.handle_keyboard_event(what, how)
  if how == 0x100 then
    if what == VK_RIGHT then
      snake.turn(0)
    elseif what == VK_UP then
      snake.turn(1)
    elseif what == VK_LEFT then
      snake.turn(2)
    elseif what == VK_DOWN then
      snake.turn(3)
    end
  end
end

glClearColor(190.0/255, 157/255.0, 118/255.0, 1)
glEnable(3042)
glBlendFunc(770, 771)

return game