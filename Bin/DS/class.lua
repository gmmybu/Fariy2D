function WrapMethod(self, f)
  return function(...)
    return f(self, ...)
  end
end

function Class(...)
  local class = {}
  local proto = {...} --lua5.2.3
  setmetatable(class, {__index = function(_, key)
    --disable __index
    for i = 1, #proto do
      local value = rawget(proto[i], key)
      if value then
        return value
      end
    end
    --enable __index
    for i = 1, #proto do
      local value = proto[i][key]
      if value then
        return value
      end
    end
  end,
  
  __call = function(_, ...)
    local obj = {}
    setmetatable(obj, {__index = function(self, key)
      local value = class[key]
      if type(value) == 'nil' then
        return value
      end
      
      if type(value) == 'function' then
        self[key] = WrapMethod(self, value)
      else
        self[key] = value
      end
      return self[key]
    end})
    if class.__init__ then class.__init__(obj, ...) end
    return obj
  end})
  return class
end
