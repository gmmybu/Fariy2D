require 'gl.define'
require 'gl.engine'
require 'gl.matrix'
require 'bomb.scene'

local textures = {
  [0] = loadImage('bomb\\0.jpg'),
  loadImage('bomb\\1.jpg'),
  loadImage('bomb\\2.jpg'),
  loadImage('bomb\\3.jpg'),
  loadImage('bomb\\4.jpg'),
  loadImage('bomb\\5.jpg'),
  loadImage('bomb\\6.jpg'),
  loadImage('bomb\\7.jpg'),
  loadImage('bomb\\8.jpg'),
}

local bomb = loadImage('bomb\\bomb.jpg')
local flag = loadImage('bomb\\flag.jpg')
local null = loadImage('bomb\\null.jpg')

local GameLayer = Class()
function GameLayer:__init__()
  math.randomseed(os.time())
  self.grids = {}
  for i = 1,24 do
    local grids = {}
    for j = 1,24 do
      local grid = {}
      grid.isbomb = math.random() < 0.15
      grids[j] = grid
    end
    self.grids[i] = grids
  end
end

local drawImage = drawImage
local color = {1, 1, 1, 1}
local function drawSprite(sprite, x, y)
  drawImage(sprite, x, y, 20, 20, 0, 0, 20, 20, color)
end

function GameLayer:render()
  local matrix = Matrix()
  matrix.translate(1, 0)
  pushMatrix(matrix, true)
  
  for i = 1,24 do
    local grids = self.grids[i]
    local y = (i - 1) * 20
    for j = 1,24 do
      local x = (j - 1) * 20
      local g = grids[j]
      local p = null
      if g.isreveal then
        p = g.isbomb and bomb or textures[g.number]
      elseif g.isflag then
        p = flag
      end
      drawSprite(p, x, y)
    end
  end
  
  for i = 0, 24 do
    drawLine(-1, i * 20, 480, i * 20, {0.5,0.5,0.5,1})
  end
  
  for i = 0, 24 do
    drawLine(i * 20, -1, i * 20, 480, {0.5,0.5,0.5,1})
  end
  
  popMatrix()
end

function GameLayer:isBomb(i, j)
  if i >= 1 and i <= 24 and j >= 1 and j <= 24 and self.grids[i][j].isbomb then
    return 1
  end
  return 0
end

function GameLayer:expand(i, j)
  if i >= 1 and i <= 24 and j >= 1 and j <= 24 then
    local grid = self.grids[i][j]
    if grid.isreveal then return end
    
    grid.isreveal = true
    if grid.isbomb then
      self.gameover = true
      return
    end
    
    grid.number = self.isBomb(i, j + 1) + 
                  self.isBomb(i, j - 1) + 
                  self.isBomb(i + 1, j + 1) +
                  self.isBomb(i + 1, j - 1) + 
                  self.isBomb(i + 1, j) + 
                  self.isBomb(i - 1, j) + 
                  self.isBomb(i - 1, j + 1) +
                  self.isBomb(i - 1, j - 1);

    if grid.number == 0 then
       self.expand(i, j + 1)
       self.expand(i, j - 1)
       self.expand(i + 1, j)
       self.expand(i - 1, j)
    end
  end
end

function GameLayer:handle_mouse_event(x, y, how, press)
  if self.gameover then return end
  
  local i = math.floor((480 - y) / 20 + 1)
  local j = math.floor(x / 20 + 1)
  if i >= 1 and i <= 24 and j >= 1 and j <= 24 then
    local grid = self.grids[i][j]
    if grid.isreveal then return end
    
    if how == Win32.WM_RBUTTONDOWN then
      grid.isflag = not grid.isflag
    elseif how == Win32.WM_LBUTTONDOWN then
      self.expand(i, j)
    end
  end
end

Game = Scene()
Game.addLayer(GameLayer())
