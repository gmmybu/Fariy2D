require 'gl.engine'
require 'gl.matrix'
require 'gl.define'
require 'tower.scene'
require 'tower.frames'

local loadImage = loadImage
local drawImage = drawImage

local texture = loadImage('window\\window.png')
local bgPos = {0, 0, 128, 128}

local borderTexture = loadImage('window\\border.png')
local bdPos = {0, 0, 32, 32}

local renderer = loadTextRenderer('黑体', 18)

local FightLayer = Class()

function FightLayer:__init__(hero, monster, gridx, gridy)  
  self.hero = hero
  self.gridx = gridx
  self.gridy = gridy
  
  self.monster = monster
  self.monsterhealth = monster.health
  
  self.timer = Timer(self.onTimer, 0.25)
  self.complete = false
  self.combatlog = {}
  
  self.heroturn = true
end

local frames = Frames['英雄下']
function FightLayer:render()
  local matrix = Matrix()
  matrix.translate(60, 105)
  pushMatrix(matrix, true)
  drawImage(texture, 0, 0, 360, 270, 0, 0, 128, 128, {1, 1, 1, .75})
  drawImageScaleGrid(borderTexture, -1, -1, 362, 272, 4, 4, 4, 4, {0.9, 0.9, 0.9, 1})
  
  --画勇士
  local image = loadImage(frames.image)
  local frame = frames.frame[1]
  drawImage(image, 58, 222, 32, 32, frame[1], frame[2], 32, 32, {1,1,1,1})
  
  drawString(renderer, '生命: '..self.hero.health,     27, 186, {0.8,0.9,1,1})
  drawString(renderer, '攻击: '..self.hero.attack,     27, 162, {0.8,0.9,1,1})
  drawString(renderer, '防御: '..self.hero.defence,    27, 138, {0.8,0.9,1,1})
  drawString(renderer, '魔防: '..self.hero.resistance, 27, 114, {0.8,0.9,1,1})
  
  drawString(renderer, 'VS', 164, 222, {0.9,0,0.5,1})
  
  
  --画怪物
  local image = loadImage(self.monster.frames.image)
  local frame = self.monster.frames.frame[1]
  drawImage(image, 270, 222, 32, 32, frame[1], frame[2], 32, 32, {1, 1, 1, 1})
  drawString(renderer, '生命: '..self.monsterhealth,   239, 186, {0.8,0.9,1,1})
  drawString(renderer, '攻击: '..self.monster.attack,  239, 162, {0.8,0.9,1,1})
  drawString(renderer, '防御: '..self.monster.defence, 239, 138, {0.8,0.9,1,1})
  
  local top = 86
  for i, v in ipairs(self.combatlog) do
    drawString(renderer, v, 27, top, {0.9,0,1,1})
    top = top - 24
  end
  
  drawString(renderer, '撤退(L)', 284, 16, {1,0,0,1})
  
  popMatrix()
end


function FightLayer:handle_keyboard_event(what, how)
  if how == Win32.WM_KEYDOWN and what == 0x4c then
    local scene = getCurrentScene()
    if scene then
      scene.delLayer(self)
    end
  end
  return true
end

function FightLayer:onTimer()
  if self.complete then
    local scene = getCurrentScene()
    if scene then
      scene.delLayer(self)
    end
    return
  end
  
  if self.heroturn then
    local damage, continue = self.hero.doAttack(self.monster)
    self.addCombatLog('勇士攻击，怪物损失'..damage..'点生命')
    self.monsterhealth = self.monsterhealth - damage
    if self.monsterhealth <= 0 then
      self.monsterhealth = 0
      self.complete = true
      self.hero.gold = self.hero.gold + self.monster.gold
      self.hero.experience = self.hero.experience + self.monster.experience
      self.addCombatLog('战斗胜利! 获得'..self.monster.experience..'经验 '..self.monster.gold..'金币')
    end
    self.heroturn = continue
  else
    local damage, continue = self.monster.doAttack(self.hero)
    self.addCombatLog('怪物攻击，勇士损失'..damage..'点生命')
    self.hero.health = self.hero.health - damage
    self.heroturn = not continue
  end
end

function FightLayer:addCombatLog(inf)
  table.insert(self.combatlog, inf)
  if #self.combatlog > 3 then
    table.remove(self.combatlog, 1)
  end
end

function FightLayer:onEnter()
  self.timer.setTimer()
end

function FightLayer:onLeave()
  self.timer.killTimer()
  
  if self.complete then
    Game.delGrid(self.gridx, self.gridy)
  end
end

function startFight(hero, monster)
  local scene = getCurrentScene()
  if scene then
    local gridx, gridy = hero.nextGrid()
    scene.addLayer(FightLayer(hero, monster, gridx, gridy))
  end
end

