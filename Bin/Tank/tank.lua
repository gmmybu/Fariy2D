require 'gl.engine'

--数值与纹理位置对应
Direction = {East = 2, West = 4, South = 1, North = 3}

TankState = {Standing = 1, Driving = 2, Slipping = 3}

Tank = Class()

function Tank:__init__(locX, locY, direction, logo, speed)
  self.locX = locX  
  self.locY = locY
  self.logo = logo
  
  self.frame = 0
  self.speed = speed
  self.state = TankState.Standing


  self.direction = direction

  self.frameTimer = Timer(self.updateFrame, speed == 4 and 0.025 or 0.05)
end

local color = {1, 1, 1, 1}

local image = loadImage('tank\\tank.png')

local drawImage = drawImage

function Tank:render()
  local left = (self.direction * 36 - 36 + self.frame * 18 + self.logo) * 32
  drawImage(image, self.locX - 16, self.locY - 16, 32, 32, left, 0, 32, 32, color)
end

function Tank:updateFrame()
  self.frame = (self.frame + 1) % 2
end

function Tank:move(direction)
  if not self.frameTimer.enabled then
    self.frameTimer.setTimer()
  end

  if direction == self.direction and self.state == TankState.Driving then
    return
  end

  --转向调整
  if direction == Direction.West or direction == Direction.East then
    self.locY = math.floor((self.locY + 7) / 16) * 16
  else
    self.locX = math.floor((self.locX + 7) / 16) * 16
  end

  self.direction = direction
  self.state = TankState.Driving
end

function Tank:stop(direction)
  if self.direction == direction and self.state == TankState.Driving then
    self.state = TankState.Standing
    self.frameTimer.killTimer()
  end
end

function Tank:detectCollision(direction, p1, p2, val)
  local locX = self.locX
  local locY = self.locY

  if direction == Direction.East then
    if p1 >= locY + 16  or p2 <= locY - 16 then
      return val
    end

    if val > locX - 16 then return locX - 16 end

    return val
  end

  if direction == Direction.West then
    if p1 >= locY + 16  or p2 <= locY - 16 then
      return val
    end

    if val < locX + 16 then return locX + 16 end

    return val
  end

  if direction == Direction.North then
    if p1 >= locX + 16  or p2 <= locX - 16 then
      return val
    end

    if val < locY + 16 then return locY + 16 end

    return val
  end

  if direction == Direction.South then
    if p1 >= locX + 16  or p2 <= locX - 16 then
      return val
    end

    if val > locY - 16 then return locY - 16 end

    return val
  end

end