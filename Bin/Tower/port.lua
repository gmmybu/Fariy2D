require 'ds.class'
require 'gl.engine'
require 'tower.sprite'

local uport = Sprites['楼梯上']
local dport = Sprites['楼梯下']
local loadImage = loadImage
local drawImage = drawImage

UpLadder = Class()
function UpLadder:__init__(target, gridx, gridy, face)
  self.target = target
  self.gridx = gridx
  self.gridy = gridy
  self.face  = face
end

function UpLadder:render(x, y)
  local image = loadImage(uport.image)
  drawImage(image, x, y, 32, 32, uport.frame[1], uport.frame[2], 32, 32, {1,1,1,1})
end

function UpLadder:onEnter()
  Game.transport(self.target, self.gridx, self.gridy, self.face)
end

function UpLadder:toEnter()
  return true
end

DownLadder = Class()
function DownLadder:__init__(target, gridx, gridy, face)
  self.target = target
  self.gridx = gridx
  self.gridy = gridy
  self.face  = face
end

function DownLadder:render(x, y)
  local image = loadImage(dport.image)
  drawImage(image, x, y, 32, 32, dport.frame[1], dport.frame[2], 32, 32, {1,1,1,1})
end

function DownLadder:onEnter()
  Game.transport(self.target, self.gridx, self.gridy, self.face)
end

function DownLadder:toEnter()
  return true
end