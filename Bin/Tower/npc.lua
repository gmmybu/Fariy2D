require 'ds.class'
require 'gl.engine'

local loadImage = loadImage
local drawImage = drawImage

NPC = Class()
function NPC:__init__(frames)
  self.frames = frames
end

function NPC:render(x, y, step)
  local image = loadImage(self.frames.image)
  local frame = self.frames.frame[step + 1]
  drawImage(image, x, y, 32, 32,
    frame[1], frame[2], 32, 32, {1, 1, 1, 1})
end
