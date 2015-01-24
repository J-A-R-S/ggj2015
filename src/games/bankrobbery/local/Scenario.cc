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

#include <cstdio>
#include <array>

#include "Events.h"
#include "Map.h"

Scenario::Scenario(game::EventManager& events, game::ResourceManager& resources) {
  events.registerHandler<HeroPositionEvent>(&Scenario::onHeroPosition, this);

  m_target.x = m_target.y = (Map::SIZE - 1.5) * 256.0f;

  m_font = resources.getFont("Averia-Bold.ttf");
  m_timer = 20;
}

game::EventStatus Scenario::onHeroPosition(game::EventType type, game::Event* event) {
  auto e = static_cast<HeroPositionEvent*>(event);
  m_hero = e->pos;
  
  return game::EventStatus::KEEP;
}

void Scenario::update(float dt) {
  m_timer -= dt;
}

void Scenario::render(sf::RenderWindow& window) {
  sf::CircleShape shape(100);
  shape.setFillColor({0xff, 0x00, 0x00, 0x80});
  shape.setOrigin(100.0, 100.0);
  shape.setPosition(m_target);

  window.draw(shape);

  sf::View saved_view = window.getView();

  auto size = window.getSize();
  sf::View view;
  view.setCenter(size.x / 2.0f, size.y / 2.0f);
  view.setSize(size.x, size.y);
  window.setView(view);

  if (m_timer > 0) {
    unsigned timer = static_cast<unsigned>(m_timer);
    std::array<char, 64> buffer;
    std::snprintf(buffer.data(), buffer.size(), "%.2u", timer);

    sf::Text text;
    text.setFont(*m_font);
    text.setCharacterSize(48);
    text.setColor(sf::Color::Red);
    text.setString(buffer.data());
    text.setPosition(0.0f, 0.0f);
    window.draw(text);
  }
  
  
  sf::Vector2f diff = m_target - m_hero;
  float angle = std::atan2(diff.y, diff.x);
  
  static constexpr float TRIANGLE_SIZE = 15;
  sf::CircleShape triangle(TRIANGLE_SIZE, 3);
  float radius = 100.0f;
  triangle.setPosition(size.x / 2 + radius * std::cos(angle), size.y / 2 + radius * std::sin(angle)); 
  triangle.setFillColor(sf::Color::Red);
  triangle.setOrigin(TRIANGLE_SIZE, TRIANGLE_SIZE);
  triangle.setRotation(angle / M_PI * 180.0 + 90.0);
  
  window.draw(triangle);
  
  window.setView(saved_view);
}
