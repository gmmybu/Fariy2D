require 'gl.engine'

--16px*16px的地图元素

Tile = Class()

Tile.Wall = 1
Tile.Iron = 2
Tile.Tree = 3
Tile.Lake = 4
Tile.Road = 5

local canDetectCollision = {1, 1, 0, 1, 0, 1, 1, 1, 1}

function Tile:__init__(type, locX, locY)
  self.type = type
  self.locX = locX
  self.locY = locY
end

local frame = 0

function Tile.updateLake()
  frame = (frame + 1) % 2
end

local color = {1, 1, 1, 1}

local image = loadImage('tank\\terr.png')

local drawImage = drawImage

function Tile:render()
  local top = self.type == Tile.Lake and frame * 16 or 0
  drawImage(image, self.locX, self.locY, 16, 16, self.type * 16, top, 16, 16, color)
end

function Tile:detectCollision(direction, p1, p2, val, oldVal)
  if not canDetectCollision[self.type] then	return val end

  local locX = self.locX
  local locY = self.locY

  if direction == Direction.East then
    if p1 >= locY + 16  or p2 <= locY or oldVal > locX then
      return val
    end

    if val > locX then return locX end

    return val
  end

  if direction == Direction.West then
    if p1 >= locY + 16  or p2 <= locY or oldVal < locX + 16 then
      return val
    end

    if val < locX + 16 then return locX + 16 end

    return val
  end

  if direction == Direction.North then
    if p1 >= locX + 16  or p2 <= locX or oldVal < locY + 16 then
      return val
    end

    if val < locY + 16 then return locY + 16 end

    return val
  end

  if direction == Direction.South then
    if p1 >= locX + 16  or p2 <= locX or oldVal > locY then
      return val
    end

    if val > locY then return locY end

    return val
  end
end