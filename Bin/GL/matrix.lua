require "ds.class"

Matrix = Class()
function Matrix:__init__()
  self.reset()
end

function Matrix:reset()
  local elemts = {}
  for i = 1, 16 do
    elemts[i] = 0
  end
  
  elemts[1]  = 1
  elemts[6]  = 1
  elemts[11] = 1
  elemts[16] = 1
  self.data = elemts
end

function Matrix:scale(scalex, scaley)
  local scale = Matrix()
  scale.data[1] = scalex
  scale.data[6] = scaley
  self.premultiply(scale)
end

function Matrix:rotate(degree)
  local rotate = Matrix()
  rotate.data[1] = cos(degree)
  rotate.data[2] = -sin(degree)
  rotate.data[5] = -rotate.data[2]
  rotate.data[6] = rotate[1]
  self.premultiply(rotate)
end

function Matrix:translate(deltax, deltay)
  local translate = Matrix()
  translate.data[4] = deltax
  translate.data[8] = deltay
  self.premultiply(translate)
end

function Matrix:orthoproject(width, height)
  self.reset()
  self.scale(2 / width, 2.0 / height)
  self.translate(-1, -1)
end

function Matrix:premultiply(mtx)
    local result = Matrix()
    
    local rdata = result.data
    local mdata = mtx.data
    local edata = self.data
    
    rdata[1]  = mdata[1]  * edata[1] + mdata[2]  * edata[5] + mdata[3]  * edata[9]  + mdata[4]  * edata[13]
    rdata[2]  = mdata[1]  * edata[2] + mdata[2]  * edata[6] + mdata[3]  * edata[10] + mdata[4]  * edata[14]
    rdata[3]  = mdata[1]  * edata[3] + mdata[2]  * edata[7] + mdata[3]  * edata[11] + mdata[4]  * edata[15]
    rdata[4]  = mdata[1]  * edata[4] + mdata[2]  * edata[8] + mdata[3]  * edata[12] + mdata[4]  * edata[16]

    rdata[5]  = mdata[5]  * edata[1] + mdata[6]  * edata[5] + mdata[7]  * edata[9]  + mdata[8]  * edata[13]
    rdata[6]  = mdata[5]  * edata[2] + mdata[6]  * edata[6] + mdata[7]  * edata[10] + mdata[8]  * edata[14]
    rdata[7]  = mdata[5]  * edata[3] + mdata[6]  * edata[7] + mdata[7]  * edata[11] + mdata[8]  * edata[15]
    rdata[8]  = mdata[5]  * edata[4] + mdata[6]  * edata[8] + mdata[7]  * edata[12] + mdata[8]  * edata[16]
    
    rdata[9]  = mdata[9]  * edata[1] + mdata[10] * edata[5] + mdata[11] * edata[9]  + mdata[12] * edata[13]
    rdata[10] = mdata[9]  * edata[2] + mdata[10] * edata[6] + mdata[11] * edata[10] + mdata[12] * edata[14]
    rdata[11] = mdata[9]  * edata[3] + mdata[10] * edata[7] + mdata[11] * edata[11] + mdata[12] * edata[15]
    rdata[12] = mdata[9]  * edata[4] + mdata[10] * edata[8] + mdata[11] * edata[12] + mdata[12] * edata[16]
    
    rdata[13] = mdata[13] * edata[1] + mdata[14] * edata[5] + mdata[15] * edata[9]  + mdata[16] * edata[13]
    rdata[14] = mdata[13] * edata[2] + mdata[14] * edata[6] + mdata[15] * edata[10] + mdata[16] * edata[14]
    rdata[15] = mdata[13] * edata[3] + mdata[14] * edata[7] + mdata[15] * edata[11] + mdata[16] * edata[15]
    rdata[16] = mdata[13] * edata[4] + mdata[14] * edata[8] + mdata[15] * edata[12] + mdata[16] * edata[16]
    
    self.data = result.data
end

function Matrix:postmultiply(mtx)
  local result = Matrix()
  for i = 1, 16 do
    result.data[i] = mdata[i]
  end
  result.premultiply(self)
  self.data = result.data
end