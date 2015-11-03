package.cpath = package.cpath..';.\\net\\?.dll'
package.path  = package.path ..';.\\net\\?.lua'

require 'Bomb.game'

glClearColor(1, 1, 1, 1)
glEnable(3042)
glBlendFunc(0x302, 0x303)
glViewport(481, 481)

setCurrentScene(Game)

local game =
{
  update = update, render = render,
  handle_mouse_event = handle_mouse_event,
  handle_keyboard_event = handle_keyboard_event
}

return game