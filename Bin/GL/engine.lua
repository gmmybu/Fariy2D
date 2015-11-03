require 'ds.queue'

local stack = Queue()
function pushMatrix(matrix, merge)
  local back = stack.back()
  if back and merge then
    matrix.premultiply(back)
  end
  stack.push_back(matrix)
end

function popMatrix()
  stack.pop_back()
end

require 'gl.matrix'
require 'gl.shader'

local prog = createProgram(texture_ucolor.vertexshader, texture_ucolor.fragshader)

local vertex = createVertexArray()
local buffer = createVertexBuffer(64 * 4)
local elementBuffer = createVertexBuffer(54 * 3 * 4)

local glViewPortO = glViewport
function glViewport(width, height)
  glViewPortO(width, height)
  
  local proj = Matrix()
  proj.orthoproject(width, height)
  stack.push_back(proj)
end

function drawImage(texture, left, top, width, height, srcleft, srctop, srcwidth, srcheight, color)
  local texWidth  = texture:width()
  local texHeight = texture:height()
  
  local vertexdata = {}
  vertexdata[1] = left
  vertexdata[2] = top
  vertexdata[3] = left
  vertexdata[4] = top + height
  vertexdata[5] = left + width
  vertexdata[6] = top
  vertexdata[7] = left + width
  vertexdata[8] = top + height
  
  vertexdata[9]  = srcleft / texWidth
  vertexdata[10] = (srctop + srcheight) / texHeight
  vertexdata[11] = srcleft / texWidth
  vertexdata[12] = srctop  / texHeight
  vertexdata[13] = (srcleft + srcwidth) / texWidth  
  vertexdata[14] = (srctop + srcheight) / texHeight
  vertexdata[15] = (srcleft + srcwidth) / texWidth 
  vertexdata[16] = srctop  / texHeight
  
  buffer:copyf(0, vertexdata)
  vertex:bindBuffer(buffer, 0, 2, 0, 0)
  vertex:bindBuffer(buffer, 1, 2, 0, 32)
  
  texture:bind(0)
  prog:setUniformi("texture", 0)
  
  prog:setUniformv("color", 4, color)
  
  local matrix = stack.back()
  prog:setUniformMatrix("MVP", matrix.data)
  prog:drawArrays(vertex, 5, 0, 4)
end

local tblinsert = table.insert
function drawImageScaleGrid(texture, left, top, width, height, scaleleft, scaleright, scaletop, scalebottom, color)
  local texWidth  = texture:width()
  local texHeight = texture:height()
  
  local xPos = {}
  local xTex = {}
  tblinsert(xPos, left)
  tblinsert(xTex, 0)
  if scaleleft > 0 then
    tblinsert(xPos, left + scaleleft)
    tblinsert(xTex, scaleleft / texWidth)
  end
  if scaleright > 0 then
    tblinsert(xPos, left + width - scaleright)
    tblinsert(xTex, (texWidth - scaleright) / texWidth)
  end
  tblinsert(xPos, left + width)
  tblinsert(xTex, 1)
  
  local yPos = {}
  local yTex = {}
  tblinsert(yPos, top)
  tblinsert(yTex, 0)
  if scaletop > 0 then
    tblinsert(yPos, top + scaletop)
    tblinsert(yTex, scaletop / texHeight)
  end
  if scalebottom > 0 then
    tblinsert(yPos, top + height - scalebottom)
    tblinsert(yTex, (texHeight - scalebottom) / texHeight)
  end
  tblinsert(yPos, top + height)
  tblinsert(yTex, 1)
  
  local points = {}
  for i = 1, #yPos do
    for j = 1, #xPos do
      tblinsert(points, xPos[j])
      tblinsert(points, yPos[i])
      tblinsert(points, xTex[j])
      tblinsert(points, yTex[i])
    end
  end
  
  local elements = {}
  for i = 0, #yPos - 2 do
    local start = i * #xPos
    for j = 0, #xPos - 2 do
      tblinsert(elements, start)
      tblinsert(elements, start + #xPos)
      tblinsert(elements, start + 1)
      tblinsert(elements, start + 1)
      tblinsert(elements, start + #xPos)
      tblinsert(elements, start + #xPos + 1)
      start = start + 1
    end
  end
  
  buffer:copyf(0, points)
  elementBuffer:copyi(0, elements)
  
  vertex:bindBuffer(buffer, 0, 2, 16, 0)
  vertex:bindBuffer(buffer, 1, 2, 16, 8)
  vertex:bindElement(elementBuffer)
  
  texture:bind(0)
  prog:setUniformi("texture", 0)
  
  prog:setUniformv("color", 4, color)
  
  local matrix = stack.back()
  prog:setUniformMatrix("MVP", matrix.data)
  prog:drawElements(vertex, 4, 54, 0)
  vertex:bindElement()
end

local textures = {}
setmetatable(textures, {__mode='v'})

local loadTextureO = loadTexture
function loadImage(name)
  local texture = textures[name]
  if not texture then
    texture = loadTextureO(name)
    textures[name] = texture
  end
  return texture
end

local renderers = {}
setmetatable(renderers, {__mode='v'})

function loadTextRenderer(font, size)
  local name = font..size
  local renderer = renderers[name]
  if not renderer then
    renderer = createTextRenderer(font, size)
    renderers[name] = renderer
  end
  return renderer
end

function drawString(renderer, text, left, bottom, color)
  local matrix = stack.back()
  renderer:render(text, left, bottom, color, matrix.data)
end

local position_ucolor_prog = createProgram(position_ucolor.vertexshader, position_ucolor.fragshader)
local position_ucolor_vert = createVertexArray()
local position_ucolor_buffer = createVertexBuffer(16 * 4)
function drawLine(x1, y1, x2, y2, color, width)
  local vertexdata = {}
  vertexdata[1] = x1
  vertexdata[2] = y1
  vertexdata[3] = x2
  vertexdata[4] = y2
  
  position_ucolor_buffer:copyf(0, vertexdata)
  position_ucolor_vert:bindBuffer(position_ucolor_buffer, 0, 2, 0, 0)
  position_ucolor_prog:setUniformv('color', 4, color)
  
  local matrix = stack.back()
  position_ucolor_prog:setUniformMatrix("MVP", matrix.data)
  position_ucolor_prog:drawArrays(position_ucolor_vert, GL_LINES, 0, 2)
  
end

function drawRectangle(left, top, right, bottom, color)
  local vertexdata = {}
  vertexdata[1] = left
  vertexdata[2] = top
  vertexdata[3] = left
  vertexdata[4] = bottom
  vertexdata[5] = right
  vertexdata[6] = top
  vertexdata[7] = right
  vertexdata[8] = bottom
  
  position_ucolor_buffer:copyf(0, vertexdata)
  position_ucolor_vert:bindBuffer(position_ucolor_buffer, 0, 2, 0, 0)
  
  position_ucolor_prog:setUniformv('color', 4, color)
  
  local matrix = stack.back()
  position_ucolor_prog:setUniformMatrix('MVP', matrix.data)
  position_ucolor_prog:drawArrays(position_ucolor_vert, GL_TRIANGLE_STRIP, 0, 4)
end
