require 'gl.engine'

Bullet = Class()

function Bullet:__init__(locX, locY, camp, power, direction)
  self.locX = locX
  self.locY = locY
  
  self.camp = camp
  self.power = power
  self.direction = direction
end

local color = {1, 1, 1, 1}

local image = loadImage('tank\\misc.png')

local drawImage = drawImage

function Bullet:render()
  drawImage(image, self.locX - 4, self.locY - 4, 8, 8, self.direction * 8 - 8, 0, 8, 8, color)
end