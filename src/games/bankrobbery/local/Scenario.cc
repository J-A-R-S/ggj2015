/*
 * Bank Robbery for Dummies
 * Copyright (C) 2015 JARS
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Scenario.h"
#include "Events.h"
#include "Map.h"

Scenario::Scenario(game::EventManager& events) {
  events.registerHandler<HeroPositionEvent>(&Scenario::onHeroPosition, this);
  
  m_target.x = m_target.y = (Map::SIZE - 1.5) * 256.0f;
}

game::EventStatus Scenario::onHeroPosition(game::EventType type, game::Event* event) {
  return game::EventStatus::KEEP;
}

void Scenario::update(float dt) {
  
}

void Scenario::render(sf::RenderWindow& window) {
  sf::CircleShape shape(100);
  shape.setFillColor({0xff, 0x00, 0x00, 0x80});
  shape.setOrigin(100.0, 100.0);
  shape.setPosition(m_target);
  
  window.draw(shape);
  
}
