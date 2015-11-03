require 'gl.python'

local Queue = Class()

function Queue:front()
  if self.head then
    return self.head.key
  end
end

function Queue:back()
  if self.tail then
    return self.tail.key
  end
end

function Queue:push_back(val)
  local item = {key = val}
  if self.tail then
    item.prev = self.tail
    self.tail.next = item
    self.tail = item
  else
    self.head = item
    self.tail = item
  end
end

function Queue:push_front(val)
  local item = {key = val}
  if self.head then
    self.head.prev = item
    item.next = self.head
    self.head = item
  else
    self.head = item
    self.tail = item
  end
end

function Queue:pop_back()
  if not self.tail then
    return
  end
  
  self.tail = self.tail.prev
  if not self.tail then
    self.head = nil
  else
    self.tail.next = nil
  end
end

function Queue:pop_front()
  if not self.head then return end

  self.head = self.head.next
  if not self.head then
    self.tail = nil
  else
    self.head.prev = nil
  end
end

Snake = Class()
function Snake:__init__(x, y)
  self.x = x
  self.y = y
  self.g = x * y
  self.m = {}
  
  self.reset()
end

function Snake:mark(px, py, val)
  self.m[1 + py * self.x + px] = val
end

function Snake:getMark(px, py)
  return self.m[1 + py * self.x + px]
end

function Snake:spawn()
  local p = math.random() * self.r
  for i, v in ipairs(self.m) do
    if v == 0 then
      p = p - 1
      if p < 1 then
        local m = i % self.x
        local d = math.floor(i / self.x)
        self.mark(m, d, 1)
        self.r = self.r - 1
        return
      end
    end
  end
end

function Snake:update(elapse)
  if self.o then return end

  self.p = self.p + elapse
  while self.p > self.i and not self.o do
    local pos = self.q.front()
    
    local nx, ny = self.forward(pos[1], pos[2])
    
    local mark = self.getMark(nx, ny)
    
    if mark == 1 then
      self.q.push_front({nx, ny})
      self.mark(nx, ny, 2)
      self.spawn()
      if self.i > 0.12 then
        self.i = self.i * 0.9
      end
      self.p = self.p + self.i / 2
    elseif mark == 2 then
      self.o = true
      return
    else
      self.q.push_front({nx, ny})
      self.mark(nx, ny, 2)
      
      local back = self.q.back()
      self.mark(back[1], back[2], 0)
      self.q.pop_back()
    end
    self.p = self.p - self.i
  end
end

function Snake:forward(x, y)
  if self.d == 0 then
    return (x + 1) % self.x, y
  elseif self.d == 1 then
    return x, (y + 1) % self.y
  elseif self.d == 2 then
    return (x - 1 + self.x) % self.x, y
  else
    return x, (y - 1 + self.y) % self.y
  end
end

local texture = loadTexture('item.png')
local texWidth = texture:width()
local texHeight = texture:height()

function Snake:render()
  for i, v in ipairs(self.m) do
    if v ~= 0 then
      local ii = i - 1
      local nx = ii % self.x
      local ny = math.floor(ii / self.x)
      drawImage(texture, nx * 40 + 1, ny * 40 + 1, texWidth, texHeight, 0, 0, texWidth, texHeight, {1, 1, 1, 1})
    end
  end
end

function Snake:renderGameOver()

end

function Snake:turn(direction)
  self.d = direction
end

function Snake:reset()
  self.i = 0.3
  self.p = 0
  
  local m = self.m
  for i = 1, self.g do
    m[i] = 0
  end
  
  local q = Queue()
  q.push_front({1, 1})
  q.push_front({2, 1})
  q.push_front({3, 1})
  q.push_front({4, 1})
  
  self.mark(1, 1, 2)
  self.mark(2, 1, 2)
  self.mark(3, 1, 2)
  self.mark(4, 1, 2)
  self.q = q
  self.o = false
  
  self.r = self.g - 4
  self.spawn()
  self.turn(0)
end