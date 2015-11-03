require 'ds.class'
require 'gl.engine'
require 'tower.fight'
require 'tower.frames'

local Monster = Class()
function Monster:__init__(name, frames, health, attack, defence, experience, gold, special)
  self.name = name
  self.frames = frames
  self.health = health
  self.attack = attack
  self.defence = defence
  self.experience = experience
  self.gold = gold
  self.special = special
end

function Monster:toEnter(player)
  startFight(player, self)
end

function Monster:doAttack(player)
  if self.attack > player.defence then
    return self.attack - player.defence
  end
  return 0
end

local MagicMonster = Class(Monster)
function MagicMonster:__init__(name, frames, health, attack, defence, experience, gold)
  Monster.__init__(self, name, frames, health, attack, defence, experience, gold, '魔法攻击')
end

function MagicMonster:doAttack(player)
  if self.attack > player.resistance then
    return self.attack - player.resistance
  end
  return 0
end

local drawImage = drawImage
local loadImage = loadImage
function Monster:render(x, y, step)
  local image = loadImage(self.frames.image)
  local frame = self.frames.frame[step + 1]
  drawImage(image, x, y, 32, 32,
    frame[1], frame[2], 32, 32, {1, 1, 1, 1})
end

Monsters = {
  ['绿色史莱姆'] = Monster('绿色史莱姆', Frames['绿色史莱姆'], 56, 20, 1, 1, 1),
  ['红色史莱姆'] = Monster('红色史莱姆', Frames['红色史莱姆'], 64, 25, 2, 2, 1),
  ['小蝙蝠']     = Monster('小蝙蝠',     Frames['小蝙蝠'],     75, 35, 4, 3, 1),
  ['骷髅怪']     = Monster('骷髅怪',     Frames['骷髅怪'],     99, 70, 0, 4, 1),
}