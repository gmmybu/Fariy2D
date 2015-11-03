require 'tower.game'

glClearColor(0, 0, 0, 1)
glEnable(3042)
glBlendFunc(0x302, 0x303)
glViewport(480, 480)

setCurrentScene(Game)

local game =
{
  update = update, render = render,
  handle_mouse_event = handle_mouse_event,
  handle_keyboard_event = handle_keyboard_event
}

return game