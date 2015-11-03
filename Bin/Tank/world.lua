require 'tank.scene'

require 'tank.tile'
require 'tank.tank'
require 'tank.rune'
require 'tank.bullet'
require 'tank.explosion'
require 'tank.stage'

WorldLayer = Class()

function WorldLayer:__init__()
  self.roads = {} --加载后数据不变
  
  self.trees = {} --加载后数据不变

  self.lakes = {} --加载后数据不变

  self.tiles = {} --可以破坏的地形

  self.tanks = {}

  self.bullets = {}

  self.explosions = {}

  self.runes = {}

  self.enemies = {}
end

--加载关卡数据
function WorldLayer:load(stage)
  local data = StageData[stage]
  for i = 1, 26 do
    for j = 1, 26 do
      local tile = data[(j - 1) * 26 + i]

      local locX = ( i - 1) * 16
      local locY = (26 - j) * 16 

      if tile == Tile.Road then
        table.insert(self.roads, Tile(Tile.Road, locX, locY))
      elseif tile == Tile.Tree then
        table.insert(self.trees, Tile(Tile.Tree, locX, locY))
      elseif tile == Tile.Lake then
        table.insert(self.lakes, Tile(Tile.Lake, locX, locY))
      else
        table.insert(self.tiles, Tile(tile, locX, locY))
      end
    end
  end

  self.player1Tank = Tank(134, 16, Direction.South, 0)
  table.insert(self.tanks, self.player1Tank)
end

function WorldLayer:onEnter()
  schedule(self.update)

  if #self.lakes > 0 then
    self.lakeTimer = Timer(Tile.updateLake, 1.0)
    self.lakeTimer.setTimer()
  end
end

function WorldLayer:onLeave()
  unschedule(self.update)

  if self.lakeTimer then
    self.lakeTimer.killTimer()
  end
end

--假定帧之间为均匀的
function WorldLayer:moveTank(tank)
  if tank.state == TankState.Standing then 
    return
  end
  local p1, p2, val, oldVal
  local direction = tank.direction
  local speed = tank.speed or 2
  if direction == Direction.East then
    p1 = tank.locY - 16
    p2 = tank.locY + 16

    oldVal = tank.locX + 16

    val = tank.locX + 16 + speed
    if val > 416 then
      val = 416
    end

  elseif direction == Direction.West then
    p1 = tank.locY - 16
    p2 = tank.locY + 16

    oldVal = tank.locX - 16

    val = tank.locX - 16 - speed
    if val < 0 then
      val = 0
    end
  elseif direction == Direction.North then
    p1 = tank.locX - 16
    p2 = tank.locX + 16

    oldVal = tank.locY - 16


    val = tank.locY - 16 - speed
    if val < 0 then val = 0 end
  else
    p1 = tank.locX - 16
    p2 = tank.locX + 16

    oldVal = tank.locY + 16

    val = tank.locY + 16 + speed
    if val > 416 then val = 416 end
  end

  print('old val:'..val)

  for i, v in ipairs(self.tanks) do
    if v ~= tank then
      val = v.detectCollision(direction, p1, p2, val, oldVal)
    end
  end

  for i, v in ipairs(self.tiles) do
    val = v.detectCollision(direction, p1, p2, val, oldVal)
  end

  for i, v in ipairs(self.lakes) do
    val = v.detectCollision(direction, p1, p2, val, oldVal)
  end

  print('new val:'..val)

  if direction == Direction.East then
    tank.locX = val - 16
  elseif direction == Direction.West then
    tank.locX = val + 16
  elseif direction == Direction.North then
    tank.locY = val + 16
  else
    tank.locY = val - 16
  end
end


function WorldLayer:update()
  --移动坦克
  for i, v in ipairs(self.tanks) do
    self.moveTank(v)
  end

  --移动子弹(击中测试)
  --
end

function WorldLayer:handle_keyboard_event(what, how)
  if how == Win32.WM_KEYDOWN then
    if what == Win32.VK_DOWN      then
      self.player1Tank.move(Direction.North)
    elseif what == Win32.VK_LEFT  then
      self.player1Tank.move(Direction.West)
    elseif what == Win32.VK_UP    then
      self.player1Tank.move(Direction.South)
    elseif what == Win32.VK_RIGHT then
      self.player1Tank.move(Direction.East)
    end
  else
    if what == Win32.VK_DOWN      then
      self.player1Tank.stop(Direction.North)
    elseif what == Win32.VK_LEFT  then
      self.player1Tank.stop(Direction.West)
    elseif what == Win32.VK_UP    then
      self.player1Tank.stop(Direction.South)
    elseif what == Win32.VK_RIGHT then
      self.player1Tank.stop(Direction.East)
    end
  end
end

function WorldLayer:renderObjects(objects)
  for _, v in ipairs(objects) do
    v.render()
  end
end

function WorldLayer:renderRightPanel()
  drawRectangle(416, 0, 480, 416, {0.5, 0.5, 0.5, 1})
end

--基地如何处理

function WorldLayer:render()
  self.renderObjects(self.roads)

  self.renderObjects(self.tiles)

  self.renderObjects(self.lakes)

  self.renderObjects(self.tanks)
  
  self.renderObjects(self.bullets)
  
  self.renderObjects(self.trees)
  
  self.renderObjects(self.explosions)

  self.renderObjects(self.runes)

  self.renderRightPanel()
end