require 'ds.class'

Queue = Class()

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

function Queue:clear()
  self.head = nil
  self.tail = nil
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

function Queue:iterator()
  local item = self.head
  return function()
    if item then
      local prev = item
      item = prev.next
      return prev.key
    end
  end
end

function Queue:riterator()
  local item  = self.tail
  return function()
    if item then
      local next = item
      item = next.prev
      return next.key
    end
  end
end

function Queue:remove(val)
  local item = self.head
  while item do
    if item.key == val then
      if item == self.head then
        self.head = item.next
        if not self.head then
          self.tail = nil
        else
          self.head.prev = nil
        end
      elseif item == self.tail then
        self.tail = item.prev
        self.tail.next = nil
      else
        item.next.prev = item.prev
        item.prev.next = item.next
      end
      break
    end
    item = item.next
  end
end

